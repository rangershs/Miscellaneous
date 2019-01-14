//
// Created by shs on 2019-01-06.
//

#include <thread>
#include <chrono>
#include "TimeWheelOpti.h"

//  Warning: not copyable variable
std::atomic<int> TimeWheelImple::sequence_(0);

struct TimeWheelImple::TimePos
{
    int     ms_pos;
    int     s_pos;
    int     min_pos;
};

struct TimeWheelImple::TimerEvent
{
    int                             interval;
    std::function<void(void)>       call_back;
    TimePos                         time_pos;
    int                             timerID;
};

TimeWheelImple::TimeWheelImple()
    : ms_tick(0),
      s_tick(0),
      min_tick(0)
{
    running.store(false, std::memory_order_release);
    ms_vec_.resize(1024);
    s_vec_.resize(64);
    min_vec_.resize(64);
}

TimeWheelImple::~TimeWheelImple()
{
    running.store(false, std::memory_order_release);

    std::lock_guard<std::mutex> lck(mutex_);
    ms_vec_.clear();
    s_vec_.clear();
    min_vec_.clear();
}

bool TimeWheelImple::init()
{
    if(running.load(std::memory_order_acquire))
        return false;

    running.store(true, std::memory_order_release);
    std::thread thd(std::bind(&TimeWheelImple::loop, this));
    thd.detach();
    return true;
}

int TimeWheelImple::add_timer(Func _func, int _interval)
{
    if (!_func ||
        _interval <=0 ||
        _interval >= 3600000)
        return -1;

    int min_interval     = _interval / 60000;
    int s_interval       = _interval % 60000 / 1000;
    int ms_interval      = _interval % 60000 % 1000;

    int ms_pos           = ms_tick + ms_interval;
    int s_pos            = s_tick + s_interval;
    int min_pos          = min_tick + min_interval;
    int overflow = 0;
    if (ms_pos >= 1000)
    {
        ms_pos %= 1000;
        overflow = 1;
    }
    s_pos += overflow;
    overflow = 0;
    if (s_pos >= 60)
    {
        s_pos %= 60;
        overflow = 1;
    }
    min_pos += overflow;
    //  最大时间单位溢出,溢出后min=0的事件实际置于min=60处理
    if (min_pos > 60)
    {
        min_pos %= 60;
    }
    else if (min_pos == 60)
    {
        min_pos = 60;
    }

    TimerEvent time_event = {};
    time_event.interval     = _interval;
    time_event.call_back    = _func;
    time_event.time_pos     = {ms_pos, s_pos, min_pos};
    time_event.timerID      = get_timeID();

    std::lock_guard<std::mutex> lck(mutex_);
    if (min_pos != min_tick)
        min_vec_[min_pos].emplace_back(time_event);
    else if (s_pos != s_tick)
        s_vec_[s_pos].emplace_back(time_event);
    else
        ms_vec_[ms_pos].emplace_back(time_event);

    return time_event.timerID;
}

bool TimeWheelImple::cancel_timer(int _timerID)
{
    std::lock_guard<std::mutex> lck(mutex_);
    if (search_delete(min_vec_, _timerID))
        return true;
    if (search_delete(s_vec_, _timerID))
        return true;
    if (search_delete(ms_vec_, _timerID))
        return true;

    return false;
}

/*
 * ms_tick:      1 -- 1000
 * s_tick:       1 -- 60
 * min_tick:     1 -- 60
 * 0,0,0 ==> 60,0,0
 */
void TimeWheelImple::loop()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        ms_tick++;
        handleTimer(TimeEpoch::TRIGGERED_MS);

        if(ms_tick >= 1000)
        {
            ms_tick = 0;
            s_tick++;
            handleTimer(TimeEpoch::TRIGGERED_S);
            if(s_tick >= 60)
            {
                s_tick = 0;
                min_tick++;
                handleTimer(TimeEpoch::TRIGGERED_MIN);
                if(min_tick >= 60)
                    min_tick = 0;
            }
        }
    }
}

void TimeWheelImple::handleTimer(TimeEpoch _timeepoch)
{
    switch (_timeepoch)
    {
        case TimeEpoch::TRIGGERED_MS :
        {
            std::lock_guard<std::mutex> lck(mutex_);
            auto& lists = ms_vec_[ms_tick];
            while (!lists.empty())
            {
                auto item = std::move(lists.front());
                lists.pop_front();
                item.call_back();
            }

        }
            break;
        case TimeEpoch::TRIGGERED_S :
        {
            std::lock_guard<std::mutex> lck(mutex_);
            auto& lists = s_vec_[s_tick];
            while (!lists.empty())
            {
                auto item = std::move(lists.front());
                lists.pop_front();
                if (item.time_pos.ms_pos)
                {
                    item.time_pos.s_pos = 0;
                    int tmp_ms = ms_tick + item.time_pos.ms_pos;
                    ms_vec_[tmp_ms].emplace_back(item);
                }
                else
                {
                    item.call_back();
                }
            }
        }
            break;
        case TimeEpoch::TRIGGERED_MIN :
        {
            std::lock_guard<std::mutex> lck(mutex_);
            auto& lists = min_vec_[min_tick];
            while (!lists.empty())
            {
                auto item = std::move(lists.front());
                lists.pop_front();
                if (item.time_pos.s_pos)
                {
                    item.time_pos.min_pos = 0;
                    int tmp_s = s_tick + item.time_pos.s_pos;
                    s_vec_[tmp_s].emplace_back(item);
                }
                else if (item.time_pos.ms_pos)
                {
                    item.time_pos.min_pos = 0;
                    int tmp_ms = ms_tick + item.time_pos.ms_pos;
                    ms_vec_[tmp_ms].emplace_back(item);
                }
                else
                {
                    item.call_back();
                }
            }
        }
            break;
        default:
            break;
    }
}

int TimeWheelImple::get_timeID()
{
    return  sequence_++;
}

bool TimeWheelImple::search_delete(std::vector<std::list<TimerEvent>>& _timercontainer, int _timerID)
{
    for (auto& item : _timercontainer)
    {
        if (item.empty())
            continue;
        for (auto itr=item.begin(); itr!=item.end(); ++itr)
        {
            if (itr->timerID == _timerID)
            {
                item.erase(itr);
                return true;
            }
        }
    }
    return false;
}