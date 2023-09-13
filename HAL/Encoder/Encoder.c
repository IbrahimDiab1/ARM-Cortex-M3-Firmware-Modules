/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : Encoder.c                                                                  */
/* Description : Functions Implementation for Motors Encoder   (HAL Layer)                  */
/********************************************************************************************/

#include "std_types.h"
#include "common_macros.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "AFIO_interface.h"
#include "EXTI_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"

#include "Encoder_interface.h"
#include "Encoder_private.h"
#include "Encoder_config.h"

static volatile int32   Encoder_Counts  [NUMBER_OF_ENCODERS_USED];  // Array to hold the encoder counts for each encoder
static volatile float32 Motors_Speed    [NUMBER_OF_ENCODERS_USED];  // Array to hold the motor speed for each encoder
static volatile int8    Encoder_CH_State[NUMBER_OF_ENCODERS_USED];  // Array to hold the state of each encoder channel[HIGH-LOW]

// Array to hold the next interrupt state for channel B for each encoder[RISING-FALLING]
static volatile int8    Encoder_CH_B_Next_Interrupt_State     [NUMBER_OF_ENCODERS_USED]={RISING};

// Array to hold the direction of each encoder if Clockwise or counter Clockwise]
static volatile Encoders_Motors_Direction_t  Encoder_Direction[NUMBER_OF_ENCODERS_USED];


// Initializes and configures the required modules for the encoder interface, including RCC, DIO, AFIO, EXTI, STK, and NVIC.
void Encoder_init(Encoders_ID_t Encoder_ID) {
    // Enable clock for encoder ports
    RCC_enableClk(RCC_APB2, ENCODER_0_A_PORT + RCC_GPIOA);
    RCC_enableClk(RCC_APB2, ENCODER_0_B_PORT + RCC_GPIOA);
    RCC_enableClk(RCC_APB2, ENCODER_1_A_PORT + RCC_GPIOA);
    RCC_enableClk(RCC_APB2, ENCODER_1_B_PORT + RCC_GPIOA);

    // Enable AFIO clock
    RCC_enableClk(RCC_APB2, RCC_AFIO);

    // Set pin directions to input floating
    DIO_setPinDirection(ENCODER_0_A_PORT, ENCODER_0_A_PIN, INPUT_FLOATING);
    DIO_setPinDirection(ENCODER_0_B_PORT, ENCODER_0_B_PIN, INPUT_FLOATING);
    DIO_setPinDirection(ENCODER_1_A_PORT, ENCODER_1_A_PIN, INPUT_FLOATING);
    DIO_setPinDirection(ENCODER_1_B_PORT, ENCODER_1_B_PIN, INPUT_FLOATING);

    // Set EXTI configuration
    AFIO_setEXTIConfig(ENCODER_0_A_PIN, ENCODER_0_A_PORT);
    AFIO_setEXTIConfig(ENCODER_0_B_PIN, ENCODER_0_B_PORT);
    AFIO_setEXTIConfig(ENCODER_1_A_PIN, ENCODER_1_A_PORT);
    AFIO_setEXTIConfig(ENCODER_1_B_PIN, ENCODER_1_B_PORT);

    // Enable EXTI interrupts and their corresponding NVIC
    NVIC_enableInterrupt(NVIC_EXTI0);
    NVIC_enableInterrupt(NVIC_EXTI1);
    NVIC_enableInterrupt(NVIC_EXTI2);
    NVIC_enableInterrupt(NVIC_EXTI3);
    NVIC_enableInterrupt(NVIC_EXTI4);

    // Set EXTI callback
    EXTI_setCallBack(ENCODER_0_A_PIN, Encoder_0_update_CH_A);
    EXTI_setCallBack(ENCODER_0_B_PIN, Encoder_0_update_CH_B);
    EXTI_setCallBack(ENCODER_1_A_PIN, Encoder_1_update_CH_A);
    EXTI_setCallBack(ENCODER_1_B_PIN, Encoder_1_update_CH_B);

    // Enable EXTI interrupts
    EXTI_enableEXTI(ENCODER_0_A_PIN, RISING);
    EXTI_enableEXTI(ENCODER_0_B_PIN, CHANGE);
    EXTI_enableEXTI(ENCODER_1_A_PIN, RISING);
    EXTI_enableEXTI(ENCODER_1_B_PIN, CHANGE);

    // Initialize and configure STK
    STK_init();
    STK_setIntervalPeriodic((SPEED_MEASUREMENT_INTERVAL_MS * 1000), updateSpeed);
}


