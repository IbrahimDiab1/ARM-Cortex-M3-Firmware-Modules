/**************************************************************************************/
/* Author      : Ibrahim Diab                                                         */
/* File Name   : Oximeter_interface.h                                                 */
/* Description : Interfacing macros for Max30102 Oximeter (HAL Layer)                 */
/**************************************************************************************/

#ifndef Oximeter_INTERFACE_H
#define Oximeter_INTERFACE_H


// Max30102 sample averaging enumeration definition
typedef enum
{
      Sample_Averaging_1  = 0b000,        /* no averaging */
      Sample_Averaging_2  = 0b001,        /* averaging 2  */
      Sample_Averaging_4  = 0b010,        /* averaging 4  */
      Sample_Averaging_8  = 0b011,        /* averaging 8  */
      Sample_Averaging_16 = 0b100,        /* averaging 16 */
      Sample_Averaging_32 = 0b101,        /* averaging 32 */
}Sample_Averaging_t;


// Max30102 mode enumeration definition
typedef enum
{
    Mode_Heart_Rate = 0x02,        /* heart rate mode */
    Mode_SPO2       = 0x03,        /* spo2 mode       */
    Mode_Multi_LED  = 0x07,        /* multi-led mode  */
} Mode_t;


// Max30102 spo2 ADC range enumeration definition
typedef enum
{
    SPO2_ADC_Range_2048  = 0,        /* range 2048  */
    SPO2_ADC_Range_4096  = 1,        /* range 4096  */
    SPO2_ADC_Range_8192  = 2,        /* range 8192  */
    SPO2_ADC_Range_16384 = 3,        /* range 16384 */
} SPO2_ADC_Range_t;


// Max30102 spo2 sample rate enumeration definition
typedef enum
{
    SPO2_Sample_Rate_50_HZ   = 0,        /* 50Hz   */
    SPO2_Sample_Rate_100_HZ  = 1,        /* 100Hz  */
    SPO2_Sample_Rate_200_HZ  = 2,        /* 200Hz  */
    SPO2_Sample_Rate_400_HZ  = 3,        /* 400Hz  */
    SPO2_Sample_Rate_800_HZ  = 4,        /* 800Hz  */
    SPO2_Sample_Rate_1000_HZ = 5,        /* 1000Hz */
    SPO2_Sample_Rate_1600_HZ = 6,        /* 1600Hz */
    SPO2_Sample_Rate_3200_HZ = 7,        /* 3200Hz */
} SPO2_Sample_Rate_t;


// Max30102 ADC resolution enumeration definition
typedef enum
{
    ADC_Resolution_15_Bit = 0,        /* 15 bits */
    ADC_Resolution_16_Bit = 1,        /* 16 bits */
    ADC_Resolution_17_Bit = 2,        /* 17 bits */
    ADC_Resolution_18_Bit = 3,        /* 18 bits */
} ADC_Resolution_t;


// Max30102 led enumeration definition
typedef enum
{
    LED_None = 0,        /* time slot is disabled */
    LED_Red  = 1,        /* enable red            */
    LED_Ir   = 2,        /* enable IR             */
} led_State_t;

// Max30102 slot enumeration definition
typedef enum
{
    Slot_1 = 0,        /* slot 1 */
    Slot_2 = 1,        /* slot 2 */
    Slot_3 = 2,        /* slot 3 */
    Slot_4 = 3,        /* slot 4 */
} Max30102_slot_t;

//
typedef enum
{
    Disable =0 ,
    Enable =1
}State_t;

// Max30102 interrupt enumeration definition
typedef enum
{
    Interrupt_FIFO_FULL_EN    = 7,        /* FIFO almost full enable                    */
    Interrupt_PPG_RDY_EN      = 6,        /* new FIFO data ready enable                 */
    Interrupt_ALC_OVF_EN      = 5,        /* ambient light cancellation overflow enable */
    Interrupt_DIE_TEMP_RDY_EN = 1,        /* internal temperature enable                */
} Interrupt_Type_t;


// Max30102 Measurement mode for temperature, SPO2 and Heart Rate values.
typedef enum
{
    Disable_Measure_Mode  ,
    Single                ,
    Interval
}Measure_Mode;


/************************************* APIs Functions Prototype **************************************/

/***************************************** General Functions *****************************************/

/*
 * This function initializes the necessary registers and configurations for the Oximeter to function properly.
 * It initializes the RCC, GPIO, EXTI, NVIC, I2C, and AFIO modules and sets the necessary pins and interrupts.
 * The Oximeter is then initialized with the values chosen in the config file.
 */
void Oximeter_init();

