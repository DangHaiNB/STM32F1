/****************************************Copyright (c)**************************************************
**
** http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name: GLCD.h
** Descriptions: SSD1963 ²Ù×÷º¯Êý¿â
**
**------------------------------------------------------------------------------------------------------
** Created by: AVRman
** Created date: 2011-2-23
** Version: 1.0
** Descriptions: The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/

#ifndef __GLCD_H
#define __GLCD_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "SSD1963_CMD.h"

/* Private define ------------------------------------------------------------*/
#define USE_16BIT_PMP
#define LCD_REG (*((volatile unsigned short *) 0x6C000000)) /* RS = 0 */
#define LCD_RAM (*((volatile unsigned short *) 0x6C000002)) /* RS = 1 */
/*********************************************************************
* Overview: Horizontal and vertical display resolution
* (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_RESOLUTION 320
#define DISP_VER_RESOLUTION 240

/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
* (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_PULSE_WIDTH 20 /* 20 */
#define DISP_HOR_BACK_PORCH 51 /* 48 */
#define DISP_HOR_FRONT_PORCH 20 /* 20 */

/*********************************************************************
* Overview: Vertical synchronization timing in lines
* (from the glass datasheet).
*********************************************************************/
#define DISP_VER_PULSE_WIDTH 2 /* 2 */
#define DISP_VER_BACK_PORCH 12 /* 16 */
#define DISP_VER_FRONT_PORCH 4 /* 4 */

/*********************************************************************
* Definition for SPI interface for HIMAX 8238-A relevant to hardware
* layout; Hardware dependent!
*********************************************************************/
#define GPIO3 3
#define GPIO2 2
#define GPIO1 1
#define GPIO0 0
#define LCD_RESET (1 &amp; lt; &amp; lt; GPIO3) /* LCD Reset signal (Reset for display panel, NOT ssd1963) */
#define LCD_SPENA (1 &amp; lt; &amp; lt; GPIO0) /* SPI EN signal */
#define LCD_SPCLK (1 &amp; lt; &amp; lt; GPIO1) /* SPI CLK */
#define LCD_SPDAT (1 &amp; lt; &amp; lt; GPIO2) /* SPI DATA */
#define RGB565CONVERT(red, green, blue) (int) (((red &amp; gt; &amp; gt; 3) &amp; lt; &amp; lt; 11) | ((green &amp; gt; &amp; gt; 2) &amp; lt; &amp; lt; 5) | (blue &amp; gt; &amp; gt; 3))

/* Private function prototypes -----------------------------------------------*/

void LCD_Initializtion(void);
void LCD_SetBacklight(uint8_t intensity);
void LCD_WriteCommand(uint16_t cmd);
void LCD_WriteData(uint16_t data);
uint16_t LCD_ReadData(void);
#endif

/*********************************************************************************************************
END FILE
*********************************************************************************************************/