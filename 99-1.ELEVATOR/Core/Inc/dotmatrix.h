#ifndef INC_DOTMATRIX_H_
#define INC_DOTMATRIX_H_

#include "main.h"

uint8_t col[4]={0,0,0,0};

unsigned char display_data[8];  // 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = {0};  // 스코롤할 데이타가 들어있는 버퍼
#define NUMBER_OF_NAME		3  // 출력할 문자 갯수

void dotmatrix_main(void);
void button_check_dotmatrix(void);
void turn_idle(void);
void init_dotmatrix_name(void);
void init_dotmatrix_up(void);
void init_dotmatrix_down(void);
void scroll_name(void);
void scroll_up(void);
void scroll_down(void);

#endif /* INC_DOTMATRIX_H_ */
