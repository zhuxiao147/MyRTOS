
#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#include "os_types.h"

void os_time_init(void);
void os_increment_tick_count(void);
os_uint32_t os_get_tick_count(void);
void os_delay(os_uint32_t ticks);
void os_delay_ms(os_uint32_t ms);

#endif // __OS_TIME_H__
