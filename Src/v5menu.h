
#ifndef ROBOTICARM_V4_V5MENU_H
#define ROBOTICARM_V4_V5MENU_H

#include <string.h>

double v5menuTimer = 0;
long lastLCDTick = 0;
int welcomeOver = 0;

void v5menuWelcomeScreen();

void renderMenu(double now, double dt);

void v5menuTick(double now, double dt)
{
    if (dt > 1) return;

    if (v5menuTimer < 1) {
        v5menuWelcomeScreen();
    } else {
        if (!welcomeOver) {
            welcomeOver = 1;
            LCD_Clear();
        } else {
            renderMenu(now, dt);
        }
    }
    v5menuTimer += dt;


    long currentTick = HAL_GetTick();

    if (currentTick >= lastLCDTick + 1) {
        lastLCDTick = currentTick;
        LCD_TickQueue();
    }

}

int welcomePrinted = 0;

void v5menuWelcomeScreen()
{
    if (!welcomePrinted) {
        welcomePrinted = 1;
        LCD_Clear();
        LCD_SetCursor(0, 0);
        LCD_Print("==<[Welcome!]>==");
    }
}

char LCD_CONTENT[2][17];
char LCD_CONTENT_BUFFER[2][17];

double lastLCDRender = 0;

int currentPage = 0;
int totalPages = 4;

int menuOptionCount = 0;

struct
{
    const char *name;

    int (*runFunPtr)(double, double);

    int (*settingsFunPtr)(double, double);
} menuOptions[16];

void addMenuOption(const char *name, int (*runPtr)(double, double), int (*settingsPtr)(double, double))
{
    menuOptions[menuOptionCount].runFunPtr = runPtr;
    menuOptions[menuOptionCount].settingsFunPtr = settingsPtr;
    menuOptions[menuOptionCount].name = name;
    menuOptionCount++;
}

#define MENU_NOTHING (0x0)
#define MENU_BACK (0x1)

#include "menu_features/alarm_feature.h"
#include "menu_features/draw_circle_feature.h"
#include "menu_features/calibrate_feature.h"
#include "menu_features/music_feature_nyan_cat.h"
#include "menu_features/music_feature_rick_roll.h"

void initMenu()
{
    addMenuOption("Medication", 0, 0);
    addMenuOption("Free move", 0, 0);
    addMenuOption("Calibrate", calibrateFeatureRun, calibrateFeatureSettings);
    addMenuOption("Alarm", alarmFun, 0);
    addMenuOption("Music: NC", music_feature_nyan_cat_run, music_feature_nyan_cat_settings);
    addMenuOption("Music: RR", music_feature_rick_roll_run, music_feature_rick_roll_settings);
    addMenuOption("Circle", drawCircleFeature, drawCircleSettings);
    totalPages = menuOptionCount;
}

struct
{
    int lastVerticalPos;
    int lastHorizontalPos;
    int lastBtnPos;
} JoystickControlCache = {0};

void joystickXChange(int change);

void joystickYChange(int change);

void joystickBtnDown();

enum
{
    IN_MENU,
    IN_SETTINGS,
    IN_FEATURE
} currentStatus = IN_MENU;

