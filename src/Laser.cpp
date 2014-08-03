#include "common.h"

Laser::Laser()
{
	active = false;
}

Laser::~Laser()
{
	
}

bool Laser::isActive()
{
	return active;
}

void Laser::activate(Enemy *e, bool _p)
{
	origin = e;
	active = true;
	polarity = _p;
	amplitude = 0;
}

void Laser::deactivate()
{
	active = false;
}

void Laser::handle()
{
	angle = ~origin->getRotation() + 64;
	x = fixtoi(origin->getx()) + fixtoi(fixcos(angle) * origin->img[0] / 2);
	y = fixtoi(origin->gety()) + fixtoi(fixsin(angle) * origin->img[1] / 2);
	amplitude += (amplitude < 320) * LASER_SPEED;
}

inline unsigned short r5g6b5(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void Laser::draw()
{
	int j = (rand() % 4) + 1;
	for(int i = 0; i < j; i++)
	{
		Fixed a = angle + (rand() % 64) - 48;
		G_particles->add(itofix(x), itofix(y), fixcos(a) / 2, fixsin(a) / 2, polarity);
	}
	
	// n2DLib's drawLine algorithm, but drawing rotated laser sprites instead of pixels
	// Algorithm by pierrotdu18
	Rect r = (Rect){x, y, 0, 0};
	int x2 = fixtoi(fixcos(angle) * amplitude) + x;
	int y2 = fixtoi(fixsin(angle) * amplitude) + y;
	int dx = abs(x2-r.x);
	int dy = abs(y2-r.y);
	int sx = (r.x < x2)?1:-1;
	int sy = (r.y < y2)?1:-1;
	int err = dx-dy;
	int e2;	
	
	while (!(r.x == x2 && r.y == y2))
	{
		drawSpriteRotated(image_entries[polarity ? image_LUT_enemy_laser_shadow : image_LUT_enemy_laser_light], &r, ~angle);
		e2 = err*2;
		if (e2 > -dy)
		{	
			err -= dy;
			r.x += sx;
		}
		if (e2 < dx)
		{	
			err += dx;
			r.y += sy;
		}
	}
	
}

bool Laser::getPolarity()
{
	return polarity;
}

Rect* Laser::getVector()
{
	static Rect r;
	// (x, y) is origin
	// (w, h) is length
	r.x = x;
	r.y = y;
	r.w = fixtoi(fixcos(angle) * amplitude);
	r.h = fixtoi(fixsin(angle) * amplitude);
	
	return &r;
}

void Laser::getSides(Rect* s1, Rect* s2)
{
	s1->x = x - fixtoi(fixsin(angle) * LASER_THICKNESS / 2);
	s1->y = y - fixtoi(fixcos(angle) * LASER_THICKNESS / 2);
	s2->x = x + fixtoi(fixsin(angle) * LASER_THICKNESS / 2);
	s2->y = y + fixtoi(fixcos(angle) * LASER_THICKNESS / 2);
}

int Laser::getAmplitude()
{
	return amplitude;
}

void Laser::setAmplitude(int _a)
{
	amplitude = _a;
}
