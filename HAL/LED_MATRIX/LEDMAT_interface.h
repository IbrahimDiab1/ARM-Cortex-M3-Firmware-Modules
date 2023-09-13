/**************************************************************************************/
/* Author      : Ibrahim Diab                                                         */
/* File Name   : LEDMAT_interface.h                                                   */
/* Description : Interfacing macros for LED-Matrix (HAL Layer)                        */
/**************************************************************************************/


#ifndef LEDMAT_INTERFACE_H
#define LEDMAT_INTERFACE_H

void LEDMAT_init();
void LEDMAT_disblayFrame(uint8 * data);

#endif // LEDMAT_INTERFACE_H