#ifndef __PORT_H__
#define __PORT_H__
#include "os_types.h"
#include "stm32f7xx.h"

// SysTick频率，非系统时钟频率
#define OS_SYSTICKFREQ 1000 // SysTick频率为1000Hz

#define OS_ENABLE_INTERRUPTS() __asm volatile("cpsie i" : : : "memory")
#define OS_DISABLE_INTERRUPTS() __asm volatile("cpsid i" : : : "memory")

#define OS_PORT_SVC_Handler SVC_Handler
#define OS_PORT_PendSV_Handler PendSV_Handler
#define OS_PORT_SysTick_Handler SysTick_Handler

#define OS_YIELD_FROM_ISR() SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk

#define OS_IDLE() HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI)

void os_nvic_init(void);
__attribute__((naked)) void first_to_psp(os_uint32_t *psp);

void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
#endif // __PORT_H__