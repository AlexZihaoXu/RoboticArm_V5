
#ifndef ROBOTICARM_V4_FREE_MOTORS_FEATURE_H
#define ROBOTICARM_V4_FREE_MOTORS_FEATURE_H

int free_motors_feature(double now, double dt)
{
    if (!power_state) {

        sprintf(LCD_CONTENT_BUFFER[0], "Feature requires");
        sprintf(LCD_CONTENT_BUFFER[1], "Ext power supply");

        if (joystickX < -.7) {
            return 1;
        }
        return 0;
    }


    sprintf(LCD_CONTENT_BUFFER[0], "Motors are freed");
    sprintf(LCD_CONTENT_BUFFER[1], "<left> to exit.");
    force_power_off = 1;

    if (joystickX < -.7) {
        force_power_off = 0;
        return 1;
    }
    return 0;
}

#endif //ROBOTICARM_V4_FREE_MOTORS_FEATURE_H
