/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : Ultrasonic.c                                                               */
/* Description : Functions Implementation for HC-SR04 Ultrasonic (HAL Layer)                */
/********************************************************************************************/

/* Library includes */
#include "Std_Types.h"
#include "Common_Macros.h"

/* MCU constant parameters includes */
#include "STM32F103C6.h"

/* Peripherals includes */
#include "RCC_interface.h"
#include "DIO_interface.h"
#include "AFIO_interface.h"
#include "EXTI_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"

/* Module files includes */
#include "Ultrasonic_interface.h"
#include "Ultrasonic_private.h"
#include "Ultrasonic_config.h"


static volatile Ultrasonic_t Ultrasonic[NUMBER_OF_ULTRASONIC_USED];
static volatile float32 Distance;
static volatile uint8 echoRisingFlag, readingState;


// Initializes and configures the required modules for the Ultrasonic interface, including RCC, DIO, AFIO, EXTI, STK, and NVIC.
void Ultrasonic_init(Ultrasonic_ID_t ID, Ultrasonic_config_t * Ultrasonic_config)
{
        // Initialize the properties of an ultrasonic sensor using the provided configuration
        Ultrasonic[ID].Echo_pin      =Ultrasonic_config->Echo_pin;
        Ultrasonic[ID].Echo_port     =Ultrasonic_config->Echo_port;
        Ultrasonic[ID].Trigger_pin   =Ultrasonic_config->Trigger_pin;
        Ultrasonic[ID].Trigger_port  =Ultrasonic_config->Trigger_port;
        Ultrasonic[ID].DistanceRange =Ultrasonic_config->DistanceRange;
        Ultrasonic[ID].Timeout       =(uint32)((Ultrasonic[ID].DistanceRange * 58.824)+.5);

        // Enable clock for Ultrasonic ports
        RCC_enableClk(RCC_APB2, Ultrasonic[ID].Echo_port);
        RCC_enableClk(RCC_APB2, Ultrasonic[ID].Trigger_port);

        // Enable AFIO clock
        RCC_enableClk(RCC_APB2, RCC_AFIO);

        // Set pin directions to input floating
        DIO_setPinDirection(Ultrasonic[ID].Echo_port, Ultrasonic[ID].Echo_pin, INPUT_FLOATING);
        DIO_setPinDirection(Ultrasonic[ID].Trigger_port, Ultrasonic[ID].Trigger_pin, OUTPUT_SPEED_2MHZ_PP);


        // Set EXTI configuration
        AFIO_setEXTIConfig(Ultrasonic[ID].Echo_port, Ultrasonic[ID].Echo_pin);


        // Enable EXTI interrupts and their corresponding NVIC
        NVIC_enableInterrupt((NVIC_EXTI0 + Ultrasonic[ID].Echo_pin));   // It will cause error in case of using MCU instead of stm32f10x series.


        // Set EXTI callback
        EXTI_setCallBack(Ultrasonic[ID].Echo_pin, CalcDistance);


        // Enable EXTI interrupts
        EXTI_enableEXTI(Ultrasonic[ID].Echo_pin, CHANGE);


        // Initialize and configure STK
        STK_init();
}

/* This function reads the distance measured by an ultrasonic sensor and return the value in Centimeter*/
float32 Ultrasonic_readDistance(Ultrasonic_ID_t ID)
{
    /* Check if the ultrasonic sensor is enabled */
    if(Ultrasonic[ID].State == Ultrasonic_enable)
    {
        /* Set flags to indicate a rising edge has been detected and a distance reading is pending */
        echoRisingFlag = RISING_DETECTED;
        readingState   = READING_PENDING;

        EXTI_enableEXTI(Ultrasonic[ID].Echo_pin, CHANGE);

        /* Trigger the ultrasonic sensor by sending a pulse to its trigger pin */
        DIO_setPinValue(Ultrasonic[ID].Trigger_port , Ultrasonic[ID].Trigger_pin, LOW);
        STK_delayMicroSec(2);
        DIO_setPinValue(Ultrasonic[ID].Trigger_port , Ultrasonic[ID].Trigger_pin, HIGH);
        STK_delayMicroSec(10);
        DIO_setPinValue(Ultrasonic[ID].Trigger_port , Ultrasonic[ID].Trigger_pin, LOW);

        /* Wait for a distance reading to be completed */
        while(readingState == READING_PENDING)
        {
            /* Check if the reading has timed out */
            if(STK_getElapsedTime() > Ultrasonic[ID].Timeout)
            {
                /* In case of reading has timed out Set the reading state to done,
                   stop the timer interval, and set the distance to out of range */
                EXTI_disableEXTI(Ultrasonic[ID].Echo_pin);
                readingState = READING_DONE;
                STK_stopInterval();
                Distance = (float32)OUT_OF_RANGE;
                break;
            }
        }
        return Distance;
    }
    else
    {
        /* Return a value(-2) indicating that the ultrasonic sensor is disabled */
        return (float32)ULTRASONIC_DISABLE;
    }
}


// This function sets the range and timeout values for an ultrasonic sensor which increase performance
void Ultrasonic_setRange(Ultrasonic_ID_t ID, uint16 Range_cm)
{
    // Set the new DistanceRange variable for the specified ultrasonic sensor
    Ultrasonic[ID].DistanceRange = Range_cm;

    // Calculate the timeout in us value for the specified ultrasonic sensor using the formula
    Ultrasonic[ID].Timeout = (uint32)((Ultrasonic[ID].DistanceRange * 58.824) + 0.5);
}

// This function sets the state for an ultrasonic sensor
// In case of the state is Ultrasonic_disable, the reading will not execute for the this sensor.
void Ultrasonic_setState(Ultrasonic_ID_t ID, Ultrasonic_state_t state)
{
    // Set the State variable for the specified ultrasonic sensor
    Ultrasonic[ID].State = state;
}


void CalcDistance()
{
    // Check if the echo signal is rising
    if(echoRisingFlag == RISING_DETECTED)
    {
        // Set a timer interval to do nothing for a fixed period of time, just to get elapsed time.
        STK_setIntervalSingle(TIMER_PERIOD, NULL);

        // Change the echo rising flag to indicate that the next signal is falling
        echoRisingFlag = FALLING_DETECTED;
    }
    else // When Echo signal is falling
    {
        // In case distance returned as OUT_OF_RANGE after timed out, Don't execute If block.
        if(readingState != READING_DONE)
        {
            // Set the reading state to done
            readingState = READING_DONE;

            // Calculate the distance in Centimeter using the elapsed time in us.
            Distance = (0.017 * STK_getElapsedTime());

            // Stop the timer interval
            STK_stopInterval();
        }
    }
}

