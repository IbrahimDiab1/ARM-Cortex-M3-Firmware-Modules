/**************************************************************************************/
/* Author      : Ibrahim Diab                                                         */
/* File Name   : TFT_interface.h                                                      */
/* Description : Interfacing macros for TFT Display (HAL Layer)                       */
/**************************************************************************************/


#ifndef TFT_INTERFACE_H
#define TFT_INTERFACE_H

#include <LIB/std_types.h>

// RGB565 color Standard
// Colors Reference: https://github.com/newdigate/rgb565_colors
typedef enum {
    TFT_COLOR_BLACK       = 0x0000,
    TFT_COLOR_RED         = 0xF800,
    TFT_COLOR_GREEN       = 0x07E0,
    TFT_COLOR_BLUE        = 0x001F,
    TFT_COLOR_Light_blue  = 0xAEBC,
    TFT_COLOR_CYAN        = 0x07FF,
    TFT_COLOR_Tufts_Blue  = 0x447B,
    TFT_COLOR_YELLOW      = 0xFFE0,
    TFT_COLOR_WHITE       = 0xFFFF
} TFTColor_t;


void TFT_init();
void TFT_displayImage(const uint16 *image, uint16 x_start, uint16 x_end, uint16 y_start, uint16 y_end);
void TFT_fillColor(TFTColor_t color);
void TFT_printChar(uint16 x, uint16 y, uint8 Char , uint8 size, TFTColor_t charColor ,TFTColor_t backgroundColor);
void TFT_printString(uint16 x, uint16 y, uint8* string, uint8 size, TFTColor_t charColor,TFTColor_t backgroundColor, uint8 stringDisplayWidth);
void TFT_printNumbers(uint16 x, uint16 y, uint32 number, uint8 size, TFTColor_t numbersColor, TFTColor_t backgroundColor, uint8 numberDisplayWidth);
void TFT_drawVerticalLine(uint16 x, uint16 y, uint8 length, TFTColor_t Color, uint8 thickness);
void TFT_drawHorizontalLine(uint16 x, uint16 y, uint8 length, TFTColor_t Color, uint8 thickness);
void TFT_drawRectangle(uint16 x, uint16 y, uint16 width, uint16 hight, TFTColor_t color);
void TFT_drawOutlineRectangle(uint16 x, uint16 y, uint16 width, uint16 hight, TFTColor_t color, uint8 thickness);
void TFT_drawPixel(uint16 x, uint16 y, TFTColor_t color);

#endif // TFT_INTERFACE_H
