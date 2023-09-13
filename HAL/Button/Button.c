/*******************************************************************/
/* Author      : Ibrahim Diab                                      */
/* File Name   : Button.c                                          */
/* Description : Functions Implementation for Button Module        */
/*******************************************************************/

#include <STM32F103x.h>

#include <LIB/common_macros.h>
#include <LIB/std_types.h>

#include "../Inc/MCAL//DIO/DIO_interface.h"
#include "../Inc/MCAL//RCC/RCC_interface.h"
#include "../Inc/MCAL//NVIC/NVIC_interface.h"
#include "../Inc/MCAL//GPT/GPT_interface.h"
#include "../Inc/MCAL//AFIO/AFIO_interface.h"
#include "../Inc/MCAL//EXTI/EXTI_interface.h"


#include "../Inc/HAL//Button/Button_config.h"
#include "../Inc/HAL//Button/Button_interface.h"
#include "../Inc/HAL//Button/Button_private.h"


void Button_init(Button_t button_id, Button_Config_t *Button_config)
{
    Button[button_id].buttonConfig.pin           = Button_config->pin;
    Button[button_id].buttonConfig.port          = Button_config->port;
    Button[button_id].buttonConfig.pullType      = Button_config->pullType;
    Button[button_id].buttonConfig.PullSource    = Button_config->PullSource;
    Button[button_id].buttonConfig.extiState     = Button_config->extiState;
    Button[button_id].buttonConfig.CallbackFunc  = Button_config->CallbackFunc;
    
    switch(Button[button_id].buttonConfig.port)
    {
        case DIO_GPIOA:
        
            RCC_enableClk (RCC_APB2, RCC_GPIOA);

#if AT_LEAST_ONE_BUTTON_USE_EXTI == 1
            if(Button[button_id].buttonConfig.extiState != DISABLED)
            {
                if(Button[button_id].buttonConfig.pin < NO_EXTI_SUPPORTED)
                {
                    RCC_enableClk (RCC_APB2, RCC_AFIO);
                    AFIO_setEXTIConfig(Button[button_id].buttonConfig.pin, AFIO_PORT_A);
                    EXTI_enableEXTI(Button[button_id].buttonConfig.pin, Button[button_id].buttonConfig.extiState -1);   // subtract 1 as the EXTI define states in EXTI driver values start from zero instead of 1
                    NVIC_enableInterrupt(NVIC_EXTI0 + Button[button_id].buttonConfig.pin );
                    EXTI_setCallBack( Button[button_id].buttonConfig.pin, Button[button_id].buttonConfig.CallbackFunc);
                }
            }
#endif
            break;
            
        case DIO_GPIOB:
        
            RCC_enableClk (RCC_APB2, RCC_GPIOB);

#if AT_LEAST_ONE_BUTTON_USE_EXTI == 1

            if(Button[button_id].buttonConfig.extiState == ENABLED)
            {
                if(Button[button_id].buttonConfig.pin <= NO_EXTI_SUPPORTED)
                {
                    RCC_enableClk (RCC_APB2, RCC_AFIO);
                    AFIO_setEXTIConfig(Button[button_id].buttonConfig.pin, AFIO_PORT_B);
                    EXTI_enableEXTI(Button[button_id].buttonConfig.pin, Button[button_id].buttonConfig.extiState -1);
                    NVIC_enableInterrupt(NVIC_EXTI0 + Button[button_id].buttonConfig.pin );
                    EXTI_setCallBack( Button[button_id].buttonConfig.pin, Button[button_id].buttonConfig.CallbackFunc);
                }
            }
#endif
            break;
            
        case DIO_GPIOC:
        
            RCC_enableClk (RCC_APB2, RCC_GPIOC);

#if AT_LEAST_ONE_BUTTON_USE_EXTI == 1
            if(Button[button_id].buttonConfig.extiState == ENABLED)
            {
                if(Button[button_id].buttonConfig.pin <= NO_EXTI_SUPPORTED)
                {
                    RCC_enableClk (RCC_APB2, RCC_AFIO);
                    AFIO_setEXTIConfig(Button[button_id].buttonConfig.pin, AFIO_PORT_C);
                    EXTI_enableEXTI(Button[button_id].buttonConfig.pin, Button[button_id].buttonConfig.extiState -1);
                    NVIC_enableInterrupt(NVIC_EXTI0 + Button[button_id].buttonConfig.pin );
                    EXTI_setCallBack( Button[button_id].buttonConfig.pin, Button[button_id].buttonConfig.CallbackFunc);
                }
            }
#endif
            break;
            
        default: 
            // Error handling   
            break;
    }

    switch(Button[button_id].buttonConfig.pullType)
    {                                                                                                                                                          
        case Pull_Up:   
        
            if(Button[button_id].buttonConfig.PullSource == Internal)
                DIO_setPinDirection( Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin, INPUT_PULLUP );
            else
                DIO_setPinDirection( Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin, INPUT_FLOATING );
            break;                                                                     
                                                                                       
        case Pull_Down: 
        
            if(Button[button_id].buttonConfig.PullSource == Internal)
                DIO_setPinDirection( Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin, INPUT_PULLDOWN );
            else
                DIO_setPinDirection( Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin, INPUT_FLOATING );
            break;
            
        default: 
            // Error handling   
            break;
    }
    
    Button[button_id].ButtonState = Button_Released; 
}

