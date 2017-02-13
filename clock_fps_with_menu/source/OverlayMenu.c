#include "global.h"
#include "ov.h"

#define LIMEGREEN   50,205,50
#define ORANGE      255,140,0
#define RED         255,0,0
#define GREEN       0,255,0
#define BLANK       255,255,255 

static u8 *menuEntries[12] =
{
    "Disable Clock", 
    "Display Clock only", 
    "Display Clock + Battery", 
    "24 Hour Clock", 
    "Enable/Disable Battery percentage",
    "Change Clock widget's screen",
    "Move the Clock widget on the screen",
    "Disable FPS Counter",
    "Display FPS of both screens",
    "Display FPS of Top Screen",
    "Display FPS of Bottom Screen",
    "Display FPS on Bottom screen"
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

extern int         g_clockWidgetPosX;
extern int         g_clockWidgetPosY;
extern int         g_clockWidth;

extern u32         g_slowLevel;
extern u32         g_percentage;
extern u32         g_clockScreen;
extern u32         g_clockType; // 0 = 24 Hour, 1 = 12 Hour
extern u32         g_clockMode; // 0 = none, 1 = Clock Only, 2 = Clock + Battery
extern u32         g_fpsScreen;
extern u32         g_fpsCounter; // 0 = disabled, 1 = both screen, 2 = top screen only, 3 = bottom screen only

u32     getKeyDebounced(void);
void    black(int x, int y, int xs, int ys);

int     ClockMoveMenu(void)
{
    static int  isChanging = 0;
    static u32  widgetPosX = 0;
    static u32  widgetPosY = 0;

    u32 key = getKeyDebounced();
    u32 posX = 75;
    u32 posY = 40;

    if (isChanging == 0)
    {
        if (key & BUTTON_A)
            return (0);
        widgetPosX = g_clockWidgetPosX;
        widgetPosY = g_clockWidgetPosY;
        isChanging = 1;
    }

    // Draw "menu"

    // Background
    black(65, 30, 270, 180);
    // Title
    posY = OvDrawString("Widget location", posX, posY, GREEN);
    posY += 12;
    posX += 12;
    // Content
    posY = OvDrawString("Use DPAD to move the widget", posX, posY, BLANK);
    posY = OvDrawString("Use L to move faster", posX, posY, BLANK);
    posY = OvDrawString("Use A to apply the change", posX, posY, BLANK);
    posY = OvDrawString("Use B to cancel", posX, posY, BLANK);

    if (key == BUTTON_B)
    {
        isChanging = 0;
        g_clockWidgetPosX = widgetPosX;
        g_clockWidgetPosY = widgetPosY;
        return (-1);
    }

    if (key == BUTTON_A)
    {
        isChanging = 0;
        return (-1); 
    }

    if (key & BUTTON_L)
    {
        if (key & BUTTON_DL)
            g_clockWidgetPosX -= 10;
        if (key & BUTTON_DR)
            g_clockWidgetPosX += 10;
        if (key & BUTTON_DU)
            g_clockWidgetPosY -= 10;
        if (key & BUTTON_DD)
            g_clockWidgetPosY += 10;        
    }
    else
    {
        if (key & BUTTON_DL)
            g_clockWidgetPosX--;
        if (key & BUTTON_DR)
            g_clockWidgetPosX++;
        if (key & BUTTON_DU)
            g_clockWidgetPosY--;
        if (key & BUTTON_DD)
            g_clockWidgetPosY++; 
    }

    if (g_clockWidgetPosY - 2 < 0)
        g_clockWidgetPosY = 2;
    if (g_clockWidgetPosY > 230)
        g_clockWidgetPosY = 230;

    if (g_clockWidgetPosX < 0)
        g_clockWidgetPosX = 0;
    if (g_clockWidgetPosX + g_clockWidth > 400)
        g_clockWidgetPosX = 400 - g_clockWidth;
    return (0);
}

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
    int     posY = 205;

    if (displayMenu && key == BUTTON_R)
    {
        mode = !mode;
        selector = 0;
        for (int i = 0x200000; i > 0; i--);
    }

    // Normal mode
    if (mode == 0)
    {
        res = showMenu("Overlay Tools Menu", 12, menuEntries, &selector);
        if (res >= 0 && res <= 2)
            g_clockMode = res;
        else if (res == 3)
            g_clockType = !g_clockType;
        else if (res == 4)
            g_percentage = !g_percentage;
        else if (res == 5)
            g_clockScreen = !g_clockScreen;
        else if (res == 6)
            mode = 2;
        else if (res >= 7 && res <= 10)
            g_fpsCounter = res - 7;
        else if (res == 11)
            g_fpsScreen = !g_fpsScreen;

        OvDrawString("Press R to open Slow Mode Menu", posX, posY, 0, 0, 255);
    }
    else if (mode == 2)
    {
        res = ClockMoveMenu();
        if (res == -1)
            mode = 0;
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
