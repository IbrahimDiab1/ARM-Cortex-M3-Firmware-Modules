/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : Encoder_config.h                                                */
/* Description : Configuration for Motors Encoder  (HAL Layer)                   */
/*********************************************************************************/

#ifndef Encoder_CONFIG_H
#define Encoder_CONFIG_H

/* Specify the number of encoders used */
#define NUMBER_OF_ENCODERS_USED         2

/* Specify the polarity of the clockwise rotation direction for each motor */
/* Options:
 *              CW
 *              CW_INVERTED
 */
#define RIGHT_MOTOR_CW_INVERTED         CW
#define LEFT_MOTOR_CW_INVERTED          CW

/* Specify the number of encoder pulses per revolution */
#define PULSES_PER_REVOLUTION           60

/* Specify the circumference of the wheel in centimeters */
#define WHEEL_CIRCUMFERENCE_IN_CM       39

/* Specify the time interval in milliseconds to measure the motor speed */
#define SPEED_MEASUREMENT_INTERVAL_MS   100


/* Specify the pins and ports for each encoder channel */
#define ENCODER_0_A_PIN                 DIO_PIN0
#define ENCODER_0_B_PIN                 DIO_PIN1
#define ENCODER_0_A_PORT                DIO_GPIOA
#define ENCODER_0_B_PORT                DIO_GPIOA

#define ENCODER_1_A_PIN                 DIO_PIN2
#define ENCODER_1_B_PIN                 DIO_PIN3
#define ENCODER_1_A_PORT                DIO_GPIOA
#define ENCODER_1_B_PORT                DIO_GPIOA




#endif //Encoder_CONFIG_H
