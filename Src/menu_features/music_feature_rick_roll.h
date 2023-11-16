#ifndef ROBOTICARM_V4_MUSIC_FEATURE_RICK_ROLL_H
#define ROBOTICARM_V4_MUSIC_FEATURE_RICK_ROLL_H

int rickrollData[]; // pitch, duration, pause

int music_feature_rr_notesIndex = 0;
double music_feature_rr_var_pauseCounter = 0;
int music_feature_rr_var_beeped = 0;

int music_feature_rr_var_btnCache = 0;
int music_feature_rr_var_locked = 0;
double music_feature_rr_var_speed = 1;

int music_feature_rick_roll_run(double now, double dt)
{

    sprintf(LCD_CONTENT_BUFFER[0], "Music: Rick Roll");
    sprintf(LCD_CONTENT_BUFFER[1], "Progress: %d%%", (int) (100 * (music_feature_rr_notesIndex / 1376.0)));
    music_feature_rr_var_pauseCounter += dt * 1150 * music_feature_rr_var_speed;
    if (music_feature_rr_notesIndex < 1019) {
        int i = music_feature_rr_notesIndex;
        int note = (int) (rickrollData[i] / music_feature_rr_var_speed);
        int duration = rickrollData[i + 1];
        int pause = rickrollData[i + 2];


        duration = (int) (duration / music_feature_rr_var_speed);
        pause = (int) (pause / music_feature_rr_var_speed);

        if (pause < 0) {
            duration += pause;
            pause = 0;
        }

        if (music_feature_rr_var_pauseCounter >= duration + pause) {
            music_feature_rr_var_beeped = 0;
            music_feature_rr_notesIndex += 3;
            music_feature_rr_var_pauseCounter = 0;

        } else {
            if (!music_feature_rr_var_beeped) {
                beeperBeep(note, duration, pause);
                music_feature_rr_var_beeped = 1;
            }
        }
    }

    if (joystickX < -.7) {
        music_feature_rr_notesIndex = 0;
        return 1;
    }

    return 0;
}


int music_feature_rick_roll_settings(double now, double dt)
{
    if (music_feature_rr_var_btnCache != joystickBtn) {
        music_feature_rr_var_btnCache = joystickBtn;
        if (joystickBtn) {
            music_feature_rr_var_locked = !music_feature_rr_var_locked;
            if (music_feature_rr_var_locked) {
                beeperBeep(600, 50, 50);
                beeperBeep(800, 50, 50);
            } else {
                beeperBeep(800, 50, 50);
                beeperBeep(600, 50, 50);
            }
        }
    }
    if (music_feature_rr_var_locked) {
        if (abs(joystickY) > .05) {
            music_feature_rr_var_speed += joystickY * dt * .15;
            music_feature_rr_var_speed = min(1.25, max(.75, music_feature_rr_var_speed));
        }
    } else if (joystickX < -0.7) {
        return MENU_BACK;
    }
    if (music_feature_rr_var_locked) {
        sprintf(LCD_CONTENT_BUFFER[1], "Speed:[%.1f%%]", music_feature_rr_var_speed * 100);
    } else {
        sprintf(LCD_CONTENT_BUFFER[1], "Speed:%.1f%%", music_feature_rr_var_speed * 100);
    }

    return 0;
}

