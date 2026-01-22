/*
 * FreeRTOS Kernel V10.2.1
 * Portion Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Portion Copyright (C) 2019 StMicroelectronics, Inc.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_BOARD_H
#define FREERTOS_CONFIG_BOARD_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
 #include <stdint.h>
 extern uint32_t SystemCoreClock;
#endif

/*-------------------- specific defines -------------------*/
#ifndef CMSIS_device_header
#define CMSIS_device_header "stm32h7xx.h"
#endif /* CMSIS_device_header */

/* No secure feature is used the configENABLE_TRUSTZONE should be set to 0 */
#define configENABLE_TRUSTZONE							0
#define configENABLE_FPU								1
#define configENABLE_MPU								0

#ifdef __NVIC_PRIO_BITS
 #define configPRIO_BITS								__NVIC_PRIO_BITS
#else
 #define configPRIO_BITS								4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			((1U << (configPRIO_BITS)) - 1)

 /* The highest interrupt priority that can be used by any interrupt service
 routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
 INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
 PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

 /* Interrupt priorities used by the kernel port layer itself.  These are generic
 to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY					( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY			( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*
* CMSISRTOS-v2 requires:
* - FreeRTOS static allocation features (configSUPPORT_STATIC_ALLOCATION == 1)
* - Max proiorities set to 56           (configMAX_PRIORITIES == 56)
* - disable the optimized task selection(configUSE_PORT_OPTIMISED_TASK_SELECTION == 0)
*/
#define configSUPPORT_STATIC_ALLOCATION					0
#define configMAX_PRIORITIES							56
#define configUSE_PORT_OPTIMISED_TASK_SELECTION			0

/*-----------------------------------------------------------------*/

#define configCPU_CLOCK_HZ								( SystemCoreClock )

/*-----------------------------------------------------------------*/

#define vPortSVCHandler									SVC_Handler
#define xPortPendSVHandler								PendSV_Handler
#define xPortSysTickHandler								SysTick_Handler

 /*-----------------------------------------------------------------*/

 /* Normal assert() semantics without relying on the provision of an assert.h header file. */
 #define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }


#endif /* FREERTOS_CONFIG_BOARD_H */

