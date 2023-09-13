/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : Oximeter.c                                                                 */
/* Description : Functions Implementation for Max30102 Oximeter (HAL Layer)                 */
/********************************************************************************************/

/* Library includes */
#include "Std_Types.h"
#include "Common_Macros.h"

/* MCU constant parameters includes */
#include "STM32F103C6.h"

/* Peripherals includes */
#include "RCC_interface.h"
#include "DIO_interface.h"
#include "AFIO_interface.h"
#include "EXTI_interface.h"
#include "NVIC_interface.h"
#include "I2C_interface.h"

/* Module files includes */
#include "Oximeter_interface.h"
#include "Oximeter_private.h"
#include "Oximeter_config.h"
#include "Oximeter_algorithm.h"

// Array to hold the interrupt status for each possible interrupt
volatile static uint8 InterruptStatus[NUM_OF_INTERRUPT_STATUS];

// Variable to store the current operating mode
volatile static Mode_t current_mode;

// Variable to store the current ADC resolution
volatile static ADC_Resolution_t current_ADC_reslolution;

// Variable to store the number of available samples in the FIFO
volatile static uint8 availableSamples;

// Variable to store the number of samples to read in case interrupt enabled.
volatile static uint8 Samples_Measured_Number;

// Variable to store Oximeter reading values mode in case interrupt enabled.
volatile static uint8 Oximeter_Measure_mode;

// Variable to store temperature value.
volatile static float32 Temperature;

// Variable to store SPO2 value.
volatile static uint8 G_SPO2_Value;

// Variable to store Heart Rate value.
volatile static uint8 G_HeartRate_value;

// Variable to store temperature mode (interval, single, disable) in case Interrupt enabled.
volatile static float32 Temperature_mode;

// Array to hold the 32 most recent IR LED samples
static uint32 IR_buffer[32];

// Array to hold the 32 most recent red LED samples
static uint32 RED_buffer[32];



/*
 * This function initializes the necessary registers and configurations for the Oximeter to function properly.
 * It initializes the RCC, GPIO, EXTI, NVIC, I2C, and AFIO modules and sets the necessary pins and interrupts.
 * The Oximeter is then initialized with the values chosen in the config file.
 */
