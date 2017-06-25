#include <avr/interrupt.h>
#include <avr/iom168.h>
#include <util/delay.h>
#include <stdbool.h>

//Motor A defintion
#define MOTORA_PORT PORTD
#define MOTORA_DDR DDRD
#define PWMA_PORT (1<<PD0)
#define INA1_PORT (1<<PD1)
#define INA2_PORT (1<<PD2)

//Motor B defintion
#define MOTORB_PORT PORTD
#define MOTORB_DDR DDRD
#define PWMB_PORT (1<<PD3)
#define INB1_PORT (1<<PD4)
#define INB2_PORT (1<<PD5)

//PORTX: 0 - forward, 1 - backwards
#define PINR_DIR (1<<PC2)
#define PINL_DIR (1<<PC1)
#define PINR_DDR DDRC
#define PINL_DDR DDRC
#define PINR_PORT PORTC
#define PINL_PORT PORTC
#define DIR_L (PINC & PINL_DIR)
#define DIR_R (PINC & PINR_DIR)

#define VELOCITY_CONSTANT 2.89

#define PWM_INTERRUPT_PERIOD 8
#define MEASUREMENT_INTERRUPT_PERIOD 32

struct MeasuredData {
	int motor_speed_l;
	int motor_speed_r;
	int robot_velocity_ref;
	int l_dir_ref;
	int r_dir_ref;
	int battery_state;
};

//Motors' speed
uint8_t pwm_value;
uint8_t min_pwm;
uint8_t max_pwm;

volatile bool pwm_flag;
volatile bool measurement_flag;
volatile uint32_t global_time_100us;

struct MeasuredData measuredData;

uint16_t measure(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF8) | channel;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return ADCW;
}

void robot_stop() {
	MOTORA_PORT &= ~INA1_PORT;
	MOTORA_PORT &= ~INA2_PORT;
	MOTORB_PORT &= ~INB1_PORT;
	MOTORB_PORT &= ~INB2_PORT;
}

void hardware_setup() {
	//Ports configuration
	MOTORA_DDR |= PWMA_PORT | INA1_PORT | INA2_PORT;
	MOTORB_DDR |= PWMB_PORT | INB1_PORT | INB2_PORT;

	//ADC initialization
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);

	//Initialization
	pwm_value = 0;
	robot_stop();

	//PWM timer configuration
	//TCCR2A |= (1<<WGM21);		//CTC mode
	TCCR2B |= (1<<CS20);		//Presdaler = 8
	//OCR2A = 30;					//Additional frequency division by 200
	//TIMSK2 |= (1<<OCIE2A);		//Compare to OCR2A register enabled
	TIMSK2 |= (1<<TOIE2);
	TCNT2 = 56;

	//Minimum and maximum PWM value to start motor's movement
	min_pwm = 0;
	max_pwm = 254;

	pwm_flag = false;

	//Direction detection ports initialization
	PINR_DDR &= ~(PINR_DIR);
	PINR_PORT |= (PINR_DIR);
	PINL_DDR &= ~(PINL_DIR);
	PINL_PORT |= (PINL_DIR);

	//Set the global interrupt flag
	sei();
}

void gather_data() {

	//measuredData.motor_speed_l = measure(5);

	//measuredData.motor_speed_r = measure(4);

	measuredData.robot_velocity_ref = measure(1)/VELOCITY_CONSTANT;
	if(measuredData.robot_velocity_ref < min_pwm) measuredData.robot_velocity_ref = min_pwm;
	if(measuredData.robot_velocity_ref > max_pwm) measuredData.robot_velocity_ref = max_pwm;
	measuredData.l_dir_ref = DIR_L;
	measuredData.r_dir_ref = DIR_R;

	//measuredData.battery_state = measure(0);
}

void set_direction() {
	if((measuredData.l_dir_ref) && (measuredData.r_dir_ref)) {
		MOTORA_PORT |= INA1_PORT;
		MOTORA_PORT &= ~INA2_PORT;
		MOTORB_PORT |= INB1_PORT;
		MOTORB_PORT &= ~INB2_PORT;
	} else if((!measuredData.l_dir_ref) && (!measuredData.r_dir_ref)) {
		MOTORA_PORT &= ~INA1_PORT;
		MOTORA_PORT |= INA2_PORT;
		MOTORB_PORT &= ~INB1_PORT;
		MOTORB_PORT |= INB2_PORT;
	}
}

int main(void) {
	hardware_setup();

	uint16_t pwm_sum = 0;
	uint8_t loop_iterator = 0;
	uint8_t pwm_counter = 0;

	while(1) {

		if(measurement_flag) {
			gather_data();
			set_direction();

			pwm_sum += measuredData.robot_velocity_ref;
			if(!(++loop_iterator % 100)) {
				pwm_value = pwm_sum/100;
				pwm_sum = 0;
			}
			measurement_flag = false;
		}
		if(pwm_flag) {
			//Direct PWM channels output control. See declarations of pwmx_x variables.
			if(pwm_counter <= pwm_value) MOTORA_PORT |= PWMA_PORT; else MOTORA_PORT &= ~PWMA_PORT;
			if(pwm_counter <= pwm_value) MOTORB_PORT |= PWMB_PORT; else MOTORB_PORT &= ~PWMB_PORT;

			pwm_counter++;
			pwm_flag = false;
		}
	}
}

//Software PWM configuration (TIMER2)
ISR( TIMER2_OVF_vect )
{
	global_time_100us++;
	if(!(global_time_100us % PWM_INTERRUPT_PERIOD)) pwm_flag = true;
	if(!(global_time_100us % MEASUREMENT_INTERRUPT_PERIOD)) measurement_flag = true;
}
