#ifndef ROBOTICARM_V4_ANGLE_ABSTRACTION_H
#define ROBOTICARM_V4_ANGLE_ABSTRACTION_H


#define ANGLE_ABSTRACTION_MAX_SPEED 90
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

Nema17 m1, m2, m3, m4;


struct
{
    double angle, v;
} ANGLE_ABSTRACTION_PRE_CACHE[4];
double ANGLE_ABSTRACTION_CACHE[4] = {0, 0, 0, 0};

int skip = 0;

void angleAbstractionInit()
{
    m1 = nemaCreate(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6);
    m2 = nemaCreate(GPIOA, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
    m3 = nemaCreate(GPIOC, GPIO_PIN_7, GPIOA, GPIO_PIN_9);
    m4 = nemaCreate(GPIOB, GPIO_PIN_10, GPIOA, GPIO_PIN_8);
    for (int i = 0; i < 4; ++i) {
        ANGLE_ABSTRACTION_PRE_CACHE[i].angle = ANGLE_ABSTRACTION_CACHE[i];
    }
}

void angleSet(int section, double angle)
{
    if (2 <= section && section <= 3)
        angle -= 90;
    ANGLE_ABSTRACTION_PRE_CACHE[section].angle = angle;


}


void angleSysTick(double dt)
{
    if (skip++ < 5) return;
    for (int i = 0; i < 4; ++i) {
        const double k = 75;
        double f = k * (ANGLE_ABSTRACTION_PRE_CACHE[i].angle - ANGLE_ABSTRACTION_CACHE[i]);
        ANGLE_ABSTRACTION_PRE_CACHE[i].v += f * dt;
        ANGLE_ABSTRACTION_PRE_CACHE[i].v -= ANGLE_ABSTRACTION_PRE_CACHE[i].v * dt * 14;
        ANGLE_ABSTRACTION_PRE_CACHE[i].v = max(-ANGLE_ABSTRACTION_MAX_SPEED,
                                               min(ANGLE_ABSTRACTION_MAX_SPEED, ANGLE_ABSTRACTION_PRE_CACHE[i].v));
        ANGLE_ABSTRACTION_CACHE[i] += ANGLE_ABSTRACTION_PRE_CACHE[i].v * dt;
    }

    nemaSetTarget(m1, (int) (ANGLE_ABSTRACTION_CACHE[0] / 90 * -1600));
    nemaSetTarget(m2, (int) ((ANGLE_ABSTRACTION_CACHE[1] + ANGLE_ABSTRACTION_CACHE[2]) / 90 * -2400));
    nemaSetTarget(m3, (int) ((ANGLE_ABSTRACTION_CACHE[1]) / 90 * -2400));
    nemaSetTarget(m4,
                  (int) ((ANGLE_ABSTRACTION_CACHE[3] + ANGLE_ABSTRACTION_CACHE[1] + ANGLE_ABSTRACTION_CACHE[2]) / 90 *
                         2400));
}

#endif //ROBOTICARM_V4_ANGLE_ABSTRACTION_H
