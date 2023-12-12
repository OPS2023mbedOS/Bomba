#pragma once
#include <stdio.h>
#include "mbed.h"
#include "globals.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#define SECOND_CONSTANT 1000ms
#define WATCHDOG_START 10000
#define DISP_X_CENTRE 240
#define DISP_Y_CENTRE 135

void countdown();
void timerA();
void displayWrite();