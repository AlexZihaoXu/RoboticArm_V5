
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

    HAL_TIM_Base_Start(&htim1);
    angleAbstractionInit();
    initMenu();

    beeperBeepBlocking(850, 90);
    beeperBeepBlocking(960, 90);
    beeperBeepBlocking(1100, 90);

}


void onUpdate(long now, long dt)
{
////    v5menuTick(now / 1000.0, dt / 1000.0);
//    char content[16];
//    sprintf(content, "count: %.2f", highResolutionNow);
//    delay(100);
//    LCD_ClearBlocking();
//    LCD_PrintBlocking(content);
//

    v5menuTick(now, dt);
    gripperTickMotor(now);
    angleSysTick(dt / 1000.0);
    nemaTickMotors(&htim1);
    beeperSchedulerTick(dt);
}
