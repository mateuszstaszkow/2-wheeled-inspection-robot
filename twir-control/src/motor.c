#include "motor.h"
#include "main_declarations.h"

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

void set_forward_direction_left() {
	GPIO_SetBits(MOTOR_DIR_LEFT_A_Port, MOTOR_DIR_LEFT_A_Pin);
	GPIO_ResetBits(MOTOR_DIR_LEFT_B_Port, MOTOR_DIR_LEFT_B_Pin);
}

void set_forward_direction_right() {
	GPIO_SetBits(MOTOR_DIR_RIGHT_A_Port, MOTOR_DIR_RIGHT_A_Pin);
	GPIO_ResetBits(MOTOR_DIR_RIGHT_B_Port, MOTOR_DIR_RIGHT_B_Pin);
}

void set_forward_direction() {
	set_forward_direction_left();
	set_forward_direction_right();
}

void set_backward_direction_left() {
	GPIO_SetBits(MOTOR_DIR_LEFT_B_Port, MOTOR_DIR_LEFT_B_Pin);
	GPIO_ResetBits(MOTOR_DIR_LEFT_A_Port, MOTOR_DIR_LEFT_A_Pin);
}

void set_backward_direction_right() {
	GPIO_SetBits(MOTOR_DIR_RIGHT_B_Port, MOTOR_DIR_RIGHT_B_Pin);
	GPIO_ResetBits(MOTOR_DIR_RIGHT_A_Port, MOTOR_DIR_RIGHT_A_Pin);
}

void set_backward_direction() {
	set_backward_direction_left();
	set_backward_direction_right();
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

int get_position_change_difference(int ref_speed) {
	static int last_pos_l = 0;
	static int last_pos_r = 0;
	
	measuredData.motor_dif = ref_speed - ((measuredData.pos_l - last_pos_l) - (measuredData.pos_r - last_pos_r));
	last_pos_l = measuredData.pos_l;
	last_pos_r = measuredData.pos_r;
	
	return measuredData.motor_dif;
}

int calculate_pid_motor_diff(int ref_speed) {
	motor_driver_pid.error = get_position_change_difference(ref_speed);
	measuredData.pid_motor = calculate_pid(&motor_driver_pid);
	
	return measuredData.pid_motor;
}

void set_real_pwm_values(int ref_speed, int16_t *pwm_l, int16_t *pwm_r) {
	int diff = calculate_pid_motor_diff(ref_speed);
	
	if(diff > 0) {
		(*pwm_r) += diff;
		(*pwm_l) -= diff;
	}
	else if(diff < 0) {
		(*pwm_l) += diff;
		(*pwm_r) -= diff;
	}
}

void set_motors_directions(int16_t *pwm_l, int16_t *pwm_r) {
	if((*pwm_l) < 0) {
		(*pwm_l) = -(*pwm_l);
		if(robot_direction_ref) set_backward_direction_left();
		else set_forward_direction_left();
	} else if((*pwm_r) < 0) {
		(*pwm_r) = -(*pwm_r);
		if(robot_direction_ref) set_backward_direction_right();
		else set_forward_direction_right();
	}
}

void set_equal_motors_speed(int ref_speed) {
	int16_t pwm_l = robot_velocity_ref;
	int16_t pwm_r = robot_velocity_ref;

	set_real_pwm_values(ref_speed, &pwm_l, &pwm_r);
	set_motors_directions(&pwm_l, &pwm_r);

	if(pwm_l > MOTOR_PWM_PERIOD) pwm_l = MOTOR_PWM_PERIOD;
	if(pwm_r > MOTOR_PWM_PERIOD) pwm_r = MOTOR_PWM_PERIOD;

	set_pwm(pwm_l, pwm_r);
}

void motor_driver(int ref_speed) {
	set_direction();
	set_equal_motors_speed(ref_speed);
}