// This function resets the count of the specified encoder to zero
void Encoder_reset(Encoders_ID_t Encoder_ID)
{
    if (Encoder_ID == ALL) {
        // if the encoder ID is set to ALL, then reset the counts for all the encoders
        for (uint8 index = 0; index < NUMBER_OF_ENCODERS_USED; index++) {
            Encoder_Counts[index] = 0;
        }
    } else {
        // reset the count of the specified encoder
        Encoder_Counts[Encoder_ID] = 0;
    }
}


// This function returns the average or the current count of the specified encoder according to Encoder_ID value
int32 Encoder_getCounts(Encoders_ID_t Encoder_ID)
{
    if (Encoder_ID == ALL) {
        // if the encoder ID is set to ALL, then calculate the average of the counts of all the encoders
        int32 averageCounts = 0;
        for (uint8 index = 0; index < NUMBER_OF_ENCODERS_USED; index++) {
            averageCounts += Encoder_Counts[index];
        }
        return (averageCounts / NUMBER_OF_ENCODERS_USED);
    } else {
        // return the count of the specified encoder
        return Encoder_Counts[Encoder_ID];
    }
}


/* Calculates and returns the converted value of the specified encoder in the specified format. */
float32 Encoder_getConvertCounts(Encoders_ID_t Encoder_ID, Encoders_Retuen_Format_t Encoders_Retuen_Format)
{

    // If Encoder_ID is ALL, calculate the average speed of all encoders
    if(Encoder_ID == ALL)
    {
        // Initialize the sum of the speeds of all encoders to 0
        float32 sumOfCounts = 0;
        for(uint8 index=0; index<NUMBER_OF_ENCODERS_USED; index++)
        {
            // Get the speed of the current encoder and add it to the sum
            sumOfCounts += Encoder_getConvertCounts(index, Encoders_Retuen_Format);
        }
        // Return the average speed of all encoders
        return (sumOfCounts / NUMBER_OF_ENCODERS_USED);
    }

    float32 converted_value;
    converted_value = (float32)Encoder_getCounts(Encoder_ID); // Get the current value of the encoder

    switch (Encoders_Retuen_Format)
    {
        case Counts:
            // The encoder value is already in counts, so return it as is
            return converted_value;

        case Degrees:
            // Convert the encoder value to degrees
            return ((converted_value / PULSES_PER_REVOLUTION) * 360);

        case Centimeters:
            // Convert the encoder value to centimeters
            return ((converted_value / PULSES_PER_REVOLUTION) * WHEEL_CIRCUMFERENCE_IN_CM);

        case Meters:
            // Convert the encoder value to meters
            return (((converted_value / PULSES_PER_REVOLUTION) * WHEEL_CIRCUMFERENCE_IN_CM) / 100);

        default:
            // Return an error value if an invalid format is specified
            return (float32)0xffffffff;
    }
}




/*
 * This function returns the average or current speed of the specified encoder in the specified format.
 * Parameters:
 * Encoder_ID: The ID of the encoder (LEFT or RIGHT).
 * Encoders_Retuen_Format: The format in which the speed is returned (Counts, Degrees, Centimeters or Meters).
 * Return value:
 * The speed of the specified encoder in the specified format.
 */
