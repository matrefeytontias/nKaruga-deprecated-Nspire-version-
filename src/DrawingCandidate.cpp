#include "common.h"

DrawingCandidate::DrawingCandidate()
{
	
}

DrawingCandidate::~DrawingCandidate()
{
	
}

void DrawingCandidate::activate(unsigned short *_img, Rect *_pos, int _camRel)
{
	img = _img;
	pos.x = _pos->x - img[0] / 2;
	pos.y = _pos->y - img[1] / 2;
	camRel = _camRel;
	rotates = false;
	active = true;
}

void DrawingCandidate::activate(unsigned short *_img, Rect *_pos, Rect *_center, Fixed _angle, int _camRel)
{
	img = _img;
	angle = _angle;
	pos.x = _pos->x;
	pos.y = _pos->y;
	if(_center)
	{
		center.x = _center->x;
		center.y = _center->y;
		centered = false;
	}
	else
		centered = true;
	camRel = _camRel;
	rotates = true;
	active = true;
}

void DrawingCandidate::deactivate()
{
	active = false;
}

void DrawingCandidate::draw()
{
	if(active)
	{
		pos.x = iToScreenX(pos.x, camRel);
		pos.y = iToScreenY(pos.y, camRel);
		if(rotates)
		{
			if(centered)
			{
				center.x = iToScreenX(center.x, camRel);
				center.y = iToScreenY(center.y, camRel);
			}
			drawSpriteRotated(img, &pos, centered ? NULL : &center, angle, 0, 0);
		}
		else
			drawSprite(img, pos.x, pos.y, 0, 0);
	}
}
