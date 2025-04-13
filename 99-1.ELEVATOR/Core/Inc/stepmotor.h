#include "main.h"
#include "button.h"
#include <stdio.h>

// btn0 : idle(stop)
// btn1 : FW <--> BW
#define IDLE     0
#define FORWARD  1
#define BACKWARD 2
#define FLOOR_MAX 4
typedef enum { ARRIVE, MOVING_UP, MOVING_DOWN } ElevatorState;

void set_rpm(int rpm);
void handle_outside_buttons(void);
void handle_inside_buttons(void);
void stepmotor_main(void);
int stepmotor_drive(int direction);
void loop(void);