void renderMenu(double now, double dt)
{

    if ((now - lastLCDRender > 1 / 5.0)) {
        int displayUpdated = 0;
        for (int i = 0; i < 16; ++i) {
            if (LCD_CONTENT[0][i] != LCD_CONTENT_BUFFER[0][i]) {
                displayUpdated = 1;
                break;
            }
            if (LCD_CONTENT[1][i] != LCD_CONTENT_BUFFER[1][i]) {
                displayUpdated = 1;
                break;
            }
        }
        if (displayUpdated) {
            lastLCDRender = now;
            strcpy(LCD_CONTENT[0], LCD_CONTENT_BUFFER[0]);
            strcpy(LCD_CONTENT[1], LCD_CONTENT_BUFFER[1]);

            LCD_Clear();
            LCD_SetCursor(0, 0);
            LCD_Print((char *) LCD_CONTENT[0]);
            LCD_SetCursor(0, 1);
            LCD_Print((char *) LCD_CONTENT[1]);
        }
    }


    if (currentStatus == IN_MENU) {
        sprintf(LCD_CONTENT_BUFFER[0], "[%d. %-11s]", currentPage + 1, menuOptions[currentPage].name);
        if (currentPage + 1 < menuOptionCount) {
            sprintf(LCD_CONTENT_BUFFER[1], " %d. %-11s ", currentPage + 2, menuOptions[currentPage + 1].name);
        } else {
            sprintf(LCD_CONTENT_BUFFER[1], "=====[END]=====");
        }
    } else if (currentStatus == IN_SETTINGS) {
        sprintf(LCD_CONTENT_BUFFER[0], "%s>Settings", menuOptions[currentPage].name);
        if (menuOptions[currentPage].settingsFunPtr) {
            int result = menuOptions[currentPage].settingsFunPtr(now, dt);
            if (result == MENU_BACK) {
                beeperBeep(1100, 50, 25);
                beeperBeep(900, 50, 25);
                beeperBeep(700, 50, 25);
                beeperBeep(500, 50, 20);
                currentStatus = IN_MENU;
            }
        } else {
            sprintf(LCD_CONTENT_BUFFER[1], "(not available)");

        }
    } else if (currentStatus == IN_FEATURE) {
        if (menuOptions[currentPage].runFunPtr(now, dt)) {
            beeperBeep(1100, 50, 25);
            beeperBeep(900, 50, 25);
            beeperBeep(700, 50, 25);
            beeperBeep(500, 50, 20);
            currentStatus = IN_MENU;
        }
    }

    if (joystickY < -.8) {
        if (JoystickControlCache.lastVerticalPos != 1) {
            joystickYChange(1);
            JoystickControlCache.lastVerticalPos = 1;
        }
    } else if (joystickY > .8) {
        if (JoystickControlCache.lastVerticalPos != -1) {
            joystickYChange(-1);

            JoystickControlCache.lastVerticalPos = -1;
        }
    } else {
        if (JoystickControlCache.lastVerticalPos != 0) {
            JoystickControlCache.lastVerticalPos = 0;
        }
    }
    if (joystickX < -.8) {
        if (JoystickControlCache.lastHorizontalPos != -1) {
            joystickXChange(-1);
            JoystickControlCache.lastHorizontalPos = -1;
        }
    } else if (joystickX > .8) {
        if (JoystickControlCache.lastHorizontalPos != 1) {
            joystickXChange(1);

            JoystickControlCache.lastHorizontalPos = 1;
        }
    } else {
        if (JoystickControlCache.lastHorizontalPos != 0) {
            JoystickControlCache.lastHorizontalPos = 0;
        }
    }
    if (joystickBtn != JoystickControlCache.lastBtnPos) {
        JoystickControlCache.lastBtnPos = joystickBtn;

        if (joystickBtn) {
            joystickBtnDown();
        }
    }
    currentPage = max(0, min(totalPages - 1, currentPage));
}

void joystickBtnDown()
{
    if (currentStatus == IN_MENU && menuOptions[currentPage].runFunPtr) {
        currentStatus = IN_FEATURE;
    }
}

void joystickYChange(int change)
{
    if (currentStatus == IN_MENU) {
        currentPage += change;
        if (0 <= currentPage && currentPage < totalPages) {
            beeperBeep(500, 100, 50);
        } else {
            beeperBeep(250, 80, 25);
            beeperBeep(250, 80, 25);
        }
    }
}

void joystickXChange(int change)
{
    if (change == 1) {
        if (currentStatus == IN_MENU) {
            currentStatus = IN_SETTINGS;
            beeperBeep(500, 50, 50);
            beeperBeep(700, 50, 50);
            beeperBeep(900, 50, 50);
            beeperBeep(1100, 50, 50);
        }
    } else if (change == -1 && currentStatus == IN_SETTINGS && menuOptions[currentPage].settingsFunPtr == 0) {
        beeperBeep(1100, 50, 50);
        beeperBeep(900, 50, 50);
        beeperBeep(700, 50, 50);
        beeperBeep(500, 50, 50);
        currentStatus = IN_MENU;
    }
}


#endif //ROBOTICARM_V4_V5MENU_H
