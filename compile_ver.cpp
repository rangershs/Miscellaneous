#include <iostream>
#include <string>

std::string GetCompileVersion()
{
    const char* Months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    const char* date = __DATE__;
    const char* time = __TIME__;
    char mon[4]; int year; int day;
    sscanf(date, "%s %d %d", mon, &day, &year);
    unsigned int i_mon = 0;
    for (; i_mon < sizeof(Months) / sizeof(char*); ++i_mon)
    {
        if (strcmp(Months[i_mon], mon) == 0)
            break;
    }
    int hour; int min; int sec;
    sscanf(time, "%d:%d:%d", &hour, &min, &sec);
    char ver[32];
    snprintf(ver, 32, "%04d%02d%02d-%02d%02d%02d", year, i_mon + 1, day, hour, min, sec);
    return ver;
}

int main()
{
    std::string ver = GetCompileVersion();
    std::cout << ver << std::endl;

    return 0;
}