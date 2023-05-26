/*
**--------------File Info-------------------------------------------------------------------------------
** File name: GLCD.c
** Descriptions: SSD1963 for emWin
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

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h"



//#define LCD_REG (*((volatile unsigned short *) 0x6C000000)) /* RS = 0 */
//#define LCD_RAM (*((volatile unsigned short *) 0x6C000002)) /* RS = 1 */




/*******************************************************************************
* Function Name : LCD_WriteReg
* Description : controller command
* Input : - cmd: writes command.
* Output : None
* Return : None
* Attention : None
*******************************************************************************/
static __inline void LCD_WriteCommand(uint16_t cmd)
{
/* Write cmd */
LCD_REG = cmd;
}

/*******************************************************************************
* Function Name : LCD_WriteRAM
* Description : Writes to the LCD RAM.
* Input : - data: the pixel color in RGB mode (5-6-5).
* Output : None
* Return : None
* Attention : None
*******************************************************************************/
static __inline void LCD_WriteData(uint16_t data)
{
/* Write 16-bit data */
LCD_RAM = data;
}

/*******************************************************************************
* Function Name : LCD_ReadRAM
* Description : Reads the LCD RAM.
* Input : None
* Output : None
* Return : LCD RAM Value.
* Attention : None
*******************************************************************************/
static __inline uint16_t LCD_ReadData(void)
{
/* Read 16-bit data */
return LCD_RAM;
}

/*******************************************************************************
* Function Name : SSD1963_SSD1963_GPIO_WR
* Description : Set a GPIO pin to state high(1) or low(0)
* Input : - pin: LCD_RESET or LCD_SPENA or LCD_SPCLK or LCD_SPDAT
* - state: 0 for low and 1 for high
* Output : None
* Return : None
* Attention : Set the GPIO pin an output prior using this function
*******************************************************************************/
static void SSD1963_GPIO_WR(uint8_t pin, uint8_t state)
{
static uint8_t _gpioStatus = 0; /* ssd1963 specific */

if( state )
{
_gpioStatus = _gpioStatus | pin;
}
else
{
_gpioStatus = _gpioStatus &amp; ( ~pin );
}

LCD_WriteCommand(0xBA); /* Set GPIO value */
LCD_WriteData(_gpioStatus);
}

/*******************************************************************************
* Function Name : SSD1963_SPI_Write
* Description : SPI Write
* Input : - byte: data
* Output : None
* Return : None
* Attention : GPIO pins for the SPI port set all output prior to
* using this function
*******************************************************************************/
static void SSD1963_SPI_Write(uint8_t byte)
{
uint8_t bit_ctr;

for( bit_ctr = 0; bit_ctr &amp; lt; 8; bit_ctr++ )
{
if( byte &amp; 0x80 )
{
SSD1963_GPIO_WR(LCD_SPDAT,1);
}
else
{
SSD1963_GPIO_WR(LCD_SPDAT,0);
}
SSD1963_GPIO_WR(LCD_SPCLK,0);
SSD1963_GPIO_WR(LCD_SPCLK,1);
byte = ( byte &amp; lt; &amp; lt; 1 );
}
}

/*******************************************************************************
* Function Name : SSD1963_SPI_WriteReg
* Description : write reg
* Input : - reg:
* - cmd:
* Output : None
* Return : None
* Attention : GPIO pins for the SPI port set all output prior to
* using this function
*******************************************************************************/
static void SSD1963_SPI_WriteReg(uint8_t reg, uint16_t cmd)
{
SSD1963_GPIO_WR(LCD_SPENA,0);
cmd = ( (reg &amp; lt; &amp; lt; 10) | (1 &amp; lt; &amp; lt; 9) | cmd );
SSD1963_SPI_Write( ( (uint8_t) (cmd) &amp; gt; &amp; gt; 8) );
SSD1963_SPI_Write( ( (uint8_t) cmd) );
SSD1963_GPIO_WR(LCD_SPENA,1);
}

