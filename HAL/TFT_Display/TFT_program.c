/********************************************************************************************/
/* Author      : Ibrahim Diab                                                               */
/* File Name   : TFT_program.c                                                              */
/* Description : Functions Implementation for TFT Display (HAL Layer)                       */
/********************************************************************************************/

#include <LIB/common_macros.h>
#include <LIB/std_types.h>

#include "../Inc/MCAL//DIO/DIO_interface.h"
#include "../Inc/MCAL//SPI/SPI_interface.h"
#include "../Inc/MCAL//STK/STK_interface.h"
#include "../Inc/MCAL//RCC/RCC_interface.h"
#include "../Inc/MCAL//NVIC/NVIC_interface.h"

#include "../Inc/HAL//TFT/TFT_interface.h"
#include "../Inc/HAL//TFT/TFT_private.h"
#include "../Inc/HAL//TFT/TFT_config.h"


void TFT_init()
{
    #if TFT_A0_PORT == DIO_GPIOA
    RCC_enableClk (RCC_APB2, RCC_GPIOA);    
    #elif TFT_A0_PORT == DIO_GPIOB
    RCC_enableClk (RCC_APB2, RCC_GPIOB);
    #endif
    
    #if TFT_RESET_PORT == DIO_GPIOA
    RCC_enableClk (RCC_APB2, RCC_GPIOA);    
    #elif TFT_RESET_PORT == DIO_GPIOB
    RCC_enableClk (RCC_APB2, RCC_GPIOB);
    #endif
    
    DIO_setPinDirection(TFT_A0_PORT    , TFT_A0_PIN    , OUTPUT_SPEED_2MHZ_PP);
    DIO_setPinDirection(TFT_RESET_PORT , TFT_RESET_PIN , OUTPUT_SPEED_2MHZ_PP);
    
    #if SPI_USED == SPI_1
    
    RCC_enableClk ( RCC_APB2, RCC_GPIOA);
    RCC_enableClk ( RCC_APB2, RCC_SPI1);
    
    DIO_setPinDirection(DIO_GPIOA , DIO_PIN5 , OUTPUT_SPEED_10MHZ_ALPP );
    DIO_setPinDirection(DIO_GPIOA , DIO_PIN7 , OUTPUT_SPEED_10MHZ_ALPP );
    
    NVIC_enableInterrupt(NVIC_SPI1);
    
    SPI1_init();
    
    #elif SPI_USED == SPI_2
    // implement code using SPI2 peripheral
    
    #endif
    
    /* Reset Pulse */
    DIO_setPinValue(TFT_RESET_PORT, TFT_RESET_PIN, HIGH);
    STK_delayMicroSec(100);
    
    DIO_setPinValue(TFT_RESET_PORT, TFT_RESET_PIN, LOW);
    STK_delayMicroSec(5);
    
    DIO_setPinValue(TFT_RESET_PORT, TFT_RESET_PIN, HIGH);
    STK_delayMicroSec(100);
    
    DIO_setPinValue(TFT_RESET_PORT, TFT_RESET_PIN, LOW);
    STK_delayMicroSec(100);
    
    DIO_setPinValue(TFT_RESET_PORT, TFT_RESET_PIN, HIGH);
    STK_delayMilliSec(120); 

    
    /* Sleep Out TFT*/
    writeCommand(TFT_CMD_SLPOUT);
    STK_delayMilliSec(150);
    
    /* Select Color mode */
    writeCommand(TFT_CMD_COLMOD);
    writeData(TFT_COLOR_STANDARD);
    
    /* Display On TFT */
    writeCommand(TFT_CMD_DISPON);
    
}


void TFT_displayImage(const uint16 *image, uint16 x_start, uint16 x_end, uint16 y_start, uint16 y_end)
{
    uint32 numPixels = ((x_end-x_start) * (y_end-y_start));

    setDisplayRegion(x_start, y_start, x_end, y_end);
    
    for(uint32 counter=0; counter <numPixels;counter++)
    {
        writeData((image[counter])>>8);             //write high byte
        writeData((uint8)(image[counter]&0xFF));    //write low byte
    }
}

void TFT_fillColor(TFTColor_t color)
{
    setDisplayRegion(0, 0, TFT_MAX_WIDTH-1, TFT_MAX_HEIGHT-1);
    
    for(uint32 counter=0; counter <TFT_TOTAL_PIXELS;counter++)
        setColor(color);
}

void TFT_drawRectangle(uint16 x, uint16 y, uint16 width, uint16 hight, TFTColor_t color)
{
    uint32 numPixels = (width * hight);
    
    setDisplayRegion(x, y, x+width-1, y+hight-1);
    
    for(uint32 counter=0; counter <numPixels;counter++)
        setColor(color);
}

void TFT_drawPixel(uint16 x, uint16 y, uint16 color)
{
    setDisplayRegion( x, y, x+1, y+1);
    setColor(color);
}


static void writeCommand(uint8 command)
{
    uint8 dummyData;
    
    DIO_setPinValue( TFT_A0_PORT, TFT_A0_PIN, LOW);
    
    #if SPI_USED == SPI_1
    SPI1_sendReceiveByteSync( command, &dummyData);
    #elif SPI_USED == SPI_2
    // implement code using SPI2 peripheral
    #endif
}

static void writeData(uint8 data)
{
    uint8 dummyData;
    
    DIO_setPinValue( TFT_A0_PORT, TFT_A0_PIN, HIGH);
    
    #if SPI_USED == SPI_1
    SPI1_sendReceiveByteSync( data, &dummyData);
    #elif SPI_USED == SPI_2
    // implement code using SPI2 peripheral
    #endif
}

