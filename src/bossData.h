#include "common.h"
#include "ExplosionEffect.h"

#define HP_PER_BAR 250
#define BOSS_NB 1
#define MAX_PATTERNS_PER_BOSS 3

// Colors for bosses' HP bars
// from low to high HP
// red (no HP), orange, yellow, green-yellow, dark green, green, cyan, navy, blue, purple, magenta, olive, dark gray, white
unsigned short color_HPbars[] = { 0xf800, 0xfd20, 0xffe0, 0xafe5, 0x03e0, 0x07e0, 0x07ff, 0x000f, 0x001f, 0x780f, 0xf81f, 0x7be0, 0x7bef, 0xffff };

enum
{
	joint_leftarm_armed,
	joint_rightarm_armed1,
	joint_leftarm_nonarmed,
	joint_rightarm_armed2,
	joint_rightarm_nonarmed,
	NB_JOINT_BOSS1
};

/* ####################
 * 
 * DATA
 *
 * ####################
 */

// source(x, y) , destination(x, y)
int boss1_jointData[NB_JOINT_BOSS1][4] = {
	{ 11, 4, 38, 22 },
	{ 11, 8, 74, 22 },
	{ 22, 8, 38, 23 },
	{ 24, 22, 73, 23 },
	{ 2, 8, 72, 22 }
};

// General data

int bossPatternsNb[BOSS_NB] = { 3 };

int bossHPperPat[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ 200, HP_PER_BAR, HP_PER_BAR * 3 }
};

int bossTimeoutPerPat[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ 150, 110, 70 }
};

/* ####################
 * 
 * UTILS
 *
 * ####################
 */

int getHPsum(int *a, int start, int end)
{
	int total = 0;
	for(int i = start; i <= end; i++)
		total += a[i];
	return total;
}

int getPatternID(BossEnemy *be)
{
	int i;
	int HPdone = -1, timeDone = -1;
	for(i = 0; i < be->patternsNb; i++)
	{
		if(HPdone < 0)
		{
			if(be->HP > getHPsum(be->HPperPattern, i, be->patternsNb - 1))
				HPdone = i - 1;
		}
		if(timeDone < 0)
		{
			if(be->getTimeout() > be->timeoutPerPattern[i])
				timeDone = i - 1;
		}
		if(!(HPdone < 0 || timeDone < 0))
			break;
	}
	return !(HPdone < 0 || timeDone < 0) ? max(HPdone, timeDone) : i - 1;
}

// Works with every angle as the jointing point is not supposed to rotate at all
// x, y = position of jointing point in-screen
// w, h = position of jointing point in the jointed sprite
Rect getJointPoint(BossEnemy *be, int data[][4], int offset)
{
	Rect result;
	result.x = fixtoi(be->getx()) - bossImage_entries[bossImage_LUT_1_body][0] / 2 + data[offset][2];
	result.y = fixtoi(be->gety()) - bossImage_entries[bossImage_LUT_1_body][1] / 2 + data[offset][3];
	result.w = data[offset][0];
	result.h = data[offset][1];
	
	return result;
}

/* ####################
 * 
 * BOSS PROGRAMMING
 *
 * ####################
 */

// [x]: usage
// refers to a register number and the usage that's made of it inside the block where the comment is put

/*
 * ######
 * BOSS 1
 * ######
 */

/*
 * ########################
 * Initialization callbacks
 * ########################
 */
 
void boss1_icb1(BossEnemy *be)
{
	const int h = bossImage_entries[bossImage_LUT_1_body][1];
	if(!be->initCallbackCalled)
	{
		be->setx(itofix(160));
		be->sety(-itofix(h) / 2);
		be->maxHP = be->HP = getHPsum(be->HPperPattern, 0, be->patternsNb - 1);
		be->setInternal(1, -32);
		be->angle = 0;
		be->initCallbackCalled = true;
		be->setInternal(31, 0);
	}
	if(be->gety() < itofix(h) / 2)
		be->sety(be->gety() + 64);
	else
	{
		// [31] : pause timer
		if(be->getInternal(31) < 256)
			be->incInternal(31);
		else
		{
			if(be->getInternal(1))
			{
				// [30] : angle increment timer
				if(!(be->getInternal(30) % 4))
					be->incInternal(1);
				be->incInternal(30);
			}
			else
			{
				be->currentPattern = be->lastPattern = 0;
				be->setInternal(30, 0);
				be->setInternal(31, 0);
				G_enemiesArray.add(itofix(20), itofix(-20), 100, image_LUT_boss1_enemy_ship_shadow, Pattern_1_boss, 0, SHADOW, true, 0, false, false);
				G_enemiesArray.add(itofix(300), itofix(-20), 100, image_LUT_boss1_enemy_ship_shadow, Pattern_1_boss, 1, SHADOW, true, 0, false, false);
				be->HP = getHPsum(be->HPperPattern, 0, be->patternsNb - 1);
				be->readyToGo = true;
			}
		}
	}
	Rect pos = { fixtoi(be->getx()), fixtoi(be->gety()), 0, 0 }, centerRot;
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, CAMREL_RELATIVE);
	
	pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed1);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed1], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
	
	// [1]: angle of sword [-32, 0]
	pos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_leftarm_armed], &pos, &centerRot, be->getInternal(1) + be->angle, CAMREL_RELATIVE);
}

