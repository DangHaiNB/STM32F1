#include"st7735.h"

uint8_t _Height = 0;
uint8_t _Width = 0;
void TFT_SPI_Init(void)
{
    SPI_GPIO_Init();
    SPI_UserConfig();
    SOFT_NSS_H;
    CMD_LINE_H;

    TFT_RESET_L;
    delay(20);
    TFT_RESET_H;
    delay(50);

    TFT_WriteCMD(ST7735_SWRESET);
    delay(150);
    TFT_WriteCMD(ST7735_SLPOUT);
    delay(500);

    TFT_WriteCMD(ST7735_FRMCTR1);
    TFT_WriteData(0x01);
    TFT_WriteData(0x2c);
    TFT_WriteData(0x2d);
    
    TFT_WriteCMD(ST7735_FRMCTR2);
    TFT_WriteData(0x01);
    TFT_WriteData(0x2c);
    TFT_WriteData(0x2d);

    TFT_WriteCMD(ST7735_FRMCTR3);
    TFT_WriteData(0x01);
    TFT_WriteData(0x2c);
    TFT_WriteData(0x2d);
    TFT_WriteData(0x01);
    TFT_WriteData(0x2c);
    TFT_WriteData(0x2d);

    TFT_WriteCMD(ST7735_INVCTR);
    TFT_WriteData(0x07);

    TFT_WriteCMD(ST7735_PWCTR1);
    TFT_WriteData(0xa2);
    TFT_WriteData(0x02);
    TFT_WriteData(0x84);

    TFT_WriteCMD(ST7735_PWCTR2);
    TFT_WriteData(0xc5);

    TFT_WriteCMD(ST7735_PWCTR3);
    TFT_WriteData(0x0a);
    TFT_WriteData(0x00);
    TFT_WriteCMD(ST7735_PWCTR4);
    TFT_WriteData(0x8a);
    TFT_WriteData(0x2a);
    TFT_WriteCMD(ST7735_PWCTR5);
    TFT_WriteData(0x8a);
    TFT_WriteData(0xee);

    TFT_WriteCMD(ST7735_VMCTR1);
    TFT_WriteData(0x0e);
    TFT_WriteCMD(ST7735_INVOFF);
    TFT_WriteCMD(ST7735_MADCTL);
    TFT_WriteData(0xc0);
    TFT_WriteCMD(ST7735_COLMOD);
    TFT_WriteData(0x05);

    TFT_WriteCMD(ST7735_CASET);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x7f);
    TFT_WriteCMD(ST7735_RASET);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x9f);
//  Gama Adjustments
    TFT_WriteCMD(ST7735_GMCTRP1);
    TFT_WriteData(0x02);
    TFT_WriteData(0x1c);
    TFT_WriteData(0x07);
    TFT_WriteData(0x12);
    TFT_WriteData(0x37);
    TFT_WriteData(0x32);
    TFT_WriteData(0x29);
    TFT_WriteData(0x2d);
    TFT_WriteData(0x29);
    TFT_WriteData(0x25);
    TFT_WriteData(0x2b);
    TFT_WriteData(0x39);
    TFT_WriteData(0x00);
    TFT_WriteData(0x01);
    TFT_WriteData(0x03);
    TFT_WriteData(0x10);
    TFT_WriteCMD(ST7735_GMCTRN1);
    TFT_WriteData(0x03);
    TFT_WriteData(0x1d);
    TFT_WriteData(0x07);
    TFT_WriteData(0x06);
    TFT_WriteData(0x2e);
    TFT_WriteData(0x2c);
    TFT_WriteData(0x29);
    TFT_WriteData(0x2d);
    TFT_WriteData(0x2e);
    TFT_WriteData(0x2e);
    TFT_WriteData(0x37);
    TFT_WriteData(0x3f);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x02);
    TFT_WriteData(0x10);
    TFT_WriteCMD(ST7735_NORON);
    delay(10);
    TFT_WriteCMD(ST7735_DISPON);
    delay(100);

}

void TFT_WriteCMD(uint8_t cmd)
{
    SOFT_NSS_L;
    CMD_LINE_L;
    SPI_ReadWrite_Byte(cmd);
//    CMD_LINE_H;
    delay(1);
    SOFT_NSS_H;
}

void TFT_WriteData(uint8_t data)
{
    SOFT_NSS_L;
    CMD_LINE_H;
    SPI_ReadWrite_Byte(data);
    SOFT_NSS_H;
}
void TFT_WriteNByte(uint8_t *pData, uint16_t n)
{
    for(uint16_t i = 0; i < n; i++)
    {
        TFT_WriteData(*pData);
        pData++;
    }
}

