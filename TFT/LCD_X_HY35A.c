/*********************************************************************
* SEGGER Microcontroller GmbH &amp; Co. KG *
* Solutions for real time microcontroller applications *
**********************************************************************
* *
* (c) 1996 - 2011 SEGGER Microcontroller GmbH &amp; Co. KG *
* *
* Internet: www.segger.com Support: support@segger.com *
* *
**********************************************************************

** emWin V5.12 - Graphical user interface for embedded applications **
All Intellectual Property rights in the Software belongs to SEGGER.
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with the following terms:

The software has been licensed to ARM LIMITED whose registered office
is situated at 110 Fulbourn Road, Cambridge CB1 9NJ, England solely
for the purposes of creating libraries for ARM7, ARM9, Cortex-M
series, and Cortex-R4 processor-based devices, sublicensed and
distributed as part of the MDK-ARM Professional under the terms and
conditions of the End User License supplied with the MDK-ARM
Professional.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File : LCD_X_16BitIF_STM32.c
Purpose : Port routines for STM32 16-bit Interface
----------------------------------------------------------------------
*/

#include &quot; GUI.h &quot;

/*********************************************************************
*
* Hardware configuration
*
**********************************************************************
*/

#include &amp; lt; stm32f10x.h &amp; gt;

#ifndef FSMC_Bank1_NORSRAM4 /* defined in ST Peripheral Library */
#define FSMC_Bank1_NORSRAM4 ((uint32_t)0x00000006)
#endif

/* Note: LCD /CS is CE4 - Bank 4 of NOR/SRAM Bank 1~4 */
#define LCD_BASE (0x60000000UL | 0x0C000000UL)
#define LCD_REG16 (*((volatile U16 *)(LCD_BASE )))
#define LCD_DAT16 (*((volatile U16 *)(LCD_BASE+2)))

/*********************************************************************
*
* Exported code
*
*********************************************************************
*/
/*********************************************************************
*
* LCD_X_Init
*
* Purpose:
* This routine should be called from your application program
* to set port pins to their initial values
*/
void LCD_X_Init(void) {

/* Configure the LCD Control pins --------------------------------------------*/
RCC- &amp; gt; APB2ENR |= 0x000001ED; /* enable GPIOA,D..G, AFIO clock */

/* PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE) */
GPIOD- &amp; gt; CRL &amp; = ~0x00FF00FF; /* clear Bits */
GPIOD- &amp; gt; CRL |= 0x00BB00BB; /* alternate function output Push-pull 50MHz */
/* PD.08(D13), PD.09(D14), PD.10(D15), PD.14(D0), PD.15(D1) */
GPIOD- &amp; gt; CRH &amp; = ~0xFF000FFF; /* clear Bits */
GPIOD- &amp; gt; CRH |= 0xBB000BBB; /* alternate function output Push-pull 50MHz */

/*PE.03 (A19),PE.04 (A20) PE.05 (A21),PE.06 (A22) PE.07(D4) */
GPIOE- &amp; gt; CRL &amp; = ~0xFFFFF000; /* clear Bits */
GPIOE- &amp; gt; CRL |= 0xBBBBB000; /* alternate function output Push-pull 50MHz */

/* PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10), PE.14(D11), PE.15(D12) */
GPIOE- &amp; gt; CRH &amp; = ~0xFFFFFFFF; /* clear Bits */
GPIOE- &amp; gt; CRH |= 0xBBBBBBBB; /* alternate function output Push-pull 50MHz */

/* PF.00(A0 (RS)) */
GPIOF- &amp; gt; CRL &amp; = ~0x0000000F; /* clear Bits */
GPIOF- &amp; gt; CRL |= 0x0000000B; /* alternate function output Push-pull 50MHz */

/*PG.9(NE2 (FLASH/CS)) PG.10(NE3 (SRAM/CS)) PG.12(NE4 (LCD/CS)) - CE3(LCD /CS) */
GPIOG- &amp; gt; CRH &amp; = ~0x000F0FF0; /* clear Bits */
GPIOG- &amp; gt; CRH |= 0x000B0BB0; /* alternate function output Push-pull 50MHz */

/*PD.7(NCE2 (NAND/CS)) */
GPIOD- &amp; gt; CRL &amp; = ~0xF0000000; /* clear Bits */
GPIOD- &amp; gt; CRL |= 0xB0000000; /* alternate function output Push-pull 50MHz */

/* PA.08(LCD Backlight */
GPIOA- &amp; gt; BRR |= 0x00000100; /* Backlight off */
GPIOA- &amp; gt; CRH &amp; = ~0x0000000F; /* clear Bits */
GPIOA- &amp; gt; CRH |= 0x00000003; /* output Push-pull 50MHz */

/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 4 ----------------------------------------*/
RCC- &amp; gt; AHBENR |= (1 &amp; lt; &amp; lt; 8); /* enable FSMC clock */

FSMC_Bank1- &amp; gt; BTCR[FSMC_Bank1_NORSRAM4+1] = /* Bank1 NOR/SRAM timing register configuration */
(0 &amp; lt; &amp; lt; 28) | /* FSMC AccessMode A */
(0 &amp; lt; &amp; lt; 24) | /* Data Latency */
(1 &amp; lt; &amp; lt; 20) | /* CLK Division */
(0 &amp; lt; &amp; lt; 16) | /* Bus Turnaround Duration */
(3 &amp; lt; &amp; lt; 8) | /* Data SetUp Time */
(0 &amp; lt; &amp; lt; 4) | /* Address Hold Time */
(1 &amp; lt; &amp; lt; 0); /* Address SetUp Time */
FSMC_Bank1- &amp; gt; BTCR[FSMC_Bank1_NORSRAM4 ] = /* Control register */
(0 &amp; lt; &amp; lt; 19) | /* Write burst disabled */
(0 &amp; lt; &amp; lt; 15) | /* Async wait disabled */
(0 &amp; lt; &amp; lt; 14) | /* Extended mode disabled */
(0 &amp; lt; &amp; lt; 13) | /* NWAIT signal is disabled */
(1 &amp; lt; &amp; lt; 12) | /* Write operation enabled */
(0 &amp; lt; &amp; lt; 11) | /* NWAIT signal is active one data cycle before wait state */
(0 &amp; lt; &amp; lt; 10) | /* Wrapped burst mode disabled */
(1 &amp; lt; &amp; lt; 9) | /* Wait signal polarity active low */
(0 &amp; lt; &amp; lt; 8) | /* Burst access mode disabled */
(1 &amp; lt; &amp; lt; 4) | /* Memory data bus width is 16 bits */
(0 &amp; lt; &amp; lt; 2) | /* Memory type is SRAM */
(0 &amp; lt; &amp; lt; 1) | /* Address/Data Multiplexing disable */
(1 &amp; lt; &amp; lt; 0); /* Memory Bank enable */

GPIOA- &amp; gt; BSRR = 0x00000100; /* Turn backlight on */
}