void boss1_icb2(BossEnemy *be)
{
	const int h = bossImage_entries[bossImage_LUT_1_body][1];
	static Fixed dx = 0, dy = 0;
	Rect pos, centerRot;
	if(!be->initCallbackCalled)
	{
		dx = (itofix(160) - be->getx()) / 128;
		dy = (itofix(h / 2) - be->gety()) / 128;
		pos = getJointPoint(be, boss1_jointData, be->currentPattern == 1 ? joint_leftarm_armed : joint_rightarm_armed2);
		initExplosionEffect(pos.x, pos.y, 150, 0);
		be->initCallbackCalled = true;
		be->angle = 0;
	}
	updateExplosionEffect();
	
	if(fixtoi(be->getx()) != 160 || fixtoi(be->gety()) != h / 2)
	{
		be->setx(be->getx() + dx);
		be->sety(be->gety() + dy);
	}
	else
	{
		// [31] : pause timer
		if(be->getInternal(31) >= 256)
		{
			be->setInternal(0, 0);
			be->setInternal(31, 0);
			G_enemiesArray.add(itofix(20), itofix(-20), 100, image_LUT_boss1_enemy_ship_light , Pattern_1_boss, 0, LIGHT, true, 0, false, false);
			G_enemiesArray.add(itofix(300), itofix(-20), 100, be->currentPattern == 1 ? image_LUT_boss1_enemy_ship_light : image_LUT_boss1_enemy_ship_shadow,
				Pattern_1_boss, 1, be->currentPattern == 1 ? LIGHT : SHADOW, true, 0, false, false);
			be->HP = getHPsum(be->HPperPattern, be->currentPattern, be->patternsNb - 1);
			be->readyToGo = true;
		}
		be->incInternal(31);
	}
		
	pos.x = fixtoi(be->getx());
	pos.y = fixtoi(be->gety());
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, CAMREL_RELATIVE);
	
	pos = getJointPoint(be, boss1_jointData, be->currentPattern == 1 ? joint_rightarm_armed1 : joint_rightarm_nonarmed);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[be->currentPattern == 1 ? bossImage_LUT_1_rightarm_armed1 : bossImage_LUT_1_rightarm_nonarmed], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
	
	pos = getJointPoint(be, boss1_jointData, joint_leftarm_nonarmed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_leftarm_nonarmed], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
}

/*
 * ###################
 * Behaviour callbacks
 * ###################
 */

