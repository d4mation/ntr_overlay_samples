#include "global.h"
#include "ov.h"


#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

#define TICKS_PER_SEC 0xFFB3D58
#define TICKS_PER_MIN 0x3BEE260A0UL
#define TICKS_IN_5_MIN 0x12BA6BE320UL

#define ADJ(posX, isBottom) (posX >= 320 - g_clockWidth && isBottom ? posX - 80 : posX)

u64     osGetTime(void);
Result  PTMU_GetBatteryLevel(u8 *out);
Result  MCU_GetBatteryLevel(u8* out);

#define LIMEGREEN   50,205,50
#define ORANGE      255,140,0
#define RED         255,0,0
#define BLANK       255,255,255

static char * g_speriod[] = { "AM", "PM"};

int     g_clockWidgetPosX = 338;
int     g_clockWidgetPosY = 11;
int     g_clockWidth = 0;

int    GetTimeString(char *output, u32 clockType)
{
    u64 timeInSeconds = osGetTime() / 1000;
    u64 dayTime = timeInSeconds % SECONDS_IN_DAY;
    
    u8 hour = dayTime / SECONDS_IN_HOUR;
    u8 min = (dayTime % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
    u8 seconds = dayTime % SECONDS_IN_MINUTE;

    int period = 0;
    
    if (clockType == 1)
    {        
        // Handle AM/PM
        if (hour == 0 || hour >= 12)
        {
            period = 1;
        }
    
        // 12 Hour Clock because 'merica
        if (hour==0)
        {
            hour = 12;
        }
        else if (hour > 12)
        {
            hour = hour - 12;
        }        
    }

    xsprintf(output, "%02d:%02d", hour, min);

    return (period);
    
}

int     DrawClockAndBattery(int isBottom, u32 percent, u32 clockType) 
{
    static u32      batval = 0;
    static u8       batPercent = 0;
    static u64      tick = 0;

    if (svc_getSystemTick() >= tick)
    {
        u8 batteryLevel = 0;
        PTMU_GetBatteryLevel(&batteryLevel);
        MCU_GetBatteryLevel(&batPercent);
        tick = svc_getSystemTick() + TICKS_IN_5_MIN;

        if (batteryLevel == 1) 
        {
            batval = 2;
        } 
        else if (batteryLevel == 2) 
        {
            batval = 5;
        } 
        else if (batteryLevel == 3) 
        {
            batval = 8;
        } 
        else if (batteryLevel == 4) 
        {
            batval = 11;
        } 
        else if (batteryLevel == 5) 
        {
            batval = 16;
        }
    }

    char buf[30] = {0}; 
    
    u32 xPos = g_clockWidgetPosX;
    u32 batteryX = xPos;
    u32 width = g_clockWidth = 57;
    u32 posY = g_clockWidgetPosY;
    
    // If it is a 12 hour clock, adjust things slightly
    if (clockType == 1)
    {
        xPos = g_clockWidgetPosX - 12;
        width = g_clockWidth = 69;
    }

    //DrawBackground
    OvDrawTranspartBlackRect(ADJ(xPos, isBottom), posY - 2, width, 12, 1);
    xPos += 3;

    // Draw battery
    OvDrawRect(ADJ(batteryX + 36, isBottom), posY, 18, 1, BLANK);
    OvDrawRect(ADJ(batteryX + 36, isBottom), posY + 7, 18, 1, BLANK);
    OvDrawRect(ADJ(batteryX + 36, isBottom), posY, 1, 8, BLANK);
    OvDrawRect(ADJ(batteryX + 53, isBottom), posY, 1, 8, BLANK);

    if (batval >= 11)
        OvDrawRect(ADJ(batteryX + 37, isBottom), posY + 1, batval, 6, LIMEGREEN);
    else if (batval >= 5 && batval <= 8)
        OvDrawRect(ADJ(batteryX + 37, isBottom), posY + 1, batval, 6, ORANGE);

    else 
        OvDrawRect(ADJ(batteryX + 37, isBottom), posY + 1, batval, 6, RED);

    OvDrawRect(ADJ(batteryX + 54, isBottom), posY + 2, 1, 4, BLANK);

    if (percent)
    {
        xsprintf(buf, "%d%%", batPercent);
        int percentageX = ADJ(batteryX + 43, isBottom);
        int len = strlen(buf) - 1;
        percentageX -= (len) * 2;
        OvDrawStringTiny(buf, percentageX, posY + 1, BLANK);        
    }



    // Draw clock
    int period = GetTimeString(buf, clockType);
    OvDrawString(buf, ADJ(xPos, isBottom), posY, BLANK);
    xPos += 31;
    if (clockType == 1)
        OvDrawString(g_speriod[period], ADJ(xPos, isBottom), posY, BLANK);

    return (1);
}

int     DrawClockOnly(int isBottom, u32 clockType) 
{

    char buf[30] = {0};

    int period = GetTimeString(buf, clockType);
    
    u32 xPos = g_clockWidgetPosX - 14;
    u32 width = g_clockWidth = 33;
    u32 posY = g_clockWidgetPosY;
    
    // If it is a 12 hour clock, adjust things slightly
    if ( clockType == 1 )
    {
        xPos = g_clockWidgetPosX + 1;
        width = g_clockWidth = 46;
    }

    //DrawBackground
    OvDrawTranspartBlackRect(ADJ(xPos, isBottom), posY - 2, width, 12, 1);
    // Draw clock
    OvDrawString(buf, ADJ(xPos + 2, isBottom), posY, 255, 255, 255);
    if (clockType == 1)
        OvDrawString(g_speriod[period], ADJ(xPos + 33, isBottom), posY, BLANK);
    return (1);
}