void setColor(TFTColor_t color)
{
    writeData((uint8)(color>>8));               //write high byte
    writeData((uint8)(color&0xFF));             //write low byte
}

void setDisplayRegion(uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end)
{
    /* Set X Address */
    writeCommand(TFT_CMD_SETXADD);
    writeData((uint8)(x_start >> 8));            // Start Address high byte
    writeData((uint8)(x_start & 0xFF));          // Start Address low byte
    writeData((uint8)(x_end >> 8));              // End Address high byte
    writeData((uint8)(x_end & 0xFF));            // End Address low byte


    /* Set Y Address */
    writeCommand(TFT_CMD_SETYADD);
    writeData((uint8)(y_start >> 8));            // Start Address high byte
    writeData((uint8)(y_start & 0xFF));          // Start Address low byte
    writeData((uint8)(y_end >> 8));              // End Address high byte
    writeData((uint8)(y_end & 0xFF));            // End Address low byte

    writeCommand(TFT_CMD_WRRAM);
}


void TFT_printChar(uint16 x, uint16 y, uint8 Char , uint8 size, TFTColor_t charColor ,TFTColor_t backgroundColor)
{

    uint8 charIndex =Char-32;   // As font array start with char equals 32 in ascll table

    setDisplayRegion(x , y , x+ (size*TFT_CHAR_WIDTH)-1, y+ (size*TFT_CHAR_HIGHT)-1 );

    for(int8 row=0; row < TFT_CHAR_HIGHT; row++)
        for(uint8 sizeLineCounter =0; sizeLineCounter < size; sizeLineCounter++)
            for(int8 column =0; column < TFT_CHAR_WIDTH; column++)
                {
                    if(TFT_font[charIndex][column] & (1<<row))
                        for(uint8 sizeColumnCounter =0; sizeColumnCounter < size; sizeColumnCounter++)
                            setColor(charColor);
                    else
                        for(uint8 sizeColumnCounter=0 ;sizeColumnCounter < size; sizeColumnCounter++)
                            setColor(backgroundColor);
                }

}

void TFT_printString(uint16 x, uint16 y, uint8* string, uint8 size, TFTColor_t charColor,TFTColor_t backgroundColor, uint8 stringDisplayWidth)
{
    uint16 charIndex =0;
    while(charIndex < strlen(string))
    {
        TFT_printChar( x, y, string[charIndex], size, charColor, backgroundColor);
        x += (size*TFT_CHAR_WIDTH);
        charIndex++;
    }

    while(charIndex < stringDisplayWidth)                               // To erase old chars
    {
        TFT_printChar( x, y, ' ' , size, charColor , backgroundColor);
        x += (size*TFT_CHAR_WIDTH);
        charIndex++;
    }
}

void TFT_printNumbers(uint16 x, uint16 y, uint32 number, uint8 size, TFTColor_t numbersColor, TFTColor_t backgroundColor, uint8 numberDisplayWidth)
{
    uint8 string[11];
    convertNumToString(number, string);
    TFT_printString( x, y, string, size, numbersColor, backgroundColor, numberDisplayWidth);
}

void TFT_drawHorizontalLine(uint16 x, uint16 y, uint8 length, TFTColor_t Color, uint8 thickness)
{
    for(uint8 counter=0; counter< thickness; counter++)
    {
        setDisplayRegion( x, y+counter, x+length, y+counter);
        for(uint16 pixels=0; pixels < length; pixels++)
            setColor(Color);
    }
}

void TFT_drawVerticalLine(uint16 x, uint16 y, uint8 length, TFTColor_t Color, uint8 thickness)
{
    for(uint8 counter=0; counter< thickness; counter++)
    {
        setDisplayRegion( x+counter, y, x+counter, y+length);
        for(uint16 pixels=0; pixels < length; pixels++)
            setColor(Color);
    }
}

void TFT_drawOutlineRectangle(uint16 x, uint16 y, uint16 width, uint16 hight, TFTColor_t color, uint8 thickness)
{
    TFT_drawVerticalLine  ( x       , y      , hight, color, thickness);
    TFT_drawHorizontalLine( x       , y+hight, width, color, thickness);
    TFT_drawVerticalLine  ( x+ width, y      , hight, color, thickness);
    TFT_drawHorizontalLine( x       , y      , width, color, thickness);
}

void convertNumToString(uint32 num, uint8* string)
{
    uint8 index=0;
    while(num)
    {
        string[index] = (num%10) + '0';
        num /=10;
        index++;
    }
    string[index] ='\0';
    reverseString(string);
    if(*string == NULL)
    {
        string[0] ='0';
        string[1] ='\0';
    }
}

void reverseString(uint8 * string)
{
    if(*string != NULL)
    {
        uint8 start =0, end =strlen(string) -1;
        while (start < end)
        {
            uint8 temp =string[start];
            string[start] = string[end];
            string[end] = temp;
            start++;
            end--;
        }
    }
}

//uint32 reverseDigits(uint32 num)
//{
//  uint32 reversed=0;
//  while(num)
//  {
//      reversed *=10;
//      reversed += num%10;
//      num /=10;
//  }
//  return reversed;
//}

uint8 strLen(uint8 *Char)
{
    uint8 len=0;
    while(*Char)
    {
        len++;
        Char++;
    }
    return len;
}

uint8 getNumDigits(uint32 num)
{
    uint8 numOfDigits=0;
    while(num)
    {
        num /=10;
        numOfDigits++;
    }
    return numOfDigits;
}
