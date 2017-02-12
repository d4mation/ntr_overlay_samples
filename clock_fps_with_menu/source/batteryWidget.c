#include "global.h"
#include "ov.h"


#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

#define TICKS_PER_SEC 0xFFB3D58
#define TICKS_PER_MIN 0x3BEE260A0UL
#define TICKS_IN_5_MIN 0x12BA6BE320UL

#define ADJ(posX, isBottom) (isBottom ? posX - 80 : posX)

u64     osGetTime(void);
Result  PTMU_GetBatteryLevel(u8 *out);
Result  MCU_GetBatteryLevel(u8* out);

#define LIMEGREEN   50,205,50
#define ORANGE      255,140,0
#define RED         255,0,0
#define BLANK       255,255,255

static char * g_speriod[] = { "AM", "PM"};

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
    
    u32 xPos = 338;
    u32 batteryX = xPos;
    u32 width = 57;
    
    // If it is a 12 hour clock, adjust things slightly
    if (clockType == 1)
    {
        xPos = 326;
        width = 69;
    }

    //DrawBackground
    OvDrawTranspartBlackRect(ADJ(xPos, isBottom), 9, width, 12, 1);
    xPos += 3;

    // Draw battery
    OvDrawRect(ADJ(batteryX + 36, isBottom), 11, 18, 1, BLANK);
    OvDrawRect(ADJ(batteryX + 36, isBottom), 18, 18, 1, BLANK);
    OvDrawRect(ADJ(batteryX + 36, isBottom), 11, 1, 8, BLANK);
    OvDrawRect(ADJ(batteryX + 53, isBottom), 11, 1, 8, BLANK);

    if (batval >= 11)
        OvDrawRect(ADJ(batteryX + 37, isBottom), 12, batval, 6, LIMEGREEN);
    else if (batval >= 5 && batval <= 8)
        OvDrawRect(ADJ(batteryX + 37, isBottom), 12, batval, 6, ORANGE);

    else 
        OvDrawRect(ADJ(batteryX + 37, isBottom), 12, batval, 6, RED);

    OvDrawRect(ADJ(batteryX + 54, isBottom), 13, 1, 4, BLANK);

    if (percent)
    {
        xsprintf(buf, "%d%%", batPercent);
        int percentageX = ADJ(batteryX + 43, isBottom);
        int len = strlen(buf) - 1;
        percentageX -= (len) * 2;
        OvDrawStringTiny(buf, percentageX, 12, BLANK);        
    }



    // Draw clock
    int period = GetTimeString(buf, clockType);
    OvDrawString(buf, ADJ(xPos, isBottom), 11, BLANK);
    xPos += 31;
    if (clockType == 1)
        OvDrawString(g_speriod[period], ADJ(xPos, isBottom), 11, BLANK);

    return (1);
}

int     DrawClockOnly(int isBottom, u32 clockType) 
{

    char buf[30] = {0};

    int period = GetTimeString(buf, clockType);
    
    u32 xPos = 352;
    u32 width = 33;
    
    // If it is a 12 hour clock, adjust things slightly
    if ( clockType == 1 )
    {
        xPos = 339;
        width = 46;
    }

    //DrawBackground
    OvDrawTranspartBlackRect(ADJ(xPos, isBottom), 9, width, 12, 1);
    // Draw clock
    OvDrawString(buf, ADJ(xPos + 2, isBottom), 11, 255, 255, 255);
    if (clockType == 1)
        OvDrawString(g_speriod[period], ADJ(xPos + 33, isBottom), 11, BLANK);
    return (1);
}
