/*******************************************************************/
/* Author      : Ibrahim Diab                                      */
/* File Name   : Button_private.h                                  */
/* Description : Private data and functions for Button Module      */
/*******************************************************************/

#ifndef _Button_PRIVATE_H_
#define _Button_PRIVATE_H_


typedef struct
{
    Button_Config_t buttonConfig;
    Button_State_t ButtonState;
    uint32 ButtonPressStartTime;
}Button_Context_t;

volatile Button_Context_t Button[MAX_BUTTONS];

#endif // _Button_PRIVATE_H_
