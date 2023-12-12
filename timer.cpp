#include "timer.h"
#include <cstdio>
#include <cstring>
#include <string>
Ticker timers;

void displayWrite(){

    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    std::string text = std::to_string(timeleft);
    BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)&text, CENTER_MODE);
}

void countdown(){
    timeleft=timeleft-1;
    displayWrite();
    if(timeleft==0){
        BSP_LCD_Clear(LCD_COLOR_RED);
    }
}

void timerA(){
    
    timers.attach(&countdown, SECOND_CONSTANT);
    Watchdog &watchdog = Watchdog::get_instance();
    watchdog.start(WATCHDOG_START);
}