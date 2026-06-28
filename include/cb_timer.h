#ifndef CB_TIMER_H
#define CB_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t start_ns;
} CB_Timer;

uint64_t cb_timer_now_ns(void);
void cb_timer_start(CB_Timer *timer);
uint64_t cb_timer_elapsed_ns(const CB_Timer *timer);
double cb_timer_elapsed_ms(const CB_Timer *timer);

#ifdef __cplusplus
}
#endif

#endif /* CB_TIMER_H */