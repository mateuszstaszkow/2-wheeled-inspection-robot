/*
 * interrupt_handler.h
 *
 *  Created on: 22 sie 2017
 *      Author: Stahu
 */

#ifndef INTERRUPT_HANDLER_H_
#define INTERRUPT_HANDLER_H_

#include "main_declarations.h"
#include "properties.h"
#include "stm32f10x.h"

// Interrupt flags
volatile bool l_cnt_flag_forward;
volatile bool l_cnt_flag_backward;
volatile bool r_cnt_flag_forward;
volatile bool r_cnt_flag_backward;

// Global variables
volatile int l_encoder_counter;
volatile int r_encoder_counter;

// For HCSR04 wave measurement: approximately 60 us equals distance of 1 cm
//  (results need to be multiplied by 0.017)
// For encoders: 101 impulses - 360 degrees, see encoder_init() declaration
// Line 2: right HCSR04 echo wave duration measurement
// Line 4: middle HCSR04 echo wave duration measurement
void EXTI4_IRQHandler();
// Line 6: left HCSR04 echo wave duration measurement
// Line 9: right encoder handler
void EXTI9_5_IRQHandler();
// Line 11: left encoder handler
void EXTI15_10_IRQHandler();

#endif /* INTERRUPT_HANDLER_H_ */