void Oximeter_init()
{
    uint8 registerValue=0;

    // Define I2C configuration parameters.
        I2C_config_t I2C_config =
        {
                I2C_SM_MODE,            // I2C clock speed is set to standard mode.
                I2C_MODE,               // I2C mode is set to master mode.
                I2C_7_BIT_ADDRESS_MODE, // I2C address mode is set to 7-bit addressing.
                I2C_DISABLE,            // I2C general call is disabled.
                I2C_ENABLE,             // I2C software reset is enabled.
                I2C_DISABLE,            // I2C wakeup from STOP mode is disabled.
                STRETCH_ENABLED,        // Clock stretching is enabled.
                I2C_DISABLE,            // I2C DMA is disabled.
                I2C_DISABLE,            // I2C SMBus is disabled.
                100,                    // I2C peripheral clock speed is set to 100 kHz.
                0x30                    // I2C own address is set to 0x30.
        };
    switch(I2C_USED)
    {
    case I2C1:
        // Enable clock for GPIOB and I2C1 peripherals.
        RCC_enableClk (RCC_APB2, RCC_GPIOB );
        RCC_enableClk (RCC_APB1, RCC_I2C1 );

        // Set pin directions for GPIOB pins 6 and 7 as output with 2 MHz speed and open drain.
        DIO_setPinDirection( DIO_GPIOB,  DIO_PIN6,  OUTPUT_SPEED_2MHZ_ALOD);
        DIO_setPinDirection( DIO_GPIOB,  DIO_PIN7,  OUTPUT_SPEED_2MHZ_ALOD);

        // Initialize I2C1 with the defined configuration parameters.
        I2C_init(I2C1 ,  &I2C_config );
    break;

    case I2C2:
        // Enable clock for GPIOB and I2C1 peripherals.
        RCC_enableClk (RCC_APB2, RCC_GPIOB );
        RCC_enableClk (RCC_APB1, RCC_I2C2 );

        // Set pin directions for GPIOB pins 6 and 7 as output with 2 MHz speed and open drain.
        DIO_setPinDirection( DIO_GPIOB,  DIO_PIN10,  OUTPUT_SPEED_2MHZ_ALOD);
        DIO_setPinDirection( DIO_GPIOB,  DIO_PIN11,  OUTPUT_SPEED_2MHZ_ALOD);

        // Initialize I2C1 with the defined configuration parameters.
        I2C_init(I2C2 ,  &I2C_config );
    break;
    }

#if SUPPORT_INTERRUPT_PIN == SUPPORTED

        // Enable clock for the interrupt pin port and the AFIO peripheral.
        RCC_enableClk(RCC_APB2, INTERRUPT_PIN_PORT);
        RCC_enableClk(RCC_APB2, RCC_AFIO);

        // Set pin directions for the interrupt pin as input floating.
        DIO_setPinDirection(INTERRUPT_PIN_PORT, INTERRUPT_PIN_NUM, INPUT_FLOATING);

        // Set EXTI configuration.
        AFIO_setEXTIConfig(INTERRUPT_PIN_PORT, INTERRUPT_PIN_NUM);

        // Enable EXTI interrupts and their corresponding NVIC.
        NVIC_enableInterrupt((NVIC_EXTI0 + INTERRUPT_PIN_NUM));

        // Set EXTI callback.
        EXTI_setCallBack(INTERRUPT_PIN_NUM, Oximeter_IntHandler);

        // Enable EXTI interrupts on the falling edge.
        EXTI_enableEXTI(INTERRUPT_PIN_NUM, FALLING);

#endif

        /* Initialize the oximeter with initial values chosen in the config file */


        // Configure interrupt enables 1 register
        registerValue = (FIFO_ALMOST_FULL_FLAG << 7)    |  // Set the FIFO Almost Full interrupt enable bit
                         (FIFO_NEW_DATA_READY << 6)     |  // Set the New FIFO Data Ready interrupt enable bit
                         (AMPIENT_LIGHT_OVERFLOW << 5);    // Set the Ambient Light Overflow interrupt enable bit
        writeRegister(INTR_ENABLE_1, &registerValue, 1);

        // Configure interrupt enables 2 register
        registerValue = 0;
        registerValue |= (TEMPERATURE_FLAG << 1);              // Set the Temperature interrupt enable bit
        writeRegister(INTR_ENABLE_2, &registerValue, 1);

        // Initialize FIFO write, read, and overflow counter pointers
        registerValue = 0;
        writeRegister(FIFO_WR_PTR, &registerValue, 1);         // Set the FIFO write pointer to zero
        writeRegister(OVF_COUNTER, &registerValue, 1);         // Set the FIFO overflow counter to zero
        writeRegister(FIFO_RD_PTR, &registerValue, 1);         // Set the FIFO read pointer to zero

        // Configure FIFO sample averaging, roll-over, and almost full values
        registerValue = (SAMPLE_AVERAGING << 5)     |          // Set the sample averaging value     (0-3)
                        (FIFO_ROLLOVER_STATE << 1)  |          // Set the FIFO roll-over state       (0-1)
                        ((32 - FIFO_ALMOST_FULL_VALUE_FLAG));  // Set the FIFO Almost Full value     (17-32)
        writeRegister(FIFO_CONFIG, &registerValue, 1);

        // Configure mode register
        registerValue = MODE;
        writeRegister(MODE_CONFIG, &registerValue, 1);

        // Configure SpO2 ADC range, sample rate, and resolution
        registerValue = 0;
        registerValue = (SPO2_ADC_RANGE << 5)   |              // Set the SpO2 ADC range   (0-3)
                        (SPO2_SAMPLE_RATE << 2) |              // Set the SpO2 sample rate (0-7)
                        (ADC_RESOLUTION);                      // Set the ADC resolution   (0-3)
        writeRegister(SPO2_CONFIG, &registerValue, 1);

        // Configure LED pulse amplitudes
        registerValue = LED_1_PULSE_AMPLITUDE;
        writeRegister(LED1_PA, &registerValue, 1);
        registerValue = LED_2_PULSE_AMPLITUDE;
        writeRegister(LED2_PA, &registerValue, 1);

        // Set LED slots for each LED
        Oximeter_setSlot(Slot_1, SLOT_1_LED);
        Oximeter_setSlot(Slot_2, SLOT_2_LED);
        Oximeter_setSlot(Slot_3, SLOT_3_LED);
        Oximeter_setSlot(Slot_4, SLOT_4_LED);

        // Set temperature,Spo2 and Heart Rate measurement mode to disable.
        Temperature_mode      = Disable;
        Oximeter_Measure_mode = Disable;

}

/*
 * This function calculates the SPO2 and heart rate values using the Maxim algorithm
 * and returns them via reference parameters in synchronous way.
 */
void Oximeter_getSPO2AndHeartRateSenq(int32 * SPO2_value, int32 * HeartRate_value, uint8 Num_samples)
{
    // Initialize validity flags for SPO2 and heart rate measurements
    int8 SPO2_Valid_Flag = 0, HR_Valid_Flag = 0;

    // Limit the number of samples to read to a maximum of 32
    if(Num_samples > 32)
        Num_samples = 32;

    // Read the values from the LEDs
    readLedsValues(Num_samples);

    /* Use the values read to calculate SPO2 and heart rate using a maxim algorithm */
    // parameter[in]  (reference)  IR_buffer              - IR sensor data buffer
    // parameter[in]  (value)      Num_samples            - IR sensor data buffer length
    // parameter[in]  (reference)  RED_buffer             - Red sensor data buffer
    // parameter[out] (reference)  SPO2_value             - Calculated SpO2 value
    // parameter[out] (reference)  &SPO2_Valid_Flag       - 1 if the calculated SpO2 value is valid
    // parameter[out] (reference)  HeartRate_value        - Calculated heart rate value
    // parameter[out] (reference)  &HR_Valid_Flag         - 1 if the calculated heart rate value is valid
    maxim_heart_rate_and_oxygen_saturation(IR_buffer, Num_samples, RED_buffer, SPO2_value, &SPO2_Valid_Flag, HeartRate_value, &HR_Valid_Flag);

    // If the SPO2 value is not valid, set it to "INVALID"
    if(!SPO2_Valid_Flag)
        *SPO2_value = INVALID;  // INVALID = 0xFF

    // If the heart rate value is not valid, set it to "INVALID"
    if(!HR_Valid_Flag)
        *HeartRate_value = INVALID; // INVALID = 0xFF
}


