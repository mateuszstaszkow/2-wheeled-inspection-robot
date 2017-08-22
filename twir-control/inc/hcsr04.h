#ifndef HCSR04_H_
#define HCSR04_H_

#include "main_declarations.h"
#include "properties.h"
#include "stm32f10x.h"

void hcsr_init_right();

void hcsr_init_middle();

void hcsr_init_left();
// Timer for 10 us high signal triggering on HCSR04 TRIG pin
void hcsr_timer_init();
// Initialize pins on 3 HCSR sensors (outputs TRIG pins and interrupts on ECHO pins)
void hcsr_init();

void hcsr_get_dist_right();

void hcsr_get_dist_middle();

void hcsr_get_dist_left();
// Triggers HCSR04 sensors and waits for ECHO wave
void hcsr_get_distance();

void turn_off_trig_right();

void turn_off_trig_middle();

void turn_off_trig_left();
// Sets low voltage on all TRIG pins (after 10 us)
void turn_off_triggers();

#endif /* HCSR04_H_ */
