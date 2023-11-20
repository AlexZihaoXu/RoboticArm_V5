
#ifndef ROBOTICARM_V4_DRAW_CIRCLE_FEATURE_H
#define ROBOTICARM_V4_DRAW_CIRCLE_FEATURE_H

double draw_circle_var_radius = 6;
int draw_circle_var_locked = 0;
int draw_circle_var_btnCache = 0;

int drawCircleFeature(double now, double dt)
{

    if (!power_state) {

        sprintf(LCD_CONTENT_BUFFER[0], "Feature requires");
        sprintf(LCD_CONTENT_BUFFER[1], "Ext power supply");

        if (joystickX < -.7) {
            return 1;
        }
        return 0;
    }

    double speed = 1.25;
    setPosition(sin(now * speed) * draw_circle_var_radius, 11 + draw_circle_var_radius + cos(now * speed) * draw_circle_var_radius, 5);

    sprintf(LCD_CONTENT_BUFFER[0], "Drawing Circle..");
    sprintf(LCD_CONTENT_BUFFER[1], "");

    if (joystickX < -.7) {
        angleSet(0, 0);
        angleSet(1, 0);
        angleSet(2, 90);
        angleSet(3, 90);
        return 1;
    }
    return 0;
}

int drawCircleSettings(double now, double dt)
{
    if (draw_circle_var_btnCache != joystickBtn) {
        draw_circle_var_btnCache = joystickBtn;
        if (joystickBtn) {
            draw_circle_var_locked = !draw_circle_var_locked;
            if (draw_circle_var_locked) {
                beeperBeep(600, 50, 50);
                beeperBeep(800, 50, 50);
            } else {
                beeperBeep(800, 50, 50);
                beeperBeep(600, 50, 50);
            }
        }
    }
    if (draw_circle_var_locked) {
        if (abs(joystickY) > .05) {
            draw_circle_var_radius += joystickY * dt * 1.25;
            draw_circle_var_radius = min(9, max(3, draw_circle_var_radius));
        }
    } else if (joystickX < -0.7) {
        return MENU_BACK;
    }
    if (draw_circle_var_locked) {
        sprintf(LCD_CONTENT_BUFFER[1], "Radius:[%.1f]", draw_circle_var_radius);
    } else {
        sprintf(LCD_CONTENT_BUFFER[1], "Radius:%.1f", draw_circle_var_radius);
    }

    return 0;
}

#endif //ROBOTICARM_V4_DRAW_CIRCLE_FEATURE_H
