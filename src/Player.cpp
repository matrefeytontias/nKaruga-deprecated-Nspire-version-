#include "common.h"

Player::Player()
{
	polarity = true;
	fireRepeat = false;
	polarityRepeat = false;
	isSwitchingPolarity = 0;
	fireDelay = 32;
	lives = 4;
	img[LIGHT] = image_entries[image_LUT_player_ship_light];
	img[SHADOW] = image_entries[image_LUT_player_ship_shadow];
	img[LIGHT + SWITCHING0] = image_entries[image_LUT_player_ship_polarityswitch_0_light];
	img[SHADOW + SWITCHING0] = image_entries[image_LUT_player_ship_polarityswitch_0_shadow];
	img[LIGHT + SWITCHING1] = image_entries[image_LUT_player_ship_polarityswitch_1_light];
	img[SHADOW + SWITCHING1] = image_entries[image_LUT_player_ship_polarityswitch_1_shadow];
	
	x = 0;
	y = 0;
}

Player::~Player()
{
}

void Player::handle(KeyEvent kEv, BulletArray *bArray)
{
	static Rect r, temp;
	
	// Display the player
	r.x = fixtoi(x) - (img[(isSwitchingPolarity / 8) * 2][0] / 2);
	r.y = fixtoi(y) - (img[(isSwitchingPolarity / 8) * 2][1] / 2);
	
	if(!G_skipFrame) drawSprite(img[((isSwitchingPolarity / 8) * 2) + (polarity ? SHADOW : LIGHT)], r.x, r.y);
	
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
	
	if(KDOWN(kEv)) y += itofix(1);
	if(KLEFT(kEv)) x -= itofix(1);
	if(KRIGHT(kEv)) x += itofix(1);
	if(KUP(kEv)) y -= itofix(1);
	
	r.x = fixtoi(x) - (img[(isSwitchingPolarity / 8) * 2][0] / 2);
	r.y = fixtoi(y) - (img[(isSwitchingPolarity / 8) * 2][1] / 2);
	
	temp.x = img[(isSwitchingPolarity / 8) * 2][0] / 2;
	temp.y = img[(isSwitchingPolarity / 8) * 2][1] / 2;
	
	x = r.x < 0 ? itofix(temp.x) : (r.x > 320 - (temp.x * 2) ? itofix(320 - temp.x) : x);
	y = r.y < 0 ? itofix(temp.y) : (r.y > 240 - (temp.y * 2) ? itofix(240 - temp.y) : y);
	
	if(KTAB(kEv))
	{
		if(!polarityRepeat)
			switchPolarity();
		polarityRepeat = true;
	}
	else
		polarityRepeat = false;
	
	if(fireDelay == 0)
	{
		if(KESC(kEv))
		{
			if(fireRepeat)
			{
				// fire 2 bullets if the key is being held
				bArray->add(x - itofix(img[0][0]) / 3, y, 0, itofix(-3), image_LUT_player_bullet_light, polarity, false);
				bArray->add(x + itofix(img[0][0]) / 3, y, 0, itofix(-3), image_LUT_player_bullet_light, polarity, false);
				fireDelay = 16;
			}
			else
			{
				// fire 1 bullet
				bArray->add(x, y, 0, itofix(-3), image_LUT_player_bullet_light, polarity, false);
				fireDelay = 24;
			}
			fireRepeat = true;
		}
		else
			fireRepeat = false;
	}
	else
		fireDelay--;
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
}

int Player::getLives()
{
	return lives;
}
