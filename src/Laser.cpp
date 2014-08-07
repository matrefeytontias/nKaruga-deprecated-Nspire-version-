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
	int x1 = x;
	int y1 = y;
	int x2 = fixtoi(fixcos(angle) * amplitude) + x1;
	int y2 = fixtoi(fixsin(angle) * amplitude) + y1;
	int dx = abs(x2-x1);
	int dy = abs(y2-y1);
	int sx = (x1 < x2)?1:-1;
	int sy = (y1 < y2)?1:-1;
	int err = dx-dy;
	int e2;
	// Sprite drawing vars
	Fixed sdx = fixsin(-angle);
	Fixed sdy = fixcos(-angle);
	unsigned short *laserSlice = image_entries[polarity ? image_LUT_enemy_laser_shadow : image_LUT_enemy_laser_light] + 3; // skip header
	
	while (!(x1 == x2 && y1 == y2))
	{
		// Draws a rotated laser "slice"
		// Optimizes the sprite drawing since it's always the same 1*25 image
		Fixed sxp = itofix(x1) - sdx * 12;
		Fixed syp = itofix(y1) - sdy * 12;
		for(int i = 0; i < 25; i++)
		{
			setPixel(fixtoi(sxp), fixtoi(syp), laserSlice[i]);
			sxp += sdx;
			syp += sdy;
		}
		
		e2 = 2*err;
		if (e2 > -dy)
		{	
			err = err - dy;
			x1 = x1 + sx;
		}
		if (e2 < dx)
		{	
			err = err + dx;
			y1 = y1 + sy;
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
