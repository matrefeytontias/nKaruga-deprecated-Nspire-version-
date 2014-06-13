#include "common.h"

Enemy::Enemy() : bArray(800, 0)
{
	active = false;
}

Enemy::~Enemy()
{
}

void Enemy::handle(Player *p)
{
	Rect er;
	
	// handle bullets and pattern first
	callback.execute(this, p);
	bArray.handle(p, true, NULL);
	
	er.x = fixtoi(x);
	er.y = fixtoi(y);
	
	if(er.x + img[0] / 2 < 0 || er.x - img[0] / 2 > 319 ||
		er.y + img[1] / 2 < 0 || er.y - img[1] / 2 > 239)
		deactivate();
	else
	{
		// then the enemy image
		if(hasRotation)
		{
			drawSpriteRotated(img, er, rotationAngle);
		}
		else
		{
			er.x -= img[0] / 2;
			er.y -= img[1] / 2;
			drawSprite(img, er.x, er.y);
		}
	}
}

bool Enemy::isActive()
{
	return active;
}

void Enemy::activate(int _HP, int shipImgID, int bulletImgID, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation)
{
	bArray.setImage(bulletImgID);
	HP = _HP;
	x = 0;
	y = 0;
	
	img = image_entries[shipImgID];
	
	polarity = _polarity;
	hasRotation = _hasRotation;
	rotationAngle = 0;
	callback.set(callback_entries[callbackID]);
	waveIndex = _waveIndex;
	for(int i = 0; i < 3; i++)
		internal[i] = 0;
	active = true;
}

void Enemy::deactivate()
{
	active = false;
}

void Enemy::damage(bool _p)
{
	if(_p == polarity)
		HP -= 1;
	else
		HP -= 2;
	
	if(HP <= 0)
		deactivate();
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