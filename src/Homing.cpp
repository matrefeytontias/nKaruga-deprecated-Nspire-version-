#include "common.h"

Homing::Homing()
{
	active = false;
}

Homing::~Homing()
{
	
}

bool Homing::isActive()
{
	return active;
}

void Homing::activate(Fixed _x, Fixed _y, Fixed initialAngle, Player *_target, bool _p)
{
	x = _x;
	y = _y;
	angle = initialAngle;
	target = _target;
	polarity = _p;
	
	aimTimer = 128;
	rotationAngle = 0;
	active = true;
	for(int i = 0; i < HOMING_TRAILING; i++)
	{
		previousX[i] = x;
		previousY[i] = y;
	}
}

void Homing::deactivate()
{
	active = false;
}

bool Homing::getPolarity()
{
	return polarity;
}

#define HOMING_SPEED itofix(1)

bool Homing::handle()
{
	for(int i = FRAGMENT_TRAILING - 1; i > 0; i--)
	{
		previousX[i] = previousX[i - 1];
		previousY[i] = previousY[i - 1];
	}
	previousX[0] = x;
	previousY[0] = y;
	
	x += fixmul(fixcos(angle), HOMING_SPEED);
	y += fixmul(fixsin(angle), HOMING_SPEED);
	
	if(aimTimer)
	{
		if(((angle - angleToPlayer(this, target)) & 0xff) > 128)
			angle++;
		else
			angle--;
		angle &= 0xff;
		aimTimer--;
	}
	
	// As for bullets, homings have a threshold of 30
	return (x + itofix(8) < itofix(-30) || x - itofix(8) > itofix(349) || y + itofix(8) < itofix(-30) || y - itofix(8) > itofix(279));
}

void Homing::draw()
{
	static Rect r;
	
	r.x = fixtoi(x);
	r.y = fixtoi(y);
	
	drawSprite(image_entries[polarity ? image_LUT_enemy_homing_bullet_shadow : image_LUT_enemy_homing_bullet_light], r.x, r.y);
	
	rotationAngle++;
	
	for(int i = 0; i < FRAGMENT_TRAILING; i++)
	{
		r.x = fixtoi(previousX[i]);
		r.y = fixtoi(previousY[i]);
		fillCircle(r.x, r.y, 8 - i / 2, polarity ? 0xfb0c : 0x65bf);
	}
}