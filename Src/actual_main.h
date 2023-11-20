
#include "stm32f4xx_hal_tim.h"
#include "lcd_async.h"
#include <stdio.h>
#include <math.h>

#include "ext_power_supply_process.h"
#include "angle_abstraction.h"
#include "inverse_kinematics.h"
#include "v5menu.h"


void onSetup()
{
    LCD_Init();

    gripperHoming();

    HAL_TIM_Base_Start(&htim1);
    angleAbstractionInit();
    initMenu();

    beeperBeepBlocking(850, 90);
    beeperBeepBlocking(960, 90);
    beeperBeepBlocking(1100, 90);

}


void onUpdate(long now, long dt)
{

    tick_ext_power_supply(now / 1000.0, dt / 1000.0);

    v5menuTick(now / 1000.0, dt / 1000.0);
    gripperTickMotor(now);
    angleSysTick(dt / 1000.0);
    nemaTickMotors(&htim1);
    beeperSchedulerTick(dt);
}