/*******************************************************************************
* Function Name : delay_ms
* Description : Delay Time
* Input : - cnt: Delay Time
* Output : None
* Return : None
* Return : None
* Attention : None
*******************************************************************************/
static void delay_ms(uint16_t ms)
{
uint16_t i,j;
for( i = 0; i &amp; lt; ms; i++ )
{
for( j = 0; j &amp; lt; 1141; j++ );
}
}

/*******************************************************************************
* Function Name : LCD_Initializtion
* Description : SSD1963 Resets
* Input : None
* Output : None
* Return : None
* Attention : None
*******************************************************************************/
void LCD_Initializtion(void)
{

/* Set MN(multipliers) of PLL, VCO = crystal freq * (N+1) */
/* PLL freq = VCO/M with 250MHz &amp; lt; VCO &amp; lt; 800MHz */
/* The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq */
LCD_WriteCommand(0xE2); /* Set PLL with OSC = 10MHz (hardware) */
/* Multiplier N = 35, VCO ( &amp; gt; 250MHz)= OSC*(N+1), VCO = 360MHz */
LCD_WriteData(0x23);
LCD_WriteData(0x02); /* Divider M = 2, PLL = 360/(M+1) = 120MHz */
LCD_WriteData(0x54); /* Validate M and N values */

LCD_WriteCommand(0xE0); /* Start PLL command */
LCD_WriteData(0x01); /* enable PLL */

delay_ms(10); /* wait stablize */
LCD_WriteCommand(0xE0); /* Start PLL command again */
LCD_WriteData(0x03); /* now, use PLL output as system clock */
/* once PLL locked (at 120MHz), the data hold time set shortest */
LCD_WriteCommand(0x01); /* Soft reset */
delay_ms(10);

/* Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously */
/* Typical DCLK for TYX350TFT320240 is 6.5MHz in 24 bit format */
/* 6.5MHz = 120MHz*(LCDC_FPR+1)/2^20 */
/* LCDC_FPR = 56796 (0x00DDDC) */
LCD_WriteCommand(0xE6);
LCD_WriteData(0x00);
LCD_WriteData(0xDD);
LCD_WriteData(0xDC);

/* Set panel mode, varies from individual manufacturer */
LCD_WriteCommand(0xB0);

LCD_WriteData(0x20); /* set 24-bit 3.5 &quot; TFT Panel */
LCD_WriteData(0x00); /* set Hsync+Vsync mode */
LCD_WriteData((DISP_HOR_RESOLUTION-1) &amp; gt; &amp; gt; 8 &amp; 0x07); /* Set panel size */
LCD_WriteData((DISP_HOR_RESOLUTION-1) &amp; 0xff);
LCD_WriteData((DISP_VER_RESOLUTION-1) &amp; gt; &amp; gt; 8 &amp; 0x07);
LCD_WriteData((DISP_VER_RESOLUTION-1) &amp; 0xff);
LCD_WriteData(0x00); /* RGB sequence */

/* Set horizontal period */
LCD_WriteCommand(0xB4);

#define HT ( DISP_HOR_RESOLUTION + DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH + DISP_HOR_FRONT_PORCH )
LCD_WriteData((HT-1) &amp; gt; &amp; gt; 8);
LCD_WriteData(HT-1);

#define HPS ( DISP_HOR_PULSE_WIDTH + DISP_HOR_BACK_PORCH )
LCD_WriteData((HPS-1) &amp; gt; &amp; gt; 8);
LCD_WriteData(HPS-1);
LCD_WriteData(DISP_HOR_PULSE_WIDTH-1);
LCD_WriteData(0x00);
LCD_WriteData(0x00);
LCD_WriteData(0x00);

/* Set vertical period */
LCD_WriteCommand(0xB6);

#define VT ( DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH + DISP_VER_FRONT_PORCH + DISP_VER_RESOLUTION )
LCD_WriteData((VT-1) &amp; gt; &amp; gt; 8);
LCD_WriteData(VT-1);

#define VSP ( DISP_VER_PULSE_WIDTH + DISP_VER_BACK_PORCH )
LCD_WriteData((VSP-1) &amp; gt; &amp; gt; 8);
LCD_WriteData(VSP-1);
LCD_WriteData(DISP_VER_PULSE_WIDTH-1);
LCD_WriteData(0x00);
LCD_WriteData(0x00);

/* Set pixel data interface */
LCD_WriteCommand(0xF0);

#ifdef USE_16BIT_PMP
LCD_WriteData(0x03); /* 16-bit(565 format) data */
#else
LCD_WriteData(0x00); /* 8-bit data for 16bpp */
#endif

LCD_WriteCommand(0xB8); /* Set all GPIOs to output, controlled by host */
LCD_WriteData(0x0f); /* Set GPIO0 as output */
LCD_WriteData(0x01); /* GPIO[3:0] used as normal GPIOs */

/* LL Reset to LCD */
SSD1963_GPIO_WR(LCD_SPENA, 1);
SSD1963_GPIO_WR(LCD_SPCLK, 1);
SSD1963_GPIO_WR(LCD_SPDAT,1);
SSD1963_GPIO_WR(LCD_RESET,1);
SSD1963_GPIO_WR(LCD_RESET,0);
delay_ms(1);
SSD1963_GPIO_WR(LCD_RESET,1);

SSD1963_SPI_WriteReg(0x00,0x07);
SSD1963_SPI_WriteReg(0x01,0x00);
SSD1963_SPI_WriteReg(0x02,0x03);
SSD1963_SPI_WriteReg(0x03,0xcc);
SSD1963_SPI_WriteReg(0x04,0x46);
SSD1963_SPI_WriteReg(0x05,0x0d);
SSD1963_SPI_WriteReg(0x06,0x00);
SSD1963_SPI_WriteReg(0x07,0x00);
SSD1963_SPI_WriteReg(0x08,0x08);
SSD1963_SPI_WriteReg(0x09,0x40);
SSD1963_SPI_WriteReg(0x0a,0x88);
SSD1963_SPI_WriteReg(0x0b,0x88);
SSD1963_SPI_WriteReg(0x0c,0x30);
SSD1963_SPI_WriteReg(0x0d,0x20);
SSD1963_SPI_WriteReg(0x0e,0x6a);
SSD1963_SPI_WriteReg(0x0f,0xa4);
SSD1963_SPI_WriteReg(0x10,0x04);
SSD1963_SPI_WriteReg(0x11,0x24);
SSD1963_SPI_WriteReg(0x12,0x24);
SSD1963_SPI_WriteReg(0x1e,0x00);
SSD1963_SPI_WriteReg(0x20,0x00);

LCD_WriteCommand(0x29); /* Turn on display; show the image on display */


LCD_SetBacklight(0xff);
}


