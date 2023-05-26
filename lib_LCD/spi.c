#include"spi.h"

void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |\
                           RCC_APB2Periph_SPI1 |\
                           RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStruct.GPIO_Pin = SPI_SCL | SPI_MOSI;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = SPI_MISO;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = SOFT_NSS |\
                               CMD_LINE |\
                               TFT_RESET;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(SPI_PORT, &GPIO_InitStruct);
}

void SPI_UserConfig(void)
{
    SPI_InitTypeDef SPI_InitStruct;

    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_ReadWrite_Byte(uint8_t data)
{
    volatile uint8_t retry = 0;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
    {
        retry++;
        if(retry > 250)
        {
            return 0;
        }
    }

    SPI_I2S_SendData(SPI1, data);
    retry = 0;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
    {
        retry++;
        if(retry > 250)
        {
            return 0;
        }
    }

    return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

uint8_t SPI_ReadByte(void)
{
    return SPI_ReadWrite_Byte(0x1f);
}





