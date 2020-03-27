#include <citro2d.h>

#include "textures.h"
#include "sprites.h"


static C2D_SpriteSheet spritesheet;

void texload(){
	spritesheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");

	kurisu = C2D_SpriteSheetGetImage(spritesheet, sprites_kurisu_idx);
	topbrick = C2D_SpriteSheetGetImage(spritesheet, sprites_top_idx);
	bottombrick = C2D_SpriteSheetGetImage(spritesheet, sprites_bottom_idx);
	Y_button = C2D_SpriteSheetGetImage(spritesheet, sprites_Ybutton_idx);
	arrow = C2D_SpriteSheetGetImage(spritesheet, sprites_arrow_idx);
	freeshop = C2D_SpriteSheetGetImage(spritesheet, sprites_freeshop_idx);

}

void Textures_Free() {
	C2D_SpriteSheetFree(spritesheet);
}