#include "common.h"

Bullet::Bullet() : Entity()
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

void Bullet::activate(Fixed _x, Fixed _y, Fixed _a, Fixed _r, int imgID, bool _p, bool _h, int camRel)
{
	active = true;
	polarity = _p;
	hurtPlayer = _h;
	x = fToScreenX(_x, camRel);
	y = fToScreenY(_y, camRel);
	dx = fixmul(fixcos(_a), _r);
	dy = fixmul(fixsin(_a), _r);
	img = image_entries[imgID + (_p ? 1 : 0)];
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
	DC->add(img, &br, CAMREL_NONE);
}
