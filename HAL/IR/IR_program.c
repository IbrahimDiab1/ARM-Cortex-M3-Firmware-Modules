/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : IR_program.c                                                               */
/* Description : Functions Implementation for IR receiver (HAL Layer)                       */
/********************************************************************************************/

#include "std_types.h"
#include "common_macros.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "STK_interface.h"
#include "NVIC_interface.h"
#include "EXTI_interface.h"
#include "AFIO_interface.h"

#include "IR_interface.h"
#include "IR_private.h"
#include "IR_config.h"

/* This function initializes the IR receiver */
void IR_init()
{
    /* Enable clock for the GPIO port that the IR receiver is connected to */
    RCC_enableClk(RCC_APB2, IR_OUTPUT_PORT);
     
    #if IR_OUTPUT_PIN  > 9
     NVIC_enableInterrupt(40);
     
    #elif IR_OUTPUT_PIN > 4
     NVIC_enableInterrupt(23);
     
    #elif IR_OUTPUT_PIN >=0
     NVIC_enableInterrupt(6 + IR_OUTPUT_PIN);
    
    #else
    /* An error occurred, since IR_OUTPUT_PIN was not between 0 and 15 */
    #error "error configuration >>please choose value between 0 and 15"
    #endif
    
    /* Set the GPIO pin that the IR receiver is connected to as an input pin */
    DIO_setPinDirection(IR_OUTPUT_PORT, IR_OUTPUT_PIN, INPUT_FLOATING);

    /* Configure the AFIO EXTI to enable the interrupt for the IR receiver */
    AFIO_setEXTIConfig(IR_OUTPUT_PIN, IR_OUTPUT_PORT);
    
    /* Initialize the SysTick timer */
    STK_init();
    
    /* Set the callback function that will be executed when the EXTI interrupt occurs */
    EXTI_setCallBack(IR_OUTPUT_PIN, getFrame);
    EXTI_enableEXTI(IR_OUTPUT_PIN, FALLING);
}

/* This function extracts the received IR frame */
void getFrame()
{
    /* If this is the start of the frame */
    if (startFlag == 0)
    {
        startFlag = 1;
        Counter = 1;
    }
    
    if (startFlag)
    {
        /* Get the elapsed time since the last falling edge */
        elapsedTime = STK_getElapsedTime();
        
        /* If the elapsed time is within the expected range for a logical 1 */
        if (elapsedTime > 1800 && elapsedTime < 2400)
        {
            Frame[Counter] = 1;
            Counter++;
        }
        /* If the elapsed time is within the expected range for a logical 0 */
        else if (elapsedTime > 800 && elapsedTime < 1200)
        {
            Frame[Counter] = 0;
            Counter++;
        }
        /* If the elapsed time is within the expected range for the end of a frame */
        else if (elapsedTime > 10000 && elapsedTime < 15000)
        {
            Counter = 1;
        }
        else
        {
            /* An error occurred in communication */
        }
    }

    /* Reset the SysTick timer and set a new interval to get the next falling edge */
    STK_setIntervalSingle(200000, setNewData);
}

/* This function checks if the received IR frame is valid */
uint8 IsFrameValid()
{
    /* Disable EXTI interrupt while validating the frame */
    EXTI_disableEXTI(IR_OUTPUT_PIN);

    /* Validate the frame */
    for(uint8 counter=1;counter<9;counter++)
    {
        if((Frame[counter] ^ Frame[counter+8])==0)
        {
            /* If frame is invalid, re-enable EXTI interrupt and return FALSE */
            EXTI_enableEXTI(IR_OUTPUT_PIN,FALLING);
            retuen FALSE; /* Typo: should be "return" instead of "return" */
        }
    }
    
    for(uint8 counter=17;counter<25;counter++)
    {
        if((Frame[counter] ^ Frame[counter+8])==0)
        {
            /* If frame is invalid, re-enable EXTI interrupt and return FALSE */
            EXTI_enableEXTI(IR_OUTPUT_PIN,FALLING);
            return FALSE;
        }
    }

    /* If frame is valid, re-enable EXTI interrupt and return TRUE */
    EXTI_enableEXTI(IR_OUTPUT_PIN,FALLING);
    return TRUE;
}


uint8 IR_voidGetData()
{
    if(IsFrameValid())
    {
        /* Disable EXTI interrupt while getting data from the frame */
        EXTI_disableEXTI(IR_OUTPUT_PIN);

        for(uint8 counter=0;counter<8;counter++)
        {
            if(Frame[8-counter]==1)
            {
                SET_BIT(IR_data,counter);
            }
            
            else 
            {
                CLR_BIT(IR_data,counter);
            }
        }
    }

    /* Re-enable EXTI interrupt and return the data */
    EXTI_enableEXTI(IR_OUTPUT_PIN,FALLING);
    return IR_data;
}


uint8 IR_voidGetAddress()
{
    if(IsFrameValid())
    {
        /* Disable EXTI interrupt while getting address from the frame */
        EXTI_disableEXTI(IR_OUTPUT_PIN);

        for(uint8 counter=0;counter<25;counter++)
        {
            if(Frame[24-counter]==1)
            {
                SET_BIT(IR_address,counter);
            }
            
            else 
            {
                CLR_BIT(IR_address,counter);
            }
        }
    }

    /* Re-enable EXTI interrupt and return the address */
    EXTI_enableEXTI(IR_OUTPUT_PIN,FALLING);
    return IR_address;
}


void setNewData()
{
    /* Set the new data flag */
    IR_newDataFlaag=1;
}


uint8 IR_checkNewData()
{
    if(IR_newDataFlaag)
    {
        /* If new data is available, clear the flag and return 1 */
        IR_newDataFlaag=0;
        return 1;
    }   
    else 
    {
        /* If new data is not available, return 0 */
        return 0;
    }
}
