#include "global.h"
#include "ov.h"

static u8 *menuEntries[11] =
{
    "Disable Clock", // 0
    "Display Clock only", // 1
    "Display Clock + Battery", // 2
    "24 Hour Clock", // 3
    "Enable/Disable Battery percentage", // 4
    "Change Clock widget's screen", // 5
    "Enable FPS Counter", // 6
    "Display FPS of both screens", // 7
    "Display FPS of Top Screen", // 8
    "Display FPS of Bottom Screen", // 9
    "Display FPS on Bottom screen" // 10
};

static u8   *menuSlowMode[6] = 
{
    "Disable",
    "Slow Mode Level 1",
    "Slow Mode Level 2",
    "Slow Mode Level 3",
    "Slow Mode Level 4",
    "Slow Mode Level 5"
};

extern u32         g_slowLevel;
extern u32         g_percentage;
extern u32         g_clockScreen;
extern u32         g_clockType; // 0 = 24 Hour, 1 = 12 Hour
extern u32         g_clockMode; // 0 = none, 1 = Clock Only, 2 = Clock + Battery
extern u32         g_fpsScreen;
extern u32         g_fpsCounter; // 0 = disabled, 1 = both screen, 2 = top screen only, 3 = bottom screen only

int     OverlayMenu(void)
{
    static int displayMenu = 0;
    static int selector = 0;
    static int mode = 0;

    u8 buf[200];
    int res = 0;
    
    u32     key = getKey();
    if (key == (BUTTON_L | BUTTON_X | BUTTON_Y))
    {
        waitKeyRelease();
        displayMenu = 1;
    }

    if (!displayMenu)
        return (0);

    int     posX = 95;
    int     posY = 199;

    if (displayMenu && key == BUTTON_R)
    {
        mode = !mode;
        selector = 0;
        for (int i = 0x200000; i > 0; i--);
    }

    // Normal mode
    if (mode == 0)
    {
        res = showMenu("Overlay Tools Menu", 11, menuEntries, &selector);
        if (res >= 0 && res <= 2)
            g_clockMode = res;
        else if (res == 3)
            g_clockType = !g_clockType;
        else if (res == 4)
            g_percentage = !g_percentage;
        else if (res == 5)
            g_clockScreen = !g_clockScreen;
        else if (res >= 6 && res <= 9)
            g_fpsCounter = res - 6;
        else if (res == 10)
            g_fpsScreen = !g_fpsScreen;

        OvDrawString("Press R to open Slow Mode Menu", posX, posY, 0, 0, 255);
    }
    // Slow down mode
    else
    {
        res = showMenu("Slow Mode Menu", 6, menuSlowMode, &selector);

        if (res == 0) g_slowLevel = 0;
        else if (res == 1) g_slowLevel = 30;
        else if (res == 2) g_slowLevel = 20;
        else if (res == 3) g_slowLevel = 15;
        else if (res == 4) g_slowLevel = 10;
        else if (res == 5) g_slowLevel = 5;

        OvDrawString("Press R to open Overlay Tools Menu", posX, posY, 0, 0, 255);
    }


    if (res == -1)
    {
        displayMenu = 0;
    }
    return (1);
}
