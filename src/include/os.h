#ifndef __OS_H__
#define __OS_H__
#include "port.h"

#include "os_config.h"
#include "os_scheduler.h"
#include "os_task.h"
#include "os_types.h"

void os_init(void);
void os_start(void);

#endif // __OS_H__