float32 Encoder_getSpeed(Encoders_ID_t Encoder_ID, Encoders_Retuen_Format_t Encoders_Retuen_Format)
{

    // If Encoder_ID is ALL, calculate the average speed of all encoders
    if(Encoder_ID == ALL)
    {
        // Initialize the sum of the speeds of all encoders to 0
        float32 sumOfSpeeds = 0;
        for(uint8 index=0; index<NUMBER_OF_ENCODERS_USED; index++)
        {
            // Get the speed of the current encoder and add it to the sum
            sumOfSpeeds += Encoder_getSpeed(index, Encoders_Retuen_Format);
        }
        // Return the average speed of all encoders
        return (sumOfSpeeds / NUMBER_OF_ENCODERS_USED);
    }

    float32 Convert_Motors_Speed ;
    Convert_Motors_Speed= (float32)Motors_Speed[Encoder_ID];


    // Convert the speed of the specified encoder to the specified format
    switch(Encoders_Retuen_Format)
    {
        case Counts:
            // The speed is already in encoder counts per second, so return it as is
            return Convert_Motors_Speed;
            break;

        case Degrees:
            // Calculate the speed in degrees per second
            return ((Convert_Motors_Speed / PULSES_PER_REVOLUTION) * 360);
            break;

        case Centimeters:
            // Calculate the speed in centimeters per second
            return ((Convert_Motors_Speed / PULSES_PER_REVOLUTION) * WHEEL_CIRCUMFERENCE_IN_CM);
            break;

        case Meters:
            // Calculate the speed in meters per second
            return (((Convert_Motors_Speed / PULSES_PER_REVOLUTION) * WHEEL_CIRCUMFERENCE_IN_CM)/100);
            break;

        default:
            // If an invalid format is specified, return an error value
            return (float32)0xffffffff ;
    }
}



// Function to update the encoder data for Encoder 0 channel A
void Encoder_0_update_CH_A()
{
            updateEncoderData(RIGHT); // Calls the updateEncoderData function for Right encoder
}

// Function to update the Encoder 0 channel B state
void Encoder_0_update_CH_B()
{
    // Switch statement to handle the Channel_B state and Channel_B next interrupt state
    switch(Encoder_CH_B_Next_Interrupt_State[RIGHT])
    {
        case RISING:
            Encoder_CH_State[RIGHT]= HIGH;
            Encoder_CH_B_Next_Interrupt_State[RIGHT]= FALLING;
        break;

        case FALLING:
            Encoder_CH_State[RIGHT]= LOW;
            Encoder_CH_B_Next_Interrupt_State[RIGHT]= RISING;
        break;
    }
}

// Function to update the encoder data for Encoder 1 channel A
void Encoder_1_update_CH_A()
{
    updateEncoderData(LEFT); // Calls the updateEncoderData function for LEFT encoder
}

// Function to update the Encoder 1 channel B state
void Encoder_1_update_CH_B()
{
    // Switch statement to handle the Channel_B state and Channel_B next interrupt state
    switch(Encoder_CH_B_Next_Interrupt_State[LEFT])
    {
        case RISING:
            Encoder_CH_State[LEFT] = HIGH;
            Encoder_CH_B_Next_Interrupt_State[LEFT] = FALLING;
            break;

        case FALLING:
            Encoder_CH_State[LEFT] = LOW;
            Encoder_CH_B_Next_Interrupt_State[LEFT] = RISING;
            break;
    }
}


