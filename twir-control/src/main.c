/**
  ******************************************************************************
  * @file    main.c
  * @author  Mateusz Staszkow
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

//rzadszy odczyt

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "MPU6050.h"
#include "esp8266.h"

//HARDWARE SETTINGS AND CONSTANTS
#define ROTATION_CONSTANT 3.5675
#define WHEEL_DIAMETER 7
#define DISTANCE_CONSTANT 1.9847
#define PI 3.141593
#define MPU_CONSTANT 180
#define VELOCITY_INTERRUPT_PERIOD 500
#define WIFI_INTERRUPT_PERIOD 200
#define MPU_INTERRUPT_PERIOD 1
#define ENCODER_INTERRUPT_FREQUENCY_DIVISOR 1
#define ENCODER_CONSTANT 0.46

#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 65.536
#define dt 0.01

struct MeasuredData {
	float fx;
	float fy;
	float fz;
	float ax;
	float ay;
	float az;
	int pos_l;
	int pos_r;
	float x;
	int xr;
	float vl;
	float vr;
	uint8_t dir;
	int16_t pid;
	int pidPosition;
};

//Calculated constants
const float angle_constant = ROTATION_CONSTANT * ENCODER_INTERRUPT_FREQUENCY_DIVISOR * ENCODER_CONSTANT;
const float position_constant = DISTANCE_CONSTANT * WHEEL_DIAMETER * PI / 360;
const float velocity_constant = WHEEL_DIAMETER * PI / 360;
//const float time_constant = ENCODER_INTERRUPT_FREQUENCY_DIVISOR*ENCODER_CONSTANT*1000/VELOCITY_INTERRUPT_PERIOD;

//Global variables
volatile uint32_t timer_ms;
volatile uint32_t global_time_ms;
volatile int l_encoder_counter;
volatile int r_encoder_counter;
int16_t robot_velocity_ref;
int16_t robot_direction_ref;
float filtered_angle = 0;
float filtered_acc = 0;
struct MeasuredData measuredData;
//MPU6050 raw data vector (6 values)
//gyro[0] - X axis
//gyro[1] - Y axis
//gyro[2] - Z axis
//gyro[3] - Y axis
//gyro[4] - X axis
//gyro[5] - Z axis
s16 gyro[6];
//ESP8266 RX buffer
char esp_buffer[10];

//interrupt flags
volatile bool velocity_flag;
volatile bool uart_flag;
volatile bool l_cnt_flag_forward;
volatile bool l_cnt_flag_backward;
volatile bool r_cnt_flag_forward;
volatile bool r_cnt_flag_backward;
volatile bool mpu_flag;

//REGULATOR CONSTANTS
//PID
// *** NO PWM
//float kp = 0.001; //no filter
//float ki = 0.0003;
//float kd = 0.00085;
//float kp = 0.0013; //avg 10
//float ki = 0.00015;
//float kd = 0.00085;
//float kp = 0.0015; //median3 + avg6
//float ki = 0.0002;
//float kd = 0.00085;
//float kp = 0.001; //test
//float ki = 0.0000;
//float kd = 0.0000;

// *** PWM
//float kp = 0.27; //avg 20
//float ki = 0.025;
//float kd = 0.2;
//float kp = 0.27; //avg 10, 11.1 V
//float ki = 0.025;
//float kd = 0.24;
//float kp = 0.6; //avg 10, 12.6 V
//float ki = 0.007;
//float kd = 0.554;
//float kp = 0.6; //avg 10, 12.1 V
//float ki = 0.01;
//float kd = 0.557;
//float kp = 0.6; //avg 10, 11.1 V - BEZ MEDIANY, BALANSUJE
//float ki = 0.015;
//float kd = 0.577;
//float kp = 0.6; //avg 10 + med 3 11.1 V
//float ki = 0.019;
//float kd = 0.59;
//float kp = 0.6; //avg 10, 11.6 V - BEZ MEDIANY, BALANSUJE
//float ki = 0.013;
//float kd = 0.57;
//float kp = 0.6; //avg 10, 12.6 V - BEZ MEDIANY, BALANSUJE
//float ki = 0.011;
//float kd = 0.578;
//float kp = 0.6; //avg 10, 11.6 V - BEZ MEDIANY, BALANSUJE
//float ki = 0.011;
//float kd = 0.5775;
float kp = 0.019; //avg 10, 11.6 V - BEZ MEDIANY, BALANSUJE
float ki = 0.011;
float kd = 0;
//Complementary filter values, A for angle and B for angular acceleration
float kA = 1.5;
float kB = 2;

void delay_ms(int time) {
	timer_ms = time;
	while(timer_ms > 0) {}
}

//Main timer interrupt handler
void SysTick_Handler()
{
	if(timer_ms) timer_ms--;
	global_time_ms++;
	if(!(global_time_ms % VELOCITY_INTERRUPT_PERIOD)) velocity_flag = true;
	if(!(global_time_ms % WIFI_INTERRUPT_PERIOD)) uart_flag = true;
	if(!(global_time_ms % MPU_INTERRUPT_PERIOD)) mpu_flag = true;
}

void global_variables_init() {
	timer_ms = 0;
	measuredData.pos_r = 0;
	measuredData.dir = 0;
	robot_velocity_ref = 0;
	measuredData.pos_l = 0;
	measuredData.x = 0;
	measuredData.vl = 0;
	measuredData.vr = 0;
	measuredData.pidPosition = 0;
	robot_direction_ref = 1;

	velocity_flag = false;
	uart_flag = false;
	l_cnt_flag_forward = false;
	l_cnt_flag_backward = false;
	r_cnt_flag_forward = false;
	r_cnt_flag_backward = false;
	mpu_flag = false;

	int i;
	for(i=0; i<6; i++) {
		gyro[i] = 0;
	}
	for(i=0; i<10; i++) {
		esp_buffer[i] = 0;
	}
}

//Absolute value calculating function
int abs(int x) {
	return (x>=0) ? x : -x;
}

//Median of 3 values calculating function
float median(float value[])
{
	float a = value[0], b = value[1], c = value[2];
    return (a<b && a>c) || (a>b && a<c) ? a : ( (a>b && b>c) || (a<b && b<c) ? b : c);
}

//Hardware PWM for wheel velocity indication
void set_pwm(TIM_OCInitTypeDef channel, int pwm_nr, int duty_cycle) {
	switch(pwm_nr) {
	case 3:
		channel.TIM_Pulse = duty_cycle;
		TIM_OC3Init(TIM3, &channel);
		break;
	case 4:
		channel.TIM_Pulse = duty_cycle;
		TIM_OC4Init(TIM4, &channel);
		break;
	}
}

void left_encoder_init()
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(GPIOC, &gpio);

	//Encoders interrupt pins
	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line11;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;//_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11);
	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void right_encoder_init()
{
	GPIO_InitTypeDef gpio;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio.GPIO_Mode = GPIO_Mode_IPU; // pull-up input resistor
	GPIO_Init(GPIOC, &gpio);

	//Encoders interrupt pins
	EXTI_StructInit(&exti);
	exti.EXTI_Line = EXTI_Line9;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;//_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);

	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void encoder_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	left_encoder_init();
	right_encoder_init();
}

void motor_driver_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef channel;

	//PA.12 - right motor direction
	//PA.11 - left motor direction
	//PA.0 - PWM duty cycle (0-3.3V)
	//PC.2 - right motor's shaft position (0-3.3V)
	//PC.3 - left motor's shaft position (0-3.3V)
	//GPIO configuration
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_15 | GPIO_Pin_0;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &gpio);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_12;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio);
//	GPIO_SetBits(GPIOC, GPIO_Pin_12);
//	GPIO_SetBits(GPIOA, GPIO_Pin_15);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpio);

//	GPIO_StructInit(&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_0;
//	//gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &gpio);

	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 1280 - 1;
	tim.TIM_Period = 1000;
	TIM_TimeBaseInit(TIM4, &tim); //50 Hz
//	TIM_TimeBaseInit(TIM2, &tim);

	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;
	channel.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &channel);
	channel.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &channel);
//	channel.TIM_Pulse = 0;
//	TIM_OC1Init(TIM2, &channel);

	TIM_Cmd(TIM4, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//
//	GPIO_StructInit(&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
//	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOD, &gpio);

//	GPIO_StructInit(&gpio);
//	gpio.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;
//	gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOC, &gpio);
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

//Right encoder handler
//101 impulses - 360 degrees
void EXTI9_5_IRQHandler()
{
//	if(!((r_encoder_counter++) % ENCODER_INTERRUPT_FREQUENCY_DIVISOR)) {
		if (EXTI_GetITStatus(EXTI_Line9) && (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 1)) {
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0) r_cnt_flag_forward = true;
			else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 1) r_cnt_flag_backward = true;

			EXTI_ClearITPendingBit(EXTI_Line9);
		}
//	} else if(EXTI_GetITStatus(EXTI_Line9)) EXTI_ClearITPendingBit(EXTI_Line9);
}

//Left encoder handler
//101 impulses - 360 degrees
void EXTI15_10_IRQHandler()
{
	//if(!((l_encoder_counter++) % ENCODER_INTERRUPT_FREQUENCY_DIVISOR)) {
		if (EXTI_GetITStatus(EXTI_Line11) && (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == 1)) {
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 0) l_cnt_flag_backward = true;
			else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) == 1) l_cnt_flag_forward = true;

			EXTI_ClearITPendingBit(EXTI_Line11);
		}
//	} else if(EXTI_GetITStatus(EXTI_Line11)) EXTI_ClearITPendingBit(EXTI_Line11);
}

//void USART3_IRQHandler(void)
//{
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//    {
//    	//measuredData.xr = USART_ReceiveData(USART3);
//    	static uint8_t buffer_iterator = 0;
//    	esp_buffer[buffer_iterator++] = USART_ReceiveData(USART3);
//    	if(esp_buffer[buffer_iterator - 1] == ';') {
//    		for(int i=0; i<10; i++) {
//    			measuredData.xr[i] = esp_buffer[i];
//    		}
//    		buffer_iterator = 0;
//    	}
//    }
//}

void hardware_setup() {
	//Encoders initialization
	encoder_init();
	//Motor driver board initialization
	motor_driver_init();
	//ESP8266 initialization
	esp_init();
	//MPU6050 initialization
	MPU6050_I2C_Init();
	MPU6050_set_DLPF_mode(6);
	MPU6050_set_DHPF_mode(1);
	MPU6050_Initialize();
	//while(!MPU6050_TestConnection());
	//Main file's global variables initialization
	global_variables_init();
	//Timer interrupt configuration
	SysTick_Config(SystemCoreClock / 1000);
}

void export_data_wifi() {
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;\r\n",
			(int)measuredData.pos_l,
			(int)measuredData.fy,
		    (int)measuredData.fz,
			(int)filtered_acc,
			(int)measuredData.ay,
			(int)measuredData.az,
			(int)measuredData.pos_l%360,
			(int)measuredData.pos_r%360,
			(int)measuredData.x,
			(int)measuredData.vl,
			(int)measuredData.dir,
			(int)measuredData.pidPosition,
			(int)measuredData.pidPosition
			);
}

void set_direction() {
	if(robot_direction_ref == 0) {
		GPIO_SetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_0);
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
		GPIO_ResetBits(GPIOC, GPIO_Pin_12);
	}
	else if(robot_direction_ref == 1) {
		GPIO_ResetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_0);
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
		GPIO_SetBits(GPIOC, GPIO_Pin_12);
	}
}

int calculate_pid_motor_diff() {
	static int last_error = 0;
	static int iterm = 0;
	static int dterm = 0;
	int pid;
	float motorP = 0.2;
	float motorI = 0;
	float motorD = 0.1;
	int error = measuredData.pos_l - measuredData.pos_r;

	iterm += motorI*error;
	if(iterm > 1000) iterm = 1000;
	if(iterm < -1000) iterm = -1000;
	dterm = motorD*(error-last_error);

	pid = motorP*error + iterm - dterm;

	if(pid > 1000) pid = 1000;
	if(pid < -1000) pid = -1000;
	last_error = error;

	return pid;
}

void motor_driver() {

	TIM_OCInitTypeDef channel;
	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;

	int diff = calculate_pid_motor_diff();
	int pwm_l = robot_velocity_ref;
	int pwm_r = robot_velocity_ref;
	//if(pwm_l < 0) pwm_l = 0;
	//if(pwm_r < 0) pwm_r = 0;
//	if(diff > 0) pwm_r += diff;
//	else if(diff < 0) pwm_l += diff;
//	if(pwm_l > 1000) pwm_l = 1000;
//	if(pwm_r > 1000) pwm_r = 1000;

	channel.TIM_Pulse = abs(pwm_l); //L
	TIM_OC3Init(TIM4, &channel);
	channel.TIM_Pulse = abs(pwm_r); //R
	TIM_OC4Init(TIM4, &channel);

	set_direction();

	//if(robot_direction_ref == 1) GPIO_SetBits(GPIOA, GPIO_Pin_11|GPIO_Pin_12);
	//else if(robot_direction_ref == 0) GPIO_ResetBits(GPIOA, GPIO_Pin_11|GPIO_Pin_12);
}

//void complementary_filter(float *pitch)
//{
//    float pitchAcc;
//
//    // Integrate the gyroscope data -> int(angularSpeed) = angle
//    *pitch += ((float)gyro[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
//
//    // Compensate for drift with accelerometer data if !bullshit
//    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
//    int forceMagnitudeApprox = abs(gyro[3]) + abs(gyro[4]) + abs(gyro[5]);
//    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
//    {
//	// Turning around the X axis results in a vector on the Y-axis
//        pitchAcc = atan2f((float)gyro[3], (float)gyro[5]) * 180 / PI;
//        *pitch = *pitch * 0.98 + pitchAcc * 0.02;
//    }
//}

void calculate_pid_output(int ref_angle, int angle) {

	static int last_error = 0;
	static int iterm = 0;
	static int dterm = 0;
	int pid;
	int error = ref_angle + angle;

	iterm += ki*error;
	if(iterm > 1000) iterm = 1000;
	if(iterm < -1000) iterm = -1000;
	dterm = kd*(error-last_error);

	pid = kp*error + iterm - dterm;

	if(pid >= 0) robot_direction_ref = 1;
	else robot_direction_ref = 0;

	if(pid > 1000) pid = 1000;
	if(pid < -1000) pid = -1000;
	last_error = error;

	measuredData.pid = pid;

	robot_velocity_ref = abs(pid);
}


int calculate_ref_pid_position(int ref_position) {

	static bool isBusy = false;
	static int pid = 0;

	if(((measuredData.dir == 0) && (measuredData.pos_l < 0))
	  || ((measuredData.dir == 1) && (measuredData.pos_l > 0))) {
		isBusy = false;
		return 0;
	}
	if(isBusy) return pid;

	static int last_error = 0;
	static int iterm = 0;
	static int dterm = 0;
	float pathP = 3;
	float pathI = 0;
	float pathD = 0;

	int error = ref_position + measuredData.pos_l;

	int saturation = measuredData.pos_l;

	iterm += pathI*error;
	if(iterm > saturation) iterm = saturation;
	if(iterm < -saturation) iterm = -saturation;
	dterm = pathD*(error-last_error);

	pid = pathP*error + iterm - dterm;

	if(pid > saturation) pid = saturation;
	if(pid < -saturation) pid = -saturation;

	measuredData.az = pid;

	return pid;
}


int calculate_pid_position(int ref_position, int filtered_data) {

	//ref_position = calculate_ref_pid_position(ref_position);

	static int last_error = 0;
	static int iterm = 0;
	static int dterm = 0;
	float pathP = 0.01;
	float pathI = 0;
	float pathD = 0;

	int pid;
	int error = ref_position + measuredData.pos_l /*+ filtered_data/5*/;

	int saturation = 300;

	iterm += pathI*error;
	if(iterm > saturation) iterm = saturation;
	if(iterm < -saturation) iterm = -saturation;
	dterm = pathD*(error-last_error);

	pid = pathP*error + iterm - dterm;

	if(pid > saturation) pid = saturation;
	if(pid < -saturation) pid = -saturation;
	last_error = error;

	measuredData.pidPosition = pid;

	return pid;
}

