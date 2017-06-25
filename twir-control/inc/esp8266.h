/*
 * esp8266.h
 *
 *  Created on: 15 kwi 2017
 *      Author: Mateusz Staszkow
 */

#ifndef ESP8266_H_
#define ESP8266_H_

#include "stm32f10x.h"

void esp_init();

void send_char(char c);

void send_string(const char* s);

int __io_putchar(int c);

#endif /* ESP8266_H_ */
