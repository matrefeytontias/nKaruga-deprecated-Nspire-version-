// Part of void Enemy::handle(Player *p, BulletArray *bArray)

// #######
// Intro 1
// #######
case Pattern_1_1:
	if(!(internal[0] % 2))
	{
		internal[1] = internal[0] / 2;
		x = itofix(320) - fixmul(sq(internal[1]), 768);
		y = itofix(internal[1]);
	}
	internal[0]++;
	rotationAngle = internal[0] / 2;
	break;	
case Pattern_1_2:
	if(!(internal[0] % 2))
	{
		internal[1] = internal[0] / 2;
		x = fixmul(sq(internal[1]), 768);
		y = itofix(internal[1]);
	}
	internal[0]++;
	rotationAngle = internal[0] / 2;
	break;
case Pattern_1_3:
	if(!(internal[0] % 3))
	{
		internal[1] = internal[0] / 3;
		x = fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256) + itofix(320);
		y = itofix(internal[1]);
	}
	internal[0]++;
	rotationAngle = internal[0] / 2;
	break;
case Pattern_1_4:
	if(!(internal[0] % 3))
	{
		internal[1] = internal[0] / 3;
		x = -fixdiv(sq(internal[0] - 240) - itofix(240), waveIndex * 128 + 256);
		y += itofix(1);
	}
	internal[0]++;
	rotationAngle = internal[0] / 2;
	break;
case Pattern_1_5:
	if(!(G_waveTimer % 3))
	{
		if(G_waveTimer < 500)
			y += itofix(1);
		else
			x += (waveIndex % 6) > 2 ? 192 : -192;
	}
	break;
case Pattern_1_6:
	angle = angleToEntity(this, p);
	if(!(internal[0] % 25))
	{
		bArray->add(x, y, angle, itofix(1), image_LUT_enemy_bullet_1_light, polarity, true, getCamRel());
	}
	internal[0]++;
	rotationAngle = ~angle + 64;
	break;
// #########
// Chapter 1
// #########
case Pattern_1_7:
	if(!(G_waveTimer % 4))
	{
		y += abs(fixcos(internal[0])) * 3;
		internal[0]++;
		if(internal[0] < 100)
			x += x > itofix(160) ? -128 : 128;
		else
			x += x > itofix(160) ? itofix(1) : itofix(-1);
	}
	break;
case Pattern_1_8:
	if(!(G_waveTimer % 4))
	{
		angle = angleToEntity(this, p);
		if(internal[0] < 35)
			y += itofix(2);
		else
			rotationAngle = ~angle + 64;
		if(internal[0] > 65 && internal[0] < 100) 
		{
			if(!(G_waveTimer % 16))
				bArray->add(x, y, angle, itofix(1), image_LUT_enemy_bullet_1_light, polarity, true, getCamRel());
		}
		else if(internal[0] > 164)
		{
			x += x < itofix(160) ? itofix(-1) : itofix(1);
			y -= itofix(1);
		}
		internal[0]++;
	}
	break;
case Pattern_1_9:
	x += (waveIndex % 2) ? 192 : -192;
	y += 128;
	
	if(!(G_waveTimer % 8))
		bArray->add(x, y, 64, itofix(2), image_LUT_enemy_bullet_1_light, polarity, true, getCamRel());
	break;
case Pattern_1_10:
	if(!(G_waveTimer % 4))
	{
		int temp = fixsin(internal[0]) * 80;
		x = ((waveIndex % 2) ? -temp : temp) + itofix(160);
		if(abs(temp) > itofix(70))
		{
			if(!(G_waveTimer % 8))
				bArray->add(x, y, 64, itofix(2), image_LUT_enemy_bullet_1_light, polarity, true, getCamRel());
		}
		y += 192;
		internal[0]++;
	}
	break;
case Pattern_1_11:
	y += itofix(1);
	break;
