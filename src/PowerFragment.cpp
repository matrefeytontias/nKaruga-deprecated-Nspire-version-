#include "common.h"

PowerFragment::PowerFragment() : Bullet()
{
	active = false;
	hasReachedAngle = false;
}

PowerFragment::~PowerFragment()
{
	
}

void PowerFragment::activate(Fixed _x, Fixed _y, Fixed initialAngle, Player *_targetP, bool _p, bool _h)
{
	x = _x;
	y = _y;
	angle = initialAngle;
	targetP = _targetP;
	polarity = _p;
	hasReachedAngle = false;
	hurtPlayer = _h;
	if(!hurtPlayer)
	{
		targetE = findNearestEnemy(x, y);
		if(G_fightingBoss)
			targetB = G_bossEnemy;
	}
	
	active = true;
	speed = itofix(1);
	skipPositionRecord = 0;
	for(int i = 0; i < FRAGMENT_TRAILING; i++)
	{
		previousX[i] = x;
		previousY[i] = y;
	}
}

bool PowerFragment::handle()
{
	if(!(skipPositionRecord % 4))
	{
		for(int i = FRAGMENT_TRAILING - 1; i > 0; i--)
		{
			previousX[i] = previousX[i - 1];
			previousY[i] = previousY[i - 1];
		}
		previousX[0] = x;
		previousY[0] = y;
	}
	skipPositionRecord++;
	
	x += fixmul(fixcos(angle), speed);
	y += fixmul(fixsin(angle), speed);
	
	// If it has not been done already, go to angle = 192 if targetting enemies or angle = 64 if targetting player
	if(!hasReachedAngle)
	{
		if(hurtPlayer)
		{
			if(((angle - 64) & 0xff) > 128)
				angle++;
			else
				angle--;
			angle &= 0xff;
			if(angle == 64)
				hasReachedAngle = true;
		}
		else
		{
			if(((angle - 192) & 0xff) > 128)
				angle++;
			else
				angle--;
			angle &= 0xff;
			if(angle == 192)
				hasReachedAngle = true;
		}
	}
	else
	{
		speed += (speed < itofix(6)) * 32;
		if(hurtPlayer)
		{
			if(targetP->isHurtable())
				angle = angleToEntity(this, targetP);
		}
		else
		{
			targetE = findNearestEnemy(x, y);
			if(targetE)
			{
				if(!G_fightingBoss)
				{
					angle = angleToEntity(this, targetE);
				}
				else if(targetB->getDistance(this) < distance(fixtoi(x), fixtoi(y), fixtoi(targetE->getx()), fixtoi(targetE->gety())))
				{
					angle = targetB->getAngle(this);
				}
				else
				{
					angle = angleToEntity(this, targetE);
				}
			}
			else if(G_fightingBoss)
			{
				angle = angleToEntity(this, targetB);
			}
		}
	}
	
	// As for bullets, power fragments have a threshold of 30
	return (x + itofix(4) < itofix(-30) || x > itofix(349) || y + itofix(4) < itofix(-30) || y > itofix(279));
}

void PowerFragment::draw()
{
	static Rect r;
	
	r.x = fixtoi(x);
	r.y = fixtoi(y);
	
	//~ drawSprite(image_entries[polarity ? image_LUT_player_homing_bullet_shadow_0 : image_LUT_player_homing_bullet_light_0], r.x, r.y);
	DC->add(image_entries[polarity ? image_LUT_player_homing_bullet_shadow_0 : image_LUT_player_homing_bullet_light_0], &r, CAMREL_NONE);
	
	for(int i = 0; i < FRAGMENT_TRAILING; i++)
	{
		r.x = fixtoi(previousX[i]);
		r.y = fixtoi(previousY[i]);
		//~ drawSprite(image_entries[(polarity ? image_LUT_player_homing_bullet_shadow_1 : image_LUT_player_homing_bullet_light_1) + i / ((FRAGMENT_TRAILING + 1) / 2)], r.x, r.y);
		DC->add(image_entries[(polarity ? image_LUT_player_homing_bullet_shadow_1 : image_LUT_player_homing_bullet_light_1) + i / ((FRAGMENT_TRAILING + 1) / 2)], &r, CAMREL_NONE);
	}
}