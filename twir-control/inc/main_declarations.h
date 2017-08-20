#ifndef MAIN_DECLARATIONS_H_
#define MAIN_DECLARATIONS_H_

#include <stdbool.h>
#include <stdlib.h>
#include "control_system.h"
#include "esp8266.h"
#include "main_declarations.h"
#include "motor.h"
#include "MPU6050.h"
#include "properties.h"
#include "stm32f10x.h"

// Global variables
extern int16_t robot_turn_speed_ref;
extern int16_t robot_linear_velocity_ref;
extern volatile uint32_t global_time_ms;
extern struct MeasuredData measuredData;

// Interrupt flags
extern volatile bool velocity_flag;
extern volatile bool uart_flag;
extern volatile bool mpu_flag;
extern volatile bool battery_flag;

extern volatile bool start_flag;
extern volatile bool execute_flag;

// Main timer interrupt handler
void SysTick_Handler();
// Initialize all robot's reference values
void init_reference_values();
// Sets all global boolean variables to false
void init_flags();
// All table type global variables initialization
void set_tables();
// All structure type global variables initialization
void init_pid_structure(struct DataPID *data_pid, const float kP, const float kI, const float kD, const int saturation);
// Main file's global variables initialization
void global_variables_init();
// Timer interrupt configuration for global timer, frequency set by SYS_TICK_INTERRUPT_FREQUENCY_HZ
void global_timer_init();
// Initialize all components in system
void hardware_setup();
// Interprate orders sent from ESP8266
void get_order();
// Checks start_flag and run all regulators
void correct_robot_position();
// Main program function, waits for mpu_flag, invokes all key functions to keep robot balancing
void stabilize();
// Starts main loop of the program
void start();
// Sends data via USART to ESP8266
void export_data_wifi();
// Checks uart_flag and sends data through UART to ESP8266 module
void update_esp_values();

#endif /* MAIN_DECLARATIONS_H_ */