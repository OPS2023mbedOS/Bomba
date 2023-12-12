#pragma once
#include "mbed.h"
#define EXPLODE_FOR 1000ms
extern int timeleft;
extern Ticker timers;
extern Mutex mut_display;
extern Mutex mut_flag;