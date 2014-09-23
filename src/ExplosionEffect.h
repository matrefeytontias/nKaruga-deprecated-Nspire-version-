#ifndef INC_C
#define INC_C

#include "common.h"

class ExpParticle
{
public:
	ExpParticle();
	~ExpParticle();
	void activate(Fixed x, Fixed y, Fixed dx, Fixed dy, Fixed gravity);
	void deactivate();
	bool isActive();
	void update();
private:
	Fixed x, y, dx, dy, gravity;
	unsigned char cIndex;
	bool active;
};

extern void initExplosionEngine();
extern void initExplosionEffect(int x, int y, int coef, Fixed g);
extern bool renderExplosionEffect();
extern void deinitExplosionEngine();

#endif