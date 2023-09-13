/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : LM35.c                                                                     */
/* Description : Functions Implementation for EEPROM  (HAL Layer)                           */
/********************************************************************************************/

#include "std_types.h"
#include "common_macros.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "I2C_interface.h"

#include "EEPROM_interface.h"
#include "EEPROM_private.h"
#include "EEPROM_config.h"


// This function initializes the EEPROM using I2C protocol.
void EEPROM_init()
{
    // Define I2C configuration parameters.
    I2C_config_t I2C_config =
    {
            I2C_SM_MODE,            // I2C clock speed is set to standard mode.
            I2C_MODE,               // I2C mode is set to master mode.
            I2C_7_BIT_ADDRESS_MODE, // I2C address mode is set to 7-bit addressing.
            I2C_DISABLE,            // I2C general call is disabled.
            I2C_ENABLE,             // I2C software reset is enabled.
            I2C_DISABLE,            // I2C wakeup from STOP mode is disabled.
            STRETCH_ENABLED,        // Clock stretching is enabled.
            I2C_DISABLE,            // I2C DMA is disabled.
            I2C_DISABLE,            // I2C SMBus is disabled.
            100,                    // I2C peripheral clock speed is set to 100 kHz.
            0x30                    // I2C own address is set to 0x30.
    };

    // Enable clock for GPIOB and I2C1 peripherals.
    RCC_enableClk (RCC_APB2, RCC_GPIOB );
    RCC_enableClk (RCC_APB1, RCC_I2C1 );

    // Set pin directions for GPIOB pins 6 and 7 as output with 2 MHz speed and open drain.
    DIO_setPinDirection( DIO_GPIOB,  DIO_PIN6,  OUTPUT_SPEED_2MHZ_ALOD);
    DIO_setPinDirection( DIO_GPIOB,  DIO_PIN7,  OUTPUT_SPEED_2MHZ_ALOD);

    // Initialize I2C1 with the defined configuration parameters.
    I2C_init(I2C1 ,  &I2C_config );
}


// This function writes multiple bytes to the EEPROM memory.
void EEPROM_writeNBytes(uint32 memoryAddress, uint8 *bytesToSend ,uint8 dataLength)
{
    // Define a buffer to store the memory address and data to be written.
    uint8 buffer[BUFFER_SIZE]={0};

    // Store the memory address in the buffer.
    buffer[0]=(uint8)(memoryAddress>>8);
    buffer[1]=(uint8)memoryAddress;

    // Store the data to be written in the buffer.
    for(uint8 i=2;i<(dataLength+2);i++)
        buffer[i]=bytesToSend[i-2];

    // Send the buffer data to the EEPROM via I2C protocol with a start condition and stop condition.
    I2C_sendData(I2C1 ,EEPROM_ADDRESS, buffer ,  (dataLength+2), START , WITH_STOP);
}


// This function reads a byte from the EEPROM memory.
void EEPROM_read_byte(uint32 address, uint8 *byteToRead, uint8 dataLength)
{
    uint8 buffer[2];

    // Store the memory address to read from in the buffer
    buffer[0] = (uint8)(address >> 8);
    buffer[1] = (uint8)address;

    // Send the memory address to the EEPROM, followed by a repeated start signal
    // without sending a stop signal to keep the bus busy
    I2C_sendData(I2C1, EEPROM_ADDRESS, buffer, 2, START, WITHOUT_STOP);

    // Read the specified number of bytes from the EEPROM into the buffer
    I2C_receiveData(I2C1, EEPROM_ADDRESS, byteToRead, dataLength, REPEATED_START, WITH_STOP);
}

