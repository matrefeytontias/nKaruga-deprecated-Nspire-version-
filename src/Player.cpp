#include "common.h"

Player::Player() : Entity()
{
	img[LIGHT] = image_entries[image_LUT_player_ship_light];
	img[SHADOW] = image_entries[image_LUT_player_ship_shadow];
	img[LIGHT + SWITCHING0] = image_entries[image_LUT_player_ship_polarityswitch_0_light];
	img[SHADOW + SWITCHING0] = image_entries[image_LUT_player_ship_polarityswitch_0_shadow];
	img[LIGHT + SWITCHING1] = image_entries[image_LUT_player_ship_polarityswitch_1_light];
	img[SHADOW + SWITCHING1] = image_entries[image_LUT_player_ship_polarityswitch_1_shadow];
	deathCounter = 0;
	active = true;
	x = 0;
	y = 0;
	reset();
}

void Player::reset()
{
	polarity = LIGHT;
	fireRepeat = false;
	polarityRepeat = false;
	isSwitchingPolarity = 0;
	fireDelay = 32;
	lives = 4;
}

Player::~Player()
{
}

void Player::handle(KeyEvent kEv, BulletArray *bArray)
{
	static Rect r, temp;
	
	if(active)
	{
		// Display the player
		r.x = fixtoi(x);
		r.y = fixtoi(y);
		
		DC->add(img[((isSwitchingPolarity / 8) * 2) + polarity], &r, CAMREL_NONE);
		
		if(deathCounter)
		{
			DC->add(image_entries[image_LUT_player_ship_invincible_light + polarity], &r, CAMREL_NONE);
			deathCounter--;
		}
		
		if(isSwitchingPolarity)
		{
			isSwitchingPolarity++;
		}
		
		if((isSwitchingPolarity / 8) == 3)
		{
			polarity = !polarity;
			isSwitchingPolarity = 0;
		}
		
		// And then only, player input
		if(G_usingTouchpad)
		{
			if(G_tpstatus.contact)
			{
				x += clamp((G_tpstatus.x - G_tpinfo->width / 2) * 768 / G_tpinfo->width, -512, 512);
				y += clamp((G_tpinfo->height / 2 - G_tpstatus.y) * 768 / G_tpinfo->height, -512, 512);
			}
		}
		else
		{
			if(KDOWN(kEv)) y += itofix(1);
			if(KLEFT(kEv)) x -= itofix(1);
			if(KRIGHT(kEv)) x += itofix(1);
			if(KUP(kEv)) y -= itofix(1);
		}
			
		r.x = fixtoi(x) - (img[(isSwitchingPolarity / 8) * 2][0] / 2);
		r.y = fixtoi(y) - (img[(isSwitchingPolarity / 8) * 2][1] / 2);
			
		temp.x = img[(isSwitchingPolarity / 8) * 2][0] / 2;
		temp.y = img[(isSwitchingPolarity / 8) * 2][1] / 2;
			
		x = r.x < 0 ? itofix(temp.x) : (r.x > 320 - (temp.x * 2) ? itofix(320 - temp.x) : x);
		y = r.y < 0 ? itofix(temp.y) : (r.y > 240 - (temp.y * 2) ? itofix(240 - temp.y) : y);
		
		if(KPOLARITY(kEv))
		{
			if(!polarityRepeat)
				switchPolarity();
			polarityRepeat = true;
		}
		else
			polarityRepeat = false;
		
		if(KPOWER(kEv))
		{
			if(G_power > 9)
			{
				G_hasFiredOnce = true;
				for(int i = 0; i < G_power / 10; i++)
					bArray->add_fragment(x, y, ((i % 2) ? -33 + (i / 2) * 11 : 161 - (i / 2) * 11), this, polarity, false);
				G_power = 0;
			}
		}
		
		if(fireDelay == 0)
		{
			if(KFIRE(kEv))
			{
				G_hasFiredOnce = true;
				if(fireRepeat)
				{
					// fire 2 bullets if the key is being held
					bArray->add(x - itofix(img[0][0]) / 3, y, 0, itofix(-3), image_LUT_player_bullet_light, polarity, false, CAMREL_NONE);
					bArray->add(x + itofix(img[0][0]) / 3, y, 0, itofix(-3), image_LUT_player_bullet_light, polarity, false, CAMREL_NONE);
					fireDelay = 16;
				}
				else
				{
					// fire 1 bullet
					bArray->add(x, y, 0, itofix(-3), image_LUT_player_bullet_light, polarity, false, CAMREL_NONE);
					fireDelay = 24;
					fireRepeat = true;
				}
			}
			else
				fireRepeat = false;
		}
		else
			fireDelay--;
	}
	else
	{
		if(deathCounter < 12)
		{
			r.x = fixtoi(x);
			r.y = fixtoi(y);
			DC->add(image_entries[image_LUT_player_explosion_0 + deathCounter], &r, CAMREL_NONE);
			// Death animation
			// Uses frameskipping as a counter
			if(!(G_skipFrame % 8))
				deathCounter++;
		}
		else if(deathCounter == 12)
		{
			polarity = LIGHT;
			x = itofix(160);
			y = itofix(260);
			deathCounter++;
		}
		// wait some frames so the player can breath
		else if(deathCounter < 256) deathCounter++;
		// get the player back in the game !
		else if(y > itofix(180))
		{
			r.x = fixtoi(x);
			r.y = fixtoi(y);
			DC->add(image_entries[image_LUT_player_ship_light], &r, CAMREL_NONE);
			y -= itofix(1);
		}
		else active = true;
	}
}

bool Player::getPolarity()
{
	return polarity;
}

void Player::switchPolarity()
{
	isSwitchingPolarity = 2;
}

void Player::hurt()
{
	lives--;
	active = false;
	deathCounter = 0;
	G_chainStatus = 0;
	G_frameChainOffset = 0;
	G_inChainCount = 0;
	G_power = 0;
}

int Player::getLives()
{
	return lives;
}

void Player::setLives(int l)
{
	lives = l;
}

bool Player::isHurtable()
{
	return active && !deathCounter;
}