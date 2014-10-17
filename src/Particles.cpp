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

void Particles::add(Fixed _x, Fixed _y, Fixed _dx, Fixed _dy, bool _p)
{
	x[counter] = _x;
	y[counter] = _y;
	dx[counter] = _dx;
	dy[counter] = _dy;
	polarity[counter] = _p;
	time[counter] = (rand() % 64) + 32;
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
			
			Rect r = { fixtoi(x[i]), fixtoi(y[i]), 0, 0 };
			//~ drawSprite(image_entries[polarity[i] ? image_LUT_particle_shadow : image_LUT_particle_light], fixtoi(x[i]), fixtoi(y[i]));
			DC->add(image_entries[polarity[i] ? image_LUT_particle_shadow : image_LUT_particle_light], &r);
		}
	}
}