/*
 * This function calculates the SPO2 and heart rate values using the Maxim algorithm
 * and returns them via reference parameters in synchronous way.
 */
void Oximeter_getSPO2AndHeartRateSenq(int32 * SPO2_value, int32 * HeartRate_value, uint8 Num_samples);

/*
 * This function calculates the SPO2 and heart rate values using the Maxim algorithm
 * and returns them via reference parameters in asynchronous way.
 */
void Oximeter_getSPO2AndHeartRateASenq(uint8 * SPO2_value, uint8 * HeartRate_value, uint8 Num_samples, Measure_Mode mode);

/*
 * Reads the temperature value from the MAX30102 sensor in synchronous mode
 * and returns it as a float.
 */
float32 Oximeter_readTemperatureSynq();

/*
 * Reads the temperature value from the MAX30102 sensor in Asynchronous mode
 * and returns it as a float reference variable.
 */
void Oximeter_readTemperatureAsenq(float32 * temperature, Measure_Mode mode);

/*
 * Function to set the mode of the oximeter
 * Available Modes: heart rate, spo2, multi-led mode.
 */
void Oximeter_setMode(Mode_t Mode);

/*
 * Function to shutdown or enable the oximeter
 * based on the passed state argument.
 */
void Oximeter_shutdowm(State_t state);

// Function to reset the oximeter
void Oximeter_reset();

/***************************************** Setting Functions *****************************************/

// Set interrupt enable/disable state for a specific interrupt
void Oximeter_setInterruptState(Interrupt_Type_t interrupt_ID,State_t state);

/*
 * Set the value of the FIFO almost full interrupt threshold by passing the number of unread data samples in the FIFO
 * before generating interrupt.
 */
void Oximeter_setFIFOAlmostFullValue(uint8 UnreadDataSamples);

/*
 * Set the sample averaging value for one FIFO sample.
 */
void Oximeter_setSampleAverging(Sample_Averaging_t sampleAveraging);

/*
 * Function: Oximeter_setSPO2ADCRange
 * ----------------------------
 * Sets the SPO2 ADC range
 * SPO2_ADC_Range: An enum that represents the desired ADC range (either 2048nA, 4096nA, 8192nA, or 16384nA)
 */
void Oximeter_setSPO2ADCRange(SPO2_ADC_Range_t SPO2_ADC_Range);

/*
 * Function: Oximeter_setSPO2SampleRate
 * ----------------------------
 *   Sets the SPO2 sample rate to the specified value
 *   SPO2_Sample_Rate: the desired sample rate
 */
void Oximeter_setSPO2SampleRate(SPO2_Sample_Rate_t SPO2_Sample_Rate);

/*
 * Function: Oximeter_setADCResolution
 * ----------------------------
 *   Sets the ADC resolution to the specified value
 *   ADC_Resolution: the desired ADC resolution
 */
void Oximeter_setADCResolution(ADC_Resolution_t ADC_Resolution);

/* The FIFO Write Pointer points to the location where the MAX30102 writes the next sample. */
void Oximeter_setFIFOReadPtr(uint8 ptrValue);

/* The FIFO Read Pointer points to the location from where the processor gets the next sample from the FIFO */
uint8 Oximeter_getFIFOReadPtr();

/* Change FIFO write pointer value location for the next sample */
void Oximeter_setFIFOWritePtr(uint8 ptrValue);

/* Get FIFO write pointer value location for the next sample */
uint8 Oximeter_getFIFOWritePtr();

/*
  When samples are lost. The function return the number of samples lost.
  When the FIFO is full, It saturates at 0x1F.
*/
uint8 Oximeter_getFIFOOverflowCounter();

/*
 This function controls the behavior of the FIFO when the FIFO becomes completely filled with data. If FIFO_ROLLOVER_EN
 is set (1), the FIFO address rolls over to zero and the FIFO continues to fill with new data. If the bit is not set (0),
 then the FIFO is not updated until FIFO_DATA is read or the WRITE/READ pointer positions are changed.
*/
void Oximeter_setRollOnFullState(State_t state);

/* This function sets the amplitude of the red LED pulse, Which increase sensor performance but consume power. */
void Oximeter_setRedPulseAmplitude(uint8 value);

/* This function sets the amplitude of the IR LED pulse, Which increase sensor performance but consume power. */
void Oximeter_setIRPulseAmplitude(uint8 value);

/*
 * This function sets the LED slot number and the LED state
 * for the given slot on the MAX30102 oximeter sensor.
 */
void Oximeter_setSlot(Max30102_slot_t slotNumber, led_State_t ledNumber);


#endif // Oximeter_INTERFACE_H
