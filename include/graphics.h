#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

extern C3D_RenderTarget *top, *bottom;

void move_arrow(float size, int speed);

void draw_freeshop();

void draw_prompt();

void fadeout(u64 shopID);

void resetAlpha();