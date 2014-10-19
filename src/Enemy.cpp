#include "common.h"

inline Fixed angleToPlayer(Enemy *e, Player *p)
{
	return (int)(atan2((double)(p->y - e->gety()), (double)(p->x - e->getx())) * 128. / M_PI);
}

Enemy::Enemy()
{
	active = false;
	diedThisFrame = false;
	isJointed = false;
}

Enemy::~Enemy()
{
}

void Enemy::handle(Player *p, BulletArray *bArray)
{
	Rect er;
	Fixed angle;
	
	if(active)
	{
		if(isJointed && diesWithJoint && !G_enemiesArray[jointedTo]->isActive())
			damage(p, !polarity, HP, bArray);
		else
		{
			// handle bullets and pattern first
			switch(callback)
			{
				// Too many patterns, needs dedicated file
				#include "patterns.h"
			}
			
			er.x = fixtoi(getx());
			er.y = fixtoi(gety());
			
			// Have a relatively big threshold for off-screen movement
			if(er.x + img[0] / 2 < -80 || er.x - img[0] / 2 > 399 ||
				er.y + img[1] / 2 < -80 || er.y - img[1] / 2 > 319)
				deactivate();
			else
			{
				// then the enemy image
				if(hasRotation)
				{
					//~ drawSpriteRotated(img, &er, NULL, rotationAngle);
					DC->add(img, &er, NULL, rotationAngle);
				}
				else
				{
					er.x -= img[0] / 2;
					er.y -= img[1] / 2;
					//~ drawSprite(img, er.x, er.y);
					DC->add(img, &er);
				}
			}
			// Check wether the player hit the enemy
			if(!p->isDying() && (p->x >= getx() - (itofix(img[0]) / 2) && p->x < getx() + (itofix(img[0]) / 2)) && (p->y >= gety() - (itofix(img[0]) / 2) && p->y <= gety() + (itofix(img[1]) / 2)))
			{
				p->hurt();
			}
		}
	}
	else
	{
		diedThisFrame = false;
		isJointed = false;
	}
}

bool Enemy::isActive()
{
	return active;
}

void Enemy::activate(int _x, int _y, int _HP, int shipImgID, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation, int _f)
{
	HP = _HP;
	x = _x;
	y = _y;
	
	img = image_entries[shipImgID];
	fireback = _f;
	polarity = _polarity;
	hasRotation = _hasRotation;
	rotationAngle = 0;
	callback = callbackID;
	waveIndex = _waveIndex;
	for(int i = 0; i < 6; i++)
		internal[i] = 0;
	active = true;
}

void Enemy::deactivate()
{
	active = false;
}

void Enemy::damage(Player *_p, bool _pol, int amount, BulletArray *bArray)
{
	HP -= amount;
	if(_pol != polarity)
		HP -= amount;
	
	if(HP <= 0)
	{
		if(G_fireback)
		{
			if(_pol == polarity || G_hardMode)
			{
				Fixed angle = angleToPlayer(this, _p);
				int famount = _pol != polarity ? fireback / 2 : fireback;
				for(int i = 0; i < famount; i++)
					bArray->add(getx(), gety(), fixcos(angle + (rand() % 16) - 8) << 1, fixsin(angle + (rand() % 16) - 8) + (rand() % 256), image_LUT_enemy_bullet_0_light, polarity, true);
			}
		}
		diedThisFrame = true;
		deactivate();
	}
}

void Enemy::joint(int _n, Fixed _x, Fixed _y, bool _d)
{
	isJointed = true;
	jointedTo = _n;
	jointX = _x;
	jointY = _y;
	diesWithJoint = _d;
}

Fixed Enemy::getRotation()
{
	return rotationAngle;
}

void Enemy::setRotation(Fixed r)
{
	rotationAngle = r;
}

bool Enemy::getPolarity()
{
	return polarity;
}

int Enemy::getWaveIndex()
{
	return waveIndex;
}

Fixed Enemy::getx()
{
	return isJointed ? x + G_enemiesArray[jointedTo]->getx() + jointX : x;
}

Fixed Enemy::gety()
{
	return isJointed ? y + G_enemiesArray[jointedTo]->gety() + jointY : y;
}

