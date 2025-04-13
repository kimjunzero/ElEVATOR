#include "dotmatrix.h"
#include "extern.h"
#include "button.h"
#include <string.h>

extern uint8_t current_floor;
extern uint8_t target_floor;
extern enum { IDLE, MOVING_UP, MOVING_DOWN } elevator_state;

int count = 0;
int idx = 0;
int sec_count = 0;
uint32_t past_time = 0;
uint8_t temp;

unsigned char display_data[8];  // 최종 8x8 출력할 데이터

void scroll_up(void);
void scroll_down(void);
void dotmatrix_display(void);
extern void handle_floor_buttons(void);


uint8_t one_floor[] =
{
   0b00001000,
   0b00011000,
   0b00101000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b00001000,
   0b01111110
};

uint8_t two_floor[] =
{
   0b00011000,
   0b00100100,
   0b00100100,
   0b00000100,
   0b00001000,
   0b00010000,
   0b00100000,
   0b01111110
};

uint8_t three_floor[] =
{
   0b00011000,
   0b00100100,
   0b00100100,
   0b00000100,
   0b00001000,
   0b00100100,
   0b00100100,
   0b00011000

};

uint8_t four_floor[] =
{
   0b00000000,
   0b00001000,
   0b00011000,
   0b00101000,
   0b01111110,
   0b00001000,
   0b00001000,
   0b00001000
};

uint8_t arrow_up[] =
{
   0b00000000,
   0b00011000,
   0b00111100,
   0b01111110,
   0b00011000,
   0b00011000,
   0b00011000,
   0b00000000
};

uint8_t arrow_down[] =
{
   0b00000000,
   0b00011000,
   0b00011000,
   0b00011000,
   0b01111110,
   0b00111100,
   0b00011000,
   0b00000000
};

void dotmatrix_main(void)
{
    while (1)
    {
        switch (elevator_state) {
            case MOVING_UP:
                scroll_up(); // 올라가는 화살표 표시
                break;
            case MOVING_DOWN:
                scroll_down(); // 내려가는 화살표 표시
                break;
            case IDLE: // 초기 상태, 도착상태
            default:
                show_floor(current_floor);  // 도착 또는 대기 상태 시 해당 층 표시
                break;
        }
    }
}

void show_floor(uint8_t floor)
{
    switch (floor) {
        case 1: memcpy(display_data, one_floor, 8); break;
        case 2: memcpy(display_data, two_floor, 8); break;
        case 3: memcpy(display_data, three_floor, 8); break;
        case 4: memcpy(display_data, four_floor, 8); break;
        default: break;
    }
    dotmatrix_display();
}

void scroll_up(void)
{
    static uint32_t up_time = 0;
    static int up_idx = 0;

    uint32_t now = HAL_GetTick();
    if (now - up_time >= 100) {
        up_time = now;

        for (int i = 0; i < 8; i++) {
            // 위로 스크롤하려면 다음 행을 가져옴 (modulo 연산으로 순환)
            display_data[i] = arrow_up[(i + up_idx) % 8];
        }

        up_idx++;
        if (up_idx >= 8) up_idx = 0;
    }

    dotmatrix_display();
}

void scroll_down(void)
{
    static uint32_t down_time = 0;
    static int down_idx = 0;

    uint32_t now = HAL_GetTick();
    if (now - down_time >= 100) {
        down_time = now;

        for (int i = 0; i < 8; i++) {
            // 아래로 스크롤하려면 이전 행을 가져옴
            display_data[i] = arrow_down[(i - down_idx + 8) % 8];
        }

        down_idx++;
        if (down_idx >= 8) down_idx = 0;
    }

    dotmatrix_display();
}


void dotmatrix_display(void)
{
    for (int i = 0; i < 8; i++) {
        col[0] = ~(1 << i);
        col[1] = display_data[i];
        for (int j = 0; j < 2; j++) {
            temp = col[j];
            for (int k = 7; k >= 0; k--) {
                HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, (temp & (1 << k)) ? 1 : 0);
                HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 1);
                HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 0);
            }
        }
        GPIOB->ODR &= ~GPIO_PIN_13;
        GPIOB->ODR |= GPIO_PIN_13;
        HAL_Delay(1);
    }
}
