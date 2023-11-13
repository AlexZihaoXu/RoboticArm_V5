//
// Created by Alex on 2023-10-29.
//

#include "stepper.h"


void setGripperStepper(const char *status)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, status[0] == '1');
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, status[1] == '1');
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, status[2] == '1');
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, status[3] == '1');

}

int gripperIndexCounter = 0;

void stepGripper(int dir)
{
    if (gripperIndexCounter == 0) {
        setGripperStepper("0011");
    } else if (gripperIndexCounter == 1) {
        setGripperStepper("0110");
    } else if (gripperIndexCounter == 2) {
        setGripperStepper("1100");
    } else {
        setGripperStepper("1001");
    }
    gripperIndexCounter += dir;
    if (gripperIndexCounter > 3) {
        gripperIndexCounter = 0;
    } else if (gripperIndexCounter < 0) {
        gripperIndexCounter = 3;
    }
}

int nemaCount = 0;
struct NemaCache
{
    void *stepAlpha;
    uint16_t stepNum;
    void *dirAlpha;
    uint16_t dirNum;
    int stepCount;
    int targetStep;
} nemasList[8];

Nema17 nemaCreate(void *stepAlpha, uint16_t stepNum, void *dirAlpha, uint16_t dirNum)
{
    struct NemaCache *nema = &nemasList[nemaCount];
    nema->stepAlpha = stepAlpha;
    nema->stepNum = stepNum;
    nema->dirAlpha = dirAlpha;
    nema->dirNum = dirNum;
    nema->stepCount = 0;
    nema->targetStep = 0;
    return nemaCount++;
}

void nemaSetTarget(Nema17 nema, int target)
{
    if (nema >= 0 && nema < nemaCount)
        nemasList[nema].targetStep = target;
}

int nemaGetTarget(Nema17 nema)
{
    return nemasList[nema].targetStep;
}

void nemaTickMotors(TIM_HandleTypeDef *tim)
{
    for (int i = 0; i < nemaCount; ++i) {
        struct NemaCache *nema = &nemasList[i];
        if (nema->targetStep > nema->stepCount) {
            HAL_GPIO_WritePin(nema->dirAlpha, nema->dirNum, 0);
            HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 0);
        } else if (nema->targetStep < nema->stepCount) {
            HAL_GPIO_WritePin(nema->dirAlpha, nema->dirNum, 1);
            HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 0);
        }

    }
    __HAL_TIM_SET_COUNTER(tim, 0);
    while (__HAL_TIM_GET_COUNTER(tim) < 125);

    for (int i = 0; i < nemaCount; ++i) {
        struct NemaCache *nema = &nemasList[i];
        if (nema->targetStep > nema->stepCount) {
            HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 1);
            nema->stepCount++;
        } else if (nema->targetStep < nema->stepCount) {
            HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 1);
            nema->stepCount--;
        }
    }

    __HAL_TIM_SET_COUNTER(tim, 0);
    while (__HAL_TIM_GET_COUNTER(tim) < 125);
}

void nemaHoming(TIM_HandleTypeDef *tim, Nema17 n, int steps)
{
    int dir = steps > 0 ? 0 : 1;
    steps = steps > 0 ? steps : -steps;
    struct NemaCache *nema = &nemasList[n];
    HAL_GPIO_WritePin(nema->dirAlpha, nema->dirNum, dir);

    for (int i = 0; i < steps; ++i) {
        HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 1);
        __HAL_TIM_SET_COUNTER(tim, 0);
        while (__HAL_TIM_GET_COUNTER(tim) < 200);
        HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 0);
        __HAL_TIM_SET_COUNTER(tim, 0);
        while (__HAL_TIM_GET_COUNTER(tim) < 200);
    }
}

void nemaSmoothTurn(TIM_HandleTypeDef *tim, Nema17 n, int steps, int duration)
{
    struct NemaCache *nema = &nemasList[n];
    HAL_GPIO_WritePin(nema->dirAlpha, nema->dirNum, steps > 0 ? 0 : 1);
    steps = steps > 0 ? steps : -steps;
    int stepCount = 0;
    long begin = (long) HAL_GetTick();
    long now = begin;
    while (now - begin < (int) duration) {
        now = (long) HAL_GetTick();
        int elapsed = now - begin;
        double x = -0.5 * cos(3.1415926535 * elapsed / (double) duration) + 0.5;
        int targetStep = (int) (x * steps);
        if (targetStep > stepCount) {
            stepCount++;

            HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 0);
            __HAL_TIM_SET_COUNTER(tim, 0);
            while (__HAL_TIM_GET_COUNTER(tim) < 200);

            HAL_GPIO_WritePin(nema->stepAlpha, nema->stepNum, 1);
            __HAL_TIM_SET_COUNTER(tim, 0);
            while (__HAL_TIM_GET_COUNTER(tim) < 200);
        }
    }

}

struct
{
    int target;
    int record;
} GripperCache;
int lastGripperTick = 0;

int gripperClosed = 1;

void gripperOpen()
{
    if (!gripperClosed) return;
    GripperCache.record = 0;
    GripperCache.target = -900;
    gripperClosed = 0;
}

void gripperClose()
{
    if (gripperClosed) return;
    GripperCache.record = 0;
    GripperCache.target = 1400;
    gripperClosed = 1;
}

void gripperRelease()
{
    GripperCache.record = 0;
    GripperCache.target = 100;
}

void gripperHoming()
{
    GripperCache.record = 0;
    GripperCache.target = 0;
    for (int i = 0; i < 200; ++i) {
        stepGripper(-1);
        HAL_Delay(1);
    }
    for (int i = 0; i < 1200; ++i) {
        stepGripper(1);
        HAL_Delay(1);
    }
}

void gripperTickMotor(int now)
{
    if (now - lastGripperTick >= 2) {
        lastGripperTick = now;
        if (GripperCache.target > GripperCache.record) {
            GripperCache.record++;
            stepGripper(1);
        } else if (GripperCache.target < GripperCache.record) {
            GripperCache.record--;
            stepGripper(-1);
        } else {
            setGripperStepper("0000");
        }
    }
}