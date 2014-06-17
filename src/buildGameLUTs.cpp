#include "common.h"
// Use 3:4 sprite set
//#include "../gfx/gfx.h"
// Use 1:2 sprite set
#include "../gfx/testMidSize/gfx.h"

unsigned short *image_entries[NB_IMAGES];
//void (*callback_entries[NB_CALLBACKS])(Enemy*, Player*);

void buildGameLUTs()
{
	image_entries[image_LUT_player_ship_light] = image_player_ship_light;
	image_entries[image_LUT_player_ship_shadow] = image_player_ship_shadow;
	image_entries[image_LUT_player_ship_polarityswitch_0_light] = image_player_ship_polarityswitch_0_light;
	image_entries[image_LUT_player_ship_polarityswitch_0_shadow] = image_player_ship_polarityswitch_0_shadow;
	image_entries[image_LUT_player_ship_polarityswitch_1_light] = image_player_ship_polarityswitch_1_light;
	image_entries[image_LUT_player_ship_polarityswitch_1_shadow] = image_player_ship_polarityswitch_1_shadow;
	image_entries[image_LUT_player_bullet_light] = image_player_bullet_light;
	image_entries[image_LUT_player_bullet_shadow] = image_player_bullet_shadow;
	image_entries[image_LUT_enemy_bullet_0_light] = image_enemy_bullet_0_light;
	image_entries[image_LUT_enemy_bullet_0_shadow] = image_enemy_bullet_0_shadow;
	image_entries[image_LUT_enemy_bullet_1_light] = image_enemy_bullet_1_light;
	image_entries[image_LUT_enemy_bullet_1_shadow] = image_enemy_bullet_1_shadow;
	image_entries[image_LUT_enemy_ship_0_light] = image_enemy_ship_0_light;
	image_entries[image_LUT_enemy_ship_0_shadow] = image_enemy_ship_0_shadow;
}
