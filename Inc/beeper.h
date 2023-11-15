//
// Created by Alex on 2023-10-29.
//

#ifndef ROBOTICARM_V4_BEEPER_H
#define ROBOTICARM_V4_BEEPER_H

#include "stm32f4xx_hal.h"

void beeperInit(TIM_HandleTypeDef* htim);

void beeperSchedulerTick(int dt);

void beeperBeep(int freq, int duration, int delay);

void beeperBeepBlocking(int freq, int duration);

#endif //ROBOTICARM_V4_BEEPER_H