void boss1_cb(BossEnemy *be, Player *p, BulletArray *bArray)
{
	Rect pos, centerRot;
	
	pos.x = fixtoi(be->getx());
	pos.y = fixtoi(be->gety());
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, NULL, be->angle, CAMREL_RELATIVE);
	
	be->currentPattern = getPatternID(be);
	if(be->currentPattern != be->lastPattern)
	{
		be->lastPattern = be->currentPattern;
		be->initCallbackCalled = false;
		be->readyToGo = false;
		G_enemiesArray.destroyAllEnemies(p, bArray);
		bArray->clear();
		return;
	}
	
	// [0]: timer
	
	// Pattern 1
	if(!be->currentPattern)
	{
		// Slowly move and rotate the boss to fire everywhere
		be->setx(be->getx() - fixcos(be->getInternal(0) / 8) / 4);
		be->angle = fixmul(12, fixsin(be->getInternal(0) / 8));
		be->incInternal(0);
		
		pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed1);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed1], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
		
		pos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_armed], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 32))
		{
			rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1], pos.x, pos.y, be->angle, &pos);
			if(timer & 32)
			{
				for(int i = 0; i < 3; i++)
				{
					Fixed angle = 64 - be->angle - (i - 1) * 32;
					bArray->add(itofix(pos.x), itofix(pos.y), angle, itofix(1), image_LUT_enemy_bullet_1_light, LIGHT, true, CAMREL_NONE);
				}
			}
			else
			{
				for(int i = 0; i < 2; i++)
				{
					Fixed angle = 64 - be->angle - (i * 32 - 16);
					bArray->add(itofix(pos.x), itofix(pos.y), angle, itofix(1), image_LUT_enemy_bullet_1_light, LIGHT, true, CAMREL_NONE);
				}
			}
		}
	}
	// Pattern 2
	else if(be->currentPattern == 1)
	{
		unsigned short *img = bossImage_entries[bossImage_LUT_1_rightarm_armed2];
		// Display things the exact same way as in pattern 1
		be->setx(be->getx() - fixcos(be->getInternal(0) / 8) / 4);
		be->angle = fixmul(12, fixsin(be->getInternal(0) / 8));
		be->incInternal(0);
		
		pos = getJointPoint(be, boss1_jointData, joint_leftarm_nonarmed);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_nonarmed], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
		
		pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed2);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed2], &pos, &centerRot, be->angle, CAMREL_RELATIVE);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 12))
		{
			static int bulletFired[3] = { 2, 2, 0 };
			static bool canFire[3] = { false, true, true };
			rotate(pos.x + img[0] / 2 - pos.w, pos.y + img[1] / 2 - pos.h , pos.x, pos.y, be->angle, &pos);
			for(int j = 0; j < 3; j++)
			{
				if(canFire[j])
				{
					Fixed angle = j * 6 + 16 - be->angle;
					for(int i = 0; i < 4; i++)
					{
						bArray->add(itofix(pos.x), itofix(pos.y), angle, itofix(1), image_LUT_enemy_bullet_1_light, SHADOW, true, CAMREL_NONE);
						angle += 21;
					}
					bulletFired[j]++;
				}
				else
				{
					bulletFired[j]--;
				}
				if(!bulletFired[j] || bulletFired[j] == 4)
					canFire[j] = !canFire[j];
			}
		}
		
	}
	// Pattern 3
	else if(be->currentPattern == 2)
	{
		// [2] : left arm angle
		// [3] : right arm angle
		pos = getJointPoint(be, boss1_jointData, joint_leftarm_nonarmed);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_nonarmed], &pos, &centerRot, be->getInternal(2), CAMREL_RELATIVE);
		
		pos = getJointPoint(be, boss1_jointData, joint_rightarm_nonarmed);
		rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_nonarmed], &pos, &centerRot, be->getInternal(3), CAMREL_RELATIVE);
		
		be->incInternal(0);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 256))
		{
			// [1] : fire stage
			// 0 - 5 : grenades
			// 6 : wait 'til the bullets from the grenades disappeared
			// 7 - 10 : homings
			int fireStage = be->getInternal(1) % 11;
			if(fireStage < 6)
			{
				// TODO : animation
				pos = getJointPoint(be, boss1_jointData, joint_leftarm_nonarmed);
				for(int i = 0; i < 4; i++)
					G_enemiesArray.add(itofix(pos.x), itofix(pos.y), 1, image_LUT_boss1_grenade_light + (fireStage % 2), Pattern_1_bossGrenade, i, fireStage % 2, true, 0, true, false);
			}
			else if(fireStage > 6)
			{
				// TODO : animation
				pos = getJointPoint(be, boss1_jointData, joint_rightarm_nonarmed);
				for(int i = 0; i < 5; i++)
				{
					bArray->add_homing(itofix(pos.x), itofix(pos.y), i * 32, p, LIGHT);
				}
			}
			be->incInternal(1);
		}
	}
}

/*
 * ###################
 * Collision callbacks
 * ###################
 */

// Hitbox : sword
int boss1_ccb1(BossEnemy *be, Bullet *b, int amount)
{
	Rect jointPos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
	const unsigned short *img = bossImage_entries[bossImage_LUT_1_leftarm_armed];
	Rect box;
	
	getBoundingBox(jointPos.x - jointPos.w, jointPos.y - jointPos.h, img[0], img[1], jointPos.x, jointPos.y, be->angle, &box);
	
	if(box.x <= fixtoi(b->getx()) && box.x + box.w >= fixtoi(b->getx()) &&
		box.y <= fixtoi(b->gety()) && box.y + box.h >= fixtoi(b->gety()))
	{
		G_score += b->getPolarity() == SHADOW ? SCORE_HIT_OP : SCORE_HIT;
		return ((b->getPolarity() == SHADOW) + 1) * amount;
	}
	return 0;
}

