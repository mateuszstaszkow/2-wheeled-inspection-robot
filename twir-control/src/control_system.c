#include "control_system.h"

int get_position_difference(int ref_speed) {
	static int last_pos = 0;
	
	measuredData.pos_dif = ref_speed + (measuredData.pos_l - last_pos);
	last_pos = measuredData.pos_l;
	
	return measuredData.pos_dif;
}

int calculate_angle_from_speed(int ref_speed) {
	velocity_pid.error = get_position_difference(ref_speed);
	measuredData.pid_position = calculate_pid(&velocity_pid);

	return measuredData.pid_position;
}

void set_reference_direction() {
	if(measuredData.pid >= 0) robot_direction_ref = 1;
	else robot_direction_ref = 0;
}

void calculate_pid_output(int ref_angle, int angle) {
	balance_pid.error = ref_angle + angle;
	measuredData.pid = calculate_pid(&balance_pid);
	set_reference_direction();
	robot_velocity_ref = abs(measuredData.pid);
}

void average_filter() {
	static int angle_sum = 0;
	static int acc_sum = 0;
	static uint8_t avg_filter_iterator = 0;
	
	angle_sum += measuredData.fx;
	acc_sum += measuredData.ax;
	
	if((++avg_filter_iterator) % BALANCE_AVERAGE) return;
	
	filtered_angle = angle_sum / BALANCE_AVERAGE;
	filtered_acc = acc_sum / BALANCE_AVERAGE;
	angle_sum = 0;
	acc_sum = 0;
	start_flag = true;
}

int simple_complementary_filter() {
	return BALANCE_KA * filtered_angle + BALANCE_KB * filtered_acc;
}