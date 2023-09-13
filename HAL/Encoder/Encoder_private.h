/*************************************************************************************/
/* Author     : Ibrahim Diab                                                         */
/* File Name  : Encoder_private.h                                                    */
/* Description: Private addresses for Motors Encoder  (HAL Layer)                    */
/*************************************************************************************/

#ifndef Encoder_PRIVATE_H
#define Encoder_PRIVATE_H

#define CW                  0
#define CW_INVERTED         1

#define CH_A                0
#define CH_B                1

#define RISING_CH_A         0
#define RISING_CH_B         1
#define FALLING_CH_A        2
#define FALLING_CH_B        3

// Update the encoder count and direction based on the input signal state
void updateEncoderData(Encoders_ID_t Encoder_ID);

/*  Updates the speed of each motor based on the change in encoder counts. */
void updateSpeed();

// Function to update the encoder data for Encoder 0 channel A
void Encoder_0_update_CH_A();

// Function to update the Encoder 0 channel B state
void Encoder_0_update_CH_B();

// Function to update the encoder data for Encoder 1 channel A
void Encoder_1_update_CH_A();

// Function to update the Encoder 1 channel B state
void Encoder_1_update_CH_B();

#endif // Encoder_PRIVATE_H
