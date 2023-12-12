#include "main.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <string>
Thread timerThread;
Thread threadDisp;
Thread explodeThread;
Thread resetThread;
Thread stopThread;

int timeleft = 10;

EventFlags event_flags;

int pole[3] = {RESET_TIME,EXPLODE_FLAG,STOP_TIME};

Mutex mut_display;
Mutex mut_flag;

void displayThread(){
    TS_StateTypeDef TS_State;
    uint16_t x,y;
    while (1){
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected){
            x = TS_State.touchX[0];
            y = TS_State.touchY[0];
            if(x>=60 && x<=140 && y>=100 && y<=180){
                mut_flag.lock();
                event_flags.set(pole[0]);
                mut_flag.unlock();
            } else if(x>=200 && x<=280 && y>=100 && y<=180){
                mut_flag.lock();
                event_flags.set(pole[1]);
                mut_flag.unlock();
            } else if(x>=340 && x<=420 && y>=100 && y<=180){
                mut_flag.lock();
                event_flags.set(pole[2]);
                mut_flag.unlock();
            }
        }
    }
}

void stop(){
    while (1) {
        event_flags.wait_any(STOP_TIME);
        //Watchdog::get_instance().stop();
        timers.detach();
        while(true){
            Watchdog::get_instance().kick();
        }
    }
}

void reset(){
    while (1) {
        event_flags.wait_any(RESET_TIME);
        Watchdog::get_instance().kick();
        timeleft=10;
    }
}

void explode(){
    while(1){
        event_flags.wait_any(EXPLODE_FLAG);
        Watchdog::get_instance().kick();
        timers.detach();
        mut_display.lock();
        BSP_LCD_Clear(LCD_COLOR_RED);
        mut_display.unlock();
        ThisThread::sleep_for(EXPLODE_FOR);
        NVIC_SystemReset();
    }
}


// main() runs in its own thread in the OS
int main()
{
    uint8_t status;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(std::begin(pole), std::end(pole), std::default_random_engine(seed));
    //random_shuffle(std::begin(pole), std::end(pole)); 
    mut_display.lock();
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    
    
    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    if (status != TS_OK) {
        BSP_LCD_Clear(LCD_COLOR_RED);
        BSP_LCD_SetBackColor(LCD_COLOR_RED);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
    } else {
        BSP_LCD_Clear(LCD_COLOR_GREEN);
        BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }

    BSP_LCD_Clear(LCD_COLOR_BLACK);

    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(60, 100, 80, 80);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(200, 100, 80, 80);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(340, 100, 80, 80);

    mut_display.unlock();
    /*BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);*/
    /*BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"Debug", CENTER_MODE);
    ThisThread::sleep_for(3000ms);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"Debug", CENTER_MODE);*/
    timerThread.start(timerA);

    threadDisp.start(displayThread);
    explodeThread.start(explode);
    resetThread.start(reset);
    stopThread.start(stop);
    while (true) {
        
    }
}

