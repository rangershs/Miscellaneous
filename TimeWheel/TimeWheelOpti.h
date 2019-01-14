//
// Created by shs on 2019-01-06.
//

#ifndef TIMERLAB_TIMEWHEELOPTI_H
#define TIMERLAB_TIMEWHEELOPTI_H

#include <functional>
#include <vector>
#include <list>
#include <mutex>
#include <atomic>

//  time-span: from 1ms to 1h
//  时间跨度足够大时,ADD,TICK的时间复杂度才是O(1)
class TimeWheelImple
{
    using Func = std::function<void()>;
    enum struct TimeEpoch
    {
        TRIGGERED_MS,
        TRIGGERED_S,
        TRIGGERED_MIN
    };
public:
    struct TimePos;
    struct TimerEvent;

public:
    bool            init();
    //  _interval unit: millisecond
    int             add_timer(Func _func, int _interval);
    bool            cancel_timer(int _timerID);

    TimeWheelImple();
    ~TimeWheelImple();

private:
    void            loop();
    void            handleTimer(TimeEpoch _timeepoch);
    static int      get_timeID();
    bool            search_delete(std::vector<std::list<TimerEvent>>& _timercontainer, int _timerID);

private:
    int                                     ms_tick;
    int                                     s_tick;
    int                                     min_tick;

    std::atomic<bool>                       running;
    static std::atomic<int>                 sequence_;

    std::mutex                              mutex_;
    std::vector<std::list<TimerEvent>>      ms_vec_;
    std::vector<std::list<TimerEvent>>      s_vec_;
    std::vector<std::list<TimerEvent>>      min_vec_;
};

#endif //TIMERLAB_TIMEWHEELOPTI_H
