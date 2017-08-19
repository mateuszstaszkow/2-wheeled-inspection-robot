/*
 * esp8266.c
 *
 *  Created on: 15 kwi 2017
 *      Author: Mateusz Staszkow
 */

#include "esp8266.h"
#include "properties.h"
#include "stm32f10x.h"

void send_char(char c)
{
	 while (USART_GetFlagStatus(ESP8266_USART, USART_FLAG_TXE) == RESET);
	 USART_SendData(ESP8266_USART, c);
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

void init_usart() {
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart;

	RCC_APB2PeriphClockCmd(ESP8266_USART_RCC_Port, ENABLE);
	RCC_APB2PeriphClockCmd(ESP8266_USART_RCC_Special, ENABLE);
	RCC_APB1PeriphClockCmd(ESP8266_USART_RCC_Periph, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = ESP8266_USART_TX_Pin;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(ESP8266_USART_Port, &gpio);

	gpio.GPIO_Pin = ESP8266_USART_RX_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(ESP8266_USART_Port, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = ESP8266_USART_Speed;
	USART_Init(ESP8266_USART, &uart);

	USART_Cmd(ESP8266_USART, ENABLE);
}

void esp_init()
{
	 init_usart();
}
