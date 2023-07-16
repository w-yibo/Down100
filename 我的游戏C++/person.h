#pragma once
#include "base.h"
class person :
	public Base
{
	int HP;
public:
	person(int x, int y, int width, int height, int dx, int dy, ACL_Image* img,roll r1,int h);
	void move(int key);
	void move(roll r1);
	int collision(roll r2);
	int getHP();
	void addHP();
	void deHP();
	void jump(int j);
	void fall();
	void changeY(int dy);
};