uint8_t TFT_ReadData_Byte(void)
{
    uint8_t tmp;
    SOFT_NSS_L;
    CMD_LINE_H;
    tmp = SPI_ReadByte();
    SOFT_NSS_H;
    return tmp;
}

uint32_t TFT_ReadID(void)
{
    SOFT_NSS_L;
    CMD_LINE_L;
    uint32_t tmp_ID = 0;
    SPI_ReadWrite_Byte(TFT_ID);
    delay(5);
    CMD_LINE_H;
    SPI_ReadByte();
    tmp_ID = SPI_ReadByte();
    tmp_ID <<= 8;
    tmp_ID |= SPI_ReadByte();
    tmp_ID <<= 8;
    tmp_ID |= SPI_ReadByte();
    SOFT_NSS_H;
    return tmp_ID;
}

uint8_t Read_ColorMode(void)
{
    SOFT_NSS_L;
    CMD_LINE_L;
    uint8_t tmp;
    SPI_ReadWrite_Byte(0x0c);
    delay(5);
    CMD_LINE_H;
    SPI_ReadByte();
    tmp = SPI_ReadByte();
    SOFT_NSS_H;
    return tmp;
}

void SetScreenColor(uint16_t color)
{
    FillRectangle(0, 0, _Width, _Height, color);
}

void SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    uint8_t data[] = {0x00, x0, 0x00, x1};
    TFT_WriteCMD(ST7735_CASET);
    TFT_WriteNByte(data, 4);
    data[1] = y0;
    data[3] = y1;
    TFT_WriteCMD(ST7735_RASET);
    TFT_WriteNByte(data, 4);
    TFT_WriteCMD(ST7735_RAMWR);
}
void SetRotation(uint8_t r)
{
    uint8_t tmp;
    uint8_t madctl = 0;
    tmp = r % 4;
    switch(tmp)
    {
        case 0:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
            _Width = ST7735_WIDTH;
            _Height = ST7735_HEIGHT;
            break;
        case 1:
            madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
            _Width = ST7735_HEIGHT;
            _Height = ST7735_WIDTH;
            break;
        case 2:
            madctl = ST7735_MADCTL_RGB;
            _Width = ST7735_WIDTH;
            _Height = ST7735_HEIGHT;
            break;
        case 3:
            madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
            _Width = ST7735_HEIGHT;
            _Height = ST7735_WIDTH;
            break;
    }
    TFT_WriteCMD(ST7735_MADCTL);
    TFT_WriteData(madctl);
}
void FillRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colorRec)
{
    SetAddrWindow(x, y, x+w-1, y+h-1);
    uint8_t dataColor[] = {(colorRec & 0xff00) >> 8, (colorRec & 0x00ff)};
    for(y = h; y >0; y--)
    {
        for(x = w; x > 0; x--)
        {
            TFT_WriteNByte(dataColor, 2);
        }
    }
}

void DrawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint16_t *pData)
{
    SetAddrWindow(x, y, x+width-1, y+height-1);
    for(int i = 0; i < width * height; i++)
    {
        uint8_t dataColor[] = {(*pData & 0xff00) >> 8, (*pData & 0x00ff)};
        TFT_WriteNByte(dataColor, 2);
        pData++;
    }
}

void TFT_WriteChar(uint8_t x, uint8_t y, char ch, FontDef Font, uint16_t color, uint16_t BGcolor)
{
    uint16_t i,j,tmp;
    SetAddrWindow(x, y, x+Font.width-1, y+Font.height-1);
    for(i = 0; i < Font.height; i++)
    {
        tmp = Font.data[(ch - 32) * Font.height + i];
        for(j = 0; j < Font.width; j++)
        {
            if((tmp << j) & 0x8000)
            {
                uint8_t colorData[] = {(color & 0xff00) >> 8, (color & 0x00ff)};
                TFT_WriteNByte(colorData, 2);
            }
            else
            {
                uint8_t colorData[] = {(BGcolor & 0xff00) >> 8, (BGcolor & 0x00ff)};
                TFT_WriteNByte(colorData, 2);
            }
        }
    }

}

void TFT_WriteString(uint8_t x, uint8_t y, char *str, FontDef Font, uint16_t color, uint16_t BGcolor)
{
    while(*str)
    {
        TFT_WriteChar(x, y, *str, Font, color, BGcolor);
        x += Font.width;
        str++;
    }
}



