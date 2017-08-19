/**
  ******************************************************************************
  * @author  Mateusz Staszków
  * @version V1.0
  * @date    01-December-2016
  * @brief   Two wheeled inspection robot control.
  ******************************************************************************
*/

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "esp8266.h"
#include "MPU6050.h"
#include "properties.h"
#include "stm32f10x.h"

// Structure with exportable data
struct MeasuredData {
	float fx;
	float fy;
	float fz;
	float ax;
	float ay;
	float az;
	int pos_l;
	int pos_r;
	float xl;
	float xr;
	float vl;
	float vr;
	uint8_t dir;
	int16_t pid;
	int pos_dif
	int pid_position;
	int battery_state;
};

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

// Calculated constants
const float ANGLE_CONSTANT = ROTATION_CONSTANT * ENCODER_INTERRUPT_FREQUENCY_DIVISOR * ENCODER_CONSTANT;
const float POSITION_CONSTANT = DISTANCE_CONSTANT * WHEEL_DIAMETER * PI / 360;
const int VELOCITY_CONSTANT = 1000 / VELOCITY_INTERRUPT_PERIOD_MS;

// Balance regulator constants
// PID regulator
const float BALANCE_KP = 0.019;
const float BALANCE_KI = 0.011;
const float BALANCE_KD = 0;
const int BALANCE_SATURATION = 1000;
// Complementary filter values, A for angle and B for angular acceleration
const float BALANCE_KA = 1.5;
const float BALANCE_KB = 2;
// Average value filtering
const uint8_t BALANCE_AVERAGE = 10;

// Motor driver regulator constants
const float MOTOR_KP = 1;
const float MOTOR_KI = 0;
const float MOTOR_KD = 0;
const int MOTOR_SATURATION = 1000;

// Linear velocity regulator constants
const float VELOCITY_KP = 2100;
const float VELOCITY_KI = 0;
const float VELOCITY_KD = 0;
const int VELOCITY_SATURATION = 5000;

// Global variables
volatile uint32_t global_time_ms;
volatile int l_encoder_counter;
volatile int r_encoder_counter;
int16_t robot_velocity_ref;
uint8_t robot_direction_ref;
int16_t robot_linear_velocity_ref;
float filtered_angle;
float filtered_acc;
float filtered_balancing_data;
struct MeasuredData measuredData;
struct DataPID balance_pid;
struct DataPID velocity_pid;
struct DataPID motor_driver_pid;

// MPU6050 raw data vector
s16 gyro[6];

// ESP8266 RX buffer
char esp_buffer[10];

// Interrupt flags
volatile bool velocity_flag;
volatile bool uart_flag;
volatile bool l_cnt_flag_forward;
volatile bool l_cnt_flag_backward;
volatile bool r_cnt_flag_forward;
volatile bool r_cnt_flag_backward;
volatile bool mpu_flag;
volatile bool battery_flag;

volatile bool start_flag;

// Main timer interrupt handler
void SysTick_Handler();
// Initialize all robot's reference values
void init_reference_values();
// Sets all global boolean variables to false
void init_flags();
// All table type global variables initialization
void set_tables();
// All structure type global variables initialization
void init_pid_structure(struct DataPID &data_pid, const float kP, const float kI, const float kD, const int saturation);
// Main file's global variables initialization
void global_variables_init();
// Absolute integer calculating function
int abs(int x);
// Median of 3 float values calculating function
float median(float value[]);
// Integration anti-windup
void anti-windup(struct DataPID &data_pid);
// Numerical integration
void integrate(struct DataPID &data_pid);
// Numerical differentiation without involving time - operating just on value differences
void differentiate(struct DataPID &data_pid);
// Calculate PID output using calculated integration and differentiation terms
int get_pid(struct DataPID &data_pid);
// PID regulator implementation
int calculate_pid(struct DataPID &data_pid);

void left_encoder_init();

