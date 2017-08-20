#ifndef MEASUREMENTS_H_
#define MEASUREMENTS_H_

#include "properties.h"
#include "main_declarations.h"

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
	int pos_dif;
	int motor_dif;
	int pid_motor;
	int pid_position;
	int battery_state;
};

// Calculated constants
extern const float ANGLE_CONSTANT = ROTATION_CONSTANT * ENCODER_INTERRUPT_FREQUENCY_DIVISOR * ENCODER_CONSTANT;
extern const float POSITION_CONSTANT = DISTANCE_CONSTANT * WHEEL_DIAMETER * PI / 360;
extern const int VELOCITY_CONSTANT = 1000 / VELOCITY_INTERRUPT_PERIOD_MS;

// MPU6050 raw data vector
extern s16 gyro[6];

// Gathers raw angle and angular acceleration data for six axises, range of values from -16000 to +16000
// Values can be converted to degrees by using MPU_CONSTANT
// gyro[0] - X axis
// gyro[1] - Y axis
// gyro[2] - Z axis
// gyro[3] - Y axis
// gyro[4] - X axis
// gyro[5] - Z axis
void mpu_read_data();

// Checks *_cnt_flag_* flags and updates motors' shafts position and measured moving direction
void update_encoder_values();
// Checks velocity_flag and updates robot linear position and velocity which will be send to ESP8266 module
void update_linear_postion();
// Checks battery_flag and updates battery state which will be send to ESP8266 module
void update_battery_state();

#endif /* MEASUREMENTS_H_ */