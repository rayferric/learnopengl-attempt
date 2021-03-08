#ifndef EYESPARK_TIMER_H
#define EYESPARK_TIMER_H

class Timer {
private:
    LARGE_INTEGER frequency;
    LARGE_INTEGER startTime;
    LARGE_INTEGER endTime;
    bool running = false;
public:
    Timer() {
        QueryPerformanceFrequency(&frequency);
        reset();
    }
    void start() {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);
        startTime.QuadPart += time.QuadPart - endTime.QuadPart;
        running = true;
    }
    void stop() {
        if(running)QueryPerformanceCounter(&endTime);
        running = false;
    }
    void reset() {
        QueryPerformanceCounter(&startTime);
        endTime.QuadPart = startTime.QuadPart;
    }
    bool isRunning() {
        return running;
    }
    double getElapsed() {
        if(running)QueryPerformanceCounter(&endTime);
        return (double)((endTime.QuadPart-startTime.QuadPart)*(1000.0/frequency.QuadPart));
    }
};

#endif // EYESPARK_TIMER_H
