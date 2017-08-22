#include "hcsr04.h"

void hcsr_init_right() {
	RCC_APB2PeriphClockCmd(HCSR04_RIGHT_ECHO_RCC | HCSR04_RIGHT_TRIG_RCC, ENABLE);

	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = HCSR04_RIGHT_ECHO_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(HCSR04_RIGHT_ECHO_Port, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = HCSR04_RIGHT_TRIG_Pin;
	GPIO_Init(HCSR04_RIGHT_TRIG_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = HCSR04_RIGHT_ECHO_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(HCSR04_RIGHT_ECHO_Interrupt_Port, HCSR04_RIGHT_ECHO_Interrupt_Pin);

	nvic.NVIC_IRQChannel = HCSR04_RIGHT_ECHO_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void hcsr_init_middle() {
	RCC_APB2PeriphClockCmd(HCSR04_MIDDLE_ECHO_RCC | HCSR04_MIDDLE_TRIG_RCC, ENABLE);

	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = HCSR04_MIDDLE_ECHO_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(HCSR04_MIDDLE_ECHO_Port, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = HCSR04_MIDDLE_TRIG_Pin;
	GPIO_Init(HCSR04_MIDDLE_TRIG_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = HCSR04_MIDDLE_ECHO_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(HCSR04_MIDDLE_ECHO_Interrupt_Port, HCSR04_MIDDLE_ECHO_Interrupt_Pin);

	nvic.NVIC_IRQChannel = HCSR04_MIDDLE_ECHO_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void hcsr_init_left() {
	RCC_APB2PeriphClockCmd(HCSR04_LEFT_ECHO_RCC | HCSR04_LEFT_TRIG_RCC, ENABLE);

	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = HCSR04_LEFT_ECHO_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(HCSR04_LEFT_ECHO_Port, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = HCSR04_LEFT_TRIG_Pin;
	GPIO_Init(HCSR04_LEFT_TRIG_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = HCSR04_LEFT_ECHO_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(HCSR04_LEFT_ECHO_Interrupt_Port, HCSR04_LEFT_ECHO_Interrupt_Pin);

	nvic.NVIC_IRQChannel = HCSR04_LEFT_ECHO_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void hcsr_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//hcsr_init_right();
	hcsr_init_middle();
	//hcsr_init_left();
}

void hcsr_get_dist_right() {
	GPIO_SetBits(HCSR04_RIGHT_TRIG_Port, HCSR04_RIGHT_TRIG_Pin);
}

void hcsr_get_dist_middle() {
	GPIO_SetBits(HCSR04_MIDDLE_TRIG_Port, HCSR04_MIDDLE_TRIG_Pin);
}

void hcsr_get_dist_left() {
	GPIO_SetBits(HCSR04_LEFT_TRIG_Port, HCSR04_LEFT_TRIG_Pin);
}

void hcsr_get_distance() {
	//hcsr_get_dist_right();
	hcsr_get_dist_middle();
	//hcsr_get_dist_left();

	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
}

void turn_off_trig_right() {
	GPIO_ResetBits(HCSR04_RIGHT_TRIG_Port, HCSR04_RIGHT_TRIG_Pin);
}

void turn_off_trig_middle() {
	GPIO_ResetBits(HCSR04_MIDDLE_TRIG_Port, HCSR04_MIDDLE_TRIG_Pin);
}

void turn_off_trig_left() {
	GPIO_ResetBits(HCSR04_LEFT_TRIG_Port, HCSR04_LEFT_TRIG_Pin);
}

void turn_off_triggers() {
	//turn_off_trig_right();
	turn_off_trig_middle();
	//turn_off_trig_left();
}
