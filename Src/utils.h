
#ifndef ROBOTICARM_V4_UTILS_H
#define ROBOTICARM_V4_UTILS_H

unsigned long long currentTickUS = 0;
#define highResolutionNow ((currentTickUS / 1.0e6))

void tickClock()
{

    unsigned long long currentCount = __HAL_TIM_GET_COUNTER(&htim2);
    if (currentCount > 0) {
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        currentTickUS += currentCount;
    }

}

void delay(double ms)
{
    double record = highResolutionNow;
    while (highResolutionNow - record < ms / 1000) {
        tickClock();
    }
}


#endif //ROBOTICARM_V4_UTILS_H
