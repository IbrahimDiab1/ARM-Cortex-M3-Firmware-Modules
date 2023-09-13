/**************************************************************************************/
/* Author     : Ibrahim Diab                                                          */
/* File Name  : LM35_interface.h                                                      */
/* Description: Interfacing macros for LM35 temperature sensor (HAL Layer)            */
/**************************************************************************************/

#ifndef LM35_INTERFACE_H
#define LM35_INTERFACE_H


/*
 * Function: LM35_init
 * -------------------
 * Enabling required RCC buses and peripherals.
 * Initializes the GPIO module.
 * Initializes the LM35 temperature sensor by configuring the ADC pin and enabling the ADC module.
 * This function should be called before using the LM35_getTemperature function.
 */
void LM35_init();


#if MEASURE_SETUP == LM35_BASIC
/*
 * Function: LM35_getTemperature
 * ----------------------------
 * Reads the temperature from an LM35 temperature sensor connected to an ADC pin
 * Returns: uint8 value representing the positive range temperature in degrees Celsius (Â°C)
 */
uint8 LM35_getTemperature();


#elif MEASURE_SETUP == LM35_FULL_RANGE
/*
 * Function: LM35_getTemperature
 * ----------------------------
 * Reads the temperature from an LM35 temperature sensor connected to an ADC pin
 * Returns: int8 value representing the full range temperature in degrees Celsius (Â°C)
 */
int8 LM35_getTemperature();
#endif



/**             *** Driver Description ***

This is a driver for LM35 temperature sensor on STM32F103C6 microcontroller.

Initialization:
Call LM35_init() to initialize the necessary peripheral(GPIO,ADC,RCC) Clocks and pins.

Configuration:
Define the MEASURE_SETUP macro to choose the desired temperature range (LM35_BASIC or LM35_FULL_RANGE).
Set the LM35_channel_ID and LM35_convertion_NUM variables to the desired values.

API:
The driver provides a single API function, LM35_getTemperature(), which returns the temperature value in degrees Celsius.
The function reads the voltage output of the LM35 sensor using an ADC,
converts the ADC reading to a temperature value using the formula provided in the code, and returns the temperature value as an uint8 type.

Error handling:
The driver does not provide any error handling functionality. However, if the LM35_channel_ID option is set to an invalid value,
the driver will not perform any initialization and will simply return without an error message.

Limitations:
The driver assumes that the LM35 sensor is linearly proportional to temperature and correctly calibrated. 
The maximum operating temperature for the LM35 sensor is 150 degrees Celsius, as defined by the LM35_MAX_TEMPERATURE macro. 
For LM35_FULL_RANGE mode, an external hardware circuit is needed.

Examples:
Here is an example code snippet that demonstrates how to use the LM35 driver to read the temperature value from the LM35 sensor:
>
> //initialize the LM35 driver
> LM35_init();
>
> //read the temperature value
> uint8 temperature = LM35_getTemperature();

Dependencies:
An STM32F103C6 microcontroller
An analog to digital converter (ADC) module
GPIO pins configured for input analog mode
RCC driver for enabling the clock of the GPIO and ADC modules

*/


#endif //LM35_INTERFACE_H
