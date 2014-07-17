#include "common.h"

ChainNotif::ChainNotif()
{
	untilDeath = 0;
}

ChainNotif::~ChainNotif()
{
	
}

void ChainNotif::activate(int _x, int _y, int _v)
{
	x = _x;
	backupX = x;
	y = _y;
	value = _v;
	untilDeath = 16;
	maxChain = value == 25600;
}

void ChainNotif::handle()
{
	if(untilDeath)
	{
		if(maxChain)
		{
			y -= 8;
			drawString(&x, &y, x, "max chain\n", 0xffff);
		}
		drawDecimal(&x, &y, value, 0xfbe0);
		x = backupX;
		untilDeath--;
	}
}