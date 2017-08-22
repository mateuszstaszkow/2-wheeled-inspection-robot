#ifndef MAIN_DECLARATIONS_H_
#define MAIN_DECLARATIONS_H_

#include <stdbool.h>
#include <stdlib.h>
#include "control_system.h"
#include "esp8266.h"
#include "hcsr04.h"
#include "interrupt_handler.h"
#include "main_declarations.h"
#include "motor.h"
#include "MPU6050.h"
#include "properties.h"

// Motors' speed when doing partial rotation for next scan
static const int SCAN_ROTATION_SPEED = 300;
static const uint32_t SCAN_ROTATION_TIME = 40;

// Global variables
int16_t robot_turn_speed_ref;
int16_t robot_linear_velocity_ref;
int16_t robot_velocity_ref;
uint8_t robot_direction_ref;
volatile uint32_t global_time_ms;
struct MeasuredData measuredData;
struct DataPID motor_driver_pid;

// Interrupt flags
volatile bool velocity_flag;
volatile bool uart_flag;
volatile bool mpu_flag;
volatile bool battery_flag;
volatile bool execute_flag;
volatile bool hcsr_flag;

// Other flags
volatile bool start_flag;
volatile bool turn_flag;
volatile bool busy_turning_flag;
volatile bool turn_mode_flag;
volatile bool sector_captured_flag;

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
// Secondary precise timer initialization (1 us precise)
void timer_us_init();
// Initialize all components in system
void hardware_setup();
// Interprate orders sent from ESP8266
void get_order();
// Partial robot rotation for a next scan
void turn_right();
//
void rotate_robot();
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
