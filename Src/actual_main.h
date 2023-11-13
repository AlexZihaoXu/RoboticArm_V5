
#include "stm32f4xx_hal_tim.h"
#include "lcd_async.h"
#include <stdio.h>
#include <math.h>

Nema17 m1, m2, m3, m4;

void onSetup()
{
    LCD_Init();
    HAL_TIM_Base_Start(&htim1);
    gripperHoming();

    beeperBeep(80, 150);
    beeperBeep(80, 150);
    beeperBeep(80, 150);

    m1 = nemaCreate(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6);
    m2 = nemaCreate(GPIOA, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
    m3 = nemaCreate(GPIOC, GPIO_PIN_7, GPIOA, GPIO_PIN_9);
    m4 = nemaCreate(GPIOB, GPIO_PIN_10, GPIOA, GPIO_PIN_8);

}

long lastPrintTick = 0;
long lastLCDTick = 0;


int buttonDown = 0;
int actionID = 0;

void tick(long now, long dt);

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

    tick(now, dt);

    gripperTickMotor(now);
    nemaTickMotors(&htim1);
    beeperSchedulerTick(dt);
}

double t = 0.5;

void tick(long now, long dt)
{
    int range = 1200;
    if (buttonDown) {
        t += dt / 2000.0;
    } else {
        t += (.5 - t) * dt / 600.0;
    }
    nemaSetTarget(0, -(int) (range / 2.0 + range / 2.0 * cos(t * 3.1415926535 * 2)));

    while (t > 1) {
        t--;
    }
}
