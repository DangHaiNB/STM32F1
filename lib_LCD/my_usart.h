#ifndef __MY_USART_H
#define __MY_USART_H

#include"stm32f10x.h"
#include<stdio.h>

void my_usart_init();
void USART_SendString(USART_TypeDef* USARTx, char *str);
uint16_t USART_ReceiveString(USART_TypeDef* USARTx);



#endif