int stop_robot_angle() {
	static int last_error = 0;
	static int iterm = 0;
	static int dterm = 0;
	static int last_pos = 0;
	float pathP = 10;
	float pathI = 0.2;
	float pathD = 8;

	int pid;
	int error = 0;

	int saturation = 3000;

	iterm += pathI*error;
	if(iterm > saturation) iterm = saturation;
	if(iterm < -saturation) iterm = -saturation;
	dterm = pathD*(error-last_error);

	pid = pathP*error + iterm - dterm;

	if(pid > saturation) pid = saturation;
	if(pid < -saturation) pid = -saturation;

	measuredData.pidPosition = pid;

	return pid;
}

int calculate_angle_from_position(int ref_speed) {
	static int last_error = 0;
	static int iterm = 0;
	static int dterm = 0;
	static int last_pos = 0;
	float pathP = 500;
	float pathI = 0;
	float pathD = 0;

	int pid;
	int error = ref_speed + (measuredData.pos_l - last_pos);

	int saturation = 3000;

	iterm += pathI*error;
	if(iterm > saturation) iterm = saturation;
	if(iterm < -saturation) iterm = -saturation;
	dterm = pathD*(error-last_error);

	pid = pathP*error + iterm - dterm;

	if(pid > saturation) pid = saturation;
	if(pid < -saturation) pid = -saturation;
	last_error = error;
	last_pos = measuredData.pos_l;

	measuredData.pidPosition = pid;

	return pid;
}

