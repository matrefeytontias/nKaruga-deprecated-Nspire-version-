#include "common.h"

BulletArray::BulletArray()
{
	for(int i = 0; i < MAX_BULLET; i++)
		data[i].deactivate();
	
	bulletCount = 0;
}

BulletArray::~BulletArray()
{
}

void BulletArray::handle(Player *p, Enemy **enemiesArray)
{
	static Rect br;
	bool carryOn = true;
	
	for(int i = 0; i < MAX_BULLET; i++)
	{
		Bullet *cb = &data[i];
		if(cb->isActive())
		{
			if(cb->hurtsPlayer())
			{
				// Check collisions with player
				// the player has a 1px hitbox (for now) (but that actually seems to be enough)
				if(p->x >= cb->x - itofix(cb->img[0] / 2) && p->x < cb->x + itofix(cb->img[0] / 2)
				&& p->y >= cb->y - itofix(cb->img[1] / 2) && p->y < cb->y + itofix(cb->img[1] / 2))
				{
					if(cb->getPolarity() != p->getPolarity())
						p->hurt();
					deactivate(i);
					
					carryOn = false;
				}
			}
			else
			{
				// Check collisions with enemies (there are much more enemies than players)
				for(int j = 0; j < MAX_ENEMY; j++)
				{
					if(enemiesArray[j]->isActive())
					{
						if(cb->x - itofix(cb->img[0] / 2) <= enemiesArray[j]->x + itofix(enemiesArray[j]->img[0] / 2) &&
						cb->x + itofix(cb->img[0] / 2) >= enemiesArray[j]->x - itofix(enemiesArray[j]->img[0] / 2) &&
						cb->y - itofix(cb->img[1] / 2) <= enemiesArray[j]->y + itofix(enemiesArray[j]->img[1] / 2) &&
						cb->y + itofix(cb->img[1] / 2) >= enemiesArray[j]->y - itofix(enemiesArray[j]->img[1] / 2))
						{
							enemiesArray[j]->damage(p, cb->getPolarity(), this);
							deactivate(i);
							carryOn = false;
							break;
						}
					}
				}
			}
			if(carryOn)
			{
				cb->x += cb->dx;
				cb->y += cb->dy;
				
				if(cb->x + itofix(cb->img[0]) < 0 || cb->x > itofix(319) || cb->y + itofix(cb->img[1]) < 0 || cb->y > itofix(239))
					deactivate(i);
				else
				{
					br.x = fixtoi(cb->x) - (cb->img[0] / 2);
					br.y = fixtoi(cb->y) - (cb->img[1] / 2);
					drawSprite(cb->img, br.x, br.y);
				}
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
		data[bulletCount].activate(_x - itofix(img[0] / 2), _y - itofix(img[1]), _dx, _dy, imgID, _p, _h);
		bulletCount++;
	}
}

void BulletArray::deactivate(int n)
{
	bulletCount--;
	for(int i = n; i < bulletCount; i++)
		data[i] = data[i + 1];
	data[bulletCount].deactivate();
}
