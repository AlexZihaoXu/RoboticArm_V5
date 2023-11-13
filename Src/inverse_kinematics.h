
#ifndef ROBOTICARM_V4_INVERSE_KINEMATICS_H
#define ROBOTICARM_V4_INVERSE_KINEMATICS_H

#define abs(x) (x > 0 ? x : -x)

void setPosition(double x, double y, double z)
{

    z += sqrt(x * x + y * y) / (22 * 22) * 8;

    double PI = 3.1415926;
    double distance = sqrt(x * x + y * y);
    double m = sqrt(distance * distance + z * z);
    double theta = acos(1 - m * m / 2 / 15 / 15) / PI * 180;

    double base, a, b, c;


    base = atan2(x, y) / PI * 180;
    a = theta / 2 - atan(z / distance) / PI * 180;
    b = 180 - theta;
    c = 180 - a - b;

    angleSet(1, a);
    angleSet(2, b);
    angleSet(3, c);
    angleSet(0, base);
}

#endif //ROBOTICARM_V4_INVERSE_KINEMATICS_H
