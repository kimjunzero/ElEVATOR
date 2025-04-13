#include "led.h"
#include "extern.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void led_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void led_all_on(void)
{
#if 1
  // printf("int %d\n", sizeof(int));   // 4로 찍히는지 확인
  *(unsigned int *) GPIOB_ODR = 0xff;
#else  // org
//	HAL_GPIO_WritePin(GPIOB, 0xff, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
			GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 1);
#endif
}

void led_all_off(void)
{
#if 1
	  // printf("int %d\n", sizeof(int));   // 4로 찍히는지 확인
	  *(unsigned int *) GPIOB_ODR = 0x00;
#else  // org
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|
			GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0);
#endif
}

// 7 6 5 4 3 <- 2 <-1 <-0
// 200ms주기로 동작 (기존에 on된 led는 off로 처리)
void led_up_on(void)
{
	for (int i=0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}
// 7-> 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0
void led_down_on(void)
{
	for (int i=0; i < 8; i++)
	{
		led_all_off();
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}

void flower_on(void)
{
#if 1  // 구조체 pointer member
	for (int i=0; i < 4; i++)
	{
	    GPIOB->ODR |= 0x10 << i | 0x08 >> i;
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
#endif

#if 0  // DMA
	for (int i=0; i < 4; i++)
	{
		*(unsigned int *) GPIOB_ODR |= 0x10 << i | 0x08 >> i;
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
#endif

#if 0  // HAL
	for (int i=0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x10 << i | 0x08 >> i , 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
#endif
}

void flower_off(void)
{
	led_all_on();
	HAL_Delay(200);
	for (int i=0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x10 << (3-i) | 0x08 >> (3-i) , 0);
		HAL_Delay(200);
	}
}

// 7 6 5 4 3 <- 2 <-1 <-0  기존 on된것 끄지 말고 유지
void shift_left_keep_ledon()
{
	for (int i=0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}

// 7->6->5....->0 기존 on된것 끄지 말고 유지
void shift_right_keep_ledon()
{
	for (int i=0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, 1);
		HAL_Delay(200);
	}
	led_all_off();
	HAL_Delay(200);
}

void led_main(void)
{

	uint8_t led_buff[8] = {0xFF, 0x0F, 0xF0, 0x00,0xFF, 0x0F, 0xF0, 0x00};

	while (1)
	{
#if 1
			  GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down ODR(Output Data Register)
			  GPIOB->ODR |= GPIO_PIN_13;    // latch핀을 pull-up ODR(Output Data Register)
			  HAL_Delay(500);

			  GPIOB->ODR &= ~ GPIO_PIN_13;
			  GPIOB->ODR |= GPIO_PIN_13;
			  HAL_Delay(500);
#else
			  for (int i=0; i < 4; i++)
			  {
				  HAL_SPI_Transmit(&hspi2, &led_buff[i], 1, 1);
				  GPIOB->ODR &= ~ GPIO_PIN_13;   // latch핀을 pull-down
				  GPIOB->ODR |= GPIO_PIN_13;   //  // latch핀을 pull-up
				  HAL_Delay(1000);
			  }
#endif

	}
}

