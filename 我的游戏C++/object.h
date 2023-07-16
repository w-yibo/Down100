#pragma once
#include "base.h"
#include"person.h"
class object :
	public Base
{
public:
	int id;
	object(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1,int id);
	void move(roll r1);
	virtual void function(person* obj)=0;
	int getID();
	roll getRoll();
	void addDisty();
	int collision(roll r2);
	void changeX(int x);
	void setdy(int dy);
};
class ci :
	public object
{
public:
	ci(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id);
	void function(person* obj);
};
class soft :
	public object
{
public:
	soft(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id);
	void function(person* obj);
};
class tough :
	public object
{
public:
	tough(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1, int id);
	void function(person* obj);
};
class flip:
	public object
{
	int jump;
public:
	flip(int x, int y, int width, int height, int dx, int dy, ACL_Image* img, roll r1,int j, int id);
	void function(person* obj);
};