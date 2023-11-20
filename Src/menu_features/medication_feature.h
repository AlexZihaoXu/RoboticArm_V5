
#ifndef ROBOTICARM_V4_MEDICATION_FEATURE_H
#define ROBOTICARM_V4_MEDICATION_FEATURE_H

#include "medication_subfeat_set_point.h"


void medication_run_start(double now, double dt);

void medication_run_end(double now, double dt);

void medication_run_tick(double now, double dt);

int medication_start_cache = 0;
int medication_options_count = 0;
int medication_current_option_index = 0;

int medication_feature_run(double now, double dt)
{
    sprintf(LCD_CONTENT_BUFFER[0], "Med>Options[%d/%d]", medication_current_option_index, medication_options_count);

    if (!medication_start_cache) {
        medication_start_cache = 1;
        medication_run_start(now, dt);
    } else {
        medication_run_tick(now, dt);
    }
    return 0;
}

void medication_run_start(double now, double dt)
{

    medication_subfeat_set_point_start(now, dt);
    medication_current_option_index = 0;
}

void medication_run_tick(double now, double dt)
{
    medication_subfeat_set_point_fun(now, dt);
}

void medication_run_end(double now, double dt)
{
    medication_start_cache = 0;
}

#endif //ROBOTICARM_V4_MEDICATION_FEATURE_H