/*********************************************************************
*
* LCD_X_Write00_16
*
* Purpose:
* Write to controller, with A0 = 0
*/
void LCD_X_Write00_16(U16 c) {
LCD_REG16 = c;
}

/*********************************************************************
*
* LCD_X_Write01_16
*
* Purpose:
* Write to controller, with A0 = 1
*/
void LCD_X_Write01_16(U16 c) {
LCD_DAT16 = c;
}

/*********************************************************************
*
* LCD_X_Read01_16
*
* Purpose:
* Read from controller, with A0 = 1
*/
u16 LCD_X_Read01_16(void) {
return (LCD_DAT16);
}

/*********************************************************************
*
* LCD_X_WriteM01_16
*
* Purpose:
* Write multiple bytes to controller, with A0 = 1
*/
void LCD_X_WriteM01_16(U16 * pData, int NumWords) {
for (; NumWords; NumWords--) {
LCD_DAT16 = *pData++;
}
}

/*********************************************************************
*
* LCD_X_ReadM01_16
*
* Purpose:
* Read multiple bytes from controller, with A0 = 1
*/
void LCD_X_ReadM01_16(U16 * pData, int NumWords) {
U16 r, g, b, p;
// LCD_REG16=0x2E;
p= LCD_DAT16;
b = p &amp; 0x1f;
g = (p &amp; gt; &amp; gt; 5) &amp; 0x3f;
r = (p &amp; gt; &amp; gt; 11);
*pData++=(r &amp; lt; &amp; lt; 11) | (g &amp; lt; &amp; lt; 2);
*pData++=(b &amp; lt; &amp; lt; 11) | (r &amp; lt; &amp; lt; 3);
*pData++=(g &amp; lt; &amp; lt; 10) | (b &amp; lt; &amp; lt; 3);
}

/*************************** End of file ****************************/