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

void BulletArray::handle(Player *p)
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
							G_enemiesArray[j]->damage(p, cb->getPolarity(), this);
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
				cb->x += cb->dx;
				cb->y += cb->dy;
				
				// Bullets have a threshold of 30
				if(cb->x + itofix(cb->img[0]) < itofix(-30) || cb->x > itofix(349) || cb->y + itofix(cb->img[1]) < itofix(-30) || cb->y > itofix(279))
					deactivate(i);
				else
				{
					br.x = fixtoi(cb->x) - (cb->img[0] / 2);
					br.y = fixtoi(cb->y) - (cb->img[1] / 2);
					if(!G_skipFrame) drawSprite(cb->img, br.x, br.y);
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
		data[bulletCount].activate(_x - itofix(img[0] / 2), _y - itofix(img[1] / 2), _dx, _dy, imgID, _p, _h);
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
