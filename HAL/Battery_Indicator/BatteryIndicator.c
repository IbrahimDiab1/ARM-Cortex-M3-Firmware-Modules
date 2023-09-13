/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : BatteryIndicator.c                                                         */
/* Description : Functions Implementation for Battery Indicator (HAL Layer)                 */
/********************************************************************************************/

/* Library includes */
#include "Std_Types.h"
#include "Common_Macros.h"

/* MCU constant parameters includes */
#include "STM32F103C6.h"

/* Peripherals includes */
#include "RCC_interface.h"
#include "DIO_interface.h"
#include "ADC_interface.h"

/* Module files includes */
#include "BatteryIndicator_private.h"
#include "BatteryIndicator_config.h"
#include "BatteryIndicator_interface.h"


/* This function initializes the battery indicator module */
void BattInd_init()
{
        // check if BattInd_channel_ID is within valid range for GPIOA
    if(BattInd_channel_ID <= ADC_channel_7 && BattInd_channel_ID >= ADC_channel_0)
    {
        // enable clock for GPIOA
        RCC_enableClk (RCC_APB2, RCC_GPIOA);
        
        // set the direction of the corresponding GPIO pin to input analog mode
        DIO_setPinDirection(DIO_GPIOA, BattInd_channel_ID, INPUT_ANALOG);
    }
    
    // check if BattInd_channel_ID is within valid range for GPIOB
    else if(BattInd_channel_ID <= ADC_channel_9)
    {
        // enable clock for GPIOB
        RCC_enableClk (RCC_APB2, RCC_GPIOB);

        // set the direction of the corresponding GPIO pin to input analog mode
        DIO_setPinDirection(DIO_GPIOB, BattInd_channel_ID, INPUT_ANALOG);
    }
    
    // check if BattInd_channel_ID is within valid range for GPIOC
    else if(BattInd_channel_ID <= ADC_channel_15)
    {
        // enable clock for GPIOC
        RCC_enableClk (RCC_APB2, RCC_GPIOC);

        // set the direction of the corresponding GPIO pin to input analog mode
        DIO_setPinDirection(DIO_GPIOC, BattInd_channel_ID, INPUT_ANALOG);
    }
    
    // if BattInd_channel_ID is out of range, do nothing or return error
    else 
    {
        // do nothing
    }
    
    // enable clock for ADC1
    RCC_enableClk (RCC_APB2, RCC_ADC1);
    
    // initialize ADC1
    ADC_CONFIG_t ADC_Config =
    {
            ADC_Mode_Independent,
            ADC_Sampling_Time_41_5_Cycle,
            ADC_Align_Right,
            ADC_Conversion_Mode_Single,
            ADC_Channel_Mode_Regular
    };

   ADC_init(ADC1,&ADC_Config);

}
 
 
#if BATTERY_DISCHARGE_CURVE == NON_LINEAR

/* Get the battery capacity level based on lookup table */
uint8 BattInd_getBatteryCapacityLevel()
{
    // Read battery voltage from ADC
    float batteryLevel = (float)ADC_readChannel(ADC1, BattInd_channel_ID);

    
    // Convert ADC value to actual voltage
    batteryLevel = ((((batteryLevel / ADC_MAX_VALUE) * ADC_REF_VOLT_VALUE) * VOLTAGE_DIVIDER_RATIO));
    
    // Check which level the battery voltage falls into and return it
    for(uint8 level = 9; level > 0; level--)
    {
        if(batteryLevel >= lookup_Table[level])
            return (level+1);
    }
    
    // If battery voltage is below the lowest level, return 0
    return 0;
}

#elif BATTERY_DISCHARGE_CURVE == LINEAR 

/*Get the battery capacity percentage based on linear curve */
uint8 BattInd_getBatteryCapacityPercentage()
{
    // Read battery voltage from ADC
    float batteryPercentage = (float)ADC_readChannel(ADC1, BattInd_channel_ID);
    
    // Convert ADC value to actual voltage
    batteryPercentage = ((((batteryPercentage / ADC_MAX_VALUE) * ADC_REF_VOLT_VALUE) * VOLTAGE_DIVIDER_RATIO));
    
    // Calculate battery capacity percentage based on voltage range
    batteryPercentage = (((batteryPercentage - VOLTAGE_FULLY_DISCHARGED)*100) / ((float)VOLTAGE_FULLY_CHARGED - VOLTAGE_FULLY_DISCHARGED));
    
    // Return the battery capacity percentage
    if(batteryPercentage >100)
        return 100;
    if(batteryPercentage <0 )
        return 0;

    return batteryPercentage;
}

#endif
