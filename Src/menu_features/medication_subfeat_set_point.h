

#ifndef ROBOTICARM_V4_MEDICATION_SUBFEAT_SET_POINT_H
#define ROBOTICARM_V4_MEDICATION_SUBFEAT_SET_POINT_H

struct
{
    double x, y, z;
} RobotArmPos = {0, 10, 5};


double lastUpdateTime = 0;

struct
{
    int buttonCache;
    int gripperDown;
} med_set_point_vars;

void medication_subfeat_set_point_start(double now, double dt) {
    med_set_point_vars.buttonCache = joystickBtn;
    med_set_point_vars.gripperDown = 0;
    gripperOpen();
}

void medication_subfeat_set_point_fun(double now, double dt)
{

    if (joystickBtn != med_set_point_vars.buttonCache) {
        med_set_point_vars.buttonCache = joystickBtn;
        if (joystickBtn) {
            beeperBeep(900, 100, 100);
            med_set_point_vars.gripperDown = !med_set_point_vars.gripperDown;
        }
    }

    double distance = sqrt(RobotArmPos.x * RobotArmPos.x + RobotArmPos.y * RobotArmPos.y);

    if (distance > 28) {
        RobotArmPos.x = RobotArmPos.x / distance * 28;
        RobotArmPos.y = RobotArmPos.y / distance * 28;
    } else if (distance < 11.5) {
        RobotArmPos.x = RobotArmPos.x / distance * 11.5;
        RobotArmPos.y = RobotArmPos.y / distance * 11.5;
    }

    if (now - lastUpdateTime > 1.0 / 3) {
        lastUpdateTime = now;
        sprintf(LCD_CONTENT_BUFFER[1], "Dis=%.1f", distance);
    }

    double speed = 16;
    RobotArmPos.x += dt * speed * joystickX;
    RobotArmPos.y += dt * speed * joystickY;
    RobotArmPos.z = med_set_point_vars.gripperDown ? 0 : 6;

    setPosition(RobotArmPos.x, RobotArmPos.y, RobotArmPos.z);

}

#endif //ROBOTICARM_V4_MEDICATION_SUBFEAT_SET_POINT_H
