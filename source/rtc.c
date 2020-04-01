#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rtc.h"
#include "common.h"




void display_current_time(float size, u32 color)
{
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);

	int hours = timeStruct->tm_hour;
	int minutes = timeStruct->tm_min;
	int seconds = timeStruct->tm_sec;
	
	// Clear the dynamic text buffer
	C2D_TextBufClear(g_dynamicBuf);

	// Generate and draw dynamic text
	char buf[160];
	C2D_Text dynText;
	snprintf(buf, sizeof(buf), "%02i:%02i:%02i\n", hours, minutes, seconds);
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor, 8.0f, 220.0f, 0.5f, 0.5f, 0.5f, color);
}

