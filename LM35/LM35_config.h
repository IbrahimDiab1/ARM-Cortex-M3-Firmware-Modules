/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : LM35_config.h                                                   */
/* Description : Configuration for LM35 temperature sensor (HAL Layer)           */
/*********************************************************************************/

#ifndef LM35_CONFIG_H
#define LM35_CONFIG_H

/*
 options :
            LM35_BASIC      :    Measure range   0c >> MAX VALUE (150c)     
            LM35_FULL_RANGE :    Measure range -55c >> MAX VALUE (150c)   Note : It needs external hardware circuit.
*/
#define MEASURE_SETUP    LM35_BASIC


/*
    Depending on LM35 version specification.
*/
#define LM35_MAX_TEMPERATURE    150     // 150c
#define SENSOR_MAX_VOLT_VALUE   1.5     // 1.5V

/*
    options :
    ADC_CHANNEL_0 .... ADC_CHANNEL_15
*/
ADC_Channel_Num LM35_channel_ID = ADC_channel_0;


/*
 The variable will be used to specify which ADC conversion to perform in STM32F103C6 microcontroller.
 
    options:
    Conversion_1 .... Conversion_16
    Depends on channel sequence.
*/
//Convertion_Number LM35_convertion_NUM = Conversion_1;

/*  channels representation on its ports and pins:

ADC1_IN0 : PA0
ADC1_IN1 : PA1
ADC1_IN2 : PA2
ADC1_IN3 : PA3
ADC1_IN4 : PA4
ADC1_IN5 : PA5
ADC1_IN6 : PA6
ADC1_IN7 : PA7
ADC1_IN8 : PB0
ADC1_IN9 : PB1
ADC1_IN10: PC0
ADC1_IN11: PC1
ADC1_IN12: PC2
ADC1_IN13: PC3
ADC1_IN14: PC4
ADC1_IN15: PC5
*/

#endif //LM35_CONFIG_H
