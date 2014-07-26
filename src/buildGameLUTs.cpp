#include "common.h"
#include "../gfx/titleScreen.h"
#include "../gfx/bg.h"
#include "../gfx/gfx.h"

unsigned short *image_entries[NB_IMAGES];

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
	image_entries[image_LUT_enemy_ship_1_light] = image_enemy_ship_1_light;
	image_entries[image_LUT_enemy_ship_1_shadow] = image_enemy_ship_1_shadow;
	image_entries[image_LUT_enemy_ship_2_light] = image_enemy_ship_2_light;
	image_entries[image_LUT_enemy_ship_2_shadow] = image_enemy_ship_2_shadow;
	image_entries[image_LUT_enemy_ship_3_light] = image_enemy_ship_3_light;
	image_entries[image_LUT_chain_hit_light] = image_chain_hit_light;
	image_entries[image_LUT_chain_hit_shadow] = image_chain_hit_shadow;
	image_entries[image_LUT_background] = image_background;
	image_entries[image_LUT_titleScreen] = image_titleScreen;
}

void freeGameLUTs()
{
	
}