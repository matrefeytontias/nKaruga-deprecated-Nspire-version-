#include "common.h"

DestroyedEnemies::DestroyedEnemies()
{
	clear();
}

DestroyedEnemies::~DestroyedEnemies()
{
}

void DestroyedEnemies::activate(Enemy *e, int c)
{
	x[c] = fixtoi(e->getx());
	y[c] = fixtoi(e->gety());
	relevant[c] = true;
}

void DestroyedEnemies::clear()
{
	for(int i = 0; i < MAX_ENEMY; i++)
		relevant[i] = false;
}