// This function reads values from the LED sensors for a given number of samples.
// It stores the values in the RED_buffer and IR_buffer arrays depending on the current mode.
void readLedsValues(uint8 numOfSamplesToRead)
{
    uint32 timeOut = TIME_OUT;      // timeout variable
    uint8 Bytes[6] = {0};           // buffer for bytes read from the device
    availableSamples = 0;           // reset the available samples count

    // Reset the read and write pointers of the FIFO buffer
    Oximeter_setFIFOReadPtr (0);
    Oximeter_setFIFOWritePtr(0);

    // Reset the values in the RED_buffer and IR_buffer arrays
    for (uint8 counter = 0; counter < numOfSamplesToRead; counter++)
    {
        RED_buffer[counter] = 0;
        IR_buffer [counter] = 0;
    }

    // Wait until the number of available samples in the buffer is equal to or greater than
    // the requested number of samples or the timeout period has elapsed.
    while (numOfSamplesToRead > availableSamples)
    {
        updateReadySamplesNum();    // Update the number of available samples in the buffer
        timeOut--;                  // Decrement the timeout counter
        if (!timeOut)               // If the timeout period has elapsed
            break;                  // Exit the loop
    }

    // Limit the available samples count to the requested number of samples
    if (availableSamples > numOfSamplesToRead)
        availableSamples = numOfSamplesToRead;

    // Read the samples from the FIFO buffer and store them in the RED_buffer and IR_buffer arrays.
    if (availableSamples >= 1)
    {
        switch (current_mode)
        {
            case Mode_Heart_Rate:

                // Read the samples from the FIFO buffer for Heart Rate mode
                for (uint16 counter = 0; counter < numOfSamplesToRead; counter++)
                {
                    readRegister(FIFO_DATA, Bytes, 3);

                    RED_buffer[counter] = (Bytes[0] & 0b11);
                    RED_buffer[counter] <<= 16;

                    RED_buffer[counter] += Bytes[1];
                    RED_buffer[counter] <<= 8;

                    RED_buffer[counter] += Bytes[2];
                    RED_buffer[counter] >>= (3 - current_ADC_reslolution);
                }
                break;

            case Mode_Multi_LED:
            case Mode_SPO2:

                // Read the samples from the FIFO buffer for Multi-LED and SpO2 modes
                for (uint16 counter = 0; counter < numOfSamplesToRead; counter++)
                {
                    readRegister(FIFO_DATA, Bytes, 6);

                    RED_buffer[counter] = (Bytes[0] & 0b11);
                    RED_buffer[counter] <<= 16;

                    RED_buffer[counter] += Bytes[1];
                    RED_buffer[counter] <<= 8;

                    RED_buffer[counter] += Bytes[2];
                    RED_buffer[counter] >>= (3 - current_ADC_reslolution);

                    IR_buffer[counter] = (Bytes[3] & 0b11);
                    IR_buffer[counter] <<= 16;

                    IR_buffer[counter] += Bytes[4];
                    IR_buffer[counter] <<= 8;

                    IR_buffer[counter] += Bytes[5];
                    IR_buffer[counter] >>= (3 - current_ADC_reslolution);
                }
                break;
        }
    }
}


// Updates the number of available samples in the FIFO buffer
void updateReadySamplesNum()
{
    // Initialize read and write pointers
    uint8 readPtr = 0, writePtr = 0;

    // Get the read and write pointers from the FIFO buffer
    writePtr = Oximeter_getFIFOWritePtr();
    readPtr  = Oximeter_getFIFOReadPtr ();

    // Calculate the number of available samples in the buffer
    if (writePtr > readPtr)
        availableSamples = writePtr - readPtr;
    else
        availableSamples = FIFO_SIZE - readPtr + writePtr;
}

// Returns the number of available samples in the FIFO buffer
uint8 Oximeter_getReadySamplesNum()
{
    // Update the number of available samples in the buffer
    updateReadySamplesNum();

    // Return the number of available samples
    return availableSamples;
}


