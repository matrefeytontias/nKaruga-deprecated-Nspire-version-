#include "common.h"

Fixed angleToEntity(Entity *e1, Entity *e2)
{
	static Fixed lastResult = 64;
	if(e2->isActive())
		lastResult = (int)(atan2((double)(e2->gety() - e1->gety()), (double)(e2->getx() - e1->getx())) * 128. / M_PI);
	return lastResult;
}

Enemy* findNearestEnemy(Fixed x, Fixed y)
{
	static Enemy* nearest;
	static Enemy* ce;
	
	// Find first if there are any living enemy
	bool hasLivingEnemy = false;
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		if(G_enemiesArray.data[i].isActive() && !G_enemiesArray.data[i].isGhost())
		{
			hasLivingEnemy = true;
			nearest = &G_enemiesArray.data[i];
			break;
		}
	}
	
	// If not, do nothing
	if(!hasLivingEnemy)
		return NULL;
	else
	{
		// If yes, find the actual nearest enemy
		int lastDistance = sq(fixtoi(nearest->getx() - x)) + sq(fixtoi(nearest->gety() - y));
		int concurrentDistance;
		
		for(int i = 0; i < MAX_ENEMY; i++)
		{
			if(G_enemiesArray.data[i].isActive() && !G_enemiesArray.data[i].isGhost())
			{
				ce = &G_enemiesArray.data[i];
				concurrentDistance = sq(fixtoi(ce->getx() - x)) + sq(fixtoi(ce->gety() - y));
				if(concurrentDistance < lastDistance)
				{
					nearest = ce;
					lastDistance = concurrentDistance;
				}
			}
		}
		return nearest;
	}
}

int distance(int x1, int y1, int x2, int y2)
{
	return sq(x1 - x2) + sq(y1 - y2);
}

KeyEvent getk(void)
{
	static KeyEvent k;
	k = isKeyPressed(G_pauseKey);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_8) << 1) + isKeyPressed(KEY_NSPIRE_7)) << 1) + isKeyPressed(KEY_NSPIRE_5)) << 1) + isKeyPressed(KEY_NSPIRE_4);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_ESC) << 1) + isKeyPressed(G_fragmentKey)) << 1) + isKeyPressed(G_polarityKey)) << 1) + isKeyPressed(G_fireKey);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_UP) << 1) + isKeyPressed(KEY_NSPIRE_RIGHT)) << 1) + isKeyPressed(KEY_NSPIRE_LEFT)) << 1) + isKeyPressed(KEY_NSPIRE_DOWN);
	return k;
}
