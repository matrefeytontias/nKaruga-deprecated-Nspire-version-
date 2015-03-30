#include "common.h"

Particles::Particles()
{
	for(int i = 0; i < MAX_PARTICLE; i++)
		time[i] = 0;
	counter = 0;
}

Particles::~Particles()
{
	
}

void Particles::add(Fixed _x, Fixed _y, Fixed _a, bool _p, int lifetime)
{
	int ratio = rand() % 256;
	x[counter] = _x;
	y[counter] = _y;
	dx[counter] = fixmul(fixcos(_a), ratio);
	dy[counter] = fixmul(fixsin(_a), ratio);
	polarity[counter] = _p;
	time[counter] = lifetime;
	dt[counter] = lifetime / PARTICLE_RADIUS;
	counter = (counter + 1) % MAX_PARTICLE;
}

void Particles::handle()
{
	for(int i = 0; i < MAX_PARTICLE; i++)
	{
		if(time[i])
		{
			time[i]--;
			x[i] += dx[i];
			y[i] += dy[i];
			
			// Rect r = { fixtoi(x[i]), fixtoi(y[i]), 0, 0 };
			// DC->add(image_entries[polarity[i] ? image_LUT_particle_shadow : image_LUT_particle_light], &r, CAMREL_NONE);
			fillCircle(fixtoi(x[i]), fixtoi(y[i]), time[i] / dt[i], polarity[i] ? 0 : 0xffff);
			
		}
	}
}