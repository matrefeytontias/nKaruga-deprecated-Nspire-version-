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
		// handle bullets and pattern first
		switch(callback)
		{
			// Too many patterns, needs dedicated file
			#include "patterns.h"
		}
		
		er.x = fixtoi(getx());
		er.y = fixtoi(gety());
		
		// Have a relatively big threshold for off-screen animations
		if(er.x + img[0] / 2 < -80 || er.x - img[0] / 2 > 399 ||
			er.y + img[1] / 2 < -80 || er.y - img[1] / 2 > 319)
			deactivate();		
		else
		{
			// then the enemy image
			if(!G_skipFrame)
			{
				if(hasRotation)
				{
					drawSpriteRotated(img, &er, rotationAngle);
				}
				else
				{
					er.x -= img[0] / 2;
					er.y -= img[1] / 2;
					drawSprite(img, er.x, er.y);
				}
			}
		}
	}
	else
		diedThisFrame = false;
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
	diedThisFrame = false;
	isJointed = false;
}

void Enemy::damage(Player *_p, bool _pol, int amount, BulletArray *bArray)
{
	HP -= amount;
	if(_pol != polarity)
		HP -= amount;
	
	if(HP <= 0)
	{
		deactivate();
		diedThisFrame = true;
		
		if(_pol == polarity)
		{
			Fixed angle = angleToPlayer(this, _p);
			for(int i = 0; i < fireback; i++)
				bArray->add(getx(), gety(), fixcos(angle + (rand() % 16) - 8) << 1, fixsin(angle + (rand() % 16) - 8) + (rand() % 256), image_LUT_enemy_bullet_0_light, polarity, true);
		}
	}
}

void Enemy::joint(int _n, Fixed _x, Fixed _y)
{
	isJointed = true;
	jointedTo = _n;
	jointX = _x;
	jointY = _y;
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

int Enemy::getInternal(int w)
{
	return internal[w];
}

void Enemy::setInternal(int w, int v)
{
	internal[w] = v;
}

Fixed Enemy::getx()
{
	return isJointed ? x + G_enemiesArray[jointedTo]->getx() + jointX : x;
}

Fixed Enemy::gety()
{
	return isJointed ? y + G_enemiesArray[jointedTo]->gety() + jointY : y;
}