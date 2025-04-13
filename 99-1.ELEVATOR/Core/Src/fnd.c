#include "main.h"
#include "extern.h"

// 외부 변수 정의
extern uint8_t current_floor;
extern uint8_t target_floor;
extern enum { IDLE, MOVING_UP, MOVING_DOWN } elevator_state;

const uint8_t fnd_font[] = {
    0xf9, // 0: '1'
    0xa4, // 1: '2'
    0xb0, // 2: '3'
    0x99, // 3: '4'
    0xc1, // 4: 'U'
    0x8c, // 5: 'P'
    0xa1, // 6: 'd'
    0xab, // 7: 'n'
    0xf7  // 8: '_'
};

enum {
    FONT_1, FONT_2, FONT_3, FONT_4,
    FONT_U, FONT_P, FONT_d, FONT_n, FONT_UNDER
};

// shift register 핀 설정
#define SR_SER   GPIO_PIN_4  // DS
#define SR_CLK   GPIO_PIN_6  // SH_CP
#define SR_LATCH GPIO_PIN_5  // ST_CP

void shift_out(uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(GPIOB, SR_CLK, 0);
        HAL_GPIO_WritePin(GPIOB, SR_SER, (data & (0x80 >> i)) ? 1 : 0);
        HAL_GPIO_WritePin(GPIOB, SR_CLK, 1);
    }
}

void latch(void)
{
    HAL_GPIO_WritePin(GPIOB, SR_LATCH, 0);
    HAL_GPIO_WritePin(GPIOB, SR_LATCH, 1);
}

void fnd_display_update(void)
{
    static uint8_t digit_index = 0;
    uint8_t font_code = FONT_UNDER;

    // 상태에 따라 digit 자리별 문자 지정
    if (elevator_state == MOVING_UP) {
        switch (digit_index) {
            case 0: font_code = FONT_U; break;
            case 1: font_code = FONT_P; break;
            case 2: font_code = FONT_UNDER; break;
            case 3:
                switch (target_floor) {
                    case 1: font_code = FONT_1; break;
                    case 2: font_code = FONT_2; break;
                    case 3: font_code = FONT_3; break;
                    case 4: font_code = FONT_4; break;
                    default: font_code = FONT_UNDER; break;
                }
                break;
        }
    } else if (elevator_state == MOVING_DOWN) {
        switch (digit_index) {
            case 0: font_code = FONT_d; break;
            case 1: font_code = FONT_n; break;
            case 2: font_code = FONT_UNDER; break;
            case 3:
                switch (target_floor) {
                    case 1: font_code = FONT_1; break;
                    case 2: font_code = FONT_2; break;
                    case 3: font_code = FONT_3; break;
                    case 4: font_code = FONT_4; break;
                    default: font_code = FONT_UNDER; break;
                }
                break;
        }
    } else { // IDLE 상태 또는 도착했을 때 → 언더바 4개 출력
        font_code = FONT_UNDER;
    }

    uint8_t digit_sel = (0x08 >> digit_index);      // 공통 애노드 → HIGH로 자리 선택
    uint8_t seg_data  = fnd_font[font_code];        // 세그먼트 값 그대로 사용

    shift_out(seg_data);      // 세그먼트 먼저
    shift_out(digit_sel);     // 자리 선택 나중
    latch();

    digit_index = (digit_index + 1) % 4;
}






