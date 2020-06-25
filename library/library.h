#ifndef TIMERLIB_LIBRARY_H
#define TIMERLIB_LIBRARY_H
#include <time.h>
#include <iostream>
using namespace std;
void hello();

class CTimer {
    //LARGE_INTEGER t_start, t_end;
#ifdef linux
    timespec t_start, t_end;
#elif _WIN64
    long long t_start_speed, t_end_speed, t_start_count, t_end_count;
#endif
    long long spent_time;
public:
    void Start();

    void End();

    void Count();
};
#endif //TIMERLIB_LIBRARY_H
