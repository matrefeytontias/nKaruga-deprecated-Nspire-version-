#include "common.h"

DestroyedEnemies::DestroyedEnemies()
{
	for(int i = 0; i < MAX_ENEMY; i++)
		relevant[i] = false;
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
