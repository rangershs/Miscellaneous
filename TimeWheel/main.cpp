#include <iostream>
#include <thread>
#include <chrono>
#include "timer.h"
#include "TimeWheelOpti.h"

using TimePoint = std::chrono::high_resolution_clock::time_point;

void TimerHandler(TimePoint _now)
{
    TimePoint now = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now-_now).count() << std::endl;
}

int main()
{
    TimeWheelImple time_wheel;
    time_wheel.init();
    TimePoint now = std::chrono::high_resolution_clock::now();
    time_wheel.add_timer(std::bind(&TimerHandler, now), 1500, true);

    int time_id = time_wheel.add_timer(std::bind(&TimerHandler, now), 10000);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    bool ret_bool = time_wheel.cancel_timer(time_id);
    std::cout << "is deletes? " << ret_bool << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    ret_bool = time_wheel.cancel_timer(time_id);
    std::cout << "is deleted? " << ret_bool << std::endl;

    //  简单测试sleep + cout,输出的结果有误差
/*    while (true)
    {
        for (auto i=0; i<1500; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        TimePoint _now = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(_now-now).count() << std::endl;
    }*/

    getchar();
    return 0;
}