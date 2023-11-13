#ifndef ROBOTICARM_V4_ANGLE_ABSTRACTION_H
#define ROBOTICARM_V4_ANGLE_ABSTRACTION_H

Nema17 m1, m2, m3, m4;


void angleAbstractionInit()
{
    m1 = nemaCreate(GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6);
    m2 = nemaCreate(GPIOA, GPIO_PIN_7, GPIOB, GPIO_PIN_6);
    m3 = nemaCreate(GPIOC, GPIO_PIN_7, GPIOA, GPIO_PIN_9);
    m4 = nemaCreate(GPIOB, GPIO_PIN_10, GPIOA, GPIO_PIN_8);
}

void angleSet(Nema17 motor, double angle)
{
    if (motor == m1) {

        nemaSetTarget(m1, (int) (angle / 90 * -1600));
    } else if (motor == m2) {
        nemaSetTarget(m2, (int) (angle / 90 * -2400));
    } else if (motor == m3) {
        nemaSetTarget(m3, (int) (angle / 90 * -2400));
    }else if (motor == m4) {
        nemaSetTarget(m4, (int) (angle / 90 * 2400));
    }

}

#endif //ROBOTICARM_V4_ANGLE_ABSTRACTION_H
