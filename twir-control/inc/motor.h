#ifndef MOTOR_H_
#define MOTOR_H_

#include "encoder.h"
#include "math_pid.h"
#include "properties.h"

// Motor driver regulator constants
static const float MOTOR_KP = 0;
static const float MOTOR_KI = 0;
static const float MOTOR_KD = 0;
static const int MOTOR_SATURATION = 1000;

// Both motors PWM pins initialization, PWM pins are hardcoded, see properties.h
void motor_pwm_pins_init();
// Both motors PWM timer channels initialization, PWM timers are hardcoded, see properties.h
void motor_pwm_timer_init();
// Pin for setting direction initialization
void motor_left_init();
// Pin for setting direction initialization
void motor_right_init();
// Both motors initialization: direction and PWM pins
void motor_driver_init();

// Sets motor's direction pins to move forward
void set_forward_direction();
// Sets motor's direction pins to move backwards
void set_backward_direction();
// Auto set motors' direction pins
void set_direction();
// Hardware PWM for wheel reference velocity
void set_pwm(int16_t pwm_l, int16_t pwm_r);
// Calculates position change of both wheels and returns difference between them
int get_position_change_difference();
// Takes difference between wheels' speed and calculates output using PID regulator (motors' speed should be equal)
int calculate_pid_motor_diff();
// Converts PWM reference duty cycle to make motors move with equal speed
void set_equal_motors_speed();
// Motor control to achieve desired reference values: robot_velocity_ref and robot_direction_ref
void motor_driver(int ref_speed);

#endif /* MOTOR_H_ */