void right_encoder_init();
// Both motors magnetic encoders initialization
// Rising Edge activated interrupts, 101 impulses for 1 rotation, 
// may be changed by setting different ENCODER_INTERRUPT_FREQUENCY_DIVISOR
void encoder_init();
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
// Timer interrupt configuration for global timer, frequency set by SYS_TICK_INTERRUPT_FREQUENCY_HZ
void global_timer_init();
// Initialize all components in system
void hardware_setup();
// Right encoder handler, 101 impulses - 360 degrees, see encoder_init() declaration
void EXTI9_5_IRQHandler();
// Left encoder handler, 101 impulses - 360 degrees, see encoder_init() declaration
void EXTI15_10_IRQHandler();
// Gathers raw angle and angular acceleration data for six axises, range of values from -16000 to +16000
// Values can be converted to degrees by using MPU_CONSTANT
// gyro[0] - X axis
// gyro[1] - Y axis
// gyro[2] - Z axis
// gyro[3] - Y axis
// gyro[4] - X axis
// gyro[5] - Z axis
void mpu_read_data();
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
void motor_driver();
// Calculates position difference comparing impulses from encoders
int get_position_difference(int ref_speed);
// Takes difference between position of motor shaft in time and calculates output using PID regulator (ref_speed = 0: balanced robot)
int calculate_angle_from_speed(int ref_speed);
// Sets reference variable to auto set direction
void set_reference_direction();
// Takes filtered angle and calculates reference PWM duty cycle using PID regulator (ref_angle = 0: balanced robot)
void calculate_pid_output(int ref_angle, int angle);
// Checks *_cnt_flag_* flags and updates motors' shafts position and measured moving direction
void update_encoder_values();
// Checks velocity_flag and updates robot linear position and velocity which will be send to ESP8266 module
void update_linear_postion();
// Checks battery_flag and updates battery state which will be send to ESP8266 module
void update_battery_state();
// Counts average value of angle and angular acceleration and sets start_flag when ready, filter parameter: BALANCE_AVERAGE
void average_filter();
// Calculates input for balance regulator, 
// parameters of filter BALANCE_KA, BALANCE_KB will be applied to filtered angle and filtered angular acceleration
int simple_complementary_filter();
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


int main(void)
{
	hardware_setup();
	start();
}





void SysTick_Handler() {
	global_time_ms++;
	if(!(global_time_ms % VELOCITY_INTERRUPT_PERIOD_MS)) velocity_flag = true;
	if(!(global_time_ms % WIFI_INTERRUPT_PERIOD_MS)) uart_flag = true;
	if(!(global_time_ms % MPU_INTERRUPT_PERIOD_MS)) mpu_flag = true;
	IF(!(global_time_ms % BATTERY_STATUS_PERIOD_MS)) battery_flag = true;
}

void init_reference_values() {
	robot_direction_ref = 1;
	robot_linear_velocity_ref = 0;
	robot_velocity_ref = 0;
}

void init_flags() {
	velocity_flag = false;
	uart_flag = false;
	l_cnt_flag_forward = false;
	l_cnt_flag_backward = false;
	r_cnt_flag_forward = false;
	r_cnt_flag_backward = false;
	mpu_flag = false;
	battery_flag = false;
	start_flag = false;
}

void set_tables() {
	int i = 0;
	
	for(i; i<6; i++) {
		gyro[i] = 0;
	}
	for(i=0; i<10; i++) {
		esp_buffer[i] = 0;
	}
}

void init_pid_structure(struct DataPID &data_pid, const float kP, const float kI, const float kD, const int saturation) {
	data_pid.kP = kP;
	data_pid.kI = kI;
	data_pid.kD = kD;
	data_pid.saturation = saturation;
	data_pid.last_error = 0;
	data_pid.iterm = 0;
	data_pid.dterm = 0;
	data_pid.error = 0;
}

void global_variables_init() {
	init_reference_values();

	init_flags();
	
	init_pid_structure(motor_driver_pid, MOTOR_KP, MOTOR_KI, MOTOR_KD, MOTOR_SATURATION);
	init_pid_structure(balance_pid, BALANCE_KP, BALANCE_KI, BALANCE_KD, BALANCE_SATURATION);
	init_pid_structure(velocity_pid, VELOCITY_KP, VELOCITY_KI, VELOCITY_KD, VELOCITY_SATURATION);

	set_tables();
}

int abs(int x) {
	return (x>=0) ? x : -x;
}

float median(float value[]) {
	float a = value[0];
	float b = value[1];
	float c = value[2];
	
    return (a<b && a>c) || (a>b && a<c) ? 
			a : 
				( (a>b && b>c) || (a<b && b<c) ? 
					b : 
						c);
}

void anti-windup(struct DataPID &data_pid) {
	if(data_pid.iterm > data_pid.saturation) data_pid.iterm = data_pid.saturation;
	if(data_pid.iterm < -data_pid.saturation) data_pid.iterm = -data_pid.saturation;
}

void integrate(struct DataPID &data_pid) {
	data_pid.iterm += data_pid.kI * data_pid.error;
	
	anti-windup(data_pid);
}

