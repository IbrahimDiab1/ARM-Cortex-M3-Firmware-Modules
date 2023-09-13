/*********************************************************************************/
/* Author      : Ibrahim Diab                                                    */
/* File Name   : Oximeter_config.h                                               */
/* Description : Configuration for Max30102 Oximeter (HAL Layer)                 */
/*********************************************************************************/

#ifndef Oximeter_CONFIG_H
#define Oximeter_CONFIG_H


/****************************  Pre Configuration for Init_function *********************/

/* options:  I2C1:              I2C2:
             - PB6: SCL         - PB10: SCL
             - PB7: SDA         - PB11: SDA
*/
#define I2C_USED                        I2C1

/*
 * options: SUPPORTED, NOT_SUPPORTED
 */
#define SUPPORT_INTERRUPT_PIN           NOT_SUPPORTED

#if SUPPORT_INTERRUPT_PIN ==SUPPORTED
/*
 * options: EXTI_LINE_0 ... EXTI_LINE_4
 */
#define INTERRUPT_PIN_NUM               EXTI_LINE_0

/*
 * options: DIO_GPIOA, DIO_GPIOB
 */
#define INTERRUPT_PIN_PORT              DIO_GPIOA



#endif

/* options: ENABLED, DISABLED*/
#define FIFO_ALMOST_FULL_FLAG           DISABLED
#define FIFO_NEW_DATA_READY             DISABLED
#define AMPIENT_LIGHT_OVERFLOW          DISABLED
#define TEMPERATURE_FLAG                DISABLED

/*
 *  options:  SAMPLE_AVERAGING_1, 2, 4... SAMPLE_AVERAGING_32
 */
#define SAMPLE_AVERAGING                SAMPLE_AVERAGING_32

/* options: ENABLE, DISABLE*/
#define FIFO_ROLLOVER_STATE             ENABLED

/* options: 17 ... 32*/
#define FIFO_ALMOST_FULL_VALUE_FLAG     32 // Number of unread data samples

/*
 *  options: HEART_RATE, SPO2, MULTI_MODE
 */
#define MODE                            MULTI_MODE

/*
 * options:  SPO2_ADC_RANGE_2048, 4096, 8192, SPO2_ADC_RANGE_16384
 */
#define SPO2_ADC_RANGE                  SPO2_ADC_RANGE_16384

/*
 * options: SPO2_SAMPLE_RATE_50_HZ, 100, 200 ...SPO2_SAMPLE_RATE_3200_HZ
 */
#define SPO2_SAMPLE_RATE                SPO2_SAMPLE_RATE_3200_HZ

/*
 * options: ADC_RESOLUTION_15_BIT, 16, 17, ADC_RESOLUTION_18_BIT
 */
#define ADC_RESOLUTION                  ADC_RESOLUTION_18_BIT

/*
 * options: 0x00 ... 0xFF
 */
#define LED_1_PULSE_AMPLITUDE           0xFF
#define LED_2_PULSE_AMPLITUDE           0xFF

/* options: RED_LED, IR_LED, NONE */
#define SLOT_1_LED                      RED_LED
#define SLOT_2_LED                      IR_LED
#define SLOT_3_LED                      NONE
#define SLOT_4_LED                      NONE

#define TIME_OUT                        500000 // Number of counts decreased in while loop block

#endif // Oximeter_CONFIG_H
