#include "common.h"

BulletArray::BulletArray()
{
	clear();
	// lasers too
	for(int i = 0; i < MAX_LASER; i++)
		data_laser[i].deactivate();
	laserCount = 0;
}

BulletArray::~BulletArray()
{
}

void BulletArray::handle(Player *p, BossEnemy *be)
{
	bool destroyBullet;
	int bossDamaged;
	
	// Bullets
	for(int i = 0; i < MAX_BULLET; i++)
	{
		Bullet *cb = &data[i];
		destroyBullet = false;
		if(cb->isActive())
		{
			if(cb->hurtsPlayer() && p->isHurtable())
			{
				// Check collisions with player if he's not dead already
				if(cb->getPolarity() != p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to be enough)
					if(p->getx() >= cb->getx() - itofix(cb->img[0] / 2) && p->getx() < cb->getx() + itofix(cb->img[0] / 2)
					&& p->gety() >= cb->gety() - itofix(cb->img[1] / 2) && p->gety() < cb->gety() + itofix(cb->img[1] / 2))
					{
						p->hurt();
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(cb->getx() - p->getx())) + sq(fixtoi(cb->gety() - p->gety())) < sq(19)) // sqrt(player.w/2 ^2 + player.h/2 ^2)
					{
						destroyBullet = true;
						G_score += SCORE_ABSORB;
						G_power += G_power < MAX_POWER;
					}
				}
			}
			else if(!cb->hurtsPlayer())
			{
				// Check collisions with enemies (there are much more enemies than players)
				for(int j = 0; j < MAX_ENEMY; j++)
				{
					if(G_enemiesArray.data[j].isActive() && !G_enemiesArray.data[j].isGhost())
					{
						if(cb->getx() - itofix(cb->img[0] / 2) <= G_enemiesArray.data[j].getx() + itofix(G_enemiesArray.data[j].img[0] / 2) &&
						cb->getx() + itofix(cb->img[0] / 2) >= G_enemiesArray.data[j].getx() - itofix(G_enemiesArray.data[j].img[0] / 2) &&
						cb->gety() - itofix(cb->img[1] / 2) <= G_enemiesArray.data[j].gety() + itofix(G_enemiesArray.data[j].img[1] / 2) &&
						cb->gety() + itofix(cb->img[1] / 2) >= G_enemiesArray.data[j].gety() - itofix(G_enemiesArray.data[j].img[1] / 2))
						{
							G_enemiesArray.data[j].damage(p, cb->getPolarity(), 1, this);
							G_score += cb->getPolarity() != G_enemiesArray.data[j].getPolarity() ? SCORE_HIT_OP : SCORE_HIT;
							destroyBullet = true;
							// The same bullet can destroy several enemies if it hits them *during the same frame* !
						}
					}
				}
				
				// and possibly the boss
				if(G_fightingBoss && be->isHurtable())
				{
					bossDamaged = (be->collisionCallbacks[be->currentPattern])(be, cb, 1);
					if(bossDamaged)
					{
						destroyBullet = true;
						be->damage(bossDamaged);
					}
				}
			}
			if(destroyBullet)
				deactivate(i);
			else
			{
				if(cb->handle())
					deactivate(i);
				else
					cb->draw();
			}
		}
		else break;
	}	
	
	// Power fragments
	for(int i = 0; i < MAX_FRAGMENT; i++)
	{
		PowerFragment *cf = &data_fragment[i];
		destroyBullet = false;
		if(cf->isActive())
		{
			if(cf->hurtsPlayer() && p->isHurtable())
			{
				// Check collisions with player
				if(cf->getPolarity() != p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to be enough)
					// power fragments have a 8*8 hitbox
					if(p->getx() >= cf->getx() - itofix(4) && p->getx() < cf->getx() + itofix(4)
					&& p->gety() >= cf->gety() - itofix(4) && p->gety() < cf->gety() + itofix(4))
					{
						p->hurt();
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(cf->getx() - p->getx())) + sq(fixtoi(cf->gety() - p->gety())) < sq(p->img[0][0] / 2))
					{
						destroyBullet = true;
						G_score += SCORE_ABSORB * 10;
						G_power = min(G_power + 10, MAX_POWER);
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
						if(cf->getx() - itofix(4) <= cf->targetE->getx() + itofix(cf->targetE->img[0] / 2) &&
						cf->getx() + itofix(4) >= cf->targetE->getx() - itofix(cf->targetE->img[0] / 2) &&
						cf->gety() - itofix(4) <= cf->targetE->gety() + itofix(cf->targetE->img[1] / 2) &&
						cf->gety() + itofix(4) >= cf->targetE->gety() - itofix(cf->targetE->img[1] / 2))
						{
							cf->targetE->damage(p, cf->getPolarity(), 10, this);
							G_score += cf->getPolarity() != cf->targetE->getPolarity() ? SCORE_HIT_OP : SCORE_HIT;
							destroyBullet = true;
						}
					}
				}
				
				// and boss, even
				if(G_fightingBoss && be->isHurtable())
				{
					// Create a placeholder bullet to pass to the collision callback
					Bullet temp;
					temp.activate(cf->getx(), cf->gety(), 0, 0, 0, cf->getPolarity(), false);
					bossDamaged = (be->collisionCallbacks[be->currentPattern])(be, &temp, 10);
					temp.deactivate();
					if(bossDamaged)
					{
						destroyBullet = true;
						be->damage(bossDamaged);
					}
				}
			}
			if(destroyBullet)
				deactivate_fragment(i);
			else
			{
				if(cf->handle())
					deactivate_fragment(i);
				else
					cf->draw();
			}
		}
		else break;
	}
	
	// Homings
	for(int i = 0; i < MAX_HOMING; i++)
	{
		Homing* ch = &data_homing[i];
		destroyBullet = false;
		if(ch->isActive())
		{
			if(p->isHurtable())
			{
				// Check collisions with player
				if(ch->getPolarity() != p->getPolarity())
				{
					// the player has a 1px hitbox (for now) (but that actually seems to be enough)
					// homings have a 16*16 hitbox
					if(p->getx() >= ch->getx() - itofix(8) && p->getx() < ch->getx() + itofix(8)
					&& p->gety() >= ch->gety() - itofix(8) && p->gety() < ch->gety() + itofix(8))
					{
						p->hurt();
						destroyBullet = true;
					}
				}
				else
				{
					if(sq(fixtoi(ch->getx() - p->getx())) + sq(fixtoi(ch->gety() - p->gety())) < sq(p->img[0][0] / 2))
					{
						destroyBullet = true;
						G_score += SCORE_ABSORB * 5;
						G_power = min(G_power + 5, MAX_POWER);
					}
				}
			}
			if(destroyBullet)
				deactivate_homing(i);
			else
			{
				if(ch->handle())
					deactivate_homing(i);
				else
					ch->draw();
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
				
				if(p->isHurtable())
				{
					// Uses cartesian hitbox for checking collision with player
					// First, see if the player is not too far
					if(sq(fixtoi(p->getx()) - r->x) + sq(fixtoi(p->gety()) - r->y) <= sq(cl->getAmplitude()))
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
							int temp = a * fixtoi(p->getx()) + b * fixtoi(p->gety());
							// Work the player's 1 px hitbox
							if(sign(temp + c1) != sign(temp + c2))
								// Hit !
								p->hurt();
						}
						else
						{
							int temp1 = a * (fixtoi(p->getx()) - p->img[0][0] / 2) + b * fixtoi(p->gety());
							int temp2 = a * (fixtoi(p->getx()) + p->img[0][0] / 2) + b * fixtoi(p->gety());
							
							if(sign(temp1 + c1) != sign(temp1 + c2) || sign(temp2 + c1) != sign(temp2 + c2))
							{
								// Hit, but doesn't hurt
								cl->setAmplitude((int)sqrt(sq(fixtoi(p->getx()) - r->x) + sq(fixtoi(p->gety()) - r->y)));
								// Using G_skipFrame as a delay
								if(!(G_skipFrame % 4))
								{
									G_power += G_power < MAX_POWER;
									G_score += SCORE_ABSORB;
								}
								// Lasers are powerful, so they push the player
								p->setx(p->getx() + fixcos(cl->angle) / 2);
								p->sety(p->gety() + fixsin(cl->angle) / 2);
								
								// Add particles ! Yeeee !
								int k = (rand() % 4) + 1;
								for(int j = 0; j < k; j++)
								{
									Fixed a = cl->angle + 128 + (rand() % 64) - 32;
									G_particles->add(p->getx(), p->gety(), fixcos(a) / 2, fixsin(a) / 2, cl->getPolarity());
								}
							}
						}
					}
				}
				
				cl->handle();
				cl->draw();
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
		data[bulletCount].activate(_x, _y, _dx, _dy, imgID, _p, _h);
		bulletCount++;
	}
}

void BulletArray::add_fragment(Fixed _x, Fixed _y, Fixed angle, Player* targetP, bool _p, bool _h)
{
	if(fragmentCount < MAX_FRAGMENT)
	{
		data_fragment[fragmentCount].activate(_x, _y, angle, targetP, _p, _h);
		fragmentCount++;
	}
}

void BulletArray::add_homing(Fixed _x, Fixed _y, Fixed angle, Player* target, bool _p)
{
	if(homingCount < MAX_HOMING)
	{
		data_homing[homingCount].activate(_x, _y, angle, target, _p);
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

void BulletArray::deactivate_fragment(int n)
{
	fragmentCount--;
	for(int i = n; i < fragmentCount; i++)
		data_fragment[i] = data_fragment[i + 1];
	data_fragment[fragmentCount].deactivate();
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

// Destroy all bullets except for lasers
void BulletArray::clear()
{
	for(int i = 0; i < MAX_BULLET; i++)
		data[i].deactivate();
	for(int i = 0; i < MAX_FRAGMENT; i++)
		data_fragment[i].deactivate();
	for(int i = 0; i < MAX_HOMING; i++)
		data_homing[i].deactivate();
	bulletCount = 0;
	fragmentCount = 0;
	homingCount = 0;
}