void differentiate(struct DataPID &data_pid) {
	data_pid.dterm = data_pid.kD * (data_pid.error - data_pid.last_error);
	data_pid.last_error = data_pid.error;
}

int get_pid(struct DataPID &data_pid) {
	int pid = 0;
	
	pid = data_pid.kP * data_pid.error + data_pid.iterm - data_pid.dterm;
	
	if(pid > data_pid.saturation) pid = data_pid.saturation;
	if(pid < -data_pid.saturation) pid = -data_pid.saturation;
	
	return pid;
}

int calculate_pid(struct DataPID &data_pid) {
	integrate(data_pid);
	differentiate(data_pid);
	
	return get_pid(data_pid)
}

void left_encoder_init() {
	RCC_APB2PeriphClockCmd(ENCODER_LEFT_RCC_Port | ENCODER_LEFT_RCC_Special, ENABLE);
	
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = ENCODER_LEFT_B_Pin | ENCODER_LEFT_A_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(ENCODER_LEFT_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = ENCODER_LEFT_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(ENCODER_LEFT_Interrupt_Port, ENCODER_LEFT_Interrupt_Pin);
	
	nvic.NVIC_IRQChannel = ENCODER_LEFT_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void right_encoder_init() {
	RCC_APB2PeriphClockCmd(ENCODER_RIGHT_RCC_Port | ENCODER_RIGHT_RCC_Special, ENABLE);
	
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = ENCODER_RIGHT_B_Pin | ENCODER_RIGHT_A_Pin;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(ENCODER_RIGHT_Port, &gpio);

	EXTI_StructInit(&exti);
	exti.EXTI_Line = ENCODER_LEFT_Interrupt_Line;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);
	GPIO_EXTILineConfig(ENCODER_RIGHT_Interrupt_Port, ENCODER_RIGHT_Interrupt_Pin);

	nvic.NVIC_IRQChannel = ENCODER_RIGHT_Interrupt_Channel;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void encoder_init() {
	left_encoder_init();
	right_encoder_init();
}

void motor_pwm_pins_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpio;
	
	// Configuration for immutable PWM timers
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpio);
}

void motor_pwm_timer_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef channel;

	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = MOTOR_PWM_PRESCALER - 1;
	tim.TIM_Period = MOTOR_PWM_PERIOD;
	TIM_TimeBaseInit(TIM4, &tim);
	
	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;
	channel.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &channel);

	TIM_Cmd(TIM4, ENABLE);
}

void motor_left_init() {
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef channel;
	
	RCC_APB2PeriphClockCmd(MOTOR_DIR_LEFT_A_RCC | MOTOR_DIR_LEFT_B_RCC, ENABLE);
	
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = MOTOR_DIR_LEFT_A_Pin;
	GPIO_Init(MOTOR_DIR_LEFT_A_Port, &gpio);
	gpio.GPIO_Pin = MOTOR_DIR_LEFT_B_Pin;
	GPIO_Init(MOTOR_DIR_LEFT_B_Port, &gpio);
}

void motor_right_init() {
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef channel;
	
	RCC_APB2PeriphClockCmd(MOTOR_DIR_RIGHT_A_RCC | MOTOR_DIR_RIGHT_B_RCC, ENABLE);
	
	GPIO_StructInit(&gpio);
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = MOTOR_DIR_RIGHT_A_Pin;
	GPIO_Init(MOTOR_DIR_RIGHT_A_Port, &gpio);
	gpio.GPIO_Pin = MOTOR_DIR_RIGHT_B_Pin;
	GPIO_Init(MOTOR_DIR_RIGHT_B_Port, &gpio);
}

void motor_driver_init() {
	motor_pwm_pins_init();
	motor_pwm_timer_init();
	motor_left_init();
	motor_right_init();
}

void global_timer_init() {
	SysTick_Config(SystemCoreClock / SYS_TICK_INTERRUPT_FREQUENCY_HZ);
}

void hardware_setup() {
	encoder_init();
	motor_driver_init();
	esp_init();
	MPU6050_I2C_Init();
	MPU6050_Initialize();
	global_variables_init();
	global_timer_init();
}

