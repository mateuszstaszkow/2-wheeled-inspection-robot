#ifndef CONTROL_SYSTEM_H_
#define CONTROL_SYSTEM_H_

#include "main_declarations.h"
#include "math_pid.h"
#include "properties.h"

// Structure with all PID regulator data
struct DataPID {
	float kP;
	float kI;
	float kD;
	int saturation;
	float last_error;
	float iterm;
	float dterm;
	float error;
};

// Balance regulator constants
// PID regulator
extern const float BALANCE_KP = 0.019;
extern const float BALANCE_KI = 0.011;
extern const float BALANCE_KD = 0;
extern const int BALANCE_SATURATION = 1000;
// Complementary filter values, A for angle and B for angular acceleration
extern const float BALANCE_KA = 1.5;
extern const float BALANCE_KB = 2;
// Average value filtering
extern const uint8_t BALANCE_AVERAGE = 10;

// Linear velocity regulator constants
extern const float VELOCITY_KP = 2100;
extern const float VELOCITY_KI = 0;
extern const float VELOCITY_KD = 0;
extern const int VELOCITY_SATURATION = 5000;

// Global variables
extern float filtered_angle;
extern float filtered_acc;
extern float filtered_balancing_data;
extern struct DataPID balance_pid;
extern struct DataPID velocity_pid;

// Calculates position difference comparing impulses from encoders
int get_position_difference(int ref_speed);
// Takes difference between position of motor shaft in time and calculates output using PID regulator (ref_speed = 0: balanced robot)
int calculate_angle_from_speed(int ref_speed);
// Sets reference variable to auto set direction
void set_reference_direction();
// Takes filtered angle and calculates reference PWM duty cycle using PID regulator (ref_angle = 0: balanced robot)
void calculate_pid_output(int ref_angle, int angle);
// Counts average value of angle and angular acceleration and sets start_flag when ready, filter parameter: BALANCE_AVERAGE
void average_filter();
// Calculates input for balance regulator, 
// parameters of filter BALANCE_KA, BALANCE_KB will be applied to filtered angle and filtered angular acceleration
int simple_complementary_filter();

#endif /* CONTROL_SYSTEM_H_ */