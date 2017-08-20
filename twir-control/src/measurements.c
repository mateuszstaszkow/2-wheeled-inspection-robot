#include "measurements.h"

void mpu_read_data() {
	MPU6050_GetRawAccelGyro(gyro);
	measuredData.fx = gyro[0];
	measuredData.fy = gyro[1]/MPU_CONSTANT;
	measuredData.fz = gyro[2]/MPU_CONSTANT;
	measuredData.ax = gyro[4];
	measuredData.ay = gyro[3]/MPU_CONSTANT;
	measuredData.az = gyro[5]/MPU_CONSTANT;
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
	
	static int last_x = 0;

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