void EXTI9_5_IRQHandler() {
	if((r_encoder_counter++) % ENCODER_INTERRUPT_FREQUENCY_DIVISOR) {
		if(EXTI_GetITStatus(ENCODER_RIGHT_Interrupt_Line)) EXTI_ClearITPendingBit(ENCODER_RIGHT_Interrupt_Line);
		return;
	}
	if (EXTI_GetITStatus(ENCODER_RIGHT_Interrupt_Line) && (GPIO_ReadInputDataBit(ENCODER_RIGHT_Port, ENCODER_RIGHT_A_Pin) != 1)) return;
	
	if(GPIO_ReadInputDataBit(ENCODER_RIGHT_Port, ENCODER_RIGHT_B_Pin) == 0) r_cnt_flag_forward = true;
	else r_cnt_flag_backward = true;
	EXTI_ClearITPendingBit(ENCODER_RIGHT_Interrupt_Line);
}

void EXTI15_10_IRQHandler() {
	if((l_encoder_counter++) % ENCODER_INTERRUPT_FREQUENCY_DIVISOR) {
		if(EXTI_GetITStatus(ENCODER_LEFT_Interrupt_Line)) EXTI_ClearITPendingBit(ENCODER_LEFT_Interrupt_Line);
		return;
	}
	if (EXTI_GetITStatus(ENCODER_LEFT_Interrupt_Line) && (GPIO_ReadInputDataBit(ENCODER_LEFT_Port, ENCODER_LEFT_A_Pin) != 1)) return;
	
	if(GPIO_ReadInputDataBit(ENCODER_LEFT_Port, ENCODER_LEFT_B_Pin) == 0) l_cnt_flag_backward = true;
	else l_cnt_flag_forward = true;

	EXTI_ClearITPendingBit(ENCODER_LEFT_Interrupt_Line);
}

void mpu_read_data() {
	MPU6050_GetRawAccelGyro(gyro);
	measuredData.fx = gyro[0];
	measuredData.fy = gyro[1]/MPU_CONSTANT;
	measuredData.fz = gyro[2]/MPU_CONSTANT;
	measuredData.ax = gyro[4];
	measuredData.ay = gyro[3]/MPU_CONSTANT;
	measuredData.az = gyro[5]/MPU_CONSTANT;
}

void set_forward_direction() {
	GPIO_SetBits(MOTOR_DIR_LEFT_A_Port, MOTOR_DIR_LEFT_A_Pin);
	GPIO_SetBits(MOTOR_DIR_RIGHT_A_Port, MOTOR_DIR_RIGHT_A_Pin);
	GPIO_ResetBits(MOTOR_DIR_LEFT_A_Port, MOTOR_DIR_LEFT_B_Pin);
	GPIO_ResetBits(MOTOR_DIR_RIGHT_B_Port, MOTOR_DIR_RIGHT_B_Pin);
}

void set_backward_direction() {
	GPIO_SetBits(MOTOR_DIR_LEFT_B_Port, MOTOR_DIR_LEFT_B_Pin);
	GPIO_SetBits(MOTOR_DIR_RIGHT_B_Port, MOTOR_DIR_RIGHT_B_Pin);
	GPIO_ResetBits(MOTOR_DIR_LEFT_A_Port, MOTOR_DIR_LEFT_A_Pin);
	GPIO_ResetBits(MOTOR_DIR_RIGHT_A_Port, MOTOR_DIR_RIGHT_A_Pin);
}

void set_direction() {
	if(robot_direction_ref == 0) {
		set_backward_direction();
	}
	else if(robot_direction_ref == 1) {
		set_forward_direction();
	}
}

void set_pwm(int16_t pwm_l, int16_t pwm_r) {
	TIM_OCInitTypeDef channel;
	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;
	
	channel.TIM_Pulse = pwm_l;
	TIM_OC3Init(TIM4, &channel);
	channel.TIM_Pulse = pwm_r;
	TIM_OC4Init(TIM4, &channel);
}

int get_position_change_difference() {
	static int last_pos_l = 0;
	static int last_pos_r = 0;
	
	measuredData.motor_dif = (measuredData.pos_l - last_pos_l) - (measuredData.pos_r - last_pos_r);
	last_pos_l = measuredData.pos_l;
	last_pos_r = measuredData.pos_r;
	
	return measuredData.motor_dif;
}

int calculate_pid_motor_diff() {
	motor_driver_pid.error = get_position_change_difference();
	measuredData.pid_motor = calculate_pid(motor_driver_pid);
	
	return measuredData.pid_motor;
}

void set_equal_motors_speed() {
	int diff = calculate_pid_motor_diff();
	uint16_t pwm_l = robot_velocity_ref;
	uint16_t pwm_r = robot_velocity_ref;
	
	if(diff > 0) pwm_r += diff;
	else if(diff < 0) pwm_l += diff;
	if(pwm_l > MOTOR_PWM_PERIOD) pwm_l = MOTOR_PWM_PERIOD;
	if(pwm_r > MOTOR_PWM_PERIOD) pwm_r = MOTOR_PWM_PERIOD;

	set_pwm(pwm_l, pwm_r);
}

