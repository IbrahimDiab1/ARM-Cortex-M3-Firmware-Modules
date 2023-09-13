/*************************************************************************************/
/* Author      : Ibrahim Diab                                                        */
/* File Name   : Oximeter_private.h                                                  */
/* Description : Private addresses for Max30102 Oximeter (HAL Layer)                 */
/*************************************************************************************/

#ifndef Oximeter_PRIVATE_H
#define Oximeter_PRIVATE_H


// I2C slave Address of Max30102 Oximeter
#define MAX30102_ADDRESS                    0x57

#define I2C_MAX_BUFFER_SIZE                 10

#define NUM_OF_INTERRUPT_STATUS             5

#define FIFO_ALMOST_FULL_FLAG_INDEX         0
#define FIFO_NEW_DATA_READY_INDEX           1
#define AMBIENT_LIGHT_OVERFLOW_INDEX        2
#define POWER_READY_FLAG_INDEX              3
#define TEMPERATURE_READY_FLAG_INDEX        4

#define FIFO_SIZE                           32

#define NOT_SUPPORTED                       0
#define SUPPORTED                           1

#define DISABLED                            0
#define ENABLED                             1

#define SAMPLE_AVERAGING_1                  0
#define SAMPLE_AVERAGING_2                  1
#define SAMPLE_AVERAGING_4                  2
#define SAMPLE_AVERAGING_8                  3
#define SAMPLE_AVERAGING_16                 4
#define SAMPLE_AVERAGING_32                 5

#define MULTI_MODE                          7
#define SPO2                                3
#define HEART_RATE                          2

#define SPO2_ADC_RANGE_2048                 0
#define SPO2_ADC_RANGE_4096                 1
#define SPO2_ADC_RANGE_8192                 2
#define SPO2_ADC_RANGE_16384                3

#define SPO2_SAMPLE_RATE_50_HZ              0
#define SPO2_SAMPLE_RATE_100_HZ             1
#define SPO2_SAMPLE_RATE_200_HZ             2
#define SPO2_SAMPLE_RATE_400_HZ             3
#define SPO2_SAMPLE_RATE_800_HZ             4
#define SPO2_SAMPLE_RATE_1000_HZ            5
#define SPO2_SAMPLE_RATE_1600_HZ            6
#define SPO2_SAMPLE_RATE_3200_HZ            7

#define ADC_RESOLUTION_15_BIT               0
#define ADC_RESOLUTION_16_BIT               1
#define ADC_RESOLUTION_17_BIT               2
#define ADC_RESOLUTION_18_BIT               3

#define NONE                                0
#define RED_LED                             1
#define IR_LED                              2

#define INVALID                             0xFF




/************************ register addresses ***************************/

/* Interrupt Status */
#define INTR_STATUS_1    (uint8)0x00
#define INTR_STATUS_2    (uint8)0x01

/* Interrupt Enable */
#define INTR_ENABLE_1    (uint8)0x02
#define INTR_ENABLE_2    (uint8)0x03

/* FIFO */
#define FIFO_WR_PTR      (uint8)0x04
#define OVF_COUNTER      (uint8)0x05
#define FIFO_RD_PTR      (uint8)0x06
#define FIFO_DATA        (uint8)0x07

/* FIFO Configuration */
#define FIFO_CONFIG      (uint8)0x08

/* Mode Configuration */
#define MODE_CONFIG      (uint8)0x09

/* SpO2 Configuration */
#define SPO2_CONFIG      (uint8)0x0A

/* LED Pulse Amplitude */
#define LED1_PA          (uint8)0x0C
#define LED2_PA          (uint8)0x0D

/* Multi-LED Mode Control Registers */
#define MULTI_LED_CTRL1  (uint8)0x11
#define MULTI_LED_CTRL2  (uint8)0x12

/* Temperature Data */
#define TEMP_INTR        (uint8)0x1F
#define TEMP_FRAC        (uint8)0x20
#define TEMP_CONFIG      (uint8)0x21

/* PART ID */
#define REV_ID           (uint8)0xFE
#define PART_ID          (uint8)0xFF

/*********************************** Private Functions Prototypes **************************************/

void writeRegister(uint8 registerAddress, uint8 * buffer, uint8 dataLength);

void readRegister(uint8 registerAddress, uint8 * buffer, uint8 datalength);

void readLedsValues(uint8 numOfSamplesToRead);

void updateReadySamplesNum();

void getInterruptStatus();

void Oximeter_IntHandler();

void read_Spo2_HR_value_Asenq();


#endif // Oximeter_PRIVATE_H
