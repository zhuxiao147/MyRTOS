#ifndef __PORT_H__
#define __PORT_H__
#include "stm32f7xx.h"

#define OS_ENABLE_INTERRUPTS() __asm volatile("cpsie i" : : : "memory")
#define OS_DISABLE_INTERRUPTS() __asm volatile("cpsid i" : : : "memory")

#define OS_PORT_SVC_Handler SVC_Handler
#define OS_PORT_PendSV_Handler PendSV_Handler
#define OS_PORT_SysTick_Handler SysTick_Handler

#define OS_YIELD_FROM_ISR() SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk

#define OS_IDLE()                                                              \
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI)

#endif // __PORT_H__