int main(void)
{
	hardware_setup();

	int last_x = 0;
	uint8_t filter_iterator = 0;
	uint8_t main_filter_iterator = 0;
	float angle_table[3];
	float acc_table[3];
	float angle_sum = 0;
	float acc_sum = 0;
	char order = '0';
	uint8_t buffer_iterator = 0;
	int ref_angle = 0;
	int filtered_data = 0;
	int ref_speed = 0;

	while(1) {
//		if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE)) {
//			order = USART_ReceiveData(USART3);
//			measuredData.xr = USART_ReceiveData(USART3);
			//static uint8_t buffer_iterator = 0;
			//esp_buffer[buffer_iterator] = USART_ReceiveData(USART3);
			//measuredData.xr = esp_buffer[buffer_iterator];
//			if(esp_buffer[buffer_iterator] == ';') {
//				if(esp_buffer[0]=='p' || esp_buffer[0]=='i' || esp_buffer[0]=='d') {
////					char number_buffer[8];
////					uint8_t i;
////					for(i=0; i<8; i++) {
////						number_buffer[i] = '0';
////					}
////					for(i=1; i<10 || esp_buffer[i] == ';'; i++) {
////						number_buffer[i] = esp_buffer[i];
////					}
////					float regulator_value = (float)atof(number_buffer);
////					measuredData.xr = (int)regulator_value;
//					//measuredData.xr = esp_buffer[1];
//				}
//
//				buffer_iterator = 0;
//			}
//			buffer_iterator++;
//			if(buffer_iterator == 10) buffer_iterator = 0;
			//measuredData.xr = USART_ReceiveData(USART3);
//		}
		if(l_cnt_flag_backward) {
				measuredData.pos_l -= angle_constant;
				measuredData.dir = 0;
				l_cnt_flag_backward = false;
		}
		if(l_cnt_flag_forward) {
				measuredData.pos_l += angle_constant;
				measuredData.dir = 1;
				l_cnt_flag_forward = false;
		}
		if(r_cnt_flag_backward) {
				measuredData.pos_r -= angle_constant;
				r_cnt_flag_backward = false;
		}
		if(r_cnt_flag_forward) {
				measuredData.pos_r += angle_constant;
				r_cnt_flag_forward = false;
		}
		if(velocity_flag) {
			measuredData.xr = measuredData.pos_r*position_constant;
			measuredData.x = measuredData.pos_l*position_constant;
			measuredData.vl = (measuredData.x-last_x)*2;
			last_x = measuredData.x;
			velocity_flag = false;
		}
		if(mpu_flag) {
			mpu_read_data();
			/*angle_table[filter_iterator] = measuredData.fx;
			acc_table[filter_iterator] = measuredData.ax;
			if(++filter_iterator==3) {
				filter_iterator = 0;
				angle_sum += median(angle_table);
				acc_sum += median(acc_table);*/
				angle_sum += measuredData.fx;
				acc_sum += measuredData.ax;
				uint8_t divisor = 10;
				if((main_filter_iterator%divisor)==0) {
//					angle_table[filter_iterator] = angle_sum/divisor;
//					acc_table[filter_iterator] = acc_sum/divisor;
//					angle_sum = 0;
//					acc_sum = 0;
//					if(++filter_iterator==3) {
//						filter_iterator = 0;
//						filtered_angle = median(angle_table);
//						filtered_acc = median(acc_table);
					filtered_angle = angle_sum/divisor;
					filtered_acc = acc_sum/divisor;
					angle_sum = 0;
					acc_sum = 0;
					//if()
					//if(filtered_angle*filtered_acc > 0) kB = 0;
					filtered_data = kA*filtered_angle + kB*filtered_acc;
					//ref_speed = calculate_pid_position(0, filtered_data);
					ref_angle = calculate_angle_from_position(0);
//					ref_angle = stop_robot_angle();
					calculate_pid_output(ref_angle, filtered_data);
			//calculate_pid_output(kA*measuredData.fx + kB*measuredData.ax);
					//robot_velocity_ref = 0;
					//robot_velocity_ref = 1;
					motor_driver();
//					}
				}
				main_filter_iterator++;
			//calculate_pid_output(kA*measuredData.fx + kB*measuredData.ax);
			//motor_driver();
			//}
			mpu_flag = false;
		}
		if(uart_flag) {
			export_data_wifi();
			uart_flag = false;
		}
		order = '0';
	}
}
