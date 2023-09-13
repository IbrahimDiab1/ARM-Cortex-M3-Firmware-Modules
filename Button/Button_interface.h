/*******************************************************************/
/* Author      : Ibrahim Diab                                      */
/* File Name   : Button_interface.h                                */
/* Description : Interfacing Functions for Button Module           */
/*******************************************************************/

#ifndef _Button_INTERFACE_H_
#define _Button_INTERFACE_H_

/* Port IDs */
#define DIO_GPIOA  0    // ID for GPIO Port A
#define DIO_GPIOB  1    // ID for GPIO Port B
#define DIO_GPIOC  2    // ID for GPIO Port C

/* Pins IDs */
#define DIO_PIN0   0    // ID for Pin 0
#define DIO_PIN1   1    // ID for Pin 1
#define DIO_PIN2   2    // ID for Pin 2
#define DIO_PIN3   3    // ID for Pin 3
#define DIO_PIN4   4    // ID for Pin 4
#define DIO_PIN5   5    // ID for Pin 5
#define DIO_PIN6   6    // ID for Pin 6
#define DIO_PIN7   7    // ID for Pin 7
#define DIO_PIN8   8    // ID for Pin 8
#define DIO_PIN9   9    // ID for Pin 9
#define DIO_PIN10  10   // ID for Pin 10
#define DIO_PIN11  11   // ID for Pin 11
#define DIO_PIN12  12   // ID for Pin 12
#define DIO_PIN13  13   // ID for Pin 13
#define DIO_PIN14  14   // ID for Pin 14
#define DIO_PIN15  15   // ID for Pin 15

typedef enum
{
    Up,
    Down,
    Left,
    Right,
    Rotate_Ok
    // insert buttons names here which total numbers equals MAX_BUTTONS
    // You can move this enumeration in App layer
}Button_t;

typedef enum
{
    Pull_Up,
    Pull_Down
}Button_Pull_Type_t;

typedef enum
{
    Internal,
    External
}Button_Pull_Source_t;

typedef enum 
{
    Button_Released,
    Button_Pressed_Pending_Read,
    Button_Pressed_Read_Once,   
    Button_Long_Pressed,
    Button_Long_Pressed_Read_Once
}Button_State_t;

typedef struct
{
    uint8 pin;                          // DIO_PIN0...DIO_PIN15
    uint8 port;                         // DIO_GPIOA, DIO_GPIOB, DIO_GPIOC
    Button_Pull_Type_t pullType;        // Pull_Up ,Pull_Down
    Button_Pull_Source_t PullSource;    // Internal, External
    uint8 extiState;                    // External Interrupt State : EXTI_STATE_DISABLED, EXTI_STATE_RISING, EXTI_STATE_FALLING ,  EXTI_STATE_CHANGE
    void (*CallbackFunc)(void);         // Callback function in case EXTI is enabled for the required button

}Button_Config_t;

#define EXTI_STATE_DISABLED     0
#define EXTI_STATE_RISING       1
#define EXTI_STATE_FALLING      2
#define EXTI_STATE_CHANGE       3

void Button_init(Button_t button_id, Button_Config_t *Button_config);
void Button_updateState();
Button_State_t Button_getState(Button_t button_id);

#endif // _Button_INTERFACE_H_
