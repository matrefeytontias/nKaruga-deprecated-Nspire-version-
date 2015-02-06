#include "common.h"

DrawingCandidates::DrawingCandidates()
{
	candidatesCount = 0;
}

DrawingCandidates::~DrawingCandidates()
{
	
}

void DrawingCandidates::add(unsigned short *img, Rect *pos)
{
	data[candidatesCount].activate(img, pos);
	candidatesCount++;
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, Rect *center, Fixed angle)
{
	data[candidatesCount].activate(img, pos, center, angle);
	candidatesCount++;
}

void DrawingCandidates::flush()
{
	if(!G_skipFrame)
	{
		for(int i = 0; i < candidatesCount; i++)
		{
			data[i].draw();
			data[i].deactivate();
		}
	}
	else
	{
		for(int i = 0; i < candidatesCount - 1; i++)
			data[i].deactivate();
	}
	
	candidatesCount = 0;
}
