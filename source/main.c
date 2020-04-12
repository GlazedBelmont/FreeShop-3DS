#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "textures.h"
#include "sprites.h"
#include "rtc.h"
#include "common.h"
#include "graphics.h"
#include "audio.h"

volatile u32 runSound, playSound;

s32 main_prio;
Thread snd_thr;
aptHookCookie thr_playhook;

OggVorbis_File vorbisFile;
vorbis_info *vi;
int i = 0;
int init_audio_1st = 0;


static const char rule11[] =
	"Please see rule 11 in #welcome-and-rules\n\n"
	"Don't...\n"
	"• ask how to pirate games\n"
	"• share full game data\n"
	"• mention piracy sites and tools by name\n"
	"• ask to share direct messages to help with piracy\n"
	"• discuss piracy in the assistance channels\n"
	"• in general commit copyright violations\n";

static const char prompt[] =
    "PRESS Y TO FRIIGAEM";



char *songs[4] = {"romfs:/pirate.ogg", "romfs:/objection.ogg", "romfs:/cornered.ogg", "romfs:/pbanj.ogg"}; // Add more songs if you want! :D


static void sceneInit(void)
{
	// Create two text buffers: one for static text, and another one for
	// dynamic text - the latter will be cleared at each frame.
	g_staticBuf  = C2D_TextBufNew(4096); // support up to 4096 glyphs in the buffer
	g_dynamicBuf = C2D_TextBufNew(4096);

	// Parse the static text strings
	C2D_TextParse(&g_staticText[0], g_staticBuf, rule11);
	C2D_TextParse(&g_staticText[1], g_staticBuf, prompt);


	// Optimize the static text strings
	C2D_TextOptimize(&g_staticText[0]);
	C2D_TextOptimize(&g_staticText[1]);

}

static void sceneExit(void)
{
	// Delete the text buffers
	C2D_TextBufDelete(g_dynamicBuf);
	C2D_TextBufDelete(g_staticBuf);
}

void threadPlayStopHook(APT_HookType hook, void *param)
{
	switch(hook) {
		case APTHOOK_ONSUSPEND:
		case APTHOOK_ONSLEEP:
			playSound = 0;
			break;

		case APTHOOK_ONRESTORE:
		case APTHOOK_ONWAKEUP:
			playSound = 1;
			break;

		case APTHOOK_ONEXIT:
			runSound = playSound = 0;
			break;

		default:
			break;
	}
}

void initAudio()
{	int max = ((sizeof(songs) / sizeof(char*))-1);

	if (i > max) i=0;
	if (i < 0) i=max;

	if ((initVorbis(songs[i], &vorbisFile, &vi)) != 0) {
		return;
	}
	initSound(vi->rate);
	if (init_audio_1st == 0) {
		svcGetThreadPriority(&main_prio, CUR_THREAD_HANDLE);
		if (main_prio < 2) svcBreak(USERBREAK_ASSERT);

		aptHook(&thr_playhook, threadPlayStopHook, NULL);

		APT_SetAppCpuTimeLimit(30);
		init_audio_1st=init_audio_1st+1;
	}

	runSound = playSound = 1;
	snd_thr = threadCreate(soundThread, &vorbisFile, 32768, main_prio, 1, true);
}

void stopAudio()
{
	runSound = playSound = 0;
	threadJoin(snd_thr, U64_MAX);
	threadDetach(snd_thr); // Fixes the crash that occurs when you switch too fast, probably because we kept adding threads over threads
	aptUnhook(&thr_playhook);
	fclose((FILE*)(vorbisFile.datasource)); /* hack */
}


