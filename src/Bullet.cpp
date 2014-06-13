#include "common.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

Rect* Bullet::makeRect()
{
	static Rect r;
	r.x = fixtoi(x);
	r.y = fixtoi(y);
	return &r;
}

bool Bullet::isActive()
{
	return active;
}

void Bullet::activate(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, bool _p)
{
	active = true;
	polarity = _p;
	x = _x;
	y = _y;
	dx = _dx;
	dy = _dy;
}

void Bullet::deactivate()
{
	active = false;
}

bool Bullet::getPolarity()
{
	return polarity;
}
