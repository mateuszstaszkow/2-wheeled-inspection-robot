#include "encoder.h"

void left_encoder_init() {
	RCC_APB2PeriphClockCmd(ENCODER_LEFT_RCC_Port | ENCODER_LEFT_RCC_Special, ENABLE);
	
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = ENCODER_LEFT_B_Pin | ENCODER_LEFT_A_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(ENCODER_LEFT_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = ENCODER_LEFT_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(ENCODER_LEFT_Interrupt_Port, ENCODER_LEFT_Interrupt_Pin);
	
	nvic.NVIC_IRQChannel = ENCODER_LEFT_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void right_encoder_init() {
	RCC_APB2PeriphClockCmd(ENCODER_RIGHT_RCC_Port | ENCODER_RIGHT_RCC_Special, ENABLE);
	
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = ENCODER_RIGHT_B_Pin | ENCODER_RIGHT_A_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(ENCODER_RIGHT_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = ENCODER_RIGHT_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(ENCODER_RIGHT_Interrupt_Port, ENCODER_RIGHT_Interrupt_Pin);

	nvic.NVIC_IRQChannel = ENCODER_RIGHT_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void encoder_init() {
	left_encoder_init();
	right_encoder_init();
}
