
#ifndef ROBOTICARM_V4_MEDICATION_FEATURE_H
#define ROBOTICARM_V4_MEDICATION_FEATURE_H

#include "medication_subfeat_set_point.h"


int medication_run_start(double now, double dt);

int medication_run_end(double now, double dt);

int medication_run_tick(double now, double dt);

int medication_start_cache = 0;
int medication_options_count = 2;
int medication_current_option_run = 0;
int med_running = 0;
int med_joystick_cache = 0;
int med_joystick_btn_cache = 0;

int medication_feature_run(double now, double dt)
{
    sprintf(LCD_CONTENT_BUFFER[0], "Med>Options[%d/%d]", 2 - medication_current_option_run, medication_options_count);

    if (med_running) {
        if (medication_run_tick(now, dt)) {
            medication_run_end(now, dt);
            med_running = 0;
        }
    } else {
        if (!medication_start_cache) {
            medication_start_cache = 1;
            medication_run_start(now, dt);
        } else {

        }
        if (joystickX < -.7) {
            med_running = 0;
            medication_start_cache = 0;
            gripperClose();

            return 1;
        }
        if (joystickBtn != med_joystick_btn_cache) {
            med_joystick_btn_cache = joystickBtn;
            if (joystickBtn) {
                if (medication_current_option_run) {

                } else {
                    medication_subfeat_set_point_start(now, dt);
                }
                med_running = 1;
            }
        }

        if (med_joystick_cache != (joystickY > 0.7 ? 1 : joystickY < -.7 ? -1 : 0)) {
            med_joystick_cache = (joystickY > 0.7 ? 1 : joystickY < -.7 ? -1 : 0);
            if (med_joystick_cache) {
                beeperBeep(800, 100, 50);
                beeperBeep(1200, 100, 50);
                medication_current_option_run = !medication_current_option_run;
            }
        }

        sprintf(LCD_CONTENT_BUFFER[1], "%s", medication_current_option_run ? "[Run]|Set Points" : "Run|[Set Points]");
    }
    return 0;
}

int medication_run_start(double now, double dt)
{
    gripperOpen();

    beeperBeep(1200, 80, 60);
    beeperBeep(1400, 80, 60);
    beeperBeep(1600, 80, 60);
    med_joystick_btn_cache = 1;


    medication_current_option_run = 1;

    return 0;
}

int medication_run_tick(double now, double dt)
{
    return medication_subfeat_set_point_fun(now, dt);
}

int medication_run_end(double now, double dt)
{
    medication_current_option_run = 0;
    medication_start_cache = 0;

    return 0;
}

#endif //ROBOTICARM_V4_MEDICATION_FEATURE_H
