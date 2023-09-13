/**************************************************************************************/
/* Author     : Ibrahim Diab                                                          */
/* File Name  : Encoder_interface.h                                                   */
/* Description: Interfacing macros for Motors Encoder  (HAL Layer)                    */
/**************************************************************************************/


#ifndef ENCODER_INTERFACE_H
#define ENCODER_INTERFACE_H

// Defines the possible encoders that can be used.
typedef enum Encoders_ID_t
{
    RIGHT , // Right encoder
    LEFT  , // Left encoder
    ALL // Average of all encoders
}Encoders_ID_t;


// Defines the return format for the encoder functions.
typedef enum Encoders_Retuen_Format_t
{
    Counts ,        // Encoder counts
    Degrees ,       // Angle of rotations in degrees
    Centimeters ,   // Distance traveled in centimeters
    Meters          // Distance traveled in meters
}Encoders_Retuen_Format_t;

// Define an enumeration for the possible encoder/motor directions
typedef enum Encoders_Motors_Direction_t
{
    DIR_CW,        // Clockwise direction
    DIR_CCW,       // Counter-clockwise direction
    DIR_RIGHT,     // Right direction
    DIR_LEFT,      // Left direction
    DIR_FORWARD,   // Forward direction
    DIR_BACKWARD   // Backward direction
} Encoders_Motors_Direction_t;


// Initializes and configures the required modules for the encoder interface, including RCC, DIO, AFIO, EXTI, STK, and NVIC.
void Encoder_init (Encoders_ID_t Encoder_ID);

/* Resets All or the specified encoder's count to zero. */
void Encoder_reset(Encoders_ID_t Encoder_ID);

// This function returns the average or the current count of the specified encoder according to Encoder_ID value
int32 Encoder_getCounts(Encoders_ID_t Encoder_ID);

/* Calculates and returns the converted value of encoders average or a specified encoder in a specified format. */
float32 Encoder_getConvertCounts(Encoders_ID_t Encoder_ID, Encoders_Retuen_Format_t Encoders_Retuen_Format);

/*
 * This function returns the average or current speed of the specified encoder in the specified format.
 * Parameters:
 * Encoder_ID: The ID of the encoder (LEFT or RIGHT).
 * Encoders_Retuen_Format: The format in which the speed is returned (Counts, Degrees, Centimeters or Meters).
 * Return value:
 * The speed of the specified encoder in the specified format.
 */
float32 Encoder_getSpeed(Encoders_ID_t Encoder_ID, Encoders_Retuen_Format_t Encoders_Retuen_Format);

// Function to get the direction of an Encoder/Vehicle
Encoders_Motors_Direction_t Encoder_getDirection(Encoders_ID_t Encoder_ID);


#endif //ENCODER_INTERFACE_H

