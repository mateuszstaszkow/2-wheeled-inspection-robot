/**
  ******************************************************************************
  * @author  Mateusz Staszków
  * @version V1.0
  * @date    01-December-2016
  * @brief   Two wheeled inspection robot control.
  ******************************************************************************
*/

#include "main_declarations.h"

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
	if(!(global_time_ms % BATTERY_STATUS_PERIOD_MS)) battery_flag = true;
	if(!(global_time_ms % ROBOT_SINGLE_TURN_PERIOD_MS) && turn_mode_flag) turn_flag = true;
	if(!(global_time_ms % WIFI_ORDER_READ_PERIOD_MS)) execute_flag = true;
}

void init_reference_values() {
	robot_direction_ref = 1;
	robot_linear_velocity_ref = 0;
	robot_velocity_ref = 0;
	robot_turn_speed_ref = 0;
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
	hcsr_flag = false;

	start_flag = false;
	execute_flag = false;
	turn_flag = false;
	busy_turning_flag = false;
	turn_mode_flag = false;
	sector_captured_flag = false;
}

void set_tables() {
	int i;
	
	for(i = 0; i<6; i++) {
		gyro[i] = 0;
	}
	for(i=0; i<10; i++) {
		esp_buffer[i] = 0;
	}
}

void init_pid_structure(struct DataPID *data_pid, const float kP, const float kI, const float kD, const int saturation) {
	data_pid->kP = kP;
	data_pid->kI = kI;
	data_pid->kD = kD;
	data_pid->saturation = saturation;
	data_pid->last_error = 0;
	data_pid->iterm = 0;
	data_pid->dterm = 0;
	data_pid->error = 0;
}

void global_variables_init() {
	init_reference_values();

	init_flags();
	
	init_pid_structure(&motor_driver_pid, MOTOR_KP, MOTOR_KI, MOTOR_KD, MOTOR_SATURATION);
	init_pid_structure(&balance_pid, BALANCE_KP, BALANCE_KI, BALANCE_KD, BALANCE_SATURATION);
	init_pid_structure(&velocity_pid, VELOCITY_KP, VELOCITY_KI, VELOCITY_KD, VELOCITY_SATURATION);

	set_tables();
}

void global_timer_init() {
	SysTick_Config(SystemCoreClock / SYS_TICK_INTERRUPT_FREQUENCY_HZ);
}

void timer_us_init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef tim;

	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 64 - 1;
	tim.TIM_Period = 10000 - 1;
	TIM_TimeBaseInit(TIM3, &tim);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void hardware_setup() {
	encoder_init();
	motor_driver_init();
	esp_init();
	hcsr_init();
	MPU6050_I2C_Init();
	MPU6050_Initialize();
	global_variables_init();
	global_timer_init();
	timer_us_init();
}

void get_order() {
	if(!(execute_flag && USART_GetFlagStatus(USART3, USART_FLAG_RXNE))) return;

	char order = USART_ReceiveData(USART3);

	if(order == 'n') {
		turn_mode_flag = true;
		hcsr_flag = true;
	}
	else if(order == 'f') {
		turn_mode_flag = false;
		busy_turning_flag = false;
		turn_flag = false;
		hcsr_flag = false;
		sector_captured_flag = false;
	}

	execute_flag = false;
}

void turn_right() {
	set_forward_direction_left();
	set_backward_direction_right();
	set_pwm(SCAN_ROTATION_SPEED, SCAN_ROTATION_SPEED);

	busy_turning_flag = true;
}

void rotate_robot() {
	static uint32_t current_time = 0;

	if(busy_turning_flag && ((global_time_ms - current_time) > SCAN_ROTATION_TIME)) {
		busy_turning_flag = false;
		turn_flag = false;
		hcsr_flag = true;
		return;
	} else if(busy_turning_flag) {
		return;
	}

	current_time = global_time_ms;
	turn_right();
}

void correct_robot_position() {
	if(!start_flag) return;
	
	if(sector_captured_flag && ((turn_flag && is_balanced()) || busy_turning_flag)) {
		rotate_robot();
	} else {
		filtered_balancing_data = simple_complementary_filter();
		calculate_pid_output(calculate_angle_from_speed(robot_linear_velocity_ref), filtered_balancing_data);
		motor_driver(robot_turn_speed_ref);
		if(hcsr_flag && is_balanced()) {
			hcsr_get_distance();
			hcsr_flag = false;
			sector_captured_flag = true;
		}
	}

	start_flag = false;
}

void stabilize() {
	if(!mpu_flag) return;
	
	mpu_read_data();
	average_filter();
	correct_robot_position();
	
	mpu_flag = false;
}

void export_data_wifi() {
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,;\r\n",
			(int)filtered_angle / MPU_CONSTANT,
			(int)measuredData.fy,
		    (int)measuredData.fz,
			(int)filtered_acc / MPU_CONSTANT,
			(int)measuredData.ay,
			(int)measuredData.az,
			(int)measuredData.pos_l % 360,
			(int)measuredData.pos_r % 360,
			(int)measuredData.xl,
			(int)measuredData.vl,
			(int)measuredData.dir,
			(int)measuredData.pid / 10,
			(int)(turn_mode_flag ? 1 : 0),
			(int)measuredData.dist_m * HCSR_CONSTANT,
			(int)filtered_balancing_data,
			(int)measuredData.pos_dif,
			(int)measuredData.pid_position,
			(int)measuredData.motor_dif;
			(int)measuredData.pid_motor;
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

void start() {
	while(1) {
		get_order();
		update_encoder_values();
		update_linear_postion();
		update_battery_state();
		stabilize();
		update_esp_values();
	}
}
