
#include "stm32f4xx_hal_tim.h"
#include "lcd_async.h"
#include <stdio.h>
#include <math.h>

#include "angle_abstraction.h"
#include "inverse_kinematics.h"
#include "v5menu.h"

void onSetup()
{
    LCD_Init();

//    LCD_ClearBlocking();
//    LCD_SetCursorBlocking(0, 0);
//    LCD_PrintBlocking("PLZ POSE THE ARM");
//    HAL_Delay(1);
//    LCD_SetCursorBlocking(1, 0);
//    LCD_PrintBlocking("BEFORE POWER ON!");
//    HAL_Delay(1000);
//    LCD_ClearBlocking();
//    LCD_PrintBlocking("Initializing ...");
    HAL_TIM_Base_Start(&htim1);
//    gripperHoming();
    angleAbstractionInit();
    initMenu();
//
//    beeperBeep(80, 150);
//    beeperBeep(80, 150);
//    beeperBeep(80, 150);
    HAL_TIM_Base_Start(&htim3);
    for (int i = 0; i < 2; ++i) {
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        int x;
        for (x = 0; x < 6000; x = x + 5) {
            __HAL_TIM_SET_AUTORELOAD(&htim3, x * 2);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, x);
            HAL_Delay(1);
        }
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
        LCD_PrintBlocking("Done.");
        HAL_Delay(1000);
        LCD_PrintBlocking("Again.");
    }

    LCD_ClearBlocking();

}


void onUpdate(long now, long dt)
{
    v5menuTick(now / 1000.0, dt / 1000.0);

    gripperTickMotor(now);
    angleSysTick(dt / 1000.0);
    nemaTickMotors(&htim1);
    beeperSchedulerTick(dt);
}
