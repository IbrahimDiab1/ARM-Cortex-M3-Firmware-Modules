/*******************************************************************/
/* Author      : Ibrahim Diab                                      */
/* File Name   : Button_config.h                                   */
/* Description : Configuration & setting for Button Module         */
/*******************************************************************/

#ifndef _Button_CONFIG_H_
#define _Button_CONFIG_H_

/*
 * Button Driver
 * --------------
 * This driver provides functions to interact with buttons on the STM32F103 micro-controller.
 *
 * Configuration:
 * - If RTOS is supported, define USE_RTOS to 1 to use RTOS Tick counter Function during Button State Updating handling.
 * - If RTOS is not supported:
 *       Initialize a timer to work as a tick counter using GPT_initTickTimer Function.
 * - In case using EXTI for at least one pin:
 *       you should change the define AT_LEAST_ONE_BUTTON_USE_EXTI value to 1.
 *       Only pins from 0 to 4 (supported in stm32f10x) for EXTI interrupt.
 *       You need to use external hardware circuit for button de-bouncing.
 *       The ISR doesn't change Button_Pressed_Read_Once to Read_Once until you read it at least one time using Button_readState function.
 *
 */

#define MAX_BUTTONS                     (5)     // Number of Max Buttons used in the project
    
#define BUTTON_DEBOUNCING_DELAY         (15)    // In millisecond

#define BUTTON_LONG_PRESS_TIME          (1250)  // In millisecond

#define FREE_RTOS_IN_USE                (0)     // 0,1 to declare if RTOS is not used then use The Tick Timer to get ticks

#define AT_LEAST_ONE_BUTTON_USE_EXTI    (0)     // 0,1 this define only increase performance and decrease memory usage in case no buttons use EXTI module.


#endif // _Button_CONFIG_H_
