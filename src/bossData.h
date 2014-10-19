#include "common.h"

#define HP_PER_BAR 400
#define BOSS_NB 1
#define MAX_PATTERNS_PER_BOSS 3

int getTotalHP(int *a, int j)
{
	int total = 0;
	for(int i = 0; i < j; i++)
		total += a[i];
	return total;
}

// Initialization callback
void boss1_ib(BossEnemy *be)
{
	be->x = 160;
	//~ be->y = -bossImage_entries[bossImage_LUT_1_body][1] / 2;
	be->y = bossImage_entries[bossImage_LUT_1_body][1] / 2;
	be->HP = getTotalHP(be->HPperPattern, be->patternsNb);
}

// Behavior callbacks

void boss1_cb(BossEnemy *be, Player *p, BulletArray *bArray)
{
	Rect pos;
	pos.x = be->x - bossImage_entries[bossImage_LUT_1_body][0] / 2;
	pos.y = be->y - bossImage_entries[bossImage_LUT_1_body][1] / 2;
	DC->add(bossImage_entries[bossImage_LUT_1_body], &pos);
	UNUSED(p);
	UNUSED(bArray);
}

// Collision CallBacks

int boss1_ccb1(BossEnemy *be, Bullet *b)
{
	unsigned short *bodyImg = bossImage_entries[bossImage_LUT_1_body];
	if(be->x - bodyImg[0] / 2 <= fixtoi(b->x) && be->x + bodyImg[0] / 2 >= fixtoi(b->x) &&
		be->y - bodyImg[1] / 2 <= fixtoi(b->y) && be->y + bodyImg[1] / 2 >= fixtoi(b->y))
		return 10000;
	
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

int bossPatternsNb[BOSS_NB] = { 3 };

int bossHPperPat[BOSS_NB][MAX_PATTERNS_PER_BOSS] = {
	{ HP_PER_BAR, HP_PER_BAR, HP_PER_BAR * 3 }
};

boss_ib bossIBdata[BOSS_NB] = { boss1_ib };

boss_cb bossCBdata[BOSS_NB] = { boss1_cb };

boss_ccb bossCCBdata[BOSS_NB][3] = {
	{ boss1_ccb1, boss1_ccb2, boss1_ccb3 }
};
