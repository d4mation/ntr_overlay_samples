#include "global.h"
#include "ov.h"

extern u32 IoBasePad;

void debounceKey() 
{
	vu32 t;
	for (t = 0; t < 0x200000; t++) {
	}
}

void black(int x, int y, int xs, int ys)
{
 	OvDrawTranspartBlackRect(x, y, xs, ys, 3);
}

u32 getKeyDebounced(void) 
{	
	u32 key = *(vu32*)(IoBasePad) ^ 0xFFF & 0xFFF;
	if (key != 0)
	{
		debounceKey();
	}
	return (key);
}

s32 showMenu(u8* title, u32 entryCount, u8* captions[], int *selector) 
{
	u32 maxCaptions = 12;
	u32 i;
	u8 buf[200];
	u32 pos = 40;
	u32 x = 75, key = 0;
	u32 drawStart, drawEnd;
	int select = *selector;



	black(65, 30, 270, 190);
	OvDrawString(title, x, pos, 0, 255, 0);
	pos += 20;
	drawStart = (select / maxCaptions) * maxCaptions;
	drawEnd = drawStart + maxCaptions;

	if (drawEnd > entryCount) 
	{
		drawEnd = entryCount;
	}
	for (i = drawStart; i < drawEnd; i++) 
	{
		strcpy(buf, (i == select) ? "* " : "  ");
		strcat(buf, captions[i]);
		OvDrawString(buf, x, pos, 255, 255, 255);
		pos += 12;
	}

	key = getKeyDebounced();

	if (key == BUTTON_DD)
	{
		*selector += 1;
		if (*selector >= entryCount) 
		{
			*selector = 0;
		}
	}
	if (key == BUTTON_DU) 
	{
		*selector -= 1;
		if (*selector < 0) {
			*selector = entryCount - 1;
		}
	}
	if (key == BUTTON_A) 
	{
		return select;
	}
	if (key == BUTTON_B) 
	{
		return (-1);
	}

	return (-2);
}

u32 getKey() 
{
	return (*(vu32*)(IoBasePad) ^ 0xFFF) & 0xFFF;
}

void 	waitKeyRelease(void)
{
	while (getKey() != 0);
}

u32 waitKey() 
{
	u32 key;
	// wait key to be released
	while (getKey() != 0);
	while (1) {
		key = getKey();
		if (key != 0) {
			break;
		}
	}
	debounceKey();
	return key;
}

