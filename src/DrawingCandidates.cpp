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
	// printf("Drawing candidate 0 : 0x%8X\n", (int)data[candidatesCount]);
	// data = (DrawingCandidate**)realloc(data, (candidatesCount + 1) * sizeof(DrawingCandidate*));
	// if(!data)
	// {
		// printf("Allocation failed\n");
		// exit(1);
	// }
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, Rect *center, Fixed angle)
{
	data[candidatesCount].activate(img, pos, center, angle);
	candidatesCount++;
	// data = (DrawingCandidate**)realloc(data, (candidatesCount + 1) * sizeof(DrawingCandidate*));
	// if(!data)
	// {
		// printf("Allocation failed\n");
		// exit(1);
	// }
}

void DrawingCandidates::flush()
{
	if(!G_skipFrame)
	{
		//~ printf("Drawing %d objects.\n", candidatesCount);
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
	
	// free(data);
	// data = (DrawingCandidate**)malloc(sizeof(DrawingCandidate*));
	candidatesCount = 0;
}
