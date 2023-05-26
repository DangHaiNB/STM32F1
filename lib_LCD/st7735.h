#ifndef __ST7735_H
#define __ST7735_H

#include"spi.h"
#include"delay1.h"
#include"fonts.h"

#define SOFT_NSS_H      GPIO_SetBits(SPI_PORT, GPIO_Pin_2)
#define SOFT_NSS_L      GPIO_ResetBits(SPI_PORT, GPIO_Pin_2)
#define CMD_LINE_H      GPIO_SetBits(SPI_PORT, GPIO_Pin_3)
#define CMD_LINE_L      GPIO_ResetBits(SPI_PORT, GPIO_Pin_3)
#define TFT_RESET_H     GPIO_SetBits(SPI_PORT, GPIO_Pin_1)
#define TFT_RESET_L     GPIO_ResetBits(SPI_PORT, GPIO_Pin_1)

#define TFT_ID          (uint8_t)0x04
#define SOFT_RE         (uint8_t)0x01

#define ST7735_WIDTH        128
#define ST7735_HEIGHT       160

/*      ST7735 REGISTERS        */
#define ST7735_MADCTL_MY  (uint8_t)0x80
#define ST7735_MADCTL_MX  (uint8_t)0x40
#define ST7735_MADCTL_MV  (uint8_t)0x20
#define ST7735_MADCTL_ML  (uint8_t)0x10
#define ST7735_MADCTL_RGB (uint8_t)0x00
#define ST7735_MADCTL_BGR (uint8_t)0x08
#define ST7735_MADCTL_MH  (uint8_t)0x04

#define ST7735_NOP     (uint8_t)0x00
#define ST7735_SWRESET (uint8_t)0x01
#define ST7735_RDDID   (uint8_t)0x04
#define ST7735_RDDST   (uint8_t)0x09

#define ST7735_SLPIN   (uint8_t)0x10
#define ST7735_SLPOUT  (uint8_t)0x11
#define ST7735_PTLON   (uint8_t)0x12
#define ST7735_NORON   (uint8_t)0x13

#define ST7735_INVOFF  (uint8_t)0x20
#define ST7735_INVON   (uint8_t)0x21
#define ST7735_DISPOFF (uint8_t)0x28
#define ST7735_DISPON  (uint8_t)0x29
#define ST7735_CASET   (uint8_t)0x2A
#define ST7735_RASET   (uint8_t)0x2B
#define ST7735_RAMWR   (uint8_t)0x2C
#define ST7735_RAMRD   (uint8_t)0x2E

#define ST7735_PTLAR   (uint8_t)0x30
#define ST7735_COLMOD  (uint8_t)0x3A
#define ST7735_MADCTL  (uint8_t)0x36

#define ST7735_FRMCTR1 (uint8_t)0xB1
#define ST7735_FRMCTR2 (uint8_t)0xB2
#define ST7735_FRMCTR3 (uint8_t)0xB3
#define ST7735_INVCTR  (uint8_t)0xB4
#define ST7735_DISSET5 (uint8_t)0xB6

#define ST7735_PWCTR1  (uint8_t)0xC0
#define ST7735_PWCTR2  (uint8_t)0xC1
#define ST7735_PWCTR3  (uint8_t)0xC2
#define ST7735_PWCTR4  (uint8_t)0xC3
#define ST7735_PWCTR5  (uint8_t)0xC4
#define ST7735_VMCTR1  (uint8_t)0xC5

#define ST7735_RDID1   (uint8_t)0xDA
#define ST7735_RDID2   (uint8_t)0xDB
#define ST7735_RDID3   (uint8_t)0xDC
#define ST7735_RDID4   (uint8_t)0xDD

#define ST7735_PWCTR6  (uint8_t)0xFC

#define ST7735_GMCTRP1 (uint8_t)0xE0
#define ST7735_GMCTRN1 (uint8_t)0xE1

// Color definitions
#define	BLACK        (uint16_t)0x0000
#define	BLUE         (uint16_t)0x001F
#define	RED          (uint16_t)0xF800
#define	GREEN        (uint16_t)0x07E0
#define CYAN         (uint16_t)0x07FF
#define MAGENTA      (uint16_t)0xF81F
#define YELLOW       (uint16_t)0xFFE0
#define WHITE        (uint16_t)0xFFFF

extern uint8_t _Height;
extern uint8_t _Width;

void TFT_SPI_Init(void);
void TFT_WriteCMD(uint8_t cmd);
uint8_t TFT_ReadData_Byte(void);
uint32_t TFT_ReadID(void);
void TFT_WriteData(uint8_t data);
uint8_t Read_ColorMode(void);
void SetScreenColor(uint16_t color);
void TFT_WriteNByte(uint8_t *pData, uint16_t n);
void SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void FillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colorRec);
void DrawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint16_t *pData);
void TFT_WriteChar(uint8_t x, uint8_t y, char ch, FontDef Font, uint16_t color, uint16_t BGcolor);
void TFT_WriteString(uint8_t x, uint8_t y, char *str, FontDef Font, uint16_t color, uint16_t BGcolor);
void SetRotation(uint8_t r);



#endif
