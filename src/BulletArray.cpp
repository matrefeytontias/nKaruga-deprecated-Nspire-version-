#include "common.h"

BulletArray::BulletArray()
{
	for(int i = 0; i < MAX_BULLET; i++)
		data[i].deactivate();
	
	bulletCount = 0;
	homingCount = 0;
}

BulletArray::~BulletArray()
{
}

void BulletArray::handle(Player *p)
{
	bool carryOn = true;
	
	// Bullets
	for(int i = 0; i < MAX_BULLET; i++)
	{
		Bullet *cb = &data[i];
		if(cb->isActive())
		{
			if(cb->hurtsPlayer())
			{
				
				// Check collisions with player
				if(cb->getPolarity() != p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to be enough)
					if(p->x >= cb->x - itofix(cb->img[0] / 2) && p->x < cb->x + itofix(cb->img[0] / 2)
					&& p->y >= cb->y - itofix(cb->img[1] / 2) && p->y < cb->y + itofix(cb->img[1] / 2))
					{
						p->hurt();
						deactivate(i);
						carryOn = false;
					}
				}
				else
				{
					if(sq(fixtoi(cb->x - p->x)) + sq(fixtoi(cb->y - p->y)) < sq(p->img[0][0] / 2))
					{
						deactivate(i);
						G_score += 100;
						G_power += G_power < MAX_POWER;
						carryOn = false;
					}
				}
			}
			else
			{
				// Check collisions with enemies (there are much more enemies than players)
				for(int j = 0; j < MAX_ENEMY; j++)
				{
					if(G_enemiesArray[j]->isActive())
					{
						if(cb->x - itofix(cb->img[0] / 2) <= G_enemiesArray[j]->getx() + itofix(G_enemiesArray[j]->img[0] / 2) &&
						cb->x + itofix(cb->img[0] / 2) >= G_enemiesArray[j]->getx() - itofix(G_enemiesArray[j]->img[0] / 2) &&
						cb->y - itofix(cb->img[1] / 2) <= G_enemiesArray[j]->gety() + itofix(G_enemiesArray[j]->img[1] / 2) &&
						cb->y + itofix(cb->img[1] / 2) >= G_enemiesArray[j]->gety() - itofix(G_enemiesArray[j]->img[1] / 2))
						{
							G_enemiesArray[j]->damage(p, cb->getPolarity(), 1, this);
							G_score += 100;
							deactivate(i);
							carryOn = false;
							break;
						}
					}
				}
			}
			if(carryOn)
			{
				if(cb->handle())
					deactivate(i);
				else if(!G_skipFrame)
					cb->draw();
			}
		}
		else break;
	}
	
	// Power fragments
	for(int i = 0; i < MAX_FRAGMENT; i++)
	{
		PowerFragment *cf = &data_homing[i];
		if(cf->isActive())
		{
			if(cf->hurtsPlayer())
			{
				// Check collisions with player
				if(cf->getPolarity() != p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to be enough)
					// power fragments have a 8*8 hitbox
					if(p->x >= cf->x - itofix(4) && p->x < cf->x + itofix(4)
					&& p->y >= cf->y - itofix(4) && p->y < cf->y + itofix(4))
					{
						p->hurt();
						deactivate_homing(i);
						carryOn = false;
					}
				}
				else
				{
					if(sq(fixtoi(cf->x - p->x)) + sq(fixtoi(cf->y - p->y)) < sq(p->img[0][0] / 2))
					{
						deactivate_homing(i);
						G_score += 100;
						carryOn = false;
					}
				}
			}
			else
			{
				// Check collisions with enemies
				// A power fragment can only hit its registered target
				if(cf->targetE)
				{
					if(cf->targetE->isActive())
					{
						if(cf->x - itofix(4) <= cf->targetE->getx() + itofix(cf->targetE->img[0] / 2) &&
						cf->x + itofix(4) >= cf->targetE->getx() - itofix(cf->targetE->img[0] / 2) &&
						cf->y - itofix(4) <= cf->targetE->gety() + itofix(cf->targetE->img[1] / 2) &&
						cf->y + itofix(4) >= cf->targetE->gety() - itofix(cf->targetE->img[1] / 2))
						{
							cf->targetE->damage(p, cf->getPolarity(), 10, this);
							G_score += 100;
							deactivate_homing(i);
							carryOn = false;
							break;
						}
					}
				}
			}
			if(carryOn)
			{
				if(cf->handle())
					deactivate_homing(i);
				else if(!G_skipFrame)
					cf->draw();
			}
		}
		else break;
	}
}

void BulletArray::add(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, int imgID, bool _p, bool _h)
{
	if(bulletCount < MAX_BULLET)
	{
		unsigned short *img = image_entries[imgID];
		data[bulletCount].activate(_x - itofix(img[0] / 2), _y - itofix(img[1] / 2), _dx, _dy, imgID, _p, _h);
		bulletCount++;
	}
}

void BulletArray::add_homing(Fixed _x, Fixed _y, Fixed angle, Player* targetP, bool _p, bool _h)
{
	if(homingCount < MAX_FRAGMENT)
	{
		data_homing[homingCount].activate(_x, _y, angle, targetP, _p, _h);
		homingCount++;
	}
}

void BulletArray::deactivate(int n)
{
	bulletCount--;
	for(int i = n; i < bulletCount; i++)
		data[i] = data[i + 1];
	data[bulletCount].deactivate();
}

void BulletArray::deactivate_homing(int n)
{
	homingCount--;
	for(int i = n; i < homingCount; i++)
		data_homing[i] = data_homing[i + 1];
	data_homing[homingCount].deactivate();
}