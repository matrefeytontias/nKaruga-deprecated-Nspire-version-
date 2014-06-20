#include "common.h"
#include "../gfx/c_gfx.h"
//~ #include "../gfx/d_gfx.h"

unsigned short *c_image_entries[NB_IMAGES];
unsigned short *d_image_entries[NB_IMAGES];
unsigned short **image_entries;

void buildGameLUTs()
{
	// Cel-shading spriteset
	c_image_entries[image_LUT_player_ship_light] = c_image_player_ship_light;
	c_image_entries[image_LUT_player_ship_shadow] = c_image_player_ship_shadow;
	c_image_entries[image_LUT_player_ship_polarityswitch_0_light] = c_image_player_ship_polarityswitch_0_light;
	c_image_entries[image_LUT_player_ship_polarityswitch_0_shadow] = c_image_player_ship_polarityswitch_0_shadow;
	c_image_entries[image_LUT_player_ship_polarityswitch_1_light] = c_image_player_ship_polarityswitch_1_light;
	c_image_entries[image_LUT_player_ship_polarityswitch_1_shadow] = c_image_player_ship_polarityswitch_1_shadow;
	c_image_entries[image_LUT_player_bullet_light] = c_image_player_bullet_light;
	c_image_entries[image_LUT_player_bullet_shadow] = c_image_player_bullet_shadow;
	c_image_entries[image_LUT_enemy_bullet_0_light] = c_image_enemy_bullet_0_light;
	c_image_entries[image_LUT_enemy_bullet_0_shadow] = c_image_enemy_bullet_0_shadow;
	c_image_entries[image_LUT_enemy_bullet_1_light] = c_image_enemy_bullet_1_light;
	c_image_entries[image_LUT_enemy_bullet_1_shadow] = c_image_enemy_bullet_1_shadow;
	c_image_entries[image_LUT_enemy_ship_0_light] = c_image_enemy_ship_0_light;
	c_image_entries[image_LUT_enemy_ship_0_shadow] = c_image_enemy_ship_0_shadow;
	c_image_entries[image_LUT_enemy_ship_1_light] = c_image_enemy_ship_1_light;
	c_image_entries[image_LUT_enemy_ship_1_shadow] = c_image_enemy_ship_1_shadow;
	c_image_entries[image_LUT_enemy_ship_2_light] = c_image_enemy_ship_2_light;
	
	// Depth spriteset
	
	// Not ready yet !
	//~ d_image_entries[image_LUT_player_ship_light] = d_image_player_ship_light;
	//~ d_image_entries[image_LUT_player_ship_shadow] = d_image_player_ship_shadow;
	//~ d_image_entries[image_LUT_player_ship_polarityswitch_0_light] = d_image_player_ship_polarityswitch_0_light;
	//~ d_image_entries[image_LUT_player_ship_polarityswitch_0_shadow] = d_image_player_ship_polarityswitch_0_shadow;
	//~ d_image_entries[image_LUT_player_ship_polarityswitch_1_light] = d_image_player_ship_polarityswitch_1_light;
	//~ d_image_entries[image_LUT_player_ship_polarityswitch_1_shadow] = d_image_player_ship_polarityswitch_1_shadow;
	//~ d_image_entries[image_LUT_player_bullet_light] = d_image_player_bullet_light;
	//~ d_image_entries[image_LUT_player_bullet_shadow] = d_image_player_bullet_shadow;
	//~ d_image_entries[image_LUT_enemy_bullet_0_light] = d_image_enemy_bullet_0_light;
	//~ d_image_entries[image_LUT_enemy_bullet_0_shadow] = d_image_enemy_bullet_0_shadow;
	//~ d_image_entries[image_LUT_enemy_bullet_1_light] = d_image_enemy_bullet_1_light;
	//~ d_image_entries[image_LUT_enemy_bullet_1_shadow] = d_image_enemy_bullet_1_shadow;
	//~ d_image_entries[image_LUT_enemy_ship_0_light] = d_image_enemy_ship_0_light;
	//~ d_image_entries[image_LUT_enemy_ship_0_shadow] = d_image_enemy_ship_0_shadow;
	//~ d_image_entries[image_LUT_enemy_ship_1_light] = d_image_enemy_ship_1_light;
	//~ d_image_entries[image_LUT_enemy_ship_1_shadow] = d_image_enemy_ship_1_shadow;
	//~ d_image_entries[image_LUT_enemy_ship_2_light] = d_image_enemy_ship_2_light;
	
	// Use cel-shading spriteset
	image_entries = c_image_entries;
}
