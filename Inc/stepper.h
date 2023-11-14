//
// Created by Alex on 2023-10-29.
//

#ifndef ROBOTICARM_V4_STEPPER_H
#define ROBOTICARM_V4_STEPPER_H

#include "stm32f4xx_hal.h"
#include <math.h>



typedef uint8_t Nema17;

void stepGripper(int step);

void gripperClose();

void gripperOpen();

void gripperRelease();

void gripperHoming();

void gripperTickMotor(int now);

Nema17 nemaCreate(void *stepAlpha, uint16_t stepNum, void *dirAlpha, uint16_t dirNum);

void nemaHoming(TIM_HandleTypeDef *tim, Nema17 nema, int steps);

void nemaSmoothTurn(TIM_HandleTypeDef *tim, Nema17 n, int steps, int duration);

void nemaSetTarget(Nema17 nema, int target);

int nemaGetTarget(Nema17 nema);

void nemaTickMotors(TIM_HandleTypeDef* tim);


#endif //ROBOTICARM_V4_STEPPER_H
