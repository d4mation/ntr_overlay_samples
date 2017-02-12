#include "global.h"
#include "ov.h"

#define TICKS_PER_MSEC (268123.480)

static int frameCount[2];
static u64 lastUpdatedTick[2];
static int fps[2];

int DrawFPSCounter(u32 isBottom) 
{ 
    char buf[30];

    frameCount[isBottom]++;
    if (frameCount[isBottom] >= 64) 
    {
        frameCount[isBottom] = 0;
        u64 tickNow = svc_getSystemTick();
        u64 diff = tickNow - lastUpdatedTick[isBottom];
        lastUpdatedTick[isBottom] = tickNow;
        fps[isBottom] = 64.0 / ((double)(diff) / TICKS_PER_MSEC / 1000.0) * 10.0;
    }

    OvDrawTranspartBlackRect(14, 9, 64, 12, 1);

    
    xsprintf(buf, "FPS: %02d.%02d", fps[isBottom] / 10, fps[isBottom] % 10);
    OvDrawString(buf, 16, 11, 255, 255, 255);
    return (1);
}

int DrawFPSCounterBottom(u32 isBottom, int mode)// 0 = disabled, 1 = both screen, 2 = top screen only, 3 = bottom screen only 
{
    char buf[30];

    frameCount[isBottom]++;
    if (frameCount[isBottom] >= 64) 
    {
        frameCount[isBottom] = 0;
        u64 tickNow = svc_getSystemTick();
        u64 diff = tickNow - lastUpdatedTick[isBottom];
        lastUpdatedTick[isBottom] = tickNow;
        fps[isBottom] = 64.0 / ((double)(diff) / TICKS_PER_MSEC / 1000.0) * 10.0;
    }

    if (isBottom)
    {
        int i = 0;
        if (mode == 1 || mode  == 2)
        {
            OvDrawTranspartBlackRect(14, 9, 64, 12, 1);
            
            xsprintf(buf, "Top: %02d.%02d", fps[0] / 10, fps[0] % 10);
            OvDrawString(buf, 16, 11, 255, 255, 255);
            i = 12;          
        }
        if (mode == 1 || mode == 3)
        {
            OvDrawTranspartBlackRect(14, 9 + i, 64, 12, 1);
            
            xsprintf(buf, "Bot: %02d.%02d", fps[0] / 10, fps[0] % 10);
            OvDrawString(buf, 16, 11 + i, 255, 255, 255);            
        }      
    }

    return (1);
}