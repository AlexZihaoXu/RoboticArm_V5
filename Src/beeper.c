//
// Created by Alex on 2023-10-29.
//

#include "beeper.h"

struct BeeperTask
{
    int beepTick;
    int delayTick;
};

#define BEEPER_SCHEDULER_QUEUE_SIZE 32
struct BeeperTask beeperSchedulerQueue[BEEPER_SCHEDULER_QUEUE_SIZE];
int beeperSchedulerReadIndex = 0;
int beeperSchedulerWriteIndex = 0;
int beeperStatus = 0;

void setBeeper(int state)
{
    if (beeperStatus != state) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, state);
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
        beeperStatus = state;
    }
}

void beeperInit()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
    beeperStatus = 0;
}

void beeperBeepBlocking(int duration) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
    HAL_Delay(duration);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
}

void beeperSchedulerTick(int dt)
{
    if (beeperSchedulerWriteIndex != beeperSchedulerReadIndex) {
        struct BeeperTask *task = &beeperSchedulerQueue[beeperSchedulerReadIndex];
        if (task->delayTick > 0) {
            task->delayTick -= dt;
            setBeeper(0);
        } else if (task->beepTick > 0) {
            setBeeper(1);
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

void beeperBeep(int duration, int delay)
{
    struct BeeperTask *task = &beeperSchedulerQueue[beeperSchedulerWriteIndex];
    task->beepTick = duration;
    task->delayTick = delay;
    beeperSchedulerWriteIndex++;
    if (beeperSchedulerWriteIndex >= BEEPER_SCHEDULER_QUEUE_SIZE) {
        beeperSchedulerWriteIndex = 0;
    }
}