#pragma once
#include"acllib.h"
typedef struct roll//�����ж��Ƿ���ײ������
{
	int x, y;
	int w, h;
}roll;
class Base

{
protected:
	int x, y;
	int width, height;
	int distx, disty;
	ACL_Image* img;
	roll r;
public:

	 Base(int x, int y, int width, int height, int dx, int dy, ACL_Image* img,roll r1);

	virtual void move(roll r1) = 0;

	void drawSprite();
	int getY();
};
