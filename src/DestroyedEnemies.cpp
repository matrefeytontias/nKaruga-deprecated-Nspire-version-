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
	x[c] = fixtoi(e->x);
	y[c] = fixtoi(e->y);
	relevant[c] = true;
}
