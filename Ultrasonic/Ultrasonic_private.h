/*************************************************************************************/
/* Author      : Ibrahim Diab                                                        */
/* File Name   : Ultrasonic_private.h                                                */
/* Description : Private addresses for HC-SR04 Ultrasonic (HAL Layer)                */
/*************************************************************************************/

#ifndef ULTRASONIC_PRIVATE_H
#define ULTRASONIC_PRIVATE_H

// This struct defines an ultrasonic sensor object with its properties
typedef struct Ultrasonic_t
{
    uint8 State;          // State of the ultrasonic sensor

    uint8 Echo_pin;       // Echo pin number (DIO_PIN0 ... DIO_PIN4)
    uint8 Echo_port;      // Echo pin port (DIO_GPIOA, DIO_GPIOB)

    uint8 Trigger_pin;    // Trigger pin number
    uint8 Trigger_port;   // Trigger pin port

    uint32 DistanceRange; // Distance range of the ultrasonic sensor
    uint32 Timeout;       // Timeout value for the ultrasonic sensor

} Ultrasonic_t;

// These constants define the out of range and disable states for the ultrasonic sensor
#define OUT_OF_RANGE        -1
#define ULTRASONIC_DISABLE  -2

// These constants define the rising and falling edge states for the ultrasonic sensor
#define RISING_DETECTED     0
#define FALLING_DETECTED    1

// These constants define the reading states for the ultrasonic sensor
#define READING_PENDING     0
#define READING_DONE        1


// This function calculates the distance for the ultrasonic sensor based on the echo signal
void CalcDistance();

#endif // ULTRASONIC_PRIVATE_H