// Update the encoder count and direction based on the input signal state
void updateEncoderData(Encoders_ID_t Encoder_ID)
{
    // Check which encoder is being updated
    switch(Encoder_ID)
    {
        case RIGHT:
            // If the input signal state is LOW, increment the encoder count
            if(Encoder_CH_State[RIGHT] == LOW)
            {
                Encoder_Counts[RIGHT]++;

                // Set the encoder direction based on the motor's CW direction
                #if RIGHT_MOTOR_CW_INVERTED == CW
                    Encoder_Direction[RIGHT] = DIR_CW;
                #else
                    Encoder_Direction[RIGHT] = DIR_CCW;
                #endif
            }
            // If the input signal state is HIGH, decrement the encoder count
            else
            {
                Encoder_Counts[RIGHT]--;

                // Set the encoder direction based on the motor's CW direction
                #if RIGHT_MOTOR_CW_INVERTED == CW
                    Encoder_Direction[RIGHT] = DIR_CCW;
                #else
                    Encoder_Direction[RIGHT] = DIR_CW;
                #endif
            }
            break;

        case LEFT:
            // If the input signal state is LOW, increment the encoder count
            if(Encoder_CH_State[LEFT] == LOW)
            {
                Encoder_Counts[LEFT]++;

                // Set the encoder direction based on the motor's CW direction
                #if LEFT_MOTOR_CW_INVERTED == CW
                    Encoder_Direction[LEFT] = DIR_CW;
                #else
                    Encoder_Direction[LEFT] = DIR_CCW;
                #endif
            }
            // If the input signal state is HIGH, decrement the encoder count
            else
            {
                Encoder_Counts[LEFT]--;

                // Set the encoder direction based on the motor's CW direction
                #if LEFT_MOTOR_CW_INVERTED == CW
                    Encoder_Direction[LEFT] = DIR_CCW;
                #else
                    Encoder_Direction[LEFT] = DIR_CW;
                #endif
            }
            break;

        default:
            // Do nothing if an invalid encoder ID is given
            break;
    }
}



/*  Updates the speed of each motor based on the change in encoder counts. */
void updateSpeed()
{
    // Initialize an array to store the previous encoder counts
    static volatile int32 Encoders_Old_Counts[NUMBER_OF_ENCODERS_USED] = {0};

    // Calculate the speed for each motor based on the change in encoder counts per time equals [SPEED_MEASUREMENT_INTERVAL_MS Second]
    // Store the speed as counts per second.
    for (uint8 index = 0; index < NUMBER_OF_ENCODERS_USED; index++)
    {
        Motors_Speed[index] = (float32)((Encoder_Counts[index] - Encoders_Old_Counts[index]) / (float32)(SPEED_MEASUREMENT_INTERVAL_MS / (float32)1000));
    }

    // Update the old encoder counts array with the current encoder counts
    for (uint8 index = 0; index < NUMBER_OF_ENCODERS_USED; index++)
    {
        Encoders_Old_Counts[index] = Encoder_Counts[index];
    }
}



// Function to get the direction of an Encoder/Vehicle
Encoders_Motors_Direction_t Encoder_getDirection(Encoders_ID_t Encoder_ID)
{
    #if NUMBER_OF_ENCODERS_USED == 2
        // In case Encoder_ID = ALL then if we have 2 encoders in vehicle, check the Vehicle directions
        if (Encoder_ID == ALL)
        {
            // If both encoders are rotating clockwise, return DIR_FORWARD
            if ((Encoder_Direction[RIGHT] == DIR_CW) && (Encoder_Direction[LEFT] == DIR_CW))
                return DIR_FORWARD;

            // If both encoders are rotating counter-clockwise, return DIR_BACKWARD
            if ((Encoder_Direction[RIGHT] == DIR_CCW) && (Encoder_Direction[LEFT] == DIR_CCW))
                return DIR_BACKWARD;

            // If the right encoder is rotating counter-clockwise and the left encoder is rotating clockwise, return DIR_RIGHT
            if ((Encoder_Direction[RIGHT] == DIR_CCW) && (Encoder_Direction[LEFT] == DIR_CW))
                return DIR_RIGHT;

            // If the right encoder is rotating clockwise and the left encoder is rotating counter-clockwise, return DIR_LEFT
            if ((Encoder_Direction[RIGHT] == DIR_CW) && (Encoder_Direction[LEFT] == DIR_CCW))
                return DIR_LEFT;
        }
    #endif

    // check a specific encoder ID, return its direction
    if (Encoder_ID < NUMBER_OF_ENCODERS_USED)
        return Encoder_Direction[Encoder_ID];

    // If the encoder ID is invalid, return -1
    return -1;
}
