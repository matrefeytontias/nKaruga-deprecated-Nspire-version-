#include "common.h"

#define incrementInternal(x) e->setInternal(x, e->getInternal(x) + 1);

// Patterns are of the form Pattern_chapter_wave
// Trajectories are parametric equations

// Chapter 1

// x = 320 - t^2 * 1.5
// y = t
void Pattern_1_1(Enemy *e, Player *p)
{
	if(!(e->getInternal(0) % 2))
	{
		e->setInternal(1, e->getInternal(0) >> 1);
		e->x = itofix(320) - fixmul(sq(e->getInternal(1)), 768);
		e->y = itofix(e->getInternal(1));
	}
	
	incrementInternal(0);
	e->setRotation(e->getInternal(0) >> 1);
}

// x = t^2 * 1.5
// y = t
void Pattern_1_2(Enemy *e, Player *p)
{
	if(!(e->getInternal(0) % 2))
	{
		e->setInternal(1, e->getInternal(0) >> 1);
		e->x = fixmul(sq(e->getInternal(1)), 768);
		e->y = itofix(e->getInternal(1));
	}
	
	incrementInternal(0);
	e->setRotation(e->getInternal(0) >> 1);
}

// x = ((t * 3 - 240)^2 - 240) / (waveIndex * .5 + 1) + 320
// y = t
void Pattern_1_3(Enemy *e, Player *p)
{
	if(!(e->getInternal(0) % 3))
	{
		e->setInternal(1, e->getInternal(0) / 3);
		e->x = fixdiv(sq(e->getInternal(0) - 240) - itofix(240), e->getWaveIndex() * 128 + 256) + itofix(320);
		e->y = itofix(e->getInternal(1));
	}
	incrementInternal(0);
	e->setRotation(e->getInternal(0) >> 1);
}

// x = -((t * 3 - 240)^2 - 240) / (waveIndex * .5 + 1)
// y = t
void Pattern_1_4(Enemy *e, Player *p)
{
	if(!(e->getInternal(0) % 3))
	{
		e->setInternal(1, e->getInternal(0) / 3);
		e->x = -fixdiv(sq(e->getInternal(0) - 240) - itofix(240), e->getWaveIndex() * 128 + 256);
		e->y = itofix(e->getInternal(1));
	}
	incrementInternal(0);
	e->setRotation(e->getInternal(0) >> 1);
}

// no parametric equation for you
void Pattern_1_5(Enemy *e, Player *p)
{
	e->setInternal(1, e->getInternal(0) >> 2);
	if(e->getInternal(0) & 1)
	{
		if(e->getInternal(1) < 140 - (int)(e->getWaveIndex() / 8) * 20)
		{
			e->x = itofix((e->getWaveIndex() & 7) * 30 + 40);
			e->y = itofix(e->getInternal(1));
		}
		else
			e->x = (e->getWaveIndex() & 7) < 4 ? e->x - itofix(1) : e->x + itofix(1);
	}
	incrementInternal(0);
}

// Test boss
void Pattern_test_boss(Enemy *e, Player *p)
{
	e->x = itofix(160) + (fixcos(e->getInternal(0)) << 5);
	e->y = itofix(60);
	if(!(e->getInternal(0) & 3))
	{
		Fixed angle = angleToPlayer(e, p);
		Fixed cura;
		for(int i = 0; i < 4; i++)
		{
			cura = ~((i << 6) + e->getInternal(0));
			e->bArray.add(e->x - itofix(e->img[0]), e->y, fixcos(cura) << 1, fixsin(cura), e->getPolarity());
			cura = ~cura;
			e->bArray.add(e->x + itofix(e->img[0]), e->y, fixcos(cura) << 1, fixsin(cura), e->getPolarity());
			cura = angle + (rand() % 32) - 16;
			e->bArray.add(e->x, e->y, fixcos(cura) << 1, fixsin(cura) << 1, e->getPolarity());
		}
	}
	incrementInternal(0);
}