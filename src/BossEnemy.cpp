#include "common.h"

#include "bossData.h"

BossData createBossData(int bossID)
{
	BossData result;
	result.patternsNb = bossPatternsNb[bossID];
	result.HPperPattern = bossHPperPat[bossID];
	result.initCallback = bossIBdata[bossID];
	result.callback = bossCBdata[bossID];
	result.collisionCallbacks = bossCCBdata[bossID];
	
	return result;
}

BossEnemy::BossEnemy()
{
	
}

BossEnemy::~BossEnemy()
{
	
}

void BossEnemy::activate(BossData *d)
{
	HP = d->HP;
	for(int i = 0; i < 32; i++)
		internal[i] = 0;
	patternsNb = d->patternsNb;
	HPperPattern = d->HPperPattern;
	initCallback = d->initCallback;
	callback = d->callback;
	collisionCallbacks = d->collisionCallbacks;
	currentPattern = 0;
	(d->initCallback)(this);
}

// Return 1 if dead
int BossEnemy::handle(Player *p, BulletArray *bArray)
{
	(callback)(this, p, bArray);
	return HP < 1;
}

void BossEnemy::damage(int amount)
{
	HP -= amount;
}