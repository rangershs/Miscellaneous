#include <iostream>
#include "timer.h"
#include <thread>
#include <chrono>

void TimerHandler()
{
    std::cout << "TimerHandler: 1s" << std::endl;
}

void TimerHandler_1()
{
    std::cout << "TimerHandler: 0.5s" << std::endl;
}

void TimerHandler_2()
{
    std::cout << "TimerHandler: 0.25s" << std::endl;
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

    getchar();
    return 0;
}