#include "base.h"
Base::Base(int x, int y, int width, int height, int dx, int dy, ACL_Image* img,roll r1)

{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->distx = dx;
	this->disty = dy;
	this->img = img;
	this->r = r1;
}
void Base::drawSprite()
{
	putImageScale(img, x, y, width, height);
}
int Base::getY() { return y; }