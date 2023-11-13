
#include "stm32f4xx_hal_tim.h"
#include "lcd_async.h"
#include <stdio.h>
#include <math.h>

#include "angle_abstraction.h"
#include "inverse_kinematics.h"

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
int actionID = 0;


void performAction(double now, double dt);


void onUpdate(long now, long dt)
{

    if (joystickBtn != buttonDown) {
        buttonDown = joystickBtn;
        if (buttonDown) {
            actionID++;
            beeperBeep(80, 0);
        }
    }

    if (now - lastPrintTick > 1000 / 4) {
        lastPrintTick = now;
        LCD_Clear();
        char s[16];
        sprintf(s, "ActionID = %d", actionID);
        LCD_Print(s);
    }
    if (now - lastLCDTick >= 1) {
        LCD_TickQueue();
        lastLCDTick = now;
    }

    gripperTickMotor(now);
    angleSysTick(dt / 1000.0);
    nemaTickMotors(&htim1);
    beeperSchedulerTick(dt);
    performAction(now / 1000.0, dt / 1000.0);
}

void performAction(double now, double dt)
{

    if (actionID == 0) {
        angleSet(0, 0);
        angleSet(1, 0);
        angleSet(2, 90);
        angleSet(3, 90);
    } else if (actionID == 1) {
        double t = now * 1.75;
        double r = 8;

        setPosition(cos(t) * r, 19 + sin(t) * r, 4.5);

    } else {
        actionID = 0;
    }
}