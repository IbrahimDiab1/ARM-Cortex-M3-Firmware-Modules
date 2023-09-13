/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : LEDMAT_config.h                                                 */
/* Description : Configuration for LED-Matrix (HAL Layer)                        */
/*********************************************************************************/


#ifndef LEDMAT_CONFIG_H
#define LEDMAT_CONFIG_H


#define NO_ROWS      8
#define NO_COLUMN    8

#define COMMON_CATHODE_ANODE  CATHODE

#define LEDMAT_ROW_0        DIO_GPIOA,DIO_PIN0
#define LEDMAT_ROW_1        DIO_GPIOA,DIO_PIN1
#define LEDMAT_ROW_2        DIO_GPIOA,DIO_PIN2
#define LEDMAT_ROW_3        DIO_GPIOA,DIO_PIN3
#define LEDMAT_ROW_4        DIO_GPIOA,DIO_PIN4
#define LEDMAT_ROW_5        DIO_GPIOA,DIO_PIN5
#define LEDMAT_ROW_6        DIO_GPIOA,DIO_PIN6
#define LEDMAT_ROW_7        DIO_GPIOA,DIO_PIN7

#define LEDMAT_COL_0        DIO_GPIOB,DIO_PIN0
#define LEDMAT_COL_1        DIO_GPIOB,DIO_PIN1
#define LEDMAT_COL_2        DIO_GPIOB,DIO_PIN5
#define LEDMAT_COL_3        DIO_GPIOB,DIO_PIN6
#define LEDMAT_COL_4        DIO_GPIOB,DIO_PIN7
#define LEDMAT_COL_5        DIO_GPIOB,DIO_PIN8
#define LEDMAT_COL_6        DIO_GPIOB,DIO_PIN9
#define LEDMAT_COL_7        DIO_GPIOB,DIO_PIN10





#endif // LEDMAT_CONFIG_H
