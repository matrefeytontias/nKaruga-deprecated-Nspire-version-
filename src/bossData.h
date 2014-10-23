#include "common.h"

#define HP_PER_BAR 200
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

/*
 * ####
 * DATA
 * ####
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
	{ 140, HP_PER_BAR, HP_PER_BAR * 3 }
};

/*
 * #####
 * UTILS
 * #####
 */

int getHPsum(int *a, int start, int end)
{
	int total = 0;
	for(int i = start; i <= end; i++)
		total += a[i];
	return total;
}

// Works with every angle as the jointing point is not supposed to rotate at all
// x, y = position of jointing point in-screen
// w, h = position of jointing point in the jointed sprite
Rect getJointPoint(BossEnemy *be, int data[][4], int offset)
{
	Rect result;
	result.x = fixtoi(be->x) - bossImage_entries[bossImage_LUT_1_body][0] / 2 + data[offset][2];
	result.y = fixtoi(be->y) - bossImage_entries[bossImage_LUT_1_body][1] / 2 + data[offset][3];
	result.w = data[offset][0];
	result.h = data[offset][1];
	
	return result;
}

/*
 * ##############
 * AI PROGRAMMING
 * ##############
 */

// [x]: usage
// refers to a register number and the usage that's made of it inside the block where the comment is put

/*
 * ######
 * BOSS 1
 * ######
 */

// Initialization callbacks
void boss1_ib1(BossEnemy *be)
{
	const int w = bossImage_entries[bossImage_LUT_1_body][0], h = bossImage_entries[bossImage_LUT_1_body][1];
	if(!be->initCallbackCalled)
	{
		be->x = itofix(160 - w / 2);
		be->y = -itofix(h);
		be->maxHP = be->HP = getHPsum(be->HPperPattern, 0, be->patternsNb - 1);
		be->setInternal(1, -32);
		be->angle = 0;
		be->initCallbackCalled = true;
		be->setInternal(31, 0);
	}
	if(be->y < 0)
		be->y += 64;
	else
	{
		if(be->getInternal(31) < 256)
			be->incInternal(31);
		else
		{
			if(be->getInternal(1))
			{
				if(!(be->getInternal(30) % 4))
					be->incInternal(1);
				be->incInternal(30);
			}
			else
			{
				be->readyToGo = true;
				be->x = itofix(160);
				be->y = itofix(h / 2);
				be->setInternal(30, 0);
				be->setInternal(31, 0);
				G_enemiesArray->add(itofix(20), itofix(-20), 100, image_LUT_boss1_enemy_ship_shadow, Pattern_1_boss, 0, SHADOW, true, 0);
				G_enemiesArray->add(itofix(300), itofix(-20), 100, image_LUT_boss1_enemy_ship_shadow, Pattern_1_boss, 1, SHADOW, true, 0);
			}
		}
	}
	Rect pos = { fixtoi(be->x), fixtoi(be->y), 0, 0 }, centerRot;
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos);
	
	pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed1);
	pos.x += w / 2;
	pos.y += h / 2;
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed1], &pos, &centerRot, be->angle);
	
	// [1]: angle of sword [-32, 0]
	pos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
	pos.x += w / 2;
	pos.y += h / 2;
	rotate(pos.x, pos.y, fixtoi(be->x + w / 2), fixtoi(be->y + h / 2), be->angle, &pos);
	centerRot.x = pos.w;
	centerRot.y = pos.h;
	DC->add(bossImage_entries[bossImage_LUT_1_leftarm_armed], &pos, &centerRot, be->getInternal(1) + be->angle);
}

void boss1_ib2(BossEnemy *be)
{
	UNUSED(be);
	be->readyToGo = true;
}

void boss1_ib3(BossEnemy *be)
{
	UNUSED(be);
}

// Behavior callbacks

