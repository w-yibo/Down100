#include "object.h"
object::object(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id)
	:Base( x,  y,  width,  height, dx,  dy, img,  r1)
{
	this->id = id;
}
void object::setdy(int dy)
{
	disty = dy;
}
void object::changeX(int x)
{
	this->x = x;
}
int object::collision(roll r2)//返回值为0，没有碰撞；返回值为1、2、3、4分别对应四种碰撞情况，人在左下，左上，右下，右上

{

	roll r1 = { x,y, width, height };

	int c = 0;
	if (r1.x < r2.x && r1.x + r1.w >r2.x)
	{
		if (r1.y > r2.y && r1.y < r2.y + r2.h)return 1;
		if (r1.y <r2.y && r1.y + r1.h >r2.y)return 2;
	}
    else
	
		if (r1.x > r2.x && r2.x + r2.w > r1.x)
		{
			if (r1.y > r2.y && r1.y < r2.y + r2.h)return 3;
			if (r1.y <r2.y && r1.y + r1.h >r2.y)return 4;
		}
	
	else return c;

}
void object::move(roll r1)
{
	y -= disty;
}
void object::addDisty()
{
	disty++;
}
int object::getID() { return id; }
roll object::getRoll() 
{
	roll r;
	r.x = x;
	r.y = y;
	r.w = width;
	r.h = height;
	return r;
}

ci::ci(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id)
	:object(x, y, width, height, dx, dy, img, r1, id)
{
}
void ci::function(person *obj)
{
	obj->deHP();
	obj->changeY(disty);
}

flip::flip(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1,int j, int id)
	:object(x, y, width, height, dx, dy, img, r1, id)
{
	jump = j;
}
void flip::function(person* obj)
{
	obj->addHP();
	obj->jump(jump);
}

soft::soft(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id)
	:object(x, y, width, height, dx, dy, img, r1, id)
{
}
void soft::function(person* obj)
{
	obj->addHP();
	obj->fall();
}

tough::tough(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id)
	:object(x, y, width, height, dx, dy, img, r1,id)
{
}
void tough::function(person* obj)
{
	obj->addHP();
	obj->changeY(disty);
}