// Set interrupt enable/disable state for a specific interrupt
void Oximeter_setInterruptState(Interrupt_Type_t interrupt_ID,State_t state)
{
     uint8 RegTemp =0;

     // Check if the interrupt type is for DIE_TEMP_RDY_EN
     if(interrupt_ID == Interrupt_DIE_TEMP_RDY_EN)
     {
        // Read the current interrupt enable register
        readRegister( INTR_ENABLE_2, &RegTemp, 1);

        // Set or clear the specific bit for the interrupt based on the state input
        if(state == Enable)
            SET_BIT(RegTemp,interrupt_ID);
        else
            CLR_BIT(RegTemp,interrupt_ID);

        // Write the modified value back to the interrupt enable register
        writeRegister( INTR_ENABLE_2, &RegTemp, 1);
     }
     else // Interrupt type is not for DIE_TEMP_RDY_EN
     {
        // Read the current interrupt enable register
        readRegister( INTR_ENABLE_1, &RegTemp, 1);

        // Set or clear the specific bit for the interrupt based on the state input
        if(state == Enable)
            SET_BIT(RegTemp,interrupt_ID);
        else
            CLR_BIT(RegTemp,interrupt_ID);

        // Write the modified value back to the interrupt enable register
        writeRegister( INTR_ENABLE_1, &RegTemp, 1);
     }
}

// Get the status of all interrupts and store them in global array (InterruptStatus).
void getInterruptStatus()
{
    uint8 RegTemp=0;

    // Read the current interrupt enable register for DIE_TEMP_RDY_EN interrupt
    readRegister( INTR_ENABLE_2, &RegTemp, 1);

    // Get the interrupt status for TEMPERATURE_READY_FLAG and store it in InterruptStatus array
    InterruptStatus[TEMPERATURE_READY_FLAG_INDEX]=GET_BIT(RegTemp,1);

    // Reset RegTemp variable to read the current interrupt enable register for other interrupts
    RegTemp=0;

    // Read the current interrupt enable register for other interrupts
    readRegister( INTR_ENABLE_1, &RegTemp, 1);

    // Get the interrupt status for FIFO_ALMOST_FULL_FLAG, FIFO_NEW_DATA_READY, AMBIENT_LIGHT_OVERFLOW and POWER_READY_FLAG and store them in InterruptStatus array
    InterruptStatus[FIFO_ALMOST_FULL_FLAG_INDEX]=  GET_BIT(RegTemp,7);
    InterruptStatus[FIFO_NEW_DATA_READY_INDEX]=    GET_BIT(RegTemp,6);
    InterruptStatus[AMBIENT_LIGHT_OVERFLOW_INDEX]= GET_BIT(RegTemp,5);
    InterruptStatus[POWER_READY_FLAG_INDEX]=       GET_BIT(RegTemp,0);
}



/*
 * Set the value of the FIFO almost full interrupt threshold by passing the number of unread data samples in the FIFO
 * before generating interrupt.
 */
void Oximeter_setFIFOAlmostFullValue(uint8 UnreadDataSamples)
{
    // Check if the value is within the acceptable range
    if(UnreadDataSamples <= 32 && UnreadDataSamples >=17 )
    {
        uint8 RegTemp=0;

        // Read the current value of the FIFO_CONFIG register
        readRegister( FIFO_CONFIG, &RegTemp, 1);

        // Calculate the desired value and modify the register accordingly
        MOD_4BIT(RegTemp, 0, (32-UnreadDataSamples));

        // Write the modified register value back to the device
        writeRegister( FIFO_CONFIG, &RegTemp, 1);
    }
}

/**
 * Set the sample averaging value for one FIFO sample.
 */
void Oximeter_setSampleAverging(Sample_Averaging_t sampleAveraging)
{
    uint8 RegTemp=0;

    // Read the current value of the FIFO_CONFIG register
    readRegister( FIFO_CONFIG, &RegTemp, 1);

    // Modify the register to set the sample averaging mode
    MOD_3BIT(RegTemp, 5, sampleAveraging);

    // Write the modified register value back to the device
    writeRegister( FIFO_CONFIG, &RegTemp, 1);
}


/*
 * Function to shutdown or enable the oximeter
 * based on the passed state argument.
 */
void Oximeter_shutdowm(State_t state)
{
    // Initialize register value to 0
    uint8 RegTemp=0;

    // Read MODE_CONFIG register and store value in RegTemp
    readRegister( MODE_CONFIG, &RegTemp, 1);

    // If state is Disable, clear bit 7 of RegTemp,
    // else set bit 7 of RegTemp
    if(state == Disable)
        CLR_BIT(RegTemp,7);
    else
        SET_BIT(RegTemp,7);

    // Write new value of RegTemp to MODE_CONFIG register
    writeRegister( MODE_CONFIG, &RegTemp, 1);
}


// Function to reset the oximeter
void Oximeter_reset()
{
    // Initialize register value to 0
    uint8 RegTemp=0;

    // Read MODE_CONFIG register and store value in RegTemp
    readRegister( MODE_CONFIG, &RegTemp, 1);

    // Set bit 6 of RegTemp to reset the oximeter
    SET_BIT(RegTemp,6);

    // Write new value of RegTemp to MODE_CONFIG register
    writeRegister( MODE_CONFIG, &RegTemp, 1);
}


