/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : TFT_config.h                                                    */
/* Description : Configuration for TFT Display (HAL Layer)                       */
/*********************************************************************************/


#ifndef TFT_CONFIG_H
#define TFT_CONFIG_H


/**
Requirements:

- Select the required configuration settings from the display configuration file before initializing the TFT driver.
- Configure the appropriate SPI peripheral for communication with the TFT display.
- Make sure the GPIO pins for SPI communication are configured correctly.
- Initialize STK before TFT initialization.

SPI Configuration:

- Ensure SPI clock polarity (SCK) is set to SPI_IDLE_HIGH.
- Set SPI clock phase (CPHA) to SPI_FAILING_EDGE.
- Choose SPI frame format to be SPI_MSB_FIRST.

Note: TFT displays typically support a maximum communication frequency of 15 MHz, so adjust the SPI prescaler accordingly to meet this requirement.
*/


// Options: TFT_RGB444 (Not supported), TFT_RGB565, TFT_RGB666 (Not supported)
#define TFT_COLOR_STANDARD      TFT_RGB565

// Options: SPI_1 , SPI_2 (Not supported)
#define SPI_USED                SPI_1

#define TFT_A0_PORT             DIO_GPIOA
#define TFT_A0_PIN              DIO_PIN2

#define TFT_RESET_PORT          DIO_GPIOA
#define TFT_RESET_PIN           DIO_PIN3


#endif // TFT_CONFIG_H
