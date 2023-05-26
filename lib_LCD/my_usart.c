#include"my_usart.h"

void my_usart_init()
{
    GPIO_InitTypeDef SP9,SP10;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_USART1, ENABLE);
    SP9.GPIO_Pin = GPIO_Pin_9; 
    SP9.GPIO_Speed = GPIO_Speed_50MHz;
    SP9.GPIO_Mode = GPIO_Mode_AF_PP;
    SP10.GPIO_Pin = GPIO_Pin_10; 
    SP10.GPIO_Speed = GPIO_Speed_50MHz;
    SP10.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &SP9);
    GPIO_Init(GPIOA, &SP10);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    USART_InitTypeDef USART_Struct;
    USART_Struct.USART_BaudRate = 9600;
    USART_Struct.USART_WordLength = USART_WordLength_8b;
    USART_Struct.USART_StopBits = USART_StopBits_1;
    USART_Struct.USART_Parity = USART_Parity_No;
    USART_Struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_Struct);
    USART_Cmd(USART1, ENABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

}

void USART_SendString(USART_TypeDef* USARTx, char *str)
{
    while(*str != '\0')
    {
        USART_SendData(USART1, *str++);
//        str++;
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }

}

uint16_t USART_ReceiveString(USART_TypeDef* USARTx)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (USART_ReceiveData(USARTx));
}

/*void USART1_IRQHandler()
{
    uint16_t tmp;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        tmp = USART_ReceiveString(USART1);
        USART_SendData(USART1,tmp);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    }
}*/


int _write(int fd, char* pBuffer, int size)
{
    for(int i=0; i < size; i++)
    {
        USART_SendData(USART1, *(pBuffer + i));
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    return size;
}

// Add syscalls.c with GCC

//#ifdef __GNUC__
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
/*PUTCHAR_PROTOTYPE
{
  
    USART_SendData(USART1, (uint16_t) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}*/
/*
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};
FILE __stdout;

int _sys_exit(int x)
{
    x = x;
    return 0;
}
*/
/*int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint16_t) ch);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return (ch);
}

int fgetc(FILE *f)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USART1);
}
*/









































