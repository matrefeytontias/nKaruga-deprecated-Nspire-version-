#include "common.h"

const static int levelStream[] = {
	cmd_wait(256),
	//
	// Introduction 1
	//
	// Wave 1
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_1, LIGHT, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_1, LIGHT, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_1, LIGHT, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_1, LIGHT, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_1, LIGHT, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_1, LIGHT, 1, 4),
	cmd_wait(256),
	cmd_newWave,
	// Wave 2
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_2, SHADOW, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_2, SHADOW, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_2, SHADOW, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_2, SHADOW, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_2, SHADOW, 1, 4),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_2, SHADOW, 1, 4),
	cmd_wait(128),
	cmd_newWave,
	// Wave 3
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_3, LIGHT, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_3, LIGHT, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_3, LIGHT, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_3, LIGHT, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_3, LIGHT, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_3, LIGHT, 1, 4),
	cmd_wait(128),
	cmd_newWave,
	// Wave 4
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_4, SHADOW, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_4, SHADOW, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_4, SHADOW, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_4, SHADOW, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_4, SHADOW, 1, 4),
	cmd_wait(96),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_4, SHADOW, 1, 4),
	cmd_killed,
	cmd_newWave,
	// Wave 5
	enemy(85, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(115, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(145, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(175, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(205, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(235, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	cmd_wait(40),
	enemy(85, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(115, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(145, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(175, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(205, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(235, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	cmd_wait(40),
	enemy(85, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(115, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(145, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(175, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(205, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(235, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	cmd_wait(40),
	enemy(85, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(115, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(145, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(175, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(205, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(235, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	cmd_wait(40),
	enemy(85, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(115, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(145, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(175, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(205, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(235, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	cmd_wait(40),
	enemy(85, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(115, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(145, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_5, LIGHT, 0, 2),
	enemy(175, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(205, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	enemy(235, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_5, SHADOW, 0, 2),
	cmd_wait(40),
	cmd_killed,
	cmd_newWave,
	// Wave 6
	enemy(0, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_1, SHADOW, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_1, SHADOW, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_1, SHADOW, 0, 2),
	cmd_wait(256),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_2, LIGHT, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_2, LIGHT, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_2, LIGHT, 0, 2),
	cmd_killed,
	cmd_newWave,
	cmd_startChapter(0),
	//
	// Chapter 1
	//
	// Wave 1
	cmd_wait(256),
	enemy(53, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	enemy(106, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	enemy(79, -35, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	cmd_wait(256),
	enemy(214, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	enemy(267, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	enemy(240, -35, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	cmd_wait(256),
	enemy(214, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	enemy(267, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	enemy(240, -35, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	cmd_wait(256),
	enemy(53, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	enemy(106, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	enemy(79, -35, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	cmd_killed,
	cmd_newWave,
	enemy(53, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	enemy(106, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	enemy(79, -35, 10, image_LUT_enemy_ship_2_light, Pattern_1_7, LIGHT, 0, 8),
	cmd_wait(64),
	enemy(214, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	enemy(267, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	enemy(240, -35, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_7, SHADOW, 0, 8),
	cmd_killed,
	cmd_newWave,
	// Wave 2
	enemy(53, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_8, LIGHT, 1, 8),
	enemy(106, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_8, LIGHT, 1, 8),
	enemy(79, -35, 10, image_LUT_enemy_ship_2_light, Pattern_1_8, LIGHT, 1, 8),
	cmd_wait(256),
	enemy(214, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_8, SHADOW, 1, 8),
	enemy(267, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_8, SHADOW, 1, 8),	
	enemy(240, -35, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_8, SHADOW, 1, 8),
	cmd_wait(768),
	cmd_newWave,
	// Wave 3
	enemy(45, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_8, SHADOW, 1, 8),
	enemy(90, -35, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_8, SHADOW, 1, 8),
	enemy(135, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_8, LIGHT, 1, 8),
	enemy(185, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_8, SHADOW, 1, 8),
	enemy(230, -35, 10, image_LUT_enemy_ship_2_light, Pattern_1_8, LIGHT, 1, 8),	
	enemy(275, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_8, LIGHT, 1, 8),
	cmd_killed,
	cmd_newWave,
	// Wave 4
	enemy(320, 0, 6, image_LUT_enemy_ship_2_light, Pattern_1_9, LIGHT, 0, 8),
	cmd_wait(64),
	enemy(0, 0, 6, image_LUT_enemy_ship_2_light, Pattern_1_9, LIGHT, 0, 8),
	cmd_wait(64),
	enemy(320, 0, 6, image_LUT_enemy_ship_2_light, Pattern_1_9, LIGHT, 0, 8),
	cmd_wait(92),
	enemy(160, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_10, LIGHT, 0, 8),
	cmd_wait(128),
	enemy(160, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_10, SHADOW, 0, 8),
	cmd_wait(128),
	enemy(160, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_10, LIGHT, 0, 8),
	cmd_wait(128),
	enemy(160, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_10, SHADOW, 0, 8),
	cmd_wait(128),
	enemy(160, 0, 10, image_LUT_enemy_ship_2_light, Pattern_1_10, LIGHT, 0, 8),
	cmd_wait(128),
	enemy(160, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_10, SHADOW, 0, 8),
	cmd_wait(128),
	enemy(50, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_11, SHADOW, 0, 8),
	enemy(270, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_11, SHADOW, 0, 8),
	cmd_wait(64),
	enemy(50, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_11, SHADOW, 0, 8),
	enemy(270, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_11, SHADOW, 0, 8),
	cmd_wait(64),
	enemy(50, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_11, SHADOW, 0, 8),
	enemy(270, 0, 10, image_LUT_enemy_ship_2_shadow, Pattern_1_11, SHADOW, 0, 8),	
	cmd_killed,
	cmd_newWave,
	// Wave 5
	enemy(0, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_1, SHADOW, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_1, SHADOW, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_1, SHADOW, 0, 2),
	cmd_wait(256),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_2, LIGHT, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_2, LIGHT, 0, 2),
	cmd_wait(64),
	enemy(0, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_2, LIGHT, 0, 2),
	cmd_killed,
	cmd_wait(128),
	cmd_newWave,
	// Wave 6
	enemy(100, -80, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(140, -80, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(180, -80, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(220, -80, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(20, -40, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(60, -40, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(100, -50, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(140, -50, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(180, -50, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(220, -50, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(260, -40, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(300, -40, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(20, -10, 0, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(60, -10, 0, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(100, -20, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(140, -20, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(180, -20, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(220, -20, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(260, -10, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(300, -10, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(100, 10, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(140, 10, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	enemy(180, 10, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_12, SHADOW, 1, 5),
	enemy(220, 10, 1, image_LUT_enemy_ship_0_light, Pattern_1_12, LIGHT, 1, 5),
	cmd_killed,
	cmd_wait(128),
	cmd_newWave,
	// Wave 7
	enemy(106, 0, 64, image_LUT_enemy_ship_3_shadow, Pattern_1_13, SHADOW, 1, 30),
	enemy(212, 0, 64, image_LUT_enemy_ship_3_light, Pattern_1_14, LIGHT, 1, 30),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_15, SHADOW, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_15, SHADOW, 1, 5),
	cmd_joint(2, 0, 0, 0, 0),
	cmd_joint(3, 0, 0, 0, 0),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_15, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_15, LIGHT, 1, 5),
	cmd_joint(4, 1, 0, 0, 0),
	cmd_joint(5, 1, 0, 0, 0),
	cmd_killed,
	cmd_wait(128),
	cmd_newWave,
	// Wave 8
	enemy(120, 0, 6, image_LUT_enemy_ship_2_shadow, Pattern_1_16, SHADOW, 0, 8),
	enemy(160, 0, 6, image_LUT_enemy_ship_2_shadow, Pattern_1_16, SHADOW, 0, 8),
	enemy(200, 0, 6, image_LUT_enemy_ship_2_shadow, Pattern_1_16, SHADOW, 0, 8),
	enemy(140, -50, 6, image_LUT_enemy_ship_2_light, Pattern_1_16, LIGHT, 0, 8),
	enemy(180, -50, 6, image_LUT_enemy_ship_2_light, Pattern_1_16, LIGHT, 0, 8),
	cmd_killed,
	cmd_newWave,
	// Wave 9
	enemy(160, -50, 60, image_LUT_enemy_ship_3_light, Pattern_1_17, LIGHT, 1, 40),
	cmd_killed,
	cmd_newWave,
	// Wave 10
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_18, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_18, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_18, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_18, SHADOW, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_18, SHADOW, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_18, SHADOW, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_18, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_18, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_light, Pattern_1_18, LIGHT, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_18, SHADOW, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_18, SHADOW, 1, 5),
	enemy(0, 0, 1, image_LUT_enemy_ship_0_shadow, Pattern_1_18, SHADOW, 1, 5),
	cmd_killed,
	cmd_newWave,
	// Wave 11
	enemy(340, 55, 120, image_LUT_enemy_ship_4_light, Pattern_1_19, LIGHT, 0, 40),
	enemy(-20, 170, 120, image_LUT_enemy_ship_4_shadow, Pattern_1_19, SHADOW, 0, 40),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_lightball, Pattern_1_20, LIGHT, 0, 10),
	cmd_joint(2, 0, -22, -3, 1),
	cmd_joint(3, 0, 22, -3, 1),
	cmd_joint(4, 0, -22, 10, 1),
	cmd_joint(5, 0, 22, 10, 1),
	cmd_joint(6, 0, -22, 31, 1),
	cmd_joint(7, 0, 22, 31, 1),
	cmd_joint(8, 0, -22, 45, 1),
	cmd_joint(9, 0, 22, 45, 1),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	enemy(0, 0, 2, image_LUT_enemy_ship_4_shadowball, Pattern_1_20, SHADOW, 0, 10),
	cmd_joint(10, 1, -22, -3, 1),
	cmd_joint(11, 1, 22, -3, 1),
	cmd_joint(12, 1, -22, 10, 1),
	cmd_joint(13, 1, 22, 10, 1),
	cmd_joint(14, 1, -22, 31, 1),
	cmd_joint(15, 1, 22, 31, 1),
	cmd_joint(16, 1, -22, 45, 1),
	cmd_joint(17, 1, 22, 45, 1),
	cmd_wait(2048),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_shadow, Pattern_1_21, SHADOW, 0, 2),
	cmd_wait(192),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	enemy(160, 0, 1, image_LUT_enemy_ship_1_light, Pattern_1_21, LIGHT, 0, 2),
	cmd_killed,
	cmd_newWave,
	cmd_wait(256),
	// Boss*/
	cmd_fightBoss(0),
	LVLSTR_CHAPTEREND,
	LVLSTR_END
	};
