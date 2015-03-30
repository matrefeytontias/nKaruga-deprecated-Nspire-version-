#include "common.h"

Enemy::Enemy() : Entity()
{
	active = false;
	diedThisFrame = false;
	isJointed = false;
	rotationAngle = 0;
}

Enemy::~Enemy()
{
	
}

void Enemy::handle(Player *p, BulletArray *bArray)
{
	Rect er;
	Rect screenRect;
	
	if(active)
	{
		if(isJointed && diesWithJoint && !G_enemiesArray.data[jointedTo].isActive())
			damage(p, !polarity, HP, bArray);
		else
		{
			// handle bullets and pattern first
			switch(callback)
			{
				Fixed angle;
				// Too many patterns, needs dedicated file
				#include "patterns.h"
			}
			
			// Bring everything back to screen coordinates for collisions
			// Normal enemies have coordinates relative to the starting point of the wave
			// Props have absolute coordinates (relative to the starting point of the level)
			
			er.x = fixtoi(getx());
			er.y = fixtoi(gety());
			screenRect.x = fToScreenX(getx(), getCamRel());
			screenRect.y = fToScreenY(gety(), getCamRel());
			
			// Have a relatively big threshold for off-screen movement
			if(fixtoi(screenRect.x) + img[0] / 2 < -80 || fixtoi(screenRect.x) - img[0] / 2 > 399 ||
				fixtoi(screenRect.y) + img[1] / 2 < -80 || fixtoi(screenRect.y) - img[1] / 2 > 319)
				deactivate();
			else
			{
				// then the enemy image
				if(hasRotation)
				{
					//~ drawSpriteRotated(img, &er, NULL, rotationAngle);
					DC->add(img, &er, NULL, rotationAngle, getCamRel());
				}
				else
				{
					//~ drawSprite(img, er.x, er.y);
					DC->add(img, &er, getCamRel());
				}
			}
			if(!ghost)
			{
				// Check wether the player hit the enemy
				if(p->isHurtable() && (p->getx() >= screenRect.x - (itofix(img[0]) / 2) && p->getx() < screenRect.x + (itofix(img[0]) / 2)) && (p->gety() >= screenRect.y - (itofix(img[0]) / 2) && p->gety() <= screenRect.y + (itofix(img[1]) / 2)))
				{
					p->hurt();
				}
			}
		}
	}
	else
	{
		diedThisFrame = false;
		isJointed = false;
	}
}

void Enemy::activate(int _x, int _y, int _HP, int shipImgID, int callbackID, int _waveIndex, bool _polarity, bool _hasRotation, int _f, bool _ghost, bool _prop)
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
	ghost = _ghost;
	prop = _prop;
	active = true;
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
				Fixed angle = angleToEntity(this, _p);
				int famount = _pol != polarity ? fireback / 2 : fireback;
				for(int i = 0; i < famount; i++)
					bArray->add(getx(), gety(), fixcos(angle + (rand() % 16) - 8) << 1, fixsin(angle + (rand() % 16) - 8) + (rand() % 256), image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
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

bool Enemy::isGhost()
{
	return ghost;
}

Fixed Enemy::getx()
{
	return isJointed ? x + G_enemiesArray.data[jointedTo].getx() + jointX : x;
}

Fixed Enemy::gety()
{
	return isJointed ? y + G_enemiesArray.data[jointedTo].gety() + jointY : y;
}

int Enemy::getCamRel()
{
	return prop ? CAMREL_ABSOLUTE : CAMREL_RELATIVE;
}