/*
 * Function to set the mode of the oximeter
 * Available Modes: heart rate, spo2, multi-led mode.
 */
void Oximeter_setMode(Mode_t Mode)
{
    // Initialize register value to 0
    uint8 RegTemp=0;

    // Read MODE_CONFIG register and store value in RegTemp
    readRegister( MODE_CONFIG, &RegTemp, 1);

    // Modify bits 0-2 of RegTemp to set the mode
    MOD_3BIT(RegTemp, 0, Mode);

    // Write new value of RegTemp to MODE_CONFIG register
    writeRegister( MODE_CONFIG, &RegTemp, 1);

    // Update current mode variable to reflect the new mode
    current_mode = Mode;
}


/*
 * Function: Oximeter_setSPO2ADCRange
 * ----------------------------
 * Sets the SPO2 ADC range
 * SPO2_ADC_Range: An enum that represents the desired ADC range (either 2048nA, 4096nA, 8192nA, or 16384nA)
 */
void Oximeter_setSPO2ADCRange(SPO2_ADC_Range_t SPO2_ADC_Range)
{
    uint8 RegTemp=0;

    // Read the SPO2 configuration register
    readRegister( SPO2_CONFIG, &RegTemp, 1);

    // Set the ADC range bits
    MOD_2BIT(RegTemp, 5, SPO2_ADC_Range);

    // Write the updated value back to the register
    writeRegister( SPO2_CONFIG, &RegTemp, 1);
}


/*
 * Function: Oximeter_setSPO2SampleRate
 * ----------------------------
 *   Sets the SPO2 sample rate to the specified value
 *   SPO2_Sample_Rate: the desired sample rate
 */
void Oximeter_setSPO2SampleRate(SPO2_Sample_Rate_t SPO2_Sample_Rate)
{
    uint8 RegTemp=0;

    // Read the SPO2 configuration register
    readRegister( SPO2_CONFIG, &RegTemp, 1);

    // Set the sample rate bits
    MOD_3BIT(RegTemp, 2, SPO2_Sample_Rate);

    // Write the updated value back to the register
    writeRegister( SPO2_CONFIG, &RegTemp, 1);
}

/*
 * Function: Oximeter_setADCResolution
 * ----------------------------
 *   Sets the ADC resolution to the specified value
 *   ADC_Resolution: the desired ADC resolution
 */
void Oximeter_setADCResolution(ADC_Resolution_t ADC_Resolution)
{
    uint8 RegTemp=0;

    // Read the SPO2 configuration register
    readRegister( SPO2_CONFIG, &RegTemp, 1);

    // Set the ADC resolution bits
    MOD_2BIT(RegTemp, 0, ADC_Resolution);

    // Write the updated value back to the register
    writeRegister( SPO2_CONFIG, &RegTemp, 1);

    // Update the current ADC resolution value
    current_ADC_reslolution = ADC_Resolution;
}


// This function writes multiple bytes to the Oximeter.
void writeRegister(uint8 registerAddress, uint8 * buffer, uint8 dataLength)
{
    // Define a buffer to store the memory address and data to be written.
    uint8 bufferToSend[I2C_MAX_BUFFER_SIZE]={0};

    // Store the memory address in the buffer.
    bufferToSend[0]=(uint8)(registerAddress);

    // Store the data to be written in the buffer.
    for(uint8 i=1;i<(dataLength+1);i++)
        bufferToSend[i]=buffer[i-1];

    // Send the buffer data to the EEPROM via I2C protocol with a start condition and stop condition.
    I2C_sendData(I2C_USED ,MAX30102_ADDRESS, bufferToSend ,  (dataLength+1), START , WITH_STOP);
}


// This function reads Data from the Oximeter.
void readRegister(uint8 registerAddress, uint8 * buffer, uint8 dataLength)
{
    uint8 bufferToSend[2];

    // Store the register address to read from in the buffer
    bufferToSend[0] = (uint8)registerAddress;

    // Send the memory address to the EEPROM, followed by a repeated start signal
    // without sending a stop signal to keep the bus busy
    I2C_sendData(I2C_USED, MAX30102_ADDRESS, bufferToSend, 2, START, WITHOUT_STOP);

    // Read the specified number of bytes from the EEPROM into the buffer
    I2C_receiveData(I2C_USED, MAX30102_ADDRESS, buffer, dataLength, REPEATED_START, WITH_STOP);
}


/*
 * Reads the temperature value from the MAX30102 sensor in synchronous mode
 * and returns it as a float.
 */
float32 Oximeter_readTemperatureSynq()
{
    uint8 ptrValue = 1, tempFraction = 0;
    int8 tempInteger = 0;
    float32 temperature = 0;

    // Start a temperature reading
    writeRegister(TEMP_CONFIG, &ptrValue, 1);

    // Wait for temperature reading to complete
    readRegister(FIFO_WR_PTR, &ptrValue, 1);
    while (GET_BIT(ptrValue, 0));

    // Read the temperature value integer and fraction.
    readRegister(TEMP_INTR, (uint8 *)&tempInteger, 1);
    readRegister(TEMP_FRAC, &tempFraction, 1);

    // Extract temperature fraction and calculate the temperature value
    tempFraction &= 0xF;
    temperature = tempInteger + (tempFraction * .0625);

    return temperature;
}



