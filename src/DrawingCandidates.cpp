#include "common.h"

DrawingCandidates::DrawingCandidates()
{
	
}

DrawingCandidates::~DrawingCandidates()
{
	
}

void DrawingCandidates::init()
{
	data = (DrawingCandidate**)malloc(sizeof(DrawingCandidate*));
	candidatesCount = 0;
}

void DrawingCandidates::add(unsigned short *img, Rect *pos)
{
	data[candidatesCount] = new DrawingCandidate(img, pos);
	candidatesCount++;
	data = (DrawingCandidate**)realloc(data, (candidatesCount + 1) * sizeof(DrawingCandidate*));
	if(!data)
	{
		printf("Allocation failed\n");
		exit(1);
	}
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, Rect *center, Fixed angle)
{
	data[candidatesCount] = new DrawingCandidate(img, pos, center, angle);
	candidatesCount++;
	data = (DrawingCandidate**)realloc(data, (candidatesCount + 1) * sizeof(DrawingCandidate*));
	if(!data)
	{
		printf("Allocation failed\n");
		exit(1);
	}
}

void DrawingCandidates::flush()
{
	if(!G_skipFrame)
	{
		//~ printf("Drawing %d objects.\n", candidatesCount);
		for(int i = 0; i < candidatesCount; i++)
		{
			data[i]->draw();
			delete data[i];
		}
	}
	else
	{
		for(int i = 0; i < candidatesCount - 1; i++)
			delete data[i];
	}
	
	free(data);
	init();
}

void DrawingCandidates::release()
{
	free(data);
}