#include "common.h"

Bullet::Bullet()
{
	active = false;
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

void Bullet::activate(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, int imgID, bool _p, bool _h)
{
	active = true;
	polarity = _p;
	hurtPlayer = _h;
	x = _x;
	y = _y;
	dx = _dx;
	dy = _dy;
	img = image_entries[imgID + (_p ? 1 : 0)];
}

void Bullet::deactivate()
{
	active = false;
}

bool Bullet::getPolarity()
{
	return polarity;
}

bool Bullet::hurtsPlayer()
{
	return hurtPlayer;
}

bool Bullet::handle()
{
	x += dx;
	y += dy;
				
	// Bullets have a threshold of 30
	return x + itofix(img[0] / 2) < itofix(-30) || x - itofix(img[0] / 2) > itofix(349) || y + itofix(img[1] / 2) < itofix(-30) || y - itofix(img[1] / 2) > itofix(279);
}

void Bullet::draw()
{
	static Rect br;
	br.x = fixtoi(x);
	br.y = fixtoi(y);
	//~ drawSprite(img, br.x, br.y);
	DC->add(img, &br);
}
