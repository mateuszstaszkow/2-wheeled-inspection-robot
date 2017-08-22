#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include "properties.h"

// 
void left_encoder_init();
// 
void right_encoder_init();
// Both motors magnetic encoders initialization
// Rising Edge activated interrupts, 101 impulses for 1 rotation, 
// may be changed by setting different ENCODER_INTERRUPT_FREQUENCY_DIVISOR
void encoder_init();

#endif /* PROPERTIES_H_ */
