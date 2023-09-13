/*************************************************************************************/
/* Author     : Ibrahim Diab                                                         */
/* File Name  : IR_private.h                                                         */
/* Description: Private addresses for IR receiver (HAL Layer)                        */
/*************************************************************************************/

#ifndef IR_PRIVATE_H
#define IR_PRIVATE_H

// Declare a static volatile array of 50 bytes to store the received IR frame
static volatile uint8 Frame[50] = {0};

// Declare a volatile variable to keep track of the number of bytes received so far
static volatile uint8 Counter = 0;

// Declare a volatile flag to indicate whether the start bit of the frame has been received
static volatile uint8 startFlag = 0;

// Declare a volatile variable to keep track of the time elapsed since the start of the frame
static volatile uint8 elapsedTime = 0;

// Declare a volatile variable to store the decoded data byte from the IR frame
static volatile uint8 IR_data = 0;

// Declare a volatile variable to store the decoded address byte from the IR frame
static volatile uint8 IR_address = 0;

// Declare a volatile flag to indicate whether new IR data has been received
static volatile uint8 IR_newDataFlaag = 0;

// Declare a function to get the full IR frame
void getFrame();

// Declare a function to check if the received IR frame is valid
uint8 IsFrameValid();

// Declare a function to set the new data flag
void setNewData();

#endif //IR_PRIVATE_H