int main(int argc, char **argv)
{
	int A = 0;
	int alpha = 0;
	int variation = 20;
	bool fade = false;


	gfxInitDefault();
	gfxSetDoubleBuffering(GFX_TOP, false);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	romfsInit();



	
	aptInit();

	initAudio();

	// Citro stuff
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);


    //u32 TopbackgroundColor = C2D_Color32f(0,0,0,alpha);
	//u32 BottombackgroundColor = C2D_Color32(0x68, 0xB0, 0xD8, alpha);


    texload();
	sceneInit();

	float size = 0.5f;

	u8 regionCode;
	u64 shopID;
	CFGU_SecureInfoGetRegion(&regionCode);
	switch (regionCode)
	{
	case 0:
		shopID = 0x0004001000020900;
		break;
	case 1:
		shopID = 0x0004001000021900;
		break;
	case 2:
		shopID = 0x0004001000022900;
		break;
	default:
		shopID = 0x00; 
	}


	

	bool debug = false;
	int set_seconds = 0;
	bool bricked = false;
	bool INIT = false;
	int init_seconds = 0;
	bool eShop = false;
	bool freeshop = false;

	while (aptMainLoop())
	{
		time_t unixTime = time(NULL);
		struct tm* timeStruct = gmtime((const time_t *)&unixTime);
/*		int minutes = timeStruct->tm_min;*/
		int seconds = timeStruct->tm_sec;

		if (INIT == false){
			init_seconds = seconds;
			if (init_seconds >= 40){
				init_seconds = (init_seconds - 60);
			}
			INIT = true;
		}
		
		hidScanInput();
		
		u32 kDown = hidKeysDown();
		//u32 kHeld = hidKeysHeld();

		if (bricked == false) {
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_SceneBegin(top);
			C2D_TargetClear(top, BLACK);
			C2D_DrawRectangle(0, 0, 0, 400, 240, BLACK, BLACK, BLACK, BLACK);
//			C2D_DrawRectangle(0, 0, 0, 400, 240, RED, GREEN, BLUE, YELLOW);
			C2D_DrawImageAt(kurisu, 70, 30, 0.0f, NULL, 1, 1);
			

			C2D_SceneBegin(bottom);
			C2D_TargetClear(bottom, BLACK);
			C2D_DrawRectangle(0, 0, 0, 320, 240, BLACK, BLACK, BLACK, BLACK);
			//C2D_DrawText(&g_staticText[0], C2D_WithColor, 8.0f, 8.0f, 0.5f, size, size, WHITE);


			if (debug == true){
				display_current_time(size, WHITE);
			}

			if (eShop == true){ // Shows the Y button and will be so you can open the eShop
				C2D_DrawImageAt(Y_button, 145, 178, 0.5f, NULL, 0.25, 0.25);
				move_arrow(size, 1);
				draw_freeshop();
				draw_prompt();
				if (kDown & KEY_Y){
					fade = true;
				}
				if (kDown & KEY_L){
					i=i-1;
					stopAudio();
					initAudio();
				}
				if (kDown & KEY_R){
					i=i+1;
					stopAudio();
					initAudio();
				}
			}
			if (fade == true){
				for(A = 0; A < 180000; A = A+5){
					int n = (int)(log(A) / log(5));
					if (pow(5, n) == A) {
						alpha += variation;
						if (alpha > 255) {
							stopAudio();
							fadeout(shopID);
							freeshop = true;
							break;
						}
						C2D_Fade (C2D_Color32(0,0,0,alpha));
					}
				}
			}

			if (kDown & KEY_SELECT){ // Debug mode for time
				if (debug == false) debug = true;

				else debug = false;
			}

	/*		if (kDown & KEY_X){ // debug!!!
				stopAudio();
				resetAlpha();
			}*/
			C3D_FrameEnd(0);
			if (freeshop == true) break;

		}
		else { // bricked == true
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, BLACK);
			C2D_SceneBegin(top);
			C2D_DrawImageAt(topbrick, 0, 0, 0.5f, NULL, 1, 1);

			C2D_TargetClear(bottom, BLACK);
			C2D_SceneBegin(bottom);
			C2D_DrawImageAt(bottombrick, 0, 0, 0.5f, NULL, 1, 1);
			C3D_FrameEnd(0);
			stopAudio();
			if (seconds == (set_seconds + 3)){
				aptExit(); // Won't close the app otherwise and if it's in the normal end part, eShop won't launch
				break;
			}
		}

		if (kDown & KEY_START){ // Switches to Bricked mode
			set_seconds = seconds;
			bricked = true;
		}
		
		if (seconds == (init_seconds + 20 )) eShop = true; // Makes it so the Y button appears after 20s
		
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
	Textures_Free();
	sceneExit();
	C2D_Fini();
	C3D_Fini();
	romfsExit();
	gfxExit();
	return 0;
}
