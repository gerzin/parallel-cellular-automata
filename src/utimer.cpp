#include <iostream>
#include <utimer.hpp>

#define START(timename) auto timename = std::chrono::system_clock::now();
#define STOP(timename, elapsed)                                                                                        \
    auto elapsed =                                                                                                     \
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();

UTimer::UTimer(const std::string m) : message(m), us_elapsed((long *)NULL)
{
    start = std::chrono::system_clock::now();
}

UTimer::UTimer(const std::string m, long *us) : message(m), us_elapsed(us)
{
    start = std::chrono::system_clock::now();
}

UTimer::~UTimer()
{
    stop = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = stop - start;
    auto musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    std::cout << message << " computed in " << musec << " usec " << std::endl;
    if (us_elapsed != NULL)
        (*us_elapsed) = musec;
}
