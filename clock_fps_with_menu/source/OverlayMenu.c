#include "global.h"

static u8 *menuEntries[10] =
{
    "Disable Clock", // 0
    "Display Clock only", // 1
    "Display Clock + Battery", // 2
    "24 Hour Clock", // 3
    "Enable/Disable Battery percentage", // 4
    "Change Clock widget's screen", // 5
    "Disable FPS Counter", // 6
    "Display FPS of both screens", // 7
    "Display FPS of Top Screen", // 8
    "Display FPS of Bottom Screen", // 9
    "Display FPS on Bottom screen" // 10
};

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

    u8 buf[200];
    int res = 0;
    
    if (getKey() == (BUTTON_L | BUTTON_X | BUTTON_Y))
    {
        waitKeyRelease();
        displayMenu = 1;
    }

    if (!displayMenu)
        return (0);

    res = showMenu("Overlay tools menu", 10, menuEntries, &selector);
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

    if (res == -1)
    {
        displayMenu = 0;
        selector = 0;
    }
    return (1);
}
