#include "common.h"
#include "bossData.h"

BossData createBossData(int bossID)
{
	BossData result;
	result.patternsNb = bossPatternsNb[bossID];
	result.HPperPattern = bossHPperPat[bossID];
	result.timeoutPerPattern = bossTimeoutPerPat[bossID];
	result.initCallbacks = bossICBdata[bossID];
	result.callback = bossCBdata[bossID];
	result.collisionCallbacks = bossCCBdata[bossID];
	result.distanceCallbacks = bossDCBdata[bossID];
	result.angleCallbacks = bossACBdata[bossID];
	return result;
}

BossEnemy::BossEnemy() : Entity()
{
	HP = 0;
	// Bind "hurtable" to "active"
	hurtable = &active;
}

BossEnemy::~BossEnemy()
{
	
}

void BossEnemy::activate(BossData *d)
{
	for(int i = 0; i < 32; i++)
		internal[i] = 0;
	patternsNb = d->patternsNb;
	HPperPattern = d->HPperPattern;
	timeoutPerPattern = d->timeoutPerPattern;
	initCallbacks = d->initCallbacks;
	callback = d->callback;
	collisionCallbacks = d->collisionCallbacks;
	distanceCallbacks = d->distanceCallbacks;
	angleCallbacks = d->angleCallbacks;
	currentPattern = 0;
	// Mark for initialization callback execution
	readyToGo = false;
	initCallbackCalled = false;
	*hurtable = false;
	remainingTime = timeoutPerPattern[0];
}

// Return 1 if dead
int BossEnemy::handle(Player *p, BulletArray *bArray)
{
	if(readyToGo)
	{
		int mHP = HP - getHPsum(HPperPattern, currentPattern + 1, patternsNb - 1);
		int color = mHP / HP_PER_BAR + 1;
		mHP %= HP_PER_BAR;
		int limit = 240 - mHP * 240 / HP_PER_BAR;
		fillRect(316, 0, 4, limit, color_HPbars[color - 1]);
		fillRect(316, limit, 4, 239 - limit, color_HPbars[color]);
		
		*hurtable = true;
		(callback)(this, p, bArray);
		// Timer 1 counts the second
		if(!timer_read(1))
		{
			remainingTime--;
			timer_load(1, 32768);
		}
		if(!remainingTime)
			HP = 0;
	}
	else
	{
		*hurtable = false;
		(initCallbacks[currentPattern])(this);
	}
	
	return HP < 1;
}

void BossEnemy::damage(int amount)
{
	HP -= amount;
}

// Equivalent to BossEnemy::isActive()
bool BossEnemy::isHurtable()
{
	return *hurtable;
}

void BossEnemy::makeHurtable()
{
	*hurtable = true;
}

void BossEnemy::setInternal(int offset, int value)
{
	internal[offset] = value;
}

void BossEnemy::incInternal(int offset)
{
	internal[offset]++;
}

void BossEnemy::decInternal(int offset)
{
	internal[offset]--;
}

int BossEnemy::getInternal(int offset)
{
	return internal[offset];
}

int BossEnemy::getTimeout()
{
	return remainingTime;
}

Fixed BossEnemy::getDistance(PowerFragment *pf)
{
	return (distanceCallbacks[currentPattern])(this, pf);
}

Fixed BossEnemy::getAngle(PowerFragment *pf)
{
	return (angleCallbacks[currentPattern])(this, pf);
}