//
// Created by shs on 2019-01-05.
//

#include "timer.h"
#ifdef _MSC_VER
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif

#include <chrono>

//////////////////////////////////////////////////////////////////////////
// Timer

Timer::Timer(TimerManager& manager)
        : manager_(manager),
          heapIndex_(-1)
{
}

Timer::~Timer()
{
    Stop();
}

void Timer::Stop()
{
    if (heapIndex_ != -1)
    {
        manager_.RemoveTimer(this);
        heapIndex_ = -1;
    }
}

void Timer::OnTimer(unsigned long long now)
{
    if (timerType_ == Timer::CIRCLE)
    {
        expires_ = interval_ + now;
        manager_.AddTimer(this);
    }
    else
    {
        heapIndex_ = -1;
    }
    timerFun_();
}

//////////////////////////////////////////////////////////////////////////
// TimerManager
//  index == ID <== the size of vector
void TimerManager::AddTimer(Timer* timer)
{
    timer->heapIndex_ = static_cast<int>(heap_.size());
    HeapEntry entry{ timer->expires_, timer };
    heap_.emplace_back(entry);
    UpHeap(static_cast<int>(heap_.size() - 1));
}

void TimerManager::RemoveTimer(Timer* timer)
{
    int index = timer->heapIndex_;
    if (!heap_.empty() && index < heap_.size())
    {
        //  末尾元素直接删除
        if (index == heap_.size() - 1)
        {
            heap_.pop_back();
        }
        else
        {
            //  vector删除元素,这种方式提高了性能,减少了内存移动
            //  向上 or 向下调整堆
            SwapHeap(index, static_cast<int>(heap_.size() - 1));
            heap_.pop_back();
            int parent = (index - 1) / 2;
            if (index > 0 && heap_[index].time < heap_[parent].time)
                UpHeap(index);
            else
                DownHeap(index);
        }
    }
}

void TimerManager::DetectTimers()
{
    unsigned long long now = GetCurrentMillisecs();

    while (!heap_.empty() && heap_[0].time <= now)
    {
        Timer* timer = heap_[0].timer;
        RemoveTimer(timer);
        timer->OnTimer(now);
    }
}

void TimerManager::UpHeap(int index)
{
    int parent = (index - 1) / 2;
    while (index > 0 && heap_[index].time < heap_[parent].time)
    {
        SwapHeap(index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

void TimerManager::DownHeap(int index)
{
    int child = index * 2 + 1;
    while (child < heap_.size())
    {
        int minChild = (child + 1 == heap_.size() || heap_[child].time < heap_[child + 1].time)
                          ? child : child + 1;
        if (heap_[index].time < heap_[minChild].time)
            break;
        SwapHeap(index, minChild);
        index = minChild;
        child = index * 2 + 1;
    }
}
//  交换的同时其index-id也要交换
void TimerManager::SwapHeap(int index1, int index2)
{
    HeapEntry tmp = heap_[index1];
    heap_[index1] = heap_[index2];
    heap_[index2] = tmp;
    heap_[index1].timer->heapIndex_ = index1;
    heap_[index2].timer->heapIndex_ = index2;
}


unsigned long long TimerManager::GetCurrentMillisecs()
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return static_cast<unsigned long long>(ms);
}