/*******************************************************************************
* Function Name : LCD_SetBacklight
* Description : This function makes use of PWM feature of ssd1963 to adjust
* the backlight intensity.
* Input : - intensity: intensity from
* 0x00 (total backlight shutdown, PWM pin pull-down to VSS)
0xff (99% pull-up, 255/256 pull-up to VDD)
* Output : None
* Return : None
* Attention : The base frequency of PWM set to around 300Hz with PLL set to 120MHz.
* This parameter is hardware dependent.
* Backlight circuit with shutdown pin connected to PWM output of ssd1963.
*******************************************************************************/
void LCD_SetBacklight(uint8_t intensity)
{
LCD_WriteCommand(0xBE); /* Set PWM configuration for backlight control */
LCD_WriteData(0x0E); /* PWMF[7:0] = 14, PWM base freq = PLL/(256*(1+14))/256 = */
/* 122Hz for a PLL freq = 120MHz */
LCD_WriteData(intensity); /* Set duty cycle, from 0x00 (total pull-down) to 0xFF */
/* (99% pull-up , 255/256) */
LCD_WriteData(0x01); /* PWM enabled and controlled by host (mcu) */
LCD_WriteData(0x00);
LCD_WriteData(0x00);
LCD_WriteData(0x00);
}



/*********************************************************************************************************
END FILE
*********************************************************************************************************/