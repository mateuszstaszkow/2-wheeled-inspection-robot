#include "math_pid.h"

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

void anti_windup(struct DataPID *data_pid) {
	if(data_pid->iterm > data_pid->saturation) data_pid->iterm = data_pid->saturation;
	if(data_pid->iterm < -data_pid->saturation) data_pid->iterm = -data_pid->saturation;
}

void integrate(struct DataPID *data_pid) {
	data_pid->iterm += data_pid->kI * data_pid->error;
	
	anti_windup(data_pid);
}

void differentiate(struct DataPID *data_pid) {
	data_pid->dterm = data_pid->kD * (data_pid->error - data_pid->last_error);
	data_pid->last_error = data_pid->error;
}

int get_pid(struct DataPID *data_pid) {
	int pid = 0;
	
	pid = data_pid->kP * data_pid->error + data_pid->iterm - data_pid->dterm;
	
	if(pid > data_pid->saturation) pid = data_pid->saturation;
	if(pid < -data_pid->saturation) pid = -data_pid->saturation;
	
	return pid;
}

int calculate_pid(struct DataPID *data_pid) {
	integrate(data_pid);
	differentiate(data_pid);
	
	return get_pid(data_pid);
}