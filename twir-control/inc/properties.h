#ifndef __PROPERTIES_H
#define __PROPERTIES_H

#include "stm32f10x.h"

// STM32F103RBT6 Hardware setup

#define STM32_SYSTEM_CORE_CLOCK				72000000

#define MPU6050_I2C                 		I2C1
#define MPU6050_I2C_Port            		GPIOB
#define MPU6050_I2C_RCC_Port        		RCC_APB2Periph_GPIOB
#define MPU6050_I2C_RCC_Periph      		RCC_APB1Periph_I2C1
#define MPU6050_I2C_SCL_Pin         		GPIO_Pin_6
#define MPU6050_I2C_SDA_Pin        			GPIO_Pin_7
#define MPU6050_I2C_Speed          			100000

#define ESP8266_USART						USART3
#define ESP8266_USART_Port					GPIOB
#define ESP8266_USART_RCC_Port				RCC_APB2Periph_GPIOB
#define ESP8266_USART_RCC_Periph			RCC_APB1Periph_USART3
#define ESP8266_USART_RCC_Special			RCC_APB2Periph_AFIO
#define ESP8266_USART_TX_Pin				GPIO_Pin_10
#define ESP8266_USART_RX_Pin				GPIO_Pin_11
#define ESP8266_USART_Speed					115200

#define ENCODER_LEFT_RCC_Port				RCC_APB2Periph_GPIOC
#define ENCODER_LEFT_RCC_Special			RCC_APB2Periph_AFIO
#define ENCODER_LEFT_Port					GPIOC
#define ENCODER_LEFT_A_Pin					GPIO_Pin_11
#define ENCODER_LEFT_B_Pin					GPIO_Pin_10
#define ENCODER_LEFT_Interrupt_Port			GPIO_PortSourceGPIOC
#define ENCODER_LEFT_Interrupt_Pin			GPIO_PinSource11
#define ENCODER_LEFT_Interrupt_Channel		EXTI15_10_IRQn
#define ENCODER_LEFT_Interrupt_Line			EXTI_Line11
#define ENCODER_RIGHT_RCC_Port				RCC_APB2Periph_GPIOC
#define ENCODER_RIGHT_RCC_Special			RCC_APB2Periph_AFIO
#define ENCODER_RIGHT_Port					GPIOC
#define ENCODER_RIGHT_A_Pin					GPIO_Pin_9
#define ENCODER_RIGHT_B_Pin					GPIO_Pin_8
#define ENCODER_RIGHT_Interrupt_Line		EXTI_Line9
#define ENCODER_RIGHT_Interrupt_Port		GPIO_PortSourceGPIOC
#define ENCODER_RIGHT_Interrupt_Pin			GPIO_PinSource9
#define ENCODER_RIGHT_Interrupt_Channel		EXTI9_5_IRQn

// PWM timers and channels are immutable. Definitions:
// 	left motor - TIM4/CH3 (PB.8)
// 	right motor - TIM4/CH4 (PB.9)
// Default PWM frequency 50 Hz for setup:
//  MOTOR_PWM_PRESCALER = 1280
//  MOTOR_PWM_PERIOD = 1000
#define MOTOR_PWM_PRESCALER					1280
#define MOTOR_PWM_PERIOD					1000
#define MOTOR_DIR_LEFT_A_RCC				RCC_APB2Periph_GPIOA
#define MOTOR_DIR_LEFT_A_Port				GPIOA
#define MOTOR_DIR_LEFT_A_Pin				GPIO_Pin_12
#define MOTOR_DIR_LEFT_B_RCC				RCC_APB2Periph_GPIOA
#define MOTOR_DIR_LEFT_B_Port				GPIOA
#define MOTOR_DIR_LEFT_B_Pin				GPIO_Pin_11
#define MOTOR_DIR_RIGHT_A_RCC				RCC_APB2Periph_GPIOC
#define MOTOR_DIR_RIGHT_A_Port				GPIOC
#define MOTOR_DIR_RIGHT_A_Pin				GPIO_Pin_12
#define MOTOR_DIR_RIGHT_B_RCC				RCC_APB2Periph_GPIOA
#define MOTOR_DIR_RIGHT_B_Port				GPIOA
#define MOTOR_DIR_RIGHT_B_Pin				GPIO_Pin_0

#define BATTERY_STATE_RCC					RCC_APB2Periph_GPIOC
#define BATTERY_STATE_Port					GPIOC
#define BATTERY_STATE_Pin					GPIO_Pin_3

// Two wheeled inspection robot hardware constants

#define ROTATION_CONSTANT 					3.5675
#define WHEEL_DIAMETER 						7
#define DISTANCE_CONSTANT 					1.9847
#define ENCODER_CONSTANT 					0.46


// Mathematical constants

#define PI 									3.141593
#define MPU_CONSTANT 						180


// Interrupt constants

#define SYS_TICK_INTERRUPT_FREQUENCY_HZ		1000
#define VELOCITY_INTERRUPT_PERIOD_MS 		500
#define WIFI_INTERRUPT_PERIOD_MS 			200
#define MPU_INTERRUPT_PERIOD_MS 			1
#define BATTERY_STATUS_PERIOD_MS			200
#define ENCODER_INTERRUPT_FREQUENCY_DIVISOR 1

#endif /* __PROPERTIES_H */
