/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : LEDMAT_program.c                                                           */
/* Description : Functions Implementation for LED-Matrix (HAL Layer)                        */
/********************************************************************************************/

#include "std_types.h"
#include "common_macros.h"

#include "DIO_interface.h"
#include "STK_interface.h"

#include "LEDMAT_interface.h"
#include "LEDMAT_private.h"
#include "LEDMAT_config.h"


void LEDMAT_init()
{
    DIO_setBinDirection(LEDMAT_ROW_0,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_1,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_2,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_3,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_4,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_5,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_6,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_ROW_7,OUTPUT_SPEED_2MHZ_PP);
        
    DIO_setBinDirection(LEDMAT_COL_0,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_1,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_2,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_3,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_4,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_5,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_6,OUTPUT_SPEED_2MHZ_PP);
    DIO_setBinDirection(LEDMAT_COL_7,OUTPUT_SPEED_2MHZ_PP);

}


void LEDMAT_disblayFrame(uint8 * data)
{
    for(uint8 idx=0;idx<8;idx++)
    {
    ActiveateColumn(idx);
    SetRowsValue(data[idx]);
    STK_delayMicroSec(2500);        // delay 2.5 msec
    
    }
    
}

void SetRowsValue(uint8 data)
{
    uint8 BitValue=0;
    
    BitValue=GET_BIT(data,0);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,1);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,2);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,3);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,4);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,5);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,6);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
     BitValue=GET_BIT(data,7);
    DIO_setBinValue(LEDMAT_ROW_0,BitValue);
    
    
}


void ActiveateColumn(uint8 NO_COL)
{
    // Disable all columns
    DIO_setBinValue(LEDMAT_COL_0,HIGH);
    DIO_setBinValue(LEDMAT_COL_1,HIGH);
    DIO_setBinValue(LEDMAT_COL_2,HIGH);
    DIO_setBinValue(LEDMAT_COL_3,HIGH);
    DIO_setBinValue(LEDMAT_COL_4,HIGH);
    DIO_setBinValue(LEDMAT_COL_5,HIGH);
    DIO_setBinValue(LEDMAT_COL_6,HIGH);
    DIO_setBinValue(LEDMAT_COL_7,HIGH);
    
    // Enable the desired column only.
    switch(NO_COL)
    {
        case 0:
                DIO_setBinValue(LEDMAT_COL_0,LOW); break;
        case 1:     
                DIO_setBinValue(LEDMAT_COL_1,LOW); break;
        case 2:     
                DIO_setBinValue(LEDMAT_COL_2,LOW); break;
        case 3:     
                DIO_setBinValue(LEDMAT_COL_3,LOW); break;
        case 4:     
                DIO_setBinValue(LEDMAT_COL_4,LOW); break;
        case 5:     
                DIO_setBinValue(LEDMAT_COL_5,LOW); break;
        case 6:     
                DIO_setBinValue(LEDMAT_COL_6,LOW); break;
        case 7:     
                DIO_setBinValue(LEDMAT_COL_7,LOW); break;
    }
}