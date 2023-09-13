/**************************************************************************************/
/* Author     : Ibrahim Diab                                                          */
/* File Name  : BatteryIndicator_interface.h                                          */
/* Description: Interfacing macros for  Battery Indicator (HAL Layer)                 */
/**************************************************************************************/

#ifndef BATTERY_INDICATOR_INTERFACE_H
#define BATTERY_INDICATOR_INTERFACE_H

/*
This function initializes the battery indicator module.
It sets up the ADC for measuring the battery voltage, configures GPIO pins
and initializes any other necessary variables or settings. 
*/
void BattInd_init();
 
 
#if BATTERY_DISCHARGE_CURVE == NON_LINEAR
/*
  This function calculates the capacity level of the battery based on the lookup table of voltage values 
  for different capacity levels, obtained from the battery discharge curve.
  It reads the ADC value of the battery voltage, converts it to voltage value, and compares it to the
  voltage values in the lookup table to determine the battery capacity level.
  Returns the capacity level (1 to 10), or 0 if the battery voltage is below the minimum lookup value.
 */
uint8 BattInd_getBatteryCapacityLevel();



#elif BATTERY_DISCHARGE_CURVE == LINEAR 
/*
  This function calculates the percentage of remaining capacity of the battery based on the linear
  discharge curve of the battery, assuming that the battery voltage decreases linearly with capacity level.
  It reads the ADC value of the battery voltage, converts it to voltage value, and calculates the
  percentage of remaining capacity based on the difference between the battery voltage and the minimum
  and maximum voltages of the discharge curve.
  Returns the remaining capacity percentage (0 to 100).
 */
uint8 BattInd_getBatteryCapacityPercentage();

#endif


/**             *** Driver Description ***

This driver is used for a battery indicator module. To use it, follow these steps:

Initialization:
Call the BattInd_init() function to initialize the driver and the necessary peripheral clocks and pins.
You can also set the channel and conversion number using the BattInd_channel_ID and Batt_convertion_NUM variables, respectively.

Configuration:
Set the battery discharge curve option using the BATTERY_DISCHARGE_CURVE macro. You can choose between LINEAR and NON_LINEAR.
If using the NON_LINEAR option, you can define a lookup table for the battery discharge curve by setting the lookup_Table array.
If using the LINEAR option, you can set the voltage values for a linear battery discharge curve by defining the VOLTAGE_FULLY_CHARGED 
and VOLTAGE_FULLY_DISCHARGED macros.
You can also set the voltage divider ratio using the VOLTAGE_DIVIDER_RATIO macro.

API:
There are two functions that can be used by the user:
In case of NON_LINEAR: BattInd_getBatteryCapacityLevel()      - returns the battery capacity level based on the lookup table.
In case of LINEAR    : BattInd_getBatteryCapacityPercentage() - returns the battery capacity percentage based on the battery discharge curve.

Error Handling:
The driver does not return any error codes or messages explicitly. However, there are some error cases that can occur:

If the specified channel ID is out of the valid range for GPIOA, GPIOB, or GPIOC, the driver will not set the direction of the corresponding GPIO pin to input analog mode.
If the specified channel ID is out of the valid range for ADC_CHANNEL_0 to ADC_CHANNEL_15, the driver will not set the channel for regular group conversion and will not set the conversion sequence.

Limitations:
The maximum value of the ADC input voltage is limited to 3 volts.
The voltage divider ratio should be specified correctly to avoid exceeding the maximum voltage input for the ADC.
The driver can only be used with STM32F103C6 microcontroller.

Examples: 
1- for NON_LINEAR :
>// Set the battery indicator channel ID and conversion number
>ADC_channel_t BattInd_channel_ID = ADC_CHANNEL_1;
>Convertion_Number Batt_convertion_NUM = Conversion_2;
>
>// Initialize the battery indicator driver
>BattInd_init();
>
>// Get the battery capacity level based on the non-linear curve
>uint8_t batteryCapacityLevel = BattInd_getBatteryCapacityLevel();

2-for LINEAR :
>// Set the battery indicator channel ID and conversion number
>ADC_channel_t BattInd_channel_ID = ADC_CHANNEL_1;
>Convertion_Number Batt_convertion_NUM = Conversion_2;
>
>// Initialize the battery indicator driver
>BattInd_init();
>
>// Get the battery capacity percentage based on the linear curve
>uint8_t batteryCapacityPercentage = BattInd_getBatteryCapacityPercentage();

Dependencies:
An STM32F103C6 microcontroller
An analog to digital converter (ADC) module
GPIO pins configured for input analog mode
RCC driver for enabling the clock of the GPIO and ADC modules
*/



#endif // BATTERY_INDICATOR_INTERFACE_H