int rickrollData[] = {
        587, 710, 3, 659, 710, 3, 440, 473, 3, 659, 710, 3, 740, 710, 3, 880, 117, 3, 784, 117, 3, 740, 236, 3, 587, 710, 3, 659, 710, 3, 440, 946, 3, 440, 117, 3, 440, 117, 3, 494, 117, 3, 587, 236, 3, 587, 117, 3, 587, 710, 3, 659, 710, 3, 440, 473, 3, 659, 710, 3, 740, 710, 3, 880, 117, 3, 784, 117, 3, 740, 236, 3, 587, 710, 3, 659, 710, 3, 440, 946, 3, 440, 117, 3, 440, 117, 3, 494, 117, 3, 587, 236, 3, 587, 117, 3, 0, 473, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 587, 236, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 946, 3, 0, 473, 3, 0, 236, 3, 494, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 473, 3, 440, 236, 3, 880, 236, 3, 0, 236, 3, 880, 236, 3, 659, 710, 3, 0, 473, 3, 494, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 587, 236, 3, 659, 236, 3, 0, 236, 3, 0, 236, 3, 554, 236, 3, 494, 236, 3, 440, 710, 3, 0, 473, 3, 0, 236, 3, 494, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 440, 473, 3, 659, 236, 3, 659, 236, 3, 659, 236, 3, 740, 236, 3, 659, 473, 3, 0, 473, 3, 587, 946, 3, 659, 236, 3, 740, 236, 3, 587, 236, 3, 659, 236, 3, 659, 236, 3, 659, 236, 3, 740, 236, 3, 659, 473, 3, 440, 473, 3, 0, 946, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 0, 236, 3, 659, 236, 3, 740, 236, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 740, 354, 3, 740, 354, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 659, 354, 3, 659, 354, 3, 587, 354, 3, 554, 117, 3, 494, 354, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 587, 473, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 236, 3, 440, 236, 3, 440, 236, 3, 659, 473, 3, 587, 946, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 740, 354, 3, 740, 354, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 880, 473, 3, 554, 236, 3, 587, 354, 3, 554, 117, 3, 494, 236, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 587, 473, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 473, 3, 440, 236, 3, 659, 473, 3, 587, 946, 3, 0, 473, 3, 0, 236, 3, 494, 236, 3, 587, 236, 3, 494, 236, 3, 587, 236, 3, 659, 473, 3, 0, 236, 3, 0, 236, 3, 554, 236, 3, 494, 236, 3, 440, 710, 3, 0, 473, 3, 0, 236, 3, 494, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 440, 473, 3, 0, 236, 3, 880, 236, 3, 880, 236, 3, 659, 236, 3, 740, 236, 3, 659, 236, 3, 587, 236, 3, 0, 236, 3, 440, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 0, 236, 3, 554, 236, 3, 494, 236, 3, 440, 710, 3, 0, 473, 3, 494, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 440, 473, 3, 0, 236, 3, 0, 236, 3, 659, 236, 3, 659, 236, 3, 740, 473, 3, 659, 710, 3, 587, 946, 3, 587, 236, 3, 659, 236, 3, 740, 236, 3, 659, 473, 3, 659, 236, 3, 659, 236, 3, 740, 236, 3, 659, 236, 3, 440, 236, 3, 440, 473, 3, 0, 710, 3, 440, 236, 3, 494, 236, 3, 554, 236, 3, 587, 236, 3, 494, 236, 3, 0, 236, 3, 659, 236, 3, 740, 236, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 740, 354, 3, 740, 354, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 659, 354, 3, 659, 354, 3, 587, 354, 3, 554, 117, 3, 494, 236, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 587, 473, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 473, 3, 440, 236, 3, 659, 473, 3, 587, 946, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 740, 354, 3, 740, 354, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 880, 473, 3, 554, 236, 3, 587, 354, 3, 554, 117, 3, 494, 236, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 587, 473, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 473, 3, 440, 236, 3, 659, 473, 3, 587, 946, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 740, 354, 3, 740, 354, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 880, 473, 3, 554, 236, 3, 587, 354, 3, 554, 117, 3, 494, 236, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 587, 473, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 473, 3, 440, 236, 3, 659, 473, 3, 587, 946, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 740, 354, 3, 740, 354, 3, 659, 710, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 880, 473, 3, 554, 236, 3, 587, 354, 3, 554, 117, 3, 494, 236, 3, 440, 117, 3, 494, 117, 3, 587, 117, 3, 494, 117, 3, 587, 473, 3, 659, 236, 3, 554, 354, 3, 494, 117, 3, 440, 473, 3, 440, 236, 3, 659, 473, 3, 587, 946, 3, 0, 473, 3
};


#endif //ROBOTICARM_V4_MUSIC_FEATURE_RICK_ROLL_H
