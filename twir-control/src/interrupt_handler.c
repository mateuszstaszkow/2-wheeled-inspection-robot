#include "interrupt_handler.h"

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        turn_off_triggers();
        TIM_Cmd(TIM2, DISABLE);
    }
}

void EXTI4_IRQHandler() {
	if(EXTI_GetITStatus(HCSR04_RIGHT_ECHO_Interrupt_Line)) {
		if(GPIO_ReadInputDataBit(HCSR04_RIGHT_ECHO_Port, HCSR04_RIGHT_ECHO_Pin) == 1) {
			TIM_SetCounter(TIM3, 0);
		} else if(GPIO_ReadInputDataBit(HCSR04_RIGHT_ECHO_Port, HCSR04_RIGHT_ECHO_Pin) == 0) {
			measuredData.dist_r = TIM_GetCounter(TIM3);
		}

		EXTI_ClearITPendingBit(HCSR04_RIGHT_ECHO_Interrupt_Line);
	}
	if(EXTI_GetITStatus(HCSR04_MIDDLE_ECHO_Interrupt_Line)) {
		if(GPIO_ReadInputDataBit(HCSR04_MIDDLE_ECHO_Port, HCSR04_MIDDLE_ECHO_Pin) == 1) {
			TIM_SetCounter(TIM3, 0);
		} else if(GPIO_ReadInputDataBit(HCSR04_MIDDLE_ECHO_Port, HCSR04_MIDDLE_ECHO_Pin) == 0) {
			measuredData.dist_m = TIM_GetCounter(TIM3);
		}

		EXTI_ClearITPendingBit(HCSR04_MIDDLE_ECHO_Interrupt_Line);
	}
}

void EXTI9_5_IRQHandler() {
	if((!((r_encoder_counter++) % ENCODER_INTERRUPT_FREQUENCY_DIVISOR)) && EXTI_GetITStatus(ENCODER_RIGHT_Interrupt_Line)) {
		if (GPIO_ReadInputDataBit(ENCODER_RIGHT_Port, ENCODER_RIGHT_A_Pin) == 1) {
			if(GPIO_ReadInputDataBit(ENCODER_RIGHT_Port, ENCODER_RIGHT_B_Pin) == 0) r_cnt_flag_forward = true;
			else r_cnt_flag_backward = true;
		}
	}
	if(EXTI_GetITStatus(ENCODER_RIGHT_Interrupt_Line)) EXTI_ClearITPendingBit(ENCODER_RIGHT_Interrupt_Line);

	if(EXTI_GetITStatus(HCSR04_LEFT_ECHO_Interrupt_Line)) {
		if(GPIO_ReadInputDataBit(HCSR04_LEFT_ECHO_Port, HCSR04_LEFT_ECHO_Pin) == 1) {
			TIM_SetCounter(TIM3, 0);
		} else if(GPIO_ReadInputDataBit(HCSR04_LEFT_ECHO_Port, HCSR04_LEFT_ECHO_Pin) == 0) {
			measuredData.dist_l = TIM_GetCounter(TIM3);
		}
		EXTI_ClearITPendingBit(HCSR04_LEFT_ECHO_Interrupt_Line);
	}
}

void EXTI15_10_IRQHandler() {
	if((!((l_encoder_counter++) % ENCODER_INTERRUPT_FREQUENCY_DIVISOR)) && EXTI_GetITStatus(ENCODER_LEFT_Interrupt_Line)) {
		if (GPIO_ReadInputDataBit(ENCODER_LEFT_Port, ENCODER_LEFT_A_Pin) == 1) {
			if(GPIO_ReadInputDataBit(ENCODER_LEFT_Port, ENCODER_LEFT_B_Pin) == 0) l_cnt_flag_backward = true;
			else l_cnt_flag_forward = true;
		}
	}
	if(EXTI_GetITStatus(ENCODER_LEFT_Interrupt_Line)) EXTI_ClearITPendingBit(ENCODER_LEFT_Interrupt_Line);
}
