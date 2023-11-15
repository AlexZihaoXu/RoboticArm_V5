//
// Created by Alex on 2023-10-29.
//

#include "beeper.h"

struct BeeperTask
{
    int beepTone;
    int beepTick;
    int delayTick;
};

#define BEEPER_SCHEDULER_QUEUE_SIZE 1024
struct BeeperTask beeperSchedulerQueue[BEEPER_SCHEDULER_QUEUE_SIZE];
int beeperSchedulerReadIndex = 0;
int beeperSchedulerWriteIndex = 0;
int beeperStatus = 0;
int stopped = 0;
TIM_HandleTypeDef *htimRef;

void setBeeper(int value)
{
    if (beeperStatus != value) {
        beeperStatus = value;


        if (value != 0) {
            int x = 6000 - value;
            x = x < 0 ? 0 : x > 6000 ? 6000 : x;
            HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);
            __HAL_TIM_SET_AUTORELOAD(htimRef, x * 2);
            __HAL_TIM_SET_COMPARE(htimRef, TIM_CHANNEL_3, x);
            HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);
        }

        if (value == 0 && !stopped) {
            HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);
            HAL_TIM_Base_Stop(htimRef);
            stopped = 1;
            return;
        } else if (stopped) {
            stopped = 0;
            HAL_TIM_Base_Start(htimRef);
            HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);
        }


    }
}

void beeperInit(TIM_HandleTypeDef *htim)
{
    htimRef = htim;
    HAL_TIM_Base_Start(htim);
    beeperStatus = 0;
    stopped = 1;
}

void beeperBeepBlocking(int tune, int duration)
{
    HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);
    int x = 6000 - tune;
    x = x < 0 ? 0 : x > 6000 ? 6000 : x;
    __HAL_TIM_SET_AUTORELOAD(htimRef, x * 2);
    __HAL_TIM_SET_COMPARE(htimRef, TIM_CHANNEL_3, x);
    HAL_Delay(duration);
    HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);
}

void beeperSchedulerTick(int dt)
{
    if (beeperSchedulerWriteIndex != beeperSchedulerReadIndex) {
        struct BeeperTask *task = &beeperSchedulerQueue[beeperSchedulerReadIndex];
        if (task->delayTick > 0) {
            task->delayTick -= dt;
            setBeeper(0);
        } else if (task->beepTick > 0) {
            setBeeper(task->beepTone);
            task->beepTick -= dt;
        } else {
            beeperSchedulerReadIndex++;

        }
    } else {
        setBeeper(0);
    }
    if (beeperSchedulerReadIndex >= BEEPER_SCHEDULER_QUEUE_SIZE) {
        beeperSchedulerReadIndex = 0;
    }
    if (beeperSchedulerWriteIndex >= BEEPER_SCHEDULER_QUEUE_SIZE) {
        beeperSchedulerWriteIndex = 0;
    }
}

void beeperBeep(int tune, int duration, int delay)
{
    struct BeeperTask *task = &beeperSchedulerQueue[beeperSchedulerWriteIndex];
    task->beepTick = 5 < duration ? duration : 5;
    task->delayTick = 5 < delay ? delay : 5;
    task->beepTone = tune;
    beeperSchedulerWriteIndex++;
    if (beeperSchedulerWriteIndex >= BEEPER_SCHEDULER_QUEUE_SIZE) {
        beeperSchedulerWriteIndex = 0;
    }
}