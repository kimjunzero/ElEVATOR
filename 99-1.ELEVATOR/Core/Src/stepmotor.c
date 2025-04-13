#include "main.h"
#include "stepmotor.h"
#include "button.h"

ElevatorState elevator_state = ARRIVE; // 엘레베이터 상태(ARRIVE, MOVING_UP, MOVING_DOWN)
uint8_t stepmotor_state = IDLE; // 모터 상태(IDLE, FORWARD, BACKWARD)
uint8_t current_floor = 1; // 현재 층 정보
uint8_t target_floor = 1; // 목표 층 정보(버튼 입력시 설정)

void set_rpm(int rpm)
{
    delay_us(60000000 / (4096 * rpm)); // 한바퀴 = 4096 기준.
}

void handle_floor_buttons(void)
{
    if (get_button(GPIOC, BTN0_Pin, BTN0) == BUTTON_PRESS)
    {
    	target_floor = 1;
    	HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
    }
    else if (get_button(GPIOC, BTN1_Pin, BTN1) == BUTTON_PRESS)
    {
    	target_floor = 2;
        HAL_GPIO_WritePin(GPIOB, 0x0f, 0);

    }
    else if (get_button(GPIOC, BTN2_Pin, BTN2) == BUTTON_PRESS)
    {
    	target_floor = 3;
    	HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
    }
    else if (get_button(GPIOC, BTN3_Pin, BTN3) == BUTTON_PRESS)
    {
    	target_floor = 4;
    	HAL_GPIO_WritePin(GPIOB, 0x0f, 0);
    }

    if (target_floor > current_floor){
    	stepmotor_state = FORWARD;
    	elevator_state = MOVING_UP;
    }
    else if (target_floor < current_floor){
    	stepmotor_state = BACKWARD;
    	elevator_state = MOVING_DOWN;
    }
    else
    {
    	stepmotor_state = IDLE;
    	elevator_state = ARRIVE;
    }
}

void stepmotor_main(void)
{
    switch(stepmotor_state) {
        case FORWARD:
            stepmotor_drive(FORWARD);
            set_rpm(13);
            break;

        case BACKWARD:
            stepmotor_drive(BACKWARD);
            set_rpm(13);
            break;

        case IDLE:
            break;
    }
}

int stepmotor_drive(int direction)
{
    static int step = 0;

    switch(step) {
        case 0: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0); break;
        //case 1: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0); break;
        case 2: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0); break;
        //case 3: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0); break;
        case 4: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 0); break;
        //case 5: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1); break;
        case 6: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1); break;
        //case 7: HAL_GPIO_WritePin(GPIOC, IN1_Pin, 1); HAL_GPIO_WritePin(GPIOC, IN2_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN3_Pin, 0); HAL_GPIO_WritePin(GPIOC, IN4_Pin, 1); break;
    }

    if(direction == FORWARD)
        step = (step + 2) % 8;
    else
        step = (step == 0) ? 6 : step - 2;

    return step;
}

void loop(void)
{
    handle_floor_buttons();     // 숫자 버튼 입력 (층 선택)
    stepmotor_main();           // 상태 FSM 실행
    HAL_Delay(1);               // 부하 감소
}


