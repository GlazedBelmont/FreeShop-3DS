#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include <time.h>
#include <stdio.h>

#include "textures.h"
#include "sprites.h"
#include "graphics.h"
#include "common.h"

C3D_RenderTarget *top, *bottom;

int alpha = 0;
u8 param[0x300];
u8 hmac[0x20];
bool fade = false;



void move_arrow(float size, int speed){
    int X = 200;
    int Y = 200;
    C2D_DrawTriangle(X+45, Y-10, WHITE, X+65, Y+10, WHITE, X+45, Y+30, WHITE, 0.5f);
    C2D_DrawRectangle(X, Y, 0.5f, 45, 20, WHITE, WHITE, WHITE, WHITE);
}

void draw_freeshop(){
    int X = 270;
    int Y = 186;
    C2D_DrawImageAt(freeshop, X, Y, 0.5f, NULL, 1, 1);

}

void draw_prompt() {
    int X = 8;
    int Y = 175;
    C2D_DrawText(&g_staticText[1], C2D_WithColor, X, Y, 1.0f, 0.5f, 0.5f, WHITE);

} 

void fadeout(u64 shopID) {
	C2D_Fade (C2D_Color32(0,0,0,255));
	memset(param, 0, sizeof(param));
	memset(hmac, 0, sizeof(hmac));
	APT_PrepareToDoApplicationJump(0, shopID, 0);
	APT_DoApplicationJump(param, sizeof(param), hmac);
}

void resetAlpha() {
    C2D_TargetClear(top, BLACK);
    C2D_TargetClear(bottom, C2D_Color32(0x68, 0xB0, 0xD8, 0xFF));
	C2D_Fade (C2D_Color32f(45,85,32,0));
	alpha = 0;
	fade = false;
}