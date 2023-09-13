/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : BatteryIndicator_config.h                                       */
/* Description : Configuration for Battery Indicator (HAL Layer)                 */
/*********************************************************************************/

#ifndef BATTERY_INDICATOR_CONFIG_H
#define BATTERY_INDICATOR_CONFIG_H

/*
    options:
                LINEAR     
                NON_LINEAR 
*/
#define BATTERY_DISCHARGE_CURVE NON_LINEAR // Choose the battery discharge curve option


/* 
 Note: TO calculate R1 & R2 values Consider that V(ADC input) doesn't exceed 3Volt
 VOLTAGE_DIVIDER_RATIO = [ V(source) / V(ADC input) ] as V(ADC input) = (R2 * V(source) ) / (R1 + R2)
 If there is no need for voltage divider then the value should equal 1 
*/
#define VOLTAGE_DIVIDER_RATIO 6


/*
    options :
    ADC_channel_0 .... ADC_channel_15
*/
ADC_Channel_Num BattInd_channel_ID = ADC_channel_1;



#if BATTERY_DISCHARGE_CURVE == NON_LINEAR
// Define the lookup table for the non-linear battery discharge curve
const float lookup_Table[10] = {11.4,11.6,11.8,11.9,12,12.1,12.2,12.4,12.6,12.7};

#elif BATTERY_DISCHARGE_CURVE == LINEAR 
// Define the voltage values for a linear battery discharge curve
#define VOLTAGE_FULLY_CHARGED     12.6
#define VOLTAGE_FULLY_DISCHARGED  11.3

#endif




#endif // BATTERY_INDICATOR_CONFIG_H