void Button_updateState()
{
    for(uint8 button_id=0; button_id < MAX_BUTTONS; button_id++)
    {
        #if FREE_RTOS_IN_USE == 1
        uint32 currentTime = xTaskGetTickCount();
        #elif FREE_RTOS_IN_USE == 0
        uint32 currentTime = GPT_getTicks();
        #endif
        
        switch(Button[button_id].ButtonState)
        {
            case Button_Released :
            
                if(Button[button_id].buttonConfig.pullType == Pull_Down)
                    {
                        if(DIO_getPinValue(Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin) == HIGH)
                            {
                                Button[button_id].ButtonState = Button_Pressed_Pending_Read;
                                Button[button_id].ButtonPressStartTime = currentTime;
                            }
                    }
                else if(Button[button_id].buttonConfig.pullType == Pull_Up)
                    {
                        if(DIO_getPinValue(Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin) == LOW)
                            {
                                Button[button_id].ButtonState = Button_Pressed_Pending_Read;
                                Button[button_id].ButtonPressStartTime = currentTime;
                            }
                    }
                break;
                
            case Button_Pressed_Pending_Read : 
            case Button_Pressed_Read_Once    :
            
                if(Button[button_id].buttonConfig.pullType == Pull_Down)
                    {
                        if((DIO_getPinValue(Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin) == LOW)  && 
                          ((currentTime-Button[button_id].ButtonPressStartTime) > BUTTON_DEBOUNCING_DELAY))
                            Button[button_id].ButtonState = Button_Released;

                        else if(currentTime-Button[button_id].ButtonPressStartTime > BUTTON_LONG_PRESS_TIME )
                            Button[button_id].ButtonState = Button_Long_Pressed;
                            
                    }
                else if(Button[button_id].buttonConfig.pullType == Pull_Up)
                    {
                        if((DIO_getPinValue(Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin) == HIGH) && 
                          ((currentTime-Button[button_id].ButtonPressStartTime) > BUTTON_DEBOUNCING_DELAY))
                            Button[button_id].ButtonState = Button_Released;

                        else if(currentTime-Button[button_id].ButtonPressStartTime > BUTTON_LONG_PRESS_TIME )
                            Button[button_id].ButtonState = Button_Long_Pressed;
                    }
                break;
                
            case Button_Long_Pressed :
            case Button_Long_Pressed_Read_Once :
            
                if(Button[button_id].buttonConfig.pullType == Pull_Down)
                    {
                        if(DIO_getPinValue(Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin) == LOW)
                            Button[button_id].ButtonState = Button_Released;    
                    }
                else if(Button[button_id].buttonConfig.pullType == Pull_Up)
                    {
                        if(DIO_getPinValue(Button[button_id].buttonConfig.port, Button[button_id].buttonConfig.pin) == HIGH)
                            Button[button_id].ButtonState = Button_Released;
                    }
                break;
            
            default :
                // Error handling
                break;
        }
    }
}

Button_State_t Button_getState(Button_t button_id)
{
    if(Button[button_id].ButtonState == Button_Pressed_Pending_Read)
    {
        Button[button_id].ButtonState = Button_Pressed_Read_Once;
        return Button_Pressed_Pending_Read;
    }
    else if(Button[button_id].ButtonState == Button_Long_Pressed)
    {
        Button[button_id].ButtonState = Button_Long_Pressed_Read_Once;
        return Button_Long_Pressed;
    }

    return Button[button_id].ButtonState;
}
