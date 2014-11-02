#include "common.h"

DrawingCandidate::DrawingCandidate(unsigned short *_img, Rect *_pos)
{
	img = _img;
	pos.x = _pos->x - img[0] / 2;
	pos.y = _pos->y - img[1] / 2;
	rotates = false;
}

DrawingCandidate::DrawingCandidate(unsigned short *_img, Rect *_pos, Rect *_center, Fixed _angle)
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
	rotates = true;
}

DrawingCandidate::~DrawingCandidate()
{
	
}

void DrawingCandidate::draw()
{
	if(rotates)
		drawSpriteRotated(img, &pos, centered ? NULL : &center, angle);
	else
		drawSprite(img, pos.x, pos.y);
}
