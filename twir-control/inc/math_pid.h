#ifndef MATH_PID_H_
#define MATH_PID_H_

// Absolute integer calculating function
int abs(int x);
// Median of 3 float values calculating function
float median(float value[]);
// Integration anti-windup
void anti_windup(struct DataPID *data_pid);
// Numerical integration
void integrate(struct DataPID *data_pid);
// Numerical differentiation without involving time - operating just on value differences
void differentiate(struct DataPID *data_pid);
// Calculate PID output using calculated integration and differentiation terms
int get_pid(struct DataPID *data_pid);
// PID regulator implementation
int calculate_pid(struct DataPID *data_pid);

#endif /* MATH_PID_H_ */