/* The FIFO Write Pointer points to the location where the MAX30102 writes the next sample. */
void Oximeter_setFIFOReadPtr(uint8 ptrValue)
{
    // Check if the given pointer value is within the valid range (0 to 31) and write to FIFO_RD_PTR register
    if(ptrValue <=31)
        writeRegister( FIFO_RD_PTR, &ptrValue, 1);
}


/* The FIFO Read Pointer points to the location from where the processor gets the next sample from the FIFO */
uint8 Oximeter_getFIFOReadPtr()
{
    uint8 ptrValue=0;   // Variable to store the FIFO read pointer value
    readRegister( FIFO_RD_PTR, &ptrValue, 1);   // Read the FIFO_RD_PTR register and store in ptrValue
    return ptrValue;   // Return the read pointer value
}


/* Change FIFO write pointer value location for the next sample */
void Oximeter_setFIFOWritePtr(uint8 ptrValue)
{
    // Check if the given pointer value is within the valid range (0 to 31) and write to FIFO_WR_PTR register
    if(ptrValue <=31)
        writeRegister( FIFO_WR_PTR, &ptrValue, 1);
}


/* Get FIFO write pointer value location for the next sample */
uint8 Oximeter_getFIFOWritePtr()
{
    uint8 ptrValue=0;   // Variable to store the FIFO write pointer value
    readRegister( FIFO_WR_PTR, &ptrValue, 1);   // Read the FIFO_WR_PTR register and store in ptrValue
    return ptrValue;   // Return the write pointer value
}


/*
  When samples are lost. The function return the number of samples lost.
  When the FIFO is full, It saturates at 0x1F.
*/
uint8 Oximeter_getFIFOOverflowCounter()
{
    uint8 ptrValue=0;   // Variable to store the FIFO overflow counter value
    readRegister( OVF_COUNTER, &ptrValue, 1);   // Read the OVF_COUNTER register and store in ptrValue
    return ptrValue;   // Return the overflow counter value
}


/*
 This function controls the behavior of the FIFO when the FIFO becomes completely filled with data. If FIFO_ROLLOVER_EN
 is set (1), the FIFO address rolls over to zero and the FIFO continues to fill with new data. If the bit is not set (0),
 then the FIFO is not updated until FIFO_DATA is read or the WRITE/READ pointer positions are changed.
*/
void Oximeter_setRollOnFullState(State_t state)
{
    uint8 RegTemp=0;   // Temporary variable to store the register value

    readRegister( FIFO_CONFIG, &RegTemp, 1);   // Read the FIFO_CONFIG register

    // If the given state is Disable, clear the 4th bit of RegTemp, otherwise set it
    if(state == Disable)
        CLR_BIT(RegTemp,4);
    else
        SET_BIT(RegTemp,4);

    writeRegister( FIFO_CONFIG, &RegTemp, 1);  // Write the modified value back to the register
}

/* This function sets the amplitude of the red LED pulse, Which increase sensor performance but consume power. */
void Oximeter_setRedPulseAmplitude(uint8 value)
{
    writeRegister( LED1_PA, &value, 1);   // Write the given value to the LED1_PA register
}

/* This function sets the amplitude of the IR LED pulse, Which increase sensor performance but consume power. */
void Oximeter_setIRPulseAmplitude(uint8 value)
{
    writeRegister( LED2_PA, &value, 1);   // Write the given value to the LED2_PA register
}



/*
 * This function sets the LED slot number and the LED state
 * for the given slot on the MAX30102 oximeter sensor.
 */
void Oximeter_setSlot(Max30102_slot_t slotNumber, led_State_t ledNumber)
{
    uint8 RegTemp=0;   // Temporary variable to store the register value

    // Switch statement to set the slot and LED state based on slot number
    switch(slotNumber)
    {

    case  Slot_1:
        readRegister( MULTI_LED_CTRL1, &RegTemp, 1);   // Read the register value for MULTI_LED_CTRL1
        MOD_3BIT(RegTemp, 0, (0b111 & ledNumber));     // Modify the 3 least significant bits with the LED number
        writeRegister( MULTI_LED_CTRL1, &RegTemp, 1);  // Write the modified value back to the register
        break;

    case  Slot_2:
        readRegister( MULTI_LED_CTRL1, &RegTemp, 1);   // Read the register value for MULTI_LED_CTRL1
        MOD_3BIT(RegTemp, 4, (0b111 & ledNumber));     // Modify the 3 least significant bits with the LED number
        writeRegister( MULTI_LED_CTRL1, &RegTemp, 1);  // Write the modified value back to the register
        break;

    case  Slot_3:
        readRegister( MULTI_LED_CTRL2, &RegTemp, 1);   // Read the register value for MULTI_LED_CTRL2
        MOD_3BIT(RegTemp, 0, (0b111 & ledNumber));     // Modify the 3 least significant bits with the LED number
        writeRegister( MULTI_LED_CTRL2, &RegTemp, 1);  // Write the modified value back to the register
        break;

    case  Slot_4:
        readRegister( MULTI_LED_CTRL2, &RegTemp, 1);   // Read the register value for MULTI_LED_CTRL2
        MOD_3BIT(RegTemp, 4, (0b111 & ledNumber));     // Modify the 3 least significant bits with the LED number
        writeRegister( MULTI_LED_CTRL2, &RegTemp, 1);  // Write the modified value back to the register
        break;

    }
}

