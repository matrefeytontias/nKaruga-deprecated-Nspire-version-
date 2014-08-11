#include "common.h"

BulletArray::BulletArray()
{
	for(int i = 0; i < MAX_BULLET; i++)
		data[i].deactivate();
	for(int i = 0; i < MAX_FRAGMENT; i++)
		data_homing[i].deactivate();
	for(int i = 0; i < MAX_LASER; i++)
		data_laser[i].deactivate();
	
	bulletCount = 0;
	homingCount = 0;
	laserCount = 0;
}

BulletArray::~BulletArray()
{
}

void BulletArray::handle(Player *p)
{
	bool destroyBullet;
	
	// Bullets
	for(int i = 0; i < MAX_BULLET; i++)
	{
		Bullet *cb = &data[i];
		destroyBullet = false;
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
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(cb->x - p->x)) + sq(fixtoi(cb->y - p->y)) < sq(19)) // sqrt(player.w/2 ^2 + player.h/2 ^2)
					{
						destroyBullet = true;
						G_score += 100;
						G_power += G_power < MAX_POWER;
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
							destroyBullet = true;
							// The same bullet can destroy several enemies if it hits them in the same frame !
						}
					}
				}
			}
			if(destroyBullet)
				deactivate(i);
			else
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
		destroyBullet = false;
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
						destroyBullet = true;
						G_power = min(G_power + 10, MAX_POWER);
					}
				}
				else
				{
					if(sq(fixtoi(cf->x - p->x)) + sq(fixtoi(cf->y - p->y)) < sq(p->img[0][0] / 2))
					{
						destroyBullet = true;
						G_score += 100;
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
							destroyBullet = true;
						}
					}
				}
			}
			if(destroyBullet)
				deactivate_homing(i);
			else
			{
				if(cf->handle())
					deactivate_homing(i);
				else if(!G_skipFrame)
					cf->draw();
			}
		}
		else break;
	}
	
	// Lasers
	for(int i = 0; i < MAX_LASER; i++)
	{
		Rect *r, r1, r2;
		Laser *cl = &data_laser[i];
		if(cl->isActive())
		{
			if(cl->origin->isActive())
			{
				r = cl->getVector();
				cl->getSides(&r1, &r2);
				
				// Uses cartesian hitbox for checking collision with player
				// First, see if the player is not too far
				if(sq(fixtoi(p->x) - r->x) + sq(fixtoi(p->y) - r->y) <= sq(cl->getAmplitude()))
				{
					// if we're not too far, carry on collision checking
					// calculate the laser's cartesian equation and apply it to each of its sides
					// ax + by + c = 0
					int a, b, c1, c2;
					a = r->h;
					b = -r->w;
					c1 = -(a * r1.x + b * r1.y);
					c2 = -(a * r2.x + b * r2.y);
					
					if(p->getPolarity() != cl->getPolarity())
					{
						int temp = a * fixtoi(p->x) + b * fixtoi(p->y);
						// Work the player's 1 px hitbox
						if(sign(temp + c1) != sign(temp + c2))
							// Hit !
							p->hurt();
					}
					else
					{
						int temp1 = a * (fixtoi(p->x) - p->img[0][0] / 2) + b * fixtoi(p->y);
						int temp2 = a * (fixtoi(p->x) + p->img[0][0] / 2) + b * fixtoi(p->y);
						
						if(sign(temp1 + c1) != sign(temp1 + c2) || sign(temp2 + c1) != sign(temp2 + c2))
						{
							// Hit, but doesn't hurt
							cl->setAmplitude((int)sqrt(sq(fixtoi(p->x) - r->x) + sq(fixtoi(p->y) - r->y)));
							// Using G_skipFrame as a delay
							if(!G_skipFrame) G_power += G_power < MAX_POWER;
							// Lasers are powerful, so they push the player
							p->x += fixcos(cl->angle) / 2;
							p->y += fixsin(cl->angle) / 2;
							
							// Add particles ! Yeeee !
							int k = (rand() % 4) + 1;
							for(int j = 0; j < k; j++)
							{
								Fixed a = cl->angle + 128 + (rand() % 64) - 32;
								G_particles->add(p->x, p->y, fixcos(a) / 2, fixsin(a) / 2, cl->getPolarity());
							}
						}
					}
				}
				
				cl->handle();
				if(!G_skipFrame) cl->draw();
			}
			else
				cl->deactivate();
		}
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

void BulletArray::fire_laser(Enemy *e, bool _p)
{
	data_laser[laserCount].activate(e, _p);
	laserCount = (laserCount + 1) % MAX_LASER;
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

void BulletArray::stop_laser(Enemy *e)
{
	for(int i = 0; i < MAX_LASER; i++)
	{
		if(data_laser[i].origin == e)
		{
			data_laser[i].deactivate();
			break;
		}
	}
}