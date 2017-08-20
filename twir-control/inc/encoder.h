#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include "properties.h"

// Interrupt flags
extern volatile bool l_cnt_flag_forward;
extern volatile bool l_cnt_flag_backward;
extern volatile bool r_cnt_flag_forward;
extern volatile bool r_cnt_flag_backward;

// Global variables
extern volatile int l_encoder_counter;
extern volatile int r_encoder_counter;

// 
void left_encoder_init();
// 
void right_encoder_init();
// Both motors magnetic encoders initialization
// Rising Edge activated interrupts, 101 impulses for 1 rotation, 
// may be changed by setting different ENCODER_INTERRUPT_FREQUENCY_DIVISOR
void encoder_init();

// Right encoder handler, 101 impulses - 360 degrees, see encoder_init() declaration
void EXTI9_5_IRQHandler();
// Left encoder handler, 101 impulses - 360 degrees, see encoder_init() declaration
void EXTI15_10_IRQHandler();

#endif /* PROPERTIES_H_ */