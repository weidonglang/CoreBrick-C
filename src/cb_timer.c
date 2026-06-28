#include "cb_timer.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#pragma comment(lib, "kernel32.lib")

uint64_t cb_timer_now_ns(void) {
    static LARGE_INTEGER freq = {0};
    static int freq_initialized = 0;

    if (!freq_initialized) {
        QueryPerformanceFrequency(&freq);
        freq_initialized = 1;
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    return (uint64_t)((now.QuadPart * 1000000000ULL) / freq.QuadPart);
}

#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <time.h>

uint64_t cb_timer_now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

#else
#error "Unsupported platform: no high-resolution timer available"
#endif

void cb_timer_start(CB_Timer *timer) {
    if (timer != NULL) {
        timer->start_ns = cb_timer_now_ns();
    }
}

uint64_t cb_timer_elapsed_ns(const CB_Timer *timer) {
    if (timer == NULL) {
        return 0;
    }
    return cb_timer_now_ns() - timer->start_ns;
}

double cb_timer_elapsed_ms(const CB_Timer *timer) {
    return (double)cb_timer_elapsed_ns(timer) / 1000000.0;
}