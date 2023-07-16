#include "person.h"
person::person(int x, int y, int width, int height, int dx, int dy, ACL_Image* img,roll r1,int h)
	:Base(x, y, width, height, dx, dy, img,r1)
{
	HP = h;
}
void person::move(roll r1)

{
	y += disty;
	if (x < r.x)x = r.x;
	if (x > (r.x + r.w - width))x = (r.x + r.w - width);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

}
void person::move(int key)
{

	switch (key)
	{
	case VK_LEFT:
		x -= distx;
		if (x <r.x)x = r.x;
		break;
	case VK_RIGHT:
		x += distx;
		if (x > (r.w - width)) x = (r.w- width);
		break;
	}
}
int person::collision(roll r2)//返回值为0，没有碰撞；返回值为1、2、3、4分别对应四种碰撞情况，人在左下，左上，右下，右上

{

	roll r1 = { x,y, width, height };

	int c = 0;
	if (r1.x < r2.x && r1.x + r1.w >r2.x)
	{
		if (r1.y > r2.y && r1.y < r2.y + r2.h)return 1;
		if (r1.y <r2.y && r1.y + r1.h >r2.y)return 2;
	}
	else
	{
		if (r1.x > r2.x && r2.x + r2.w > r1.x)
		{
			if (r1.y > r2.y && r1.y < r2.y + r2.h)return 3;
			if (r1.y <r2.y && r1.y + r1.h >r2.y)return 4;
		}
	}


	return c;

}

int person::getHP()

{

	return HP ;

}
void person::addHP()

{
	if (HP == 100) return;
	HP++;

}
void person::deHP()

{
	if (HP == 0) return;
	HP--;

}
void person::jump(int j)
{
	y -= j;
}
void person::fall()
{
	y += disty;
}
void person::changeY(int dy)
{
	y -= dy;
}