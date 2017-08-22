#ifndef INTERRUPT_HANDLER_H_
#define INTERRUPT_HANDLER_H_

#include "encoder.h"
#include "hcsr04.h"
#include "main_declarations.h"
#include "properties.h"
#include "stm32f10x.h"

// Timer interrupt handlers:
// 10 us period triggering function, used to send signals on HCSR04 TRIG pins
void TIM2_IRQHandler();

// Pin interrupt handlers:
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
