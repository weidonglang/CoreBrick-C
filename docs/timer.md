# Timer

## Overview

Cross-platform high-resolution timer for measuring elapsed time. Supports Windows (QueryPerformanceCounter) and POSIX (clock_gettime) platforms.

## API Reference

### `cb_timer_now_ns`
```c
uint64_t cb_timer_now_ns(void);
```
Returns current timestamp in nanoseconds since an unspecified epoch.

### `cb_timer_start`
```c
void cb_timer_start(CB_Timer *timer);
```
Records the start time. Safe to call with NULL.

### `cb_timer_elapsed_ns`
```c
uint64_t cb_timer_elapsed_ns(const CB_Timer *timer);
```
Returns elapsed nanoseconds since start. Returns 0 if timer is NULL.

### `cb_timer_elapsed_ms`
```c
double cb_timer_elapsed_ms(const CB_Timer *timer);
```
Returns elapsed milliseconds since start. Returns 0.0 if timer is NULL.