#pragma once
#include <stdio.h>
#include "mbed.h"
#include "timer.h"
#include "globals.h"
#define EXPLODE_FLAG (1UL << 0)
#define RESET_TIME (1UL << 1)
#define STOP_TIME (1UL << 2)
#include <algorithm>
#include <chrono>
#include <random>