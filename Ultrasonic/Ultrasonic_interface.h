/**************************************************************************************/
/* Author      : Ibrahim Diab                                                         */
/* File Name   : Ultrasonic_interface.h                                               */
/* Description : Interfacing macros for HC-SR04 Ultrasonic (HAL Layer)                */
/**************************************************************************************/

#ifndef ULTRASONIC_INTERFACE_H
#define ULTRASONIC_INTERFACE_H

/*
 * Enum for the ID of the ultrasonic sensors.
 * The three sensors are named Right, Left, and Middle.
 */
typedef enum Ultrasonic_ID_t
{
    Right,
    Left,
    Middle
}Ultrasonic_ID_t;

/*
 * Enum for the state of the ultrasonic sensors.
 * The sensors can be either enabled or disabled.
 * In case of disabled, User can't read distance.
 */
typedef enum Ultrasonic_state_t
{
    Ultrasonic_enable,
    Ultrasonic_disable
}Ultrasonic_state_t;



/* This structure defines the configuration parameters for an ultrasonic sensor */
typedef struct Ultrasonic_config_t
{
    uint16 DistanceRange;       // The maximum distance range that the sensor can measure, then send OUT_OF_RANGE(-1) value.
    Ultrasonic_state_t State;   // The current state of the sensor (enabled or disabled)

    uint8 Echo_pin;             // The input pin used to receive the echo signal from the sensor (DIO_PIN0 ... DIO_PIN4)
    uint8 Echo_port;            // The GPIO port used to receive the echo signal (DIO_GPIOA or DIO_GPIOB)

    uint8 Trigger_pin;          // The output pin used to send the trigger signal to the sensor
    uint8 Trigger_port;         // The GPIO port used to send the trigger signal

}Ultrasonic_config_t;


/* Initializes and configures the required modules for the Ultrasonic interface, including RCC, DIO, AFIO, EXTI, STK, and NVIC. */
void Ultrasonic_init(Ultrasonic_ID_t ID, Ultrasonic_config_t * Ultrasonic_config);

/* This function reads the distance measured by an ultrasonic sensor and return the value in Centimeter */
float32 Ultrasonic_readDistance(Ultrasonic_ID_t Ultrasonic_ID);

/* This function sets the range and timeout values for an ultrasonic sensor which increase performance. */
void Ultrasonic_setRange(Ultrasonic_ID_t Ultrasonic_ID, uint16 Range_cm);

// This function sets the state for an ultrasonic sensor
// In case of the state is Ultrasonic_disable, the reading will not execute for the this sensor.
void Ultrasonic_setState(Ultrasonic_ID_t Ultrasonic_ID, Ultrasonic_state_t state);

#endif //ULTRASONIC_INTERFACE_H
