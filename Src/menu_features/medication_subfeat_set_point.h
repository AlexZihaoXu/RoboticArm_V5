

#ifndef ROBOTICARM_V4_MEDICATION_SUBFEAT_SET_POINT_H
#define ROBOTICARM_V4_MEDICATION_SUBFEAT_SET_POINT_H

struct
{
    double x, y, z;
} RobotArmPos = {0, 10, 5};

struct
{
    double originX;
    double originY;

    double destX;
    double destY;
} MedTasks[16];
int medTasksCount = 0;
int medTasksIndex = 0;

double lastUpdateTime = 0;

struct
{
    int startCounter;
    int buttonCache;
    int yAxisCache;
    int gripperDown;
    int originOrDestCache;
    double downCounter;
    int leaveConfirmed;
} med_set_point_vars;

void medication_subfeat_set_point_start(double now, double dt)
{
    med_set_point_vars.buttonCache = joystickBtn;
    med_set_point_vars.gripperDown = 0;
    med_set_point_vars.originOrDestCache = 0;
    med_set_point_vars.startCounter = 0;
    med_set_point_vars.downCounter = 0;
    med_set_point_vars.leaveConfirmed = 0;
    medTasksIndex = 0;
    medTasksCount = 0;
}

int medication_subfeat_set_point_fun(double now, double dt)
{

    if (joystickBtn != med_set_point_vars.buttonCache) {
        med_set_point_vars.buttonCache = joystickBtn;

        if (!joystickBtn) {
            if (med_set_point_vars.downCounter > 1.5) {
                angleSet(0, 0);
                angleSet(1, 0);
                angleSet(2, 90);
                angleSet(3, 90);
                medTasksCount = medTasksIndex + 1;

                medTasksIndex = 0;
                medTasksCount = 0;
                med_set_point_vars.leaveConfirmed = 0;
                return 1;
            }
            if (med_set_point_vars.startCounter < 1) {
                med_set_point_vars.startCounter++;
            } else {
                beeperBeep(900, 100, 100);
                med_set_point_vars.gripperDown = !med_set_point_vars.gripperDown;
                med_set_point_vars.yAxisCache = 0;
            }
        } else {
            med_set_point_vars.downCounter = 0;
        }
    }

    if (joystickBtn) {
        med_set_point_vars.downCounter += dt;
        if (med_set_point_vars.downCounter > 1.5 && !med_set_point_vars.leaveConfirmed) {
            med_set_point_vars.leaveConfirmed = 1;
            beeperBeep(1400, 150, 60);
            beeperBeep(1000, 150, 60);
        }
    }

    double distance = sqrt(RobotArmPos.x * RobotArmPos.x + RobotArmPos.y * RobotArmPos.y);

    if (distance > 28) {
        RobotArmPos.x = RobotArmPos.x / distance * 28;
        RobotArmPos.y = RobotArmPos.y / distance * 28;
    } else if (distance < 10.5) {
        RobotArmPos.x = RobotArmPos.x / distance * 10.5;
        RobotArmPos.y = RobotArmPos.y / distance * 10.5;
    }

    if (now - lastUpdateTime > 1.0 / 5) {
        lastUpdateTime = now;
        sprintf(LCD_CONTENT_BUFFER[1], "Set %s[%d]", !med_set_point_vars.originOrDestCache ? "Origin" : "Dest", medTasksIndex);
    }

    double speed = 16;
    if (!med_set_point_vars.gripperDown) {
        RobotArmPos.x += dt * speed * joystickX;
        RobotArmPos.y += dt * speed * joystickY;
    } else {
        if (med_set_point_vars.yAxisCache != (joystickY < -.7)) {
            med_set_point_vars.yAxisCache = joystickY < -.7;
            if (joystickY >= -.7) {
                if (med_set_point_vars.originOrDestCache) {
                    beeperBeep(1400, 150, 50);
                    beeperBeep(1400, 150, 50);
                    MedTasks[medTasksIndex].destX = RobotArmPos.x;
                    MedTasks[medTasksIndex].destY = RobotArmPos.y;
                    medTasksIndex++;
                } else {
                    beeperBeep(1100, 150, 50);
                    beeperBeep(1100, 150, 50);
                    MedTasks[medTasksIndex].originX = RobotArmPos.x;
                    MedTasks[medTasksIndex].originY = RobotArmPos.y;
                }
                med_set_point_vars.originOrDestCache = !med_set_point_vars.originOrDestCache;
                med_set_point_vars.gripperDown = 0;
            }
        }
    }
    RobotArmPos.z = med_set_point_vars.gripperDown ? 0 : 8;

    setPosition(RobotArmPos.x, RobotArmPos.y, RobotArmPos.z);

    return 0;
}

#endif //ROBOTICARM_V4_MEDICATION_SUBFEAT_SET_POINT_H
