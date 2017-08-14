/*
 * esp8266.c
 *
 *  Created on: 15 kwi 2017
 *      Author: Mateusz Staszkow
 */

#include "stm32f10x.h"

void send_char(char c)
{
	 while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	 USART_SendData(USART3, c);
}

void send_string(const char* s)
{
	 while (*s)
	 send_char(*s++);
}

int __io_putchar(int c)
{
	send_char(c);
	return c;
}

void esp_init()
{
	 GPIO_InitTypeDef gpio;
	 USART_InitTypeDef uart;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	 //TX
	 GPIO_StructInit(&gpio);
	 gpio.GPIO_Pin = GPIO_Pin_10;
	 gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_Init(GPIOB, &gpio);

	 //RX
	 gpio.GPIO_Pin = GPIO_Pin_11;
	 gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOB, &gpio);

	 USART_StructInit(&uart);
	 uart.USART_BaudRate = 115200;
	 USART_Init(USART3, &uart);

//	 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	 NVIC_EnableIRQ(USART3_IRQn);

	 USART_Cmd(USART3, ENABLE);
}
