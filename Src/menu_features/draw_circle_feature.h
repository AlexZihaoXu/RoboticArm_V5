
#ifndef ROBOTICARM_V4_DRAW_CIRCLE_FEATURE_H
#define ROBOTICARM_V4_DRAW_CIRCLE_FEATURE_H

double drawCircleRadius = 6;
int locked = 0;
int drawCircleButtonCache = 0;

void drawCircleFeature(double now, double dt)
{

}

void drawCircleSettings(double now, double dt)
{
    if (drawCircleButtonCache != joystickBtn) {
        drawCircleButtonCache = joystickBtn;
        if (joystickBtn) {
            locked = !locked;
        }
    }
    if (locked) {
        drawCircleRadius += joystickY * dt;
        drawCircleRadius = min(8, max(4, drawCircleRadius));
    }
    if (locked) {
        sprintf(LCD_CONTENT_BUFFER[1], "Radius:[%.1f]", drawCircleRadius);
    } else {
        sprintf(LCD_CONTENT_BUFFER[1], "Radius:%.1f", drawCircleRadius);
    }
}

#endif //ROBOTICARM_V4_DRAW_CIRCLE_FEATURE_H