case Pattern_1_12:
	if(!(G_waveTimer % 4))
	{
		if(internal[0] < 45)
			y += itofix(2);
		else if(internal[0] == 45)
		{
			internal[1] = (waveIndex % 2) ? 0 : 128;
			internal[2] = x + ((waveIndex % 2) ? itofix(-20) : itofix(20));
			internal[3] = y;
		}
		else if(internal[0] - 45 < 416)
		{
			x = fixcos(internal[1]) * 20 + internal[2];
			y = fixsin(internal[1]) * 20 + internal[3];
			internal[4] = (internal[0] - 45) % 64;
			if(internal[4] >= 32)
			{
				if(!(internal[1] % 256))
				{
					angle = angleToEntity(this, p);
					bArray->add(x, y, angle, itofix(1), image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
				}
				internal[1] += 4;
			}
		}
		else
			x += x < itofix(160) ? itofix(-2) : itofix(2);
		
		internal[0]++;
		rotationAngle = internal[0];
	}
	break;
case Pattern_1_13:
	rotationAngle = 16 + fixcos(G_waveTimer) / 32;
	if(!internal[1])
	{
		internal[1] = 1;
		internal[0] = -1;
	}
	if(y < itofix(30))
	{
		y += 128;
	}
	else if(internal[2] < 3)
	{
		if(internal[0] > 2 && internal[0] < 12)
		{
			if(!(G_waveTimer % 4))
			{
				for(int i = 0; i < 4; i++)
				{
					int k = i * 8 - 20 + 48;
					bArray->add(fixcos(k + (internal[0] - 3) * 8) * img[0] / 2 + x, fixsin(k + (internal[0] - 3) * 8) * img[1] / 2 + y,
								k, (internal[0] - 2) * 32 + 128, image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
				}
				internal[0]++;
			}
		}
		else if(internal[0] == 12)
		{
			internal[0] = 0;
			internal[2]++;
		}
		else
		{
			if(!(G_waveTimer % 128))
			{
				if(internal[0] < 2 && internal[0] >= 0)
				{
					for(int i = 0; i < 5; i++)
					{
						int k = (i - 2) * 16 + 48;
						for(int j = 0; j < 3; j++)
						{
							
							bArray->add(fixcos(k) * img[0] / 2 + x - fixsin(k) * (j - 1) * 6, fixsin(k) * img[1] / 2 + y + fixcos(k) * (j - 1) * 6,
										k, itofix(1), image_LUT_enemy_bullet_2_light, polarity, true, getCamRel());
						}
					}
				}
				internal[0]++;
			}
		}
	}
	else
		x -= 64;
	break;
case Pattern_1_14:
	rotationAngle = -16 + fixcos(G_waveTimer) / 32;
	if(y < itofix(30))
	{
		y += 128;
	}
	else if(internal[2] < 3)
	{
		if(internal[0] > 2 && internal[0] < 12)
		{
			if(!(G_waveTimer % 4))
			{
				for(int i = 0; i < 4; i++)
				{
					int k = i * 8 - 20 + 80;
					bArray->add(fixcos(k - (internal[0] - 3) * 8) * img[0] / 2 + x, fixsin(k - (internal[0] - 3) * 8) * img[1] / 2 + y,
								k, (internal[0] - 2) * 32 + 128, image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
				}
				internal[0]++;
			}
		}
		else if(internal[0] == 12)
		{
			internal[0] = 0;
			internal[2]++;
		}
		else
		{
			if(!(G_waveTimer % 128))
			{
				if(internal[0] < 2)
				{
					for(int i = 0; i < 5; i++)
					{
						int k = (i - 2) * 16 + 80;
						for(int j = 0; j < 3; j++)
						{
							
							bArray->add(fixcos(k) * img[0] / 2 + x - fixsin(k) * (j - 1) * 6, fixsin(k) * img[1] / 2 + y + fixcos(k) * (j - 1) * 6,
										k, itofix(1), image_LUT_enemy_bullet_2_light, polarity, true, getCamRel());
						}
					}
				}
				internal[0]++;
			}
		}
	}
	else
		x += 64;
	break;
case Pattern_1_15:
	x = fixcos(internal[0] + ((waveIndex % 2) ? 128 : 0)) * 30;
	y = fixsin(internal[0] + ((waveIndex % 2) ? 128 : 0)) * 30;
	if(!(G_waveTimer % 4))
	{
		if(!(internal[0] % 128))
		{
			angle = angleToEntity(this, p);
			bArray->add(getx(), gety(), angle, 128, image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
		}
		internal[0]++;
		rotationAngle = internal[0];
	}
	break;
case Pattern_1_16:
	if(y <= itofix(80))
	{
		if(!(G_waveTimer % 32) && y < itofix(60))
			bArray->add(x, y, 64, 384, image_LUT_enemy_bullet_1_light, polarity, true, getCamRel());
		y += 256 - fixtoi(y) * 3;
		x += polarity ? -64 : 64;
	}
	else
	{
		y += 64;
		x += polarity ? -128 : 128;
	}
	break;
case Pattern_1_17:
	if(!(G_waveTimer % 4))
	{
		// Y position is timed to be starting position + 96 due to the enemy needing to rotate by 32 degrees in a single movement
		// So it moves by 3 pixels and rotates by one degree at a time
		if(y < itofix(46) && !internal[0])
		{
			rotationAngle--;
			// 1.5
			y += 384;
		}
		else if(y == itofix(46) && !internal[0])
		{
			// FIRE DA L4Z000000000000000R
			bArray->fire_laser(this, polarity, 64);
			rotationAngle++;
			internal[0] = 1;
			internal[1] = rotationAngle * 2;
		}
		else if(rotationAngle < 32)
		{
			internal[1]++;
			rotationAngle = internal[1] / 2;
		}
		else if(rotationAngle == 32)
		{
			bArray->stop_laser(this);
			rotationAngle++;
		}
		else
		{
			rotationAngle++;
			y -= itofix(1);
		}
	}
	break;
case Pattern_1_18:
	if(!(G_waveTimer % 4))
	{
		x = fixcos(G_waveTimer / 2 + waveIndex * 21) * max(200 - internal[0], 80) + itofix(160);
		y = fixsin(G_waveTimer / 2 + waveIndex * 21) * max(200 - internal[0], 80) + itofix(120);
		
		if(G_waveTimer < 1024)
			internal[0] += (internal[0] < 120) * 2;
		else
			internal[0] -= 2;
		rotationAngle += 3;
	}
	break;
case Pattern_1_19:
	if(!internal[3])
	{
		internal[3] = 1;
		internal[0] = waveIndex ? 64 : -64;
		internal[1] = waveIndex ? -64 : 64;
		internal[2] = -1;
	}
	if(internal[2] < 5)
	{
		switch(internal[2] % 4)
		{
			case 1:
				y += internal[1];
				if(y <= itofix(img[1] / 2) || y >= itofix(240 - img[1] / 2 - 15))
					internal[2]++;
				break;
			case 2:
				x -= internal[0];
				if(x <= itofix(img[0] / 2 + 20) || x >= itofix(320 - img[0] / 2 - 20))
					internal[2]++;
				break;
			case 3:
				y -= internal[1];
				if(y <= itofix(img[1] / 2) || y >= itofix(240 - img[1] / 2 - 15))
					internal[2]++;
				break;
			default:
				x += internal[0];
				if(x == itofix(img[0] / 2 + 20) || x == itofix(320 - img[0] / 2 - 20))
					internal[2]++;
		}
	}
	else
		y += internal[1];
	break;
case Pattern_1_20:
	if(!(G_waveTimer % 32))
		bArray->add(getx(), gety(), (waveIndex + 1 % 2) * 128, itofix(1), image_LUT_enemy_bullet_2_light, polarity, true, getCamRel());
	break;
case Pattern_1_21:
	if(!internal[0] && waveIndex % 2)
		internal[0] = 128;
	y += 48;
	x = fixcos(internal[0]) * 14 + itofix(160);
	internal[0]++;
	break;
case Pattern_1_boss:
	if(y < itofix(20))
		y += 128;
	else
	{
		rotationAngle++;
		if(!(G_waveTimer % 96))
		{
			angle = angleToEntity(this, p);
			bArray->add(getx(), gety(), angle, itofix(1), image_LUT_enemy_bullet_1_light, polarity, true, getCamRel());
		}
	}
	break;
case Pattern_1_bossGrenade:
	// internal[0] is timer before explosion
	// internal[1] and [2] are the dx/dy to position
	// internal[3] is the time to wait before explosion
	if(!internal[0])
	{
		internal[3] = 256 + (rand() % 128);
		internal[2] = (itofix(120) - y) * 2 / internal[3];
		switch(waveIndex % 4)
		{
			case 0:
				internal[1] = itofix(20) - x;
				break;
			case 1:
				internal[1] = itofix(110) - x;
				break;
			case 2:
				internal[1] = itofix(210) - x;
				break;
			default:
				internal[1] = itofix(300) - x;
		}
		internal[1] /= internal[3] / 2;
	}
	if(y < itofix(120))
	{
		x += fixmul(internal[1], internal[3] - internal[0]);
		y += fixmul(internal[2], internal[3] - internal[0]);
	}
	if(internal[0] >= internal[3])
	{
		for(int i = 0; i < 32; i++)
			bArray->add(getx(), gety(), i * 8, itofix(1), image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
		damage(p, !polarity, HP, bArray);
	}
	internal[0]++;
	rotationAngle++;
	break;
// #######
// Intro 2
// #######
case Pattern_2_1:
	if(!internal[0])
	{
		if(fixtoi(x) != 160)
		{
			internal[3] = (fixtoi(x) < 160) * 2 - 1;
			x += itofix((x < itofix(160)) * 2 - 1);
			if(abs(fixtoi(x) - 160) == 80)
			{
				Fixed angle = angleToEntity(this, p);
				bArray->add(getx(), gety(), angle, itofix(1), image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
			}
		}
		else
		{
			internal[0] = 1;
			internal[1] = 192;
			internal[2] = itofix(120) - y;
		}
	}
	else
	{
		x = fixmul(fixcos(internal[1]), internal[2]) + itofix(160);
		y = fixmul(fixsin(internal[1]), internal[2]) + itofix(120);
		if(G_waveTimer % 2) internal[1] += internal[3];
		internal[2] += 32;
	}
	rotationAngle += internal[3];
	break;
case Pattern_2_2:
	// x = 60 or 260
	if(!internal[0])
	{
		if(fixtoi(y) != 120)
		{
			internal[3] = fixtoi(x) > 160;
			y += itofix(1);
		}
		else
		{
			internal[0] = 1;
			internal[1] = internal[3] ? 0 : 128;
			internal[2] = abs(itofix(160) - x);
		}
		if(fixtoi(x) == fixtoi(y) || 320 - fixtoi(x) == fixtoi(y))
		{
			Fixed angle = angleToEntity(this, p);
			bArray->add(getx(), gety(), angle, itofix(1), image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
		}
	}
	else if((internal[1] & 0xff) != (internal[3] ? 224 : 160))
	{
		x = fixmul(fixcos(internal[1]), internal[2]) + itofix(160);
		y = fixmul(fixsin(internal[1]), internal[2]) + itofix(120);
		if(G_waveTimer % 2) internal[1] += internal[3] * 2 - 1;
		rotationAngle = -internal[1] + !internal[3] * 128;
	}
	else
	{
		y += itofix(1);
		x += itofix(internal[3] * 2 - 1);
	}
	break;
case Pattern_2_3:
	// y = 60
	if(!internal[0])
	{
		if(fixtoi(x) != 160)
		{
			internal[3] = fixtoi(x) < 160;
			x += itofix(internal[3] * 2 - 1);
			rotationAngle = 64 * (internal[3] * 2 - 1);
		}
		else
		{
			internal[0] = 1;
			internal[1] = 192 + internal[3] * 2 - 1;
			internal[2] = abs(itofix(120) - y);
		}
		if(fixtoi(x) == fixtoi(y) || 320 - fixtoi(x) == fixtoi(y))
		{
			Fixed angle = angleToEntity(this, p);
			bArray->add(getx(), gety(), angle, itofix(1), image_LUT_enemy_bullet_0_light, polarity, true, getCamRel());
		}
	}
	else
	{
		x = fixmul(fixcos(internal[1]), internal[2]) + itofix(160);
		y = fixmul(fixsin(internal[1]), internal[2]) + itofix(120);
		if(G_waveTimer % 2) internal[1] += internal[3] * 2 - 1;
		rotationAngle = -internal[1] + !internal[3] * 128;
		if((internal[1] & 0xff) == 192) internal[4] = 1;
		internal[2] += internal[4] * 128;
	}
	break;
case Pattern_2_4:
	if(x < itofix(120))
	{
		if(x == itofix(60))
			bArray->fire_laser(this, polarity, 64);
		x += 128;
	}
	else if(x > itofix(200))
	{
		if(x == itofix(260))
			bArray->fire_laser(this, polarity, 64);
		x -= 128;
	}
	else if(fixtoi(x) == 120 || fixtoi(x) == 200)
	{
		if(internal[1] < 1024)
			internal[1]++;
		else if(internal[1] == 1024)
		{
			bArray->stop_laser(this);
			internal[1]++;
		}
		else
			y -= 128;
	}
	break;
case Pattern_2_5:
	if(x < itofix(160))
	{
		if(!internal[0] && x < itofix(80 - (waveIndex % 2) * 20))
			x += 128;
		else
		{
			internal[0]++;
			if(internal[0] > 768)
				x -= 128;
		}
	}
	else
	{
		if(!internal[0] && x > itofix(240 + (waveIndex % 2) * 20))
			x -= 128;
		else
		{
			internal[0]++;
			if(internal[0] > 768)
				x += 128;
		}
	}
	rotationAngle += (waveIndex % 2) * 2 - 1;
	break;
case Pattern_2_6:
	// Those are the 14th, 15th, 16th and 17th enemies of the wave (starting at 0)
	if(!internal[0])
		internal[0] = waveIndex - 13; // avoid having 0
	if(!internal[1] && y < itofix(135 - (internal[0] - 1) * 30))
		y += 128;
	else
	{
		internal[1]++;
		if(internal[1] > 768)
			x += itofix(1) * (((internal[0] - 1) % 2) * 2 - 1);
	}
	rotationAngle++;
	break;
case Pattern_2_7:
	// This starts at waveIndex = 18 -> odd parity
	// Odd parity is left, even parity is right
	if(y < itofix(135))
	{
		y += itofix(1);
	}
	else
	{
		if(y == itofix(135))
			internal[1] = 128;
		if(waveIndex % 2)
			x -= internal[1];
		else
			x += internal[1];
		internal[1] += 8;
		y += 128;
	}
	break;
// #########
// Chapter 2
// #########