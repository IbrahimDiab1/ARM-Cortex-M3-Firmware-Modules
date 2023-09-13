/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : STP_config.h                                                    */
/* Description : Configuration for Serial to parallel converter (HAL Layer)      */
/*********************************************************************************/


#ifndef STP_CONFIG_H
#define STP_CONFIG_H


#define NO_OUTPUT_PINS 8

#define STP_DATA        DIO_GPIOA,DIO_PIN0
#define STP_SHIFT_CLK   DIO_GPIOA,DIO_PIN1
#define STP_STORE_CLK   DIO_GPIOA,DIO_PIN2


#endif // STP_CONFIG_H
