#include "common.h"

Fixed angleToPlayer(Enemy *e, Player *p)
{
	return (int)(atan2((double)(p->y - e->gety()), (double)(p->x - e->getx())) * 128. / M_PI);
}

Fixed angleToEnemy(PowerFragment *pf, Enemy *e)
{
	return (int)(atan2((double)(e->gety() - pf->y), (double)(e->getx() - pf->x)) * 128. / M_PI);
}

Fixed angleToPlayer(PowerFragment *pf, Player *p)
{
	return (int)(atan2((double)(p->y - pf->y), (double)(p->x - pf->x)) * 128. / M_PI);
}

Enemy* findNearestEnemy(Fixed x, Fixed y)
{
	static Enemy* nearest;
	static Enemy* ce;
	
	// Find first if there are any living enemy
	bool hasLivingEnemy = false;
	
	for(int i = 0; i < MAX_ENEMY; i++)
	{
		if(G_enemiesArray[i]->isActive())
		{
			hasLivingEnemy = true;
			nearest = G_enemiesArray[i];
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
			if(G_enemiesArray[i]->isActive())
			{
				ce = G_enemiesArray[i];
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

KeyEvent getk(void)
{
	static KeyEvent k;
	k = (((((isKeyPressed(KEY_NSPIRE_8) << 1) + isKeyPressed(KEY_NSPIRE_7)) << 1) + isKeyPressed(KEY_NSPIRE_5)) << 1) + isKeyPressed(KEY_NSPIRE_4);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_ESC) << 1) + isKeyPressed(G_fragmentKey)) << 1) + isKeyPressed(G_polarityKey)) << 1) + isKeyPressed(G_fireKey);
	k <<= 4;
	k |= (((((isKeyPressed(KEY_NSPIRE_UP) << 1) + isKeyPressed(KEY_NSPIRE_RIGHT)) << 1) + isKeyPressed(KEY_NSPIRE_LEFT)) << 1) + isKeyPressed(KEY_NSPIRE_DOWN);
	/*if(isKeyPressed(KEY_NSPIRE_UPRIGHT)) k |= 0x0c;
	if(isKeyPressed(KEY_NSPIRE_RIGHTDOWN)) k |= 0x05;
	if(isKeyPressed(KEY_NSPIRE_DOWNLEFT)) k |= 0x03;
	if(isKeyPressed(KEY_NSPIRE_LEFTUP)) k |= 0x0a;*/
	return k;
}
