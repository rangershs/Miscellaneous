//
// Created by shs on 2019-01-05.
//
/*
 * Todo: 最小堆的实现方法提供了一种不错的思路,在一定程度上加深了堆排序的理解
 *       但是代码的耦合性较强,接口设计也不够完善,若用于产品中还需要重构/优化
 */

#ifndef TIMERLAB_TIMER_H
#define TIMERLAB_TIMER_H

#include <vector>
#include <functional>

class TimerManager;

class Timer
{
public:
    enum TimerType
    {
        ONCE,
        CIRCLE
    };

    explicit Timer(TimerManager& manager);
    ~Timer();

    template<typename Fun>
    void Start(Fun fun, unsigned interval, TimerType timeType = CIRCLE);

    void Stop();

private:
    void OnTimer(unsigned long long now);

private:
    friend class TimerManager;
    TimerManager&               manager_;
    TimerType                   timerType_;
    std::function<void(void)>   timerFun_;
    unsigned                    interval_;
    unsigned long long          expires_;

    int                         heapIndex_;
};

class TimerManager
{
public:
    static unsigned long long   GetCurrentMillisecs();
    void                        DetectTimers();
private:
    friend class Timer;
    void AddTimer(Timer* timer);
    void RemoveTimer(Timer* timer);

    void UpHeap(int index);
    void DownHeap(int index);
    void SwapHeap(int index1, int index2);

private:
    struct HeapEntry
    {
        unsigned long long time;
        Timer* timer;
    };
    std::vector<HeapEntry> heap_;
};

template<typename Fun>
inline void Timer::Start(Fun fun, unsigned interval, TimerType timeType)
{
    Stop();
    interval_   = interval;
    timerFun_   = fun;
    timerType_  = timeType;
    expires_    = interval_ + TimerManager::GetCurrentMillisecs();
    manager_.AddTimer(this);
}

#endif //TIMERLAB_TIMER_H
