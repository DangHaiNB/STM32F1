#ifndef __SPI_H
#define __SPI_H

#include"stm32f10x.h"

#define     SPI_MOSI        GPIO_Pin_7
#define     SPI_SCL         GPIO_Pin_5
#define     SPI_MISO        GPIO_Pin_6

#define     SPI_PORT        GPIOA
#define     SOFT_NSS        GPIO_Pin_2
#define     CMD_LINE        GPIO_Pin_3
#define     TFT_RESET       GPIO_Pin_1


void SPI_GPIO_Init(void);
void SPI_UserConfig(void);
uint8_t SPI_ReadWrite_Byte(uint8_t data);
uint8_t SPI_ReadByte(void);







#endif








