#include "common.h"

inline Fixed angleToPlayer(Enemy *e, Player *p)
{
	return (int)(atan2((double)(p->y - e->y), (double)(p->x - e->x)) * 128. / M_PI);
}

Enemy::Enemy()
{
	active = false;
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
			// Intro 1
			case Pattern_1_1:
				if(!(internal[0] % 2))
				{
					internal[1] = internal[0] / 2;
					x = itofix(320) - fixmul(sq(internal[1]), 768);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;	
			case Pattern_1_2:
				if(!(internal[0] % 2))
				{
					internal[1] = internal[0] / 2;
					x = fixmul(sq(internal[1]), 768);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;
			case Pattern_1_3:
				if(!(internal[0] % 3))
				{
					internal[1] = internal[0] / 3;
					x = fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256) + itofix(320);
					y = itofix(internal[1]);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;
			case Pattern_1_4:
				if(!(internal[0] % 3))
				{
					internal[1] = internal[0] / 3;
					x = -fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256);
					y += itofix(1);
				}
				internal[0]++;
				rotationAngle = internal[0] / 2;
				break;
			case Pattern_1_5:
				if(!(G_waveTimer % 3))
				{
					if(G_waveTimer < 500)
						y += itofix(1);
					else
						x += (waveIndex % 6) > 2 ? 192 : -192;
				}
				break;
			case Pattern_1_6:
				angle = angleToPlayer(this, p);
				if(!(internal[0] % 25))
				{
					bArray->add(x, y, fixcos(angle), fixsin(angle), image_LUT_enemy_bullet_1_light, polarity, true);
				}
				internal[0]++;
				rotationAngle = ~angle + 64;
				break;
			// Chapter 1
			case Pattern_1_7:
				if(!(G_waveTimer % 4))
				{
					y += abs(fixcos(internal[0])) * 3;
					internal[0]++;
					if(y < itofix(128))
						x += x > itofix(160) ? -128 : 128;
					else
						x += x > itofix(160) ? itofix(1) : itofix(-1);
				}
				break;
			// Test boss
				/*
			case Pattern_1_7:
				angle = angleToPlayer(this, p);
				x = itofix(160) + (fixcos(internal[0]) << 5);
				if(!(internal[0] % 4))
				{
					Fixed cura;
					for(int i = 0; i < 4; i++)
					{
						cura = ~((i << 6) + internal[0]);
						bArray->add(x - itofix(img[0] / 2), y, fixcos(cura) << 1, fixsin(cura), image_LUT_enemy_bullet_0_light, polarity, true);
						cura = ~cura;
						bArray->add(x + itofix(img[0] / 2), y, fixcos(cura) << 1, fixsin(cura), image_LUT_enemy_bullet_0_light, polarity, true);
						cura = angle + (rand() % 32) - 16;
						bArray->add(x, y, fixcos(cura) << 1, fixsin(cura) << 1, image_LUT_enemy_bullet_0_light, polarity, true);
					}
				}
				internal[0]++;
				rotationAngle = ~angle + 64;
				break;
				*/
		}
		
		er.x = fixtoi(x);
		er.y = fixtoi(y);
		
		if(er.x + img[0] / 2 < 0 || er.x - img[0] / 2 > 319 ||
			er.y + img[1] / 2 < 0 || er.y - img[1] / 2 > 239)
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
}

bool Enemy::isActive()
{
	return active;
}

void Enemy::activate(int _x, int _y, int _HP, int shipImgID, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation)
{
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

void Enemy::damage(Player *_p, bool _pol, BulletArray *bArray)
{
	HP--;
	if(_pol != polarity)
		HP--;
	
	if(HP <= 0)
	{
		deactivate();
		
		if(_pol == polarity)
		{
			Fixed angle = angleToPlayer(this, _p);
			for(int i = 0; i < 16; i++)
				bArray->add(x, y, fixcos(angle + (rand() % 16) - 8) << 1, fixsin(angle + (rand() % 16) - 8) + (rand() % 256), image_LUT_enemy_bullet_0_light, polarity, true);
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