/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : STP_program.c                                                              */
/* Description : Functions Implementation for Serial to parallel converter (HAL Layer)      */
/********************************************************************************************/

#include "std_types.h"
#include "common_macros.h"

#include "DIO_interface.h"
#include "STK_interface.h"

#include "STP_interface.h"
#include "STP_private.h"
#include "STP_config.h"


/* Function to send data to serial to parallel converter */
void STP_SendData(uint32 STP_DataToSend)
{
    /* Declare a variable to store the value of each bit */
    uint8 bitValue;
    /* Loop through each bit of the data to send */
for(int8 indexBit = (NO_OUTPUT_PINS-1) ; indexBit >= 0 ; indexBit--)
{
    /* Get the value of the current bit */
    bitValue = GET_BIT(STP_DataToSend,indexBit);
    
    /* Set the value of the data pin to the current bit value */
    DIO_setPinValue(STP_DATA,bitValue);
    
    /* Pulse the shift clock pin to shift the data into the shift register */
    DIO_setPinValue(STP_SHIFT_CLK,HIGH);
    STK_delayMicroSec(5); /* Wait for 5 microseconds */
    DIO_setPinValue(STP_SHIFT_CLK,LOW);
    STK_delayMicroSec(5); /* Wait for 5 microseconds */
}

    /* Pulse the store clock pin to latch the data in the output pins */
    DIO_setPinValue(STP_STORE_CLK,HIGH);
    STK_delayMicroSec(5); /* Wait for 5 microseconds */
    DIO_setPinValue(STP_STORE_CLK,LOW);
    STK_delayMicroSec(5); /* Wait for 5 microseconds */

}