/*
 * Reads the temperature value from the MAX30102 sensor in Asynchronous mode
 * and returns it as a float reference variable.
 */
void Oximeter_readTemperatureAsenq(float32 * temperature, Measure_Mode mode)
{
    switch (mode)
    {
    case Disable_Measure_Mode :
        Oximeter_setInterruptState(Interrupt_DIE_TEMP_RDY_EN, Disable );
        Temperature_mode = Disable;
        break;

    case Single:
        Oximeter_setInterruptState(Interrupt_DIE_TEMP_RDY_EN, Enable );
        Temperature_mode = Single;
        break;

    case Interval:
        Oximeter_setInterruptState(Interrupt_DIE_TEMP_RDY_EN, Enable );
        Temperature_mode = Interval;
        break;

    }
    temperature = (float32 *)&Temperature;
}



void Oximeter_IntHandler()
{
    uint8 ptrValue = 1, tempFraction = 0;
    static uint8 Counter=0;
    int8 tempInteger = 0;

     getInterruptStatus();

     if(InterruptStatus[FIFO_ALMOST_FULL_FLAG_INDEX])
     {
         if(Oximeter_Measure_mode == Disable_Measure_Mode)
         {
                Oximeter_setInterruptState(Interrupt_FIFO_FULL_EN, Disable );
                Oximeter_setInterruptState(Interrupt_PPG_RDY_EN  , Disable );
         }
         else
         {
             read_Spo2_HR_value_Asenq();

             if(Oximeter_Measure_mode == Single)
             {
                    Oximeter_setInterruptState(Interrupt_FIFO_FULL_EN, Disable );
                    Oximeter_setInterruptState(Interrupt_PPG_RDY_EN  , Disable );
             }
         }
     }
     if(InterruptStatus[FIFO_NEW_DATA_READY_INDEX])
     {
         Counter++;
         if(Counter >=Samples_Measured_Number)
         {
             Counter =0;

             if(Oximeter_Measure_mode == Disable_Measure_Mode)
             {
                    Oximeter_setInterruptState(Interrupt_FIFO_FULL_EN, Disable );
                    Oximeter_setInterruptState(Interrupt_PPG_RDY_EN  , Disable );
             }
             else
             {
                 read_Spo2_HR_value_Asenq();

                 if(Oximeter_Measure_mode == Single)
                 {
                     Oximeter_setInterruptState(Interrupt_FIFO_FULL_EN, Disable );
                     Oximeter_setInterruptState(Interrupt_PPG_RDY_EN  , Disable );
                 }

                 if(Oximeter_Measure_mode == Interval)
                 {
                     // Reset the read and write pointers of the FIFO buffer
                     Oximeter_setFIFOReadPtr (0);
                     Oximeter_setFIFOWritePtr(0);

                     // Reset the values in the RED_buffer and IR_buffer arrays
                     for (uint8 counter = 0; counter < Samples_Measured_Number; counter++)
                        {
                            RED_buffer[counter] = 0;
                            IR_buffer [counter] = 0;
                        }
                 }
             }
         }
     }

     if(InterruptStatus[TEMPERATURE_READY_FLAG_INDEX])
     {
         // Read the temperature value integer and fraction.
         readRegister(TEMP_INTR, (uint8 *)&tempInteger, 1);
         readRegister(TEMP_FRAC, &tempFraction, 1);

         // Extract temperature fraction and calculate the temperature value
         tempFraction &= 0xF;
         Temperature = tempInteger + (tempFraction * .0625);

         if(Temperature_mode == Interval)
         {
             // Start a temperature reading
             writeRegister(TEMP_CONFIG, &ptrValue, 1);
         }
     }

}


/*
 * This function calculates the SPO2 and heart rate values using the Maxim algorithm
 * and returns them via reference parameters in asynchronous way.
 */
