#include "extint.h"
#include "stepmotor.h"
#include "buzzer.h"

extern uint8_t stepmotor_state;
extern uint8_t elevator_state;
extern uint8_t current_floor;
extern uint8_t target_floor;
/**
 * move from Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c to here
 * external INT callback function
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	switch(GPIO_Pin){
//	case GPIO_PIN_0:   // PC0 == BTN0
//		stepmotor_state == IDLE;
//printf("GPIO_PIN_0\n");
//		break;
//	case GPIO_PIN_4:   // PC4 == BTN1
//		if(stepmotor_state == IDLE)
//			stepmotor_state = FORWARD;
//		else if(stepmotor_state == FORWARD)
//			stepmotor_state = BACKWARD;
//		else if(stepmotor_state == BACKWARD)
//			stepmotor_state = FORWARD;
//printf("GPIO_PIN_1\n");
//		break;
//	}
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin) {
        case GPIO_PIN_0: current_floor = 1; break;
        case GPIO_PIN_1: current_floor = 2; break;
        case GPIO_PIN_4: current_floor = 3; break;
        case GPIO_PIN_7: current_floor = 4; break;
    }

    if (current_floor == target_floor) {
        stepmotor_state = IDLE;
        elevator_state = ARRIVE;
        printf("도착: %d층\n", current_floor);
        beep(1);
    }


    switch(current_floor) {
        case 1:
        	if(current_floor == target_floor)
        	{
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
        	}
        	else
        	{
            	HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
        	}
        	break;
        case 2:
        	if(current_floor == target_floor)
        	{
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
        	}
        	else
        	{
        		HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
        	}
        	break;
        	break;
        case 3:
        	if(current_floor == target_floor)
        	{
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
        	}
        	else
        	{
        		HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
        	}
        	break;
        case 4:
        	if(current_floor == target_floor)
        	{
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);
            	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
        	}
        	else
        	{
        		HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
        	}
        	break;
    }
}


