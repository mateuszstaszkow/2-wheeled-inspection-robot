#ifndef HCSR04_H_
#define HCSR04_H_

#include "main_declarations.h"
#include "properties.h"
#include "stm32f10x.h"

void hcsr_init_right();

void hcsr_init_middle();

void hcsr_init_left();

void hcsr_init();

void hcsr_get_dist_right();

void hcsr_get_dist_middle();

void hcsr_get_dist_left();

void hcsr_get_distance();

void turn_off_trig_right();

void turn_off_trig_middle();

void turn_off_trig_left();

void turn_off_triggers();

#endif /* HCSR04_H_ */