void Oximeter_getSPO2AndHeartRateASenq(uint8 * SPO2_value, uint8 * HeartRate_value, uint8 Num_samples, Measure_Mode mode)
{
    SPO2_value      =  (uint8 *)&G_SPO2_Value;
    HeartRate_value =  (uint8 *)&G_HeartRate_value;

    // Limit the number of samples to read to a maximum of 32
    if(Num_samples > 32)
        Num_samples = 32;
    switch(mode)
    {
    case Disable_Measure_Mode:
            Oximeter_Measure_mode = Disable_Measure_Mode;
        break;

    case Single:
            Oximeter_Measure_mode = Single;
        break;

    case Interval:
            Oximeter_Measure_mode = Interval;
        break;

    }
        if(Num_samples >= 17)
        {
            Samples_Measured_Number = Num_samples;

            // Reset the read and write pointers of the FIFO buffer
            Oximeter_setFIFOReadPtr (0);
            Oximeter_setFIFOWritePtr(0);

            // Reset the values in the RED_buffer and IR_buffer arrays
            for (uint8 counter = 0; counter < Samples_Measured_Number; counter++)
            {
                RED_buffer[counter] = 0;
                IR_buffer [counter] = 0;
            }

            Oximeter_setFIFOAlmostFullValue(Num_samples);
            Oximeter_setInterruptState(Interrupt_FIFO_FULL_EN, Enable  );
            Oximeter_setInterruptState(Interrupt_PPG_RDY_EN  , Disable );
        }
        else
        {
            Samples_Measured_Number = Num_samples;

            // Reset the read and write pointers of the FIFO buffer
            Oximeter_setFIFOReadPtr (0);
            Oximeter_setFIFOWritePtr(0);

            // Reset the values in the RED_buffer and IR_buffer arrays
            for (uint8 counter = 0; counter < Samples_Measured_Number; counter++)
            {
                RED_buffer[counter] = 0;
                IR_buffer [counter] = 0;
            }

            Oximeter_setInterruptState(Interrupt_FIFO_FULL_EN, Disable );
            Oximeter_setInterruptState(Interrupt_PPG_RDY_EN  , Enable  );
        }
}


void read_Spo2_HR_value_Asenq()
{
    uint8 Bytes[6] = {0};           // buffer for bytes read from the device

    // Initialize validity flags for SPO2 and heart rate measurements
    int8 SPO2_Valid_Flag = 0, HR_Valid_Flag = 0;

     switch (current_mode)
            {
                case Mode_Heart_Rate:

                    // Read the samples from the FIFO buffer for Heart Rate mode
                    for (uint16 counter = 0; counter < Samples_Measured_Number; counter++)
                    {
                        readRegister(FIFO_DATA, Bytes, 3);

                        RED_buffer[counter] = (Bytes[0] & 0b11);
                        RED_buffer[counter] <<= 16;

                        RED_buffer[counter] += Bytes[1];
                        RED_buffer[counter] <<= 8;

                        RED_buffer[counter] += Bytes[2];
                        RED_buffer[counter] >>= (3 - current_ADC_reslolution);
                    }
                    break;

                case Mode_Multi_LED:
                case Mode_SPO2:

                    // Read the samples from the FIFO buffer for Multi-LED and SpO2 modes
                    for (uint16 counter = 0; counter < Samples_Measured_Number; counter++)
                    {
                        readRegister(FIFO_DATA, Bytes, 6);

                        RED_buffer[counter] = (Bytes[0] & 0b11);
                        RED_buffer[counter] <<= 16;

                        RED_buffer[counter] += Bytes[1];
                        RED_buffer[counter] <<= 8;

                        RED_buffer[counter] += Bytes[2];
                        RED_buffer[counter] >>= (3 - current_ADC_reslolution);

                        IR_buffer[counter] = (Bytes[3] & 0b11);
                        IR_buffer[counter] <<= 16;

                        IR_buffer[counter] += Bytes[4];
                        IR_buffer[counter] <<= 8;

                        IR_buffer[counter] += Bytes[5];
                        IR_buffer[counter] >>= (3 - current_ADC_reslolution);
                    }
                    break;
            }

    /* Use the values read to calculate SPO2 and heart rate using a maxim algorithm */
    // parameter[in]  (reference)  IR_buffer              - IR sensor data buffer
    // parameter[in]  (value)      Num_samples            - IR sensor data buffer length
    // parameter[in]  (reference)  RED_buffer             - Red sensor data buffer
    // parameter[out] (reference)  SPO2_value             - Calculated SpO2 value
    // parameter[out] (reference)  &SPO2_Valid_Flag       - 1 if the calculated SpO2 value is valid
    // parameter[out] (reference)  HeartRate_value        - Calculated heart rate value
    // parameter[out] (reference)  &HR_Valid_Flag         - 1 if the calculated heart rate value is valid
    maxim_heart_rate_and_oxygen_saturation(IR_buffer, Samples_Measured_Number, RED_buffer, (int32*)&G_SPO2_Value, &SPO2_Valid_Flag, (int32*)&G_HeartRate_value, &HR_Valid_Flag);

    // If the SPO2 value is not valid, set it to "INVALID"
    if(!SPO2_Valid_Flag)
        G_SPO2_Value = INVALID; // INVALID = 0xFF

    // If the heart rate value is not valid, set it to "INVALID"
    if(!HR_Valid_Flag)
        G_HeartRate_value = INVALID;    // INVALID = 0xFF*/
}
