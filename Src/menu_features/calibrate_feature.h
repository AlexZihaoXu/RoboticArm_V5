

#ifndef ROBOTICARM_V4_CALIBRATE_FEATURE_H
#define ROBOTICARM_V4_CALIBRATE_FEATURE_H


int calibrateFeatureRun(double now, double dt)
{

    if (!power_state) {

        sprintf(LCD_CONTENT_BUFFER[0], "Feature requires");
        sprintf(LCD_CONTENT_BUFFER[1], "Ext power supply");

        if (joystickX < -.7) {
            return 1;
        }
        return 0;
    }

    sprintf(LCD_CONTENT_BUFFER[0], "Please pose all");
    sprintf(LCD_CONTENT_BUFFER[1], "joints to 90 deg");
    force_power_off = 1;

    if (joystickX < -.7) {
        force_power_off = 0;
        for (int i = 0; i < 4; ++i) {
            ANGLE_ABSTRACTION_CACHE[i] = 0;
            ANGLE_ABSTRACTION_PRE_CACHE[i].angle = ANGLE_ABSTRACTION_CACHE[0];
            ANGLE_ABSTRACTION_PRE_CACHE[i].v = 0;
        }
        return 1;
    }

    return 0;

}


#endif //ROBOTICARM_V4_CALIBRATE_FEATURE_H
