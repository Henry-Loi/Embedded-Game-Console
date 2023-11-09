#pragma once

#include <stdint.h>

int controller_tft(int r);

typedef enum{
    BTN_UP = 0,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT,
    JOY_L_X = 0,
    JOY_L_Y,
    JOY_R_X,
    JOY_R_Y
} button_t ;

typedef struct{
    uint32_t joystick[4];
    uint8_t button[4];
} controller ;
