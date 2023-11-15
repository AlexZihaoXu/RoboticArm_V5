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

void setBeeper(int freq)
{
    if (beeperStatus != freq) {
        beeperStatus = freq;


        if (freq != 0) {

            HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);

            __HAL_TIM_SET_AUTORELOAD(htimRef, 4000000 / freq - 1);
            __HAL_TIM_SET_COMPARE(htimRef, TIM_CHANNEL_3, 4000000 / freq / 2 - 1);

            HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);
        }

        if (freq == 0 && !stopped) {
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

void beeperBeepBlocking(int freq, int duration)
{
    HAL_TIM_Base_Start(htimRef);
    HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);

    __HAL_TIM_SET_AUTORELOAD(htimRef, 4000000 / freq - 1);
    __HAL_TIM_SET_COMPARE(htimRef, TIM_CHANNEL_3, 4000000 / freq / 2 - 1);

    HAL_Delay(duration);

    HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);
    HAL_TIM_Base_Stop(htimRef);
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

void beeperBeep(int freq, int duration, int delay)
{
    struct BeeperTask *task = &beeperSchedulerQueue[beeperSchedulerWriteIndex];
    task->beepTick = 5 < duration ? duration : 5;
    task->delayTick = 5 < delay ? delay : 5;
    task->beepTone = freq;
    beeperSchedulerWriteIndex++;
    if (beeperSchedulerWriteIndex >= BEEPER_SCHEDULER_QUEUE_SIZE) {
        beeperSchedulerWriteIndex = 0;
    }
}