void motor_driver() {
	set_equal_motors_speed();
	set_direction();
}

int get_position_difference(int ref_speed) {
	static int last_pos = 0;
	
	mesauredData.pos_dif = ref_speed + (measuredData.pos_l - last_pos);
	last_pos = measuredData.pos_l;
	
	return mesauredData.pos_dif;
}

int calculate_angle_from_speed(int ref_speed) {
	velocity_pid.error = get_postion_difference(ref_speed);
	measuredData.pid_position = calculate_pid(velocity_pid);

	return measuredData.pid_position;
}

void set_reference_direction() {
	if(measuredData.pid >= 0) robot_direction_ref = 1;
	else robot_direction_ref = 0;
}

void calculate_pid_output(int ref_angle, int angle) {
	balance_pid.error = ref_angle + angle;
	measuredData.pid = calculate_pid(balance_pid);
	set_reference_direction();
	robot_velocity_ref = abs(pid);
}

void update_encoder_values() {
	if(l_cnt_flag_backward) {
		measuredData.pos_l -= ANGLE_CONSTANT;
		measuredData.dir = 0;
		l_cnt_flag_backward = false;
	}
	if(l_cnt_flag_forward) {
		measuredData.pos_l += ANGLE_CONSTANT;
		measuredData.dir = 1;
		l_cnt_flag_forward = false;
	}
	if(r_cnt_flag_backward) {
		measuredData.pos_r -= ANGLE_CONSTANT;
		r_cnt_flag_backward = false;
	}
	if(r_cnt_flag_forward) {
		measuredData.pos_r += ANGLE_CONSTANT;
		r_cnt_flag_forward = false;
	}
}

void update_linear_postion() {
	if(!velocity_flag) return;
	
	measuredData.xr = measuredData.pos_r * POSITION_CONSTANT;
	measuredData.xl = measuredData.pos_l * POSITION_CONSTANT;
	measuredData.vl = (measuredData.xl - last_x) * VELOCITY_CONSTANT;
	last_x = measuredData.xl;
	velocity_flag = false;
}

void update_battery_state() {
	if(!battery_flag) return;
	
	//TODO
	
	battery_flag = false;
}

void average_filter() {
	static int angle_sum = 0;
	static int acc_sum = 0;
	static uint8_t avg_filter_iterator = 0;
	
	angle_sum += measuredData.fx;
	acc_sum += measuredData.ax;
	
	if(!((++avg_filter_iterator) % BALANCE_AVERAGE)) {
		filtered_angle = angle_sum / BALANCE_AVERAGE;
		filtered_acc = acc_sum / BALANCE_AVERAGE;
		angle_sum = 0;
		acc_sum = 0;
		start_flag = true;
	}
}

int simple_complementary_filter() {
	return BALANCE_KA * filtered_angle + BALANCE_KB * filtered_acc;
}

void correct_robot_position() {
	if(!start_flag) return;
	
	filtered_balancing_data = simple_complementary_filter();
	ref_angle = calculate_angle_from_speed(robot_linear_velocity_ref);
	calculate_pid_output(ref_angle, filtered_balancing_data);
	motor_driver();
	
	start_flag = false;
}

void stabilize() {
	if(!mpu_flag) return;
	
	mpu_read_data();
	average_filter();
	correct_robot_position();
	
	mpu_flag = false;
}

void start() {
	while(1) {
		update_encoder_values();
		update_linear_postion();
		update_battery_state();
		stabilize();
		update_esp_values();
	}
}

void export_data_wifi() {
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;\r\n",
			(int)filtered_angle/MPU_CONSTANT,
			(int)measuredData.fy,
		    (int)measuredData.fz,
			(int)filtered_acc/MPU_CONSTANT,
			(int)measuredData.ay,
			(int)measuredData.az,
			(int)measuredData.pos_l%360,
			(int)measuredData.pos_r%360,
			(int)measuredData.xl,
			(int)measuredData.vl,
			(int)measuredData.dir,
			(int)measuredData.pid,
			(int)filtered_balancing_data,
			(int)measuredData.pos_dif,
			(int)measuredData.pid_position,
			(int)measuredData.xr,
			(int)measuredData.vr,
			(int)measuredData.battery_state
	);
}

void update_esp_values() {
	if(!uart_flag) return;
	
	export_data_wifi();
	uart_flag = false;
}