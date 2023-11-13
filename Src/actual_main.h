
#include "stm32f4xx_hal_tim.h"
#include "lcd_async.h"
#include <stdio.h>
#include <math.h>

#include "angle_abstraction.h"

void onSetup()
{
    LCD_Init();
    HAL_TIM_Base_Start(&htim1);
    gripperHoming();
    angleAbstractionInit();

    beeperBeep(80, 150);
    beeperBeep(80, 150);
    beeperBeep(80, 150);

}

long lastPrintTick = 0;
long lastLCDTick = 0;


int buttonDown = 0;
int motorID = 0;

void tick(long now, long dt);

double angles[4];

void onUpdate(long now, long dt)
{

    if (joystickBtn != buttonDown) {
        buttonDown = joystickBtn;
        if (buttonDown) {
            motorID++;
            motorID = motorID % 4;
            beeperBeep(80, 0);
        }
    }

    if (now - lastPrintTick > 1000 / 4) {
        lastPrintTick = now;
        LCD_Clear();
        char s[16];
        sprintf(s, "M[%d] = %.1fdeg", motorID, angles[motorID]);
        LCD_Print(s);
    }
    if (now - lastLCDTick >= 1) {
        LCD_TickQueue();
        lastLCDTick = now;
    }
    angles[motorID] += joystickY * joystickY * joystickY * dt / 60.0;
    angleSet(0, angles[0]);
    angleSet(1, angles[1]);
    angleSet(2, angles[2]);
    angleSet(3, angles[3]);

    gripperTickMotor(now);
    nemaTickMotors(&htim1);
    beeperSchedulerTick(dt);
}