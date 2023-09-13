/**************************************************************************************/
/* Author     : Ibrahim Diab                                                          */
/* File Name  : LM35_interface.h                                                      */
/* Description: Interfacing macros for EEPROM (HAL Layer)                             */
/**************************************************************************************/

#ifndef EEPROM_INTERFACE_H
#define EEPROM_INTERFACE_H

/*
   Initialize the EEPROM module by configuring the I2C communication and GPIO pins
   This function initializes the I2C1 peripheral with the specified configuration parameters
   and configures the GPIO pins for I2C communication. It also enables the RCC clock for the
   GPIOB and I2C1 peripherals.
*/
void EEPROM_init();


/*
  Write the specified number of bytes to the EEPROM memory starting from the specified memory address
  Parameters:
   - memoryAddress: the starting memory address to write the bytes to
   - bytesToSend: a pointer to the byte array that contains the data to write to the EEPROM memory
   - dataLength: the number of bytes to write to the EEPROM memory
*/
void EEPROM_writeNBytes(uint32 memoryAddress, uint8 *bytesToSend, uint8 dataLength);

/*
  Read the specified number of bytes from the EEPROM memory starting from the specified memory address
  Parameters:
  - address: the starting memory address to read the bytes from
  - byteToRead: a pointer to the variable that will hold the byte read from the EEPROM memory
  - dataLength: the number of bytes to read from the EEPROM memory
 */
void EEPROM_read_byte(uint32 address, uint8 *byteToRead, uint8 dataLength);



#endif //EEPROM_INTERFACE_H
