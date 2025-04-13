extern uint8_t rx_data;   // uart2 rx byte
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
extern volatile int TIM11_1ms_counter;
extern t_print o_prt;

extern void led_main(void);
extern void button_led_toggle_test(void);
extern void ds1302_main(void);
extern void pc_command_processing(void);
extern void set_rtc(char *date_time);
extern void delay_us(int us);
extern void flash_main();
extern void flash_set_time(void);

extern void i2c_lcd_main(void);
extern void i2c_lcd_init(void);
extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);

// piezo function
extern void siren(int repeat);
extern void fire(int repeat);
extern void amb(int repeat);
extern void noTone(void);

// survo function
extern void servo_motor_main(void);

// dotmatrix function
extern void dotmatrix_main(void);

//stepmotor function
extern void stepmotor_main(void);

// led function
extern void led_on(void);

// fnd function
extern void fnd_display_update(void);
