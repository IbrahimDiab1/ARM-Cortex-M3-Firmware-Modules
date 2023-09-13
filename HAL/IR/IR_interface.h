/**************************************************************************************/
/* Author     : Ibrahim Diab                                                          */
/* File Name  : IR_interface.h                                                        */
/* Description: Interfacing macros for IR receiver (HAL Layer)                        */
/**************************************************************************************/

#ifndef IR_INTERFACE_H
#define IR_INTERFACE_H

/* Initializes the IR receiver */
void IR_init();

/* Retrieves data from the IR receiver */
uint8 IR_getData();

/* Checks if there is new data available from the IR receiver */
uint8 IR_checkNewData();

/* Retrieves the address from the IR receiver */
uint8 IR_getAddress();

#endif //IR_INTERFACE_H