void boss1_cb(BossEnemy *be, Player *p, BulletArray *bArray)
{
	Rect pos, centerRot;
	UNUSED(p);
	be->makeHurtable();
	
	pos.x = fixtoi(be->x);
	pos.y = fixtoi(be->y);
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos, NULL, be->angle);
	
	// [0]: timer
	
	// Pattern 1
	if(be->maxHP - be->HP < be->HPperPattern[0])
	{
		be->currentPattern = 0;
		// Slowly move and rotate the boss to fire everywhere
		be->x -= fixcos(be->getInternal(0) / 8) / 4;		
		be->angle = fixmul(12, fixsin(be->getInternal(0) / 8));
		
		pos = getJointPoint(be, boss1_jointData, joint_rightarm_armed1);
		rotate(pos.x, pos.y, fixtoi(be->x), fixtoi(be->y), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_rightarm_armed1], &pos, &centerRot, be->angle);
		
		pos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
		rotate(pos.x, pos.y, fixtoi(be->x), fixtoi(be->y), be->angle, &pos);
		centerRot.x = pos.w;
		centerRot.y = pos.h;
		DC->add(bossImage_entries[bossImage_LUT_1_leftarm_armed], &pos, &centerRot, be->angle);
		
		int timer = be->getInternal(0);
		
		if(!(timer % 32))
		{
			rotate(pos.x, pos.y + bossImage_entries[bossImage_LUT_1_leftarm_armed][1], pos.x, pos.y, be->getInternal(1) + be->angle, &pos);
			if(timer & 32)
			{
				for(int i = 0; i < 3; i++)
				{
					Fixed angle = 64 - be->angle - (i - 1) * 32;
					bArray->add(itofix(pos.x), itofix(pos.y), fixcos(angle), fixsin(angle), image_LUT_enemy_bullet_1_light, LIGHT, true);
				}
			}
			else
			{
				for(int i = 0; i < 2; i++)
				{
					Fixed angle = 64 - be->angle - (i * 32 - 16);
					bArray->add(itofix(pos.x), itofix(pos.y), fixcos(angle), fixsin(angle), image_LUT_enemy_bullet_1_light, LIGHT, true);
				}
			}
		}
		be->incInternal(0);
	}
	// Pattern 2
	else if(be->maxHP - be->HP < be->HPperPattern[1])
	{
		be->currentPattern = 1;
		be->HP = 0;
	}
	// Pattern 3
	else if(be->maxHP - be->HP < be->HPperPattern[2])
	{
		be->currentPattern = 2;
	}
}

// Collision CallBacks

// Hitbox : sword
int boss1_ccb1(BossEnemy *be, Bullet *b)
{
	Rect jointPos = getJointPoint(be, boss1_jointData, joint_leftarm_armed);
	unsigned short *img = bossImage_entries[bossImage_LUT_1_leftarm_armed];
	Rect box;
	
	getBoundingBox(jointPos.x - jointPos.w, jointPos.y - jointPos.h, img[0], img[1], jointPos.x, jointPos.y, be->angle, &box);
	
	if(box.x <= fixtoi(b->x) && box.x + box.w >= fixtoi(b->x) &&
		box.y <= fixtoi(b->y) && box.y + box.h >= fixtoi(b->y))
	{
		G_score += b->getPolarity() == SHADOW ? SCORE_HIT_OP : SCORE_HIT;
		return (b->getPolarity() == SHADOW) + 1;
	}
	return 0;
}

int boss1_ccb2(BossEnemy *be, Bullet *b)
{
	UNUSED(be);
	UNUSED(b);
	return 0;
}

int boss1_ccb3(BossEnemy *be, Bullet *b)
{
	UNUSED(be);
	UNUSED(b);
	return 0;
}

boss_ib bossIBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_ib1, boss1_ib2, boss1_ib3 }
};

boss_cb bossCBdata[BOSS_NB] = { boss1_cb };

boss_ccb bossCCBdata[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ boss1_ccb1, boss1_ccb2, boss1_ccb3 }
};