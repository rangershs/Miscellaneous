#include <iostream>
#include <thread>
#include <chrono>
#include "timer.h"
#include "TimeWheelOpti.h"

using TimePoint = std::chrono::high_resolution_clock::time_point;
TimeWheelImple time_wheel;

void TimerHandler(TimePoint _now)
{
    TimePoint now = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now-_now).count() << std::endl;
    time_wheel.add_timer(std::bind(&TimerHandler, now), 10);
}

int main()
{
    //  绑定/引用至同一个manager执行
/*    TimerManager tm;
    Timer t(tm);
    Timer t1(tm);
    t.Start(&TimerHandler, 1000);
    t1.Start(&TimerHandler_1, 500);
    std::thread thd([&tm]()
    {
        while (true)
        {
            tm.DetectTimers();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    Timer t2(tm);
    t2.Start(&TimerHandler_2, 250);*/

/*    int interval = 127110;
    std::cout << interval / 60000 << std::endl;
    std::cout << interval % 60000 / 1000 << std::endl;
    std::cout << interval % 60000 % 1000 << std::endl;*/


    time_wheel.init();
    TimePoint now = std::chrono::high_resolution_clock::now();
    time_wheel.add_timer(std::bind(&TimerHandler, now), 10);

    getchar();
    return 0;
}