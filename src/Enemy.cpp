#include "common.h"

Enemy::Enemy() : bArray(800, 0)
{
	active = false;
	firingBack = false;
}

Enemy::~Enemy()
{
}

void Enemy::handle(Player *p)
{
	Rect er;
	Fixed angle;
	
	if(active)
	{
		// handle bullets and pattern first
		switch(callback)
		{
			// Chapter 1
			// Wave 1
			case Pattern_1_1:
				if(!(internal[0] & 1))
				{
					internal[1] = internal[0] >> 1;
					x = itofix(320) - fixmul(sq(internal[1]), 768);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] >> 1;
				break;
			// Wave 2
			case Pattern_1_2:
				if(!(internal[0] & 1))
				{
					internal[1] = internal[0] >> 1;
					x = fixmul(sq(internal[1]), 768);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] >> 1;
				break;
			// Wave 3
			case Pattern_1_3:
				if(!(internal[0] % 3))
				{
					internal[1] = internal[0] / 3;
					x = fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256) + itofix(320);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] >> 1;
				break;
			// Wave 4
			case Pattern_1_4:
				if(!(internal[0] % 3))
				{
					internal[1] = internal[0] / 3;
					x = -fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] >> 1;
				break;
			// Wave 5
			case Pattern_1_5:
				internal[1] = internal[0] >> 2;
				if(internal[0] & 1)
				{
					if(internal[1] < 140 - (int)(waveIndex >> 3) * 20)
						y = itofix(internal[1]);
					else
						x = (waveIndex & 7) < 4 ? x - itofix(1) : x + itofix(1);
				}
				internal[0]++;
				break;
			// Wave 6
			case Pattern_1_6:
				angle = angleToPlayer(this, p);
				if(!(internal[0] & 15))
				{
					bArray.add(x, y, fixcos(angle), fixsin(angle), polarity);
				}
				internal[0]++;
				rotationAngle = ~angle + 64;
				break;
			// Test boss
			case Pattern_1_7:
				angle = angleToPlayer(this, p);
				x = itofix(160) + (fixcos(internal[0]) << 5);
				if(!(internal[0] & 3))
				{
					Fixed cura;
					for(int i = 0; i < 4; i++)
					{
						cura = ~((i << 6) + internal[0]);
						bArray.add(x - itofix(img[0] >> 1), y, fixcos(cura) << 1, fixsin(cura), polarity);
						cura = ~cura;
						bArray.add(x + itofix(img[0] >> 1), y, fixcos(cura) << 1, fixsin(cura), polarity);
						cura = angle + (rand() & 31) - 16;
						bArray.add(x, y, fixcos(cura) << 1, fixsin(cura) << 1, polarity);
					}
				}
				internal[0]++;
				rotationAngle = ~angle + 64;
				break;
		}
		
		bArray.handle(p, true, NULL);
		
		er.x = fixtoi(x);
		er.y = fixtoi(y);
		
		if(er.x + (img[0] >> 1) < 0 || er.x - (img[0] >> 1) > 319 ||
			er.y + (img[1] >> 1) < 0 || er.y - (img[1] >> 1) > 239)
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
				er.x -= img[0] >> 1;
				er.y -= img[1] >> 1;
				drawSprite(img, er.x, er.y);
			}
		}
	}
	else if(firingBack)
	{
		bArray.handle(p, true, NULL);
		firingBack = false;
		for(int i = 0; i < bArray.maxBullet; i++)
			if(bArray.data[i].isActive())
			{
				firingBack = true;
				break;
			}
	}
}

bool Enemy::isActive()
{
	return active;
}

void Enemy::activate(int _x, int _y, int _HP, int shipImgID, int bulletImgID, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation)
{
	bArray.setImage(bulletImgID);
	HP = _HP;
	x = _x;
	y = _y;
	
	img = image_entries[shipImgID];
	
	polarity = _polarity;
	hasRotation = _hasRotation;
	rotationAngle = 0;
	callback = callbackID;
	waveIndex = _waveIndex;
	for(int i = 0; i < 3; i++)
		internal[i] = 0;
	active = true;
}

void Enemy::deactivate()
{
	active = false;
}

void Enemy::damage(Player *_p, bool _pol)
{
	HP--;
	if(_pol != polarity)
		HP--;
	
	if(HP <= 0)
	{
		bArray.reset();
		deactivate();
		
		if(_pol == polarity)
		{
			Fixed angle = angleToPlayer(this, _p);
			for(int i = 0; i < 16; i++)
				bArray.add(x, y, fixcos(angle + (rand() & 31) - 16) << 1, fixsin(angle + (rand() & 31) - 16) + (rand() & 255), polarity);
			firingBack = true;
		}
	}
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