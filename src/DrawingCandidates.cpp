#include "common.h"

//
// Camera travelling handlers
//
void cthIntro1(Camera *cam)
{
	// None needed in level 1
	UNUSED(cam);
	cam->absX = cam->relX = 100;
}

void cthChap1(Camera *cam)
{
	UNUSED(cam);
	cam->absX = cam->absY = cam->relX = cam->relY = 0;
}

void cthIntro2(Camera *cam)
{
	UNUSED(cam);
}

cameraTravelling camTrav[] = { cthIntro1, cthChap1, cthIntro2 };

// 
// DrawingCandidates class
// 

DrawingCandidates::DrawingCandidates()
{
	candidatesCount = 0;
	cam.absX = cam.absY = cam.relX = cam.relY = 0;
	cameraPath = camTrav[0];
}

DrawingCandidates::~DrawingCandidates()
{
	
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, int camRel)
{
	data[candidatesCount].activate(img, pos, camRel);
	candidatesCount++;
}

void DrawingCandidates::add(unsigned short *img, Rect *pos, Rect *center, Fixed angle, int camRel)
{
	data[candidatesCount].activate(img, pos, center, angle, camRel);
	candidatesCount++;
}

void DrawingCandidates::flush()
{
	if(!(G_skipFrame % 4))
	{
		for(int i = 0; i < candidatesCount; i++)
		{
			data[i].draw(&cam);
			data[i].deactivate();
		}
	}
	else
	{
		for(int i = 0; i < candidatesCount - 1; i++)
			data[i].deactivate();
	}
	
	candidatesCount = 0;
	(cameraPath)(&cam);
}

void DrawingCandidates::loadCameraPath(int id)
{
	cameraPath = camTrav[id];
}