// Hitbox : shield
int boss1_ccb2(BossEnemy *be, Bullet *b, int amount)
{
	Rect jointPos = getJointPoint(be, boss1_jointData, joint_rightarm_armed2);
	const unsigned short *img = bossImage_entries[bossImage_LUT_1_rightarm_armed2];
	Rect box;
	
	getBoundingBox(jointPos.x - jointPos.w, jointPos.y - jointPos.h, img[0], img[1], jointPos.x, jointPos.y, be->angle, &box);
	
	if(box.x <= fixtoi(b->getx()) && box.x + box.w >= fixtoi(b->getx()) &&
		box.y <= fixtoi(b->gety()) && box.y + box.h >= fixtoi(b->gety()))
	{
		G_score += b->getPolarity() == LIGHT ? SCORE_HIT_OP : SCORE_HIT;
		return ((b->getPolarity() == LIGHT) + 1) * amount;
	}
	return 0;
}

// Hitbox : body
int boss1_ccb3(BossEnemy *be, Bullet *b, int amount)
{
	const unsigned short *img = bossImage_entries[bossImage_LUT_1_body];
	
	if(be->getx() - itofix(img[0]) / 2 <= b->getx() && be->getx() + itofix(img[0]) / 2 >= b->getx() &&
		be->gety() - itofix(img[1]) / 2 <= b->gety() && be->gety() + itofix(img[1]) / 2 >= b->gety())
	{
		G_score += b->getPolarity() == SHADOW ? SCORE_HIT_OP : SCORE_HIT;
		return ((b->getPolarity() == SHADOW) + 1) * amount;
	}
	return 0;
}

/*
 * ##################
 * Distance callbacks
 * ##################
 */

// Target point : center of sword
Fixed boss1_dcb1(BossEnemy *be, PowerFragment *pf)
{
	Rect pos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1] / 2, pos.x, pos.y, be->angle, &pos);
	return distance(pos.x, pos.y, fixtoi(pf->getx()), fixtoi(pf->gety()));
}

// Target point : center of shield
Fixed boss1_dcb2(BossEnemy *be, PowerFragment *pf)
{
	Rect pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed2);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	return distance(pos.x, pos.y, fixtoi(pf->getx()), fixtoi(pf->gety()));
}

// Target point : center of body
Fixed boss1_dcb3(BossEnemy *be, PowerFragment *pf)
{
	return distance(fixtoi(be->getx()), fixtoi(be->gety()), fixtoi(pf->getx()), fixtoi(pf->gety()));
}

/*
 * ###############
 * Angle callbacks
 * ###############
 *
 * Same target points as the distance callbacks
 */

Fixed boss1_acb1(BossEnemy *be, PowerFragment *pf)
{
	Entity temp;
	Rect pos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1] / 2, pos.x, pos.y, be->angle, &pos);
	temp.activate();
	temp.setx(itofix(pos.x));
	temp.sety(itofix(pos.y));
	return angleToEntity(pf, &temp);
}

Fixed boss1_acb2(BossEnemy *be, PowerFragment *pf)
{
	Entity temp;
	Rect pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed2);
	rotate(pos.x, pos.y, fixtoi(be->getx()), fixtoi(be->gety()), be->angle, &pos);
	temp.activate();
	temp.setx(itofix(pos.x));
	temp.sety(itofix(pos.y));
	return angleToEntity(pf, &temp);
}

Fixed boss1_acb3(BossEnemy *be, PowerFragment *pf)
{
	return angleToEntity(pf, be);
}

/* ####################
 * 
 * ADDITIONAL DATA
 *
 * ####################
 */

boss_icb bossICBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_icb1, boss1_icb2, boss1_icb2 }
};

boss_cb bossCBdata[BOSS_NB] = { boss1_cb };

boss_ccb bossCCBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_ccb1, boss1_ccb2, boss1_ccb3 }
};

boss_dcb bossDCBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_dcb1, boss1_dcb2, boss1_dcb3 }
};

boss_acb bossACBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_acb1, boss1_acb2, boss1_acb3 }
};