#include "common.h"

BulletArray::BulletArray(int _maxBullet, int bulletImgID)
{
	maxBullet = _maxBullet;
	data = (Bullet*)malloc(sizeof(Bullet) * maxBullet);
	if(!data)
	{
		printf("Couldn't allocate %d bytes for BulletArray::BulletArray.\n", sizeof(Bullet) * maxBullet);
		exit(0);
	}
	for(int i = 0; i < maxBullet; i++)
	{
		data[i] = Bullet();
		data[i].deactivate();
	}
	
	// first comes light bullet, then shadow bullet - ALWAYS !!!
	img[LIGHT] = image_entries[bulletImgID];
	img[SHADOW] = image_entries[bulletImgID + 1];
	
	bulletCount = 0;
}

BulletArray::~BulletArray()
{
	free(data);
}

void BulletArray::handle(Player *p, bool hurtPlayer, Enemy **enemiesArray)
{
	static Rect br;
	bool carryOn = true;
	
	for(int i = 0; i < maxBullet; i++)
	{
		if(data[i].isActive())
		{
			if(hurtPlayer)
			{
				// Check collisions with player
				// the player has a 1px hitbox (for now) (but that actually seems to be enough)
				if(p->x >= data[i].x - itofix(img[0][0]) / 2 && p->x < data[i].x + itofix(img[0][0]) / 2
				&& p->y >= data[i].y - itofix(img[0][1]) / 2 && p->y < data[i].y + itofix(img[0][1]) / 2)
				{
					if(data[i].getPolarity() != p->getPolarity())
						p->hurt();
					data[i].deactivate();
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
						if(data[i].x - itofix(img[0][0]) / 2 <= enemiesArray[j]->x + itofix(enemiesArray[j]->img[0]) / 2 &&
						data[i].x + itofix(img[0][0]) / 2 >= enemiesArray[j]->x - itofix(enemiesArray[j]->img[0]) / 2 &&
						data[i].y - itofix(img[0][1]) / 2 <= enemiesArray[j]->y + itofix(enemiesArray[j]->img[1]) / 2 &&
						data[i].y + itofix(img[0][1]) / 2 >= enemiesArray[j]->y - itofix(enemiesArray[j]->img[1]) / 2)
						{
							enemiesArray[j]->damage(p, data[i].getPolarity());
							data[i].deactivate();
							carryOn = false;
							break;
						}
					}
				}
			}
			if(carryOn)
			{
				data[i].x += data[i].dx;
				data[i].y += data[i].dy;
				
				if(data[i].x + itofix(img[0][0]) < 0 || data[i].x > itofix(319) || data[i].y + itofix(img[0][1]) < 0 || data[i].y > itofix(239))
					data[i].deactivate();
				else
				{
					br.x = fixtoi(data[i].x) - img[0][0] / 2;
					br.y = fixtoi(data[i].y) - img[0][1] / 2;
					drawSprite(img[data[i].getPolarity() ? SHADOW : LIGHT], br.x, br.y);
				}
			}
		}
	}
}

void BulletArray::setImage(int ID)
{
	img[LIGHT] = image_entries[ID];
	img[SHADOW] = image_entries[ID + 1];
}

void BulletArray::add(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, bool _p)
{
	data[bulletCount].activate(_x - itofix(img[_p][0]) / 2, _y - itofix(img[_p][1]), _dx, _dy, _p);
	bulletCount = (bulletCount + 1) % maxBullet;
}
