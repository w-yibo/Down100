#include"acllib.h"
#include "object.h"
#include<time.h>
#include<stdio.h>
#include "person.h"
int floor = 0/*起始层*/, life = 100/*起始生命值*/;
int fall = 1/*人物下降速度和软木板下落距离*/, disty = 2/*木板上升高度*/, distx = 3/*人物左右移动速度*/;
int ciNum = 6/*控制尖刺出现概率*/;
int nowNum = 0, createtimerInterval = 1500, movetimerInterval = 20;
roll winRect;
const int winWidth = 800, winHeight = 600;//游戏窗口大小
const int maxNum = 8/*最大生成木板*/;
const int personwidth = 25, personheight = 40; //人物大小
const int personid = 3, objectcreat = 2, objectmove = 1; //timerID
const int objectwidth = 170, objectheight = 25, jump = 30;//木板大小和弹簧上弹高度
ACL_Image personImg, flipImg, TopCiImg, blockImg, blocksoftImg, ciImg;
ACL_Sound bgm, win, lose;
const char* per = "person.bmp";
const char* TopCi = "TopCi.bmp";
const char* block = "block.bmp";
const char* blocksoft = "blocksoft.bmp";
const char* Flip = "flip.bmp";
const char* Ci = "ci.bmp";
object* autosprite[maxNum] = { 0 };
person*  usr = NULL;
void timerEvent(int id);//带有Interva的常量为计时器间隔
void keyEvent(int key, int event);//键盘控制人物左右移动
void paint();
void loadimg();//加载素材（图片、声音）
void createData(object** autosprite);
void createData(person** usr);
void initialization();
int Setup()
{
	winRect.x = DEFAULT; winRect.y = DEFAULT; winRect.w = winWidth; winRect.h = winHeight;
	srand((unsigned)time(NULL));//随机数种子
	initWindow("是优秀成电学子就下100层", DEFAULT, DEFAULT, winWidth, winHeight);
	loadimg();//加载素材
	initialization();
	createData(&usr);//人物和开始木板的初始位置
	registerTimerEvent(timerEvent);
	startTimer(personid, 0);//人物随时间自动向下移动
	startTimer(objectcreat, createtimerInterval);//随机在底部生成木板
	startTimer(objectmove, movetimerInterval);//木板随时间自动向上移动
	registerKeyboardEvent(keyEvent);//键盘控 制人物左右移动
	paint();//绘图
	playSound(bgm, 1);//背景音乐
	return 0;
}
void loadimg()
{
	loadImage(TopCi, &TopCiImg);
	loadImage(per, &personImg);
	loadImage(block, &blockImg);
	loadImage(blocksoft, &blocksoftImg);
	loadImage(Flip, &flipImg);
	loadImage(Ci, &ciImg);
	loadSound("bgm.mp3", &bgm);
	loadSound("win.mp3", &win);
	loadSound("lose.mp3", &lose);
}
void timerEvent(int id)
{


	switch (id)

	{

	case 1:

		for (int i = 0; i < nowNum; ++i)
			if (autosprite[i])

			{

				autosprite[i]->move(winRect);

				if (autosprite[i]->getY() < 0)
				{
					floor++;
					if (floor >= 100)//成功来到100层
					{
						stopSound(bgm);
						cancelTimer(0);
						cancelTimer(1);
						cancelTimer(2);
					}
					if (floor % 30 == 0)//随着层数增加，生成尖刺概率变大
					{
						ciNum++;
					}
					if (floor % 20 == 0)//随着层数增加，木板上移速度加快
					{
						disty++;
						for (int j = 0; j < nowNum; j++)
						{
							autosprite[j]->setdy(disty);
						}

					}
					delete autosprite[i];
					autosprite[i]=NULL;
					int x=rand() %(winWidth-objectwidth) ;

					int y = winHeight + 100;

					if (x < 0)x = -x;


					int m = rand() % ciNum;//根据ciNum随机生成不同种类的物块

					switch (m)
					{
					case 0://生成硬石块
						autosprite[i] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
					case 1://软木块
						autosprite[i] = new soft(x, y, objectwidth, objectheight, 0, disty, &blocksoftImg, winRect, 1); break;
					case 2://跳板
						autosprite[i] = new flip(x, y, objectwidth, objectheight, 0, disty, &flipImg, winRect, jump, 2); break;
					case 3://硬石块
						autosprite[i] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
					default://尖刺
						autosprite[i] = new ci(x, y, objectwidth, objectheight, 0, disty, &ciImg, winRect, 3); break;
					}
					for (int j = 0; j < nowNum; j++)
					{
						if (j != i)
						{
							int c = autosprite[j]->collision(autosprite[i]->getRoll());
							while (c)
							{ 
								x = rand() % (winWidth - objectwidth);  
							    autosprite[i]->changeX(x); 
							    c = autosprite[i]->collision(autosprite[j]->getRoll());
							}
						}

					}
				

				}
			}
		break;	case 2:
		if (nowNum < maxNum)
		{
			createData(autosprite);
		}

		break;
	case 3:
		for (int i = 0; i < nowNum; ++i)
		{
			if (autosprite[i])
			{	
					int c = usr->collision(autosprite[i]->getRoll());
					if (c == 2 || c == 4)
						autosprite[i]->function(usr);						
			}
		}
		usr->move(winRect);
		
		break;
	}
	if (usr->getHP() <= 0 || floor >= 100 || usr->getY() >= winHeight + personheight)//通过键盘控制显示游戏结束界面
	{
		stopSound(bgm);
		cancelTimer(3);
		cancelTimer(1);
		cancelTimer(2);
		beginPaint();
		char str3[50]{ NULL };
		if (usr->getHP() <= 0 || usr->getY() >= winHeight + personheight)
		{
			sprintf_s(str3, "再接再厉，加油！");
			playSound(lose, 1);
		}
		else
		{
			sprintf_s(str3, "你就是成电优秀学子!");
			playSound(win, 1);
		}
		setTextSize(60);
		paintText(150, 250, str3);
		endPaint();
	}

	paint();

}

void createData(object** autosprite)

{

	int x = rand() % (winWidth - objectwidth);

	int y = winHeight + 100;

	if (x < 0)x = -x;

	int m = rand() % ciNum;//根据ciNum随机生成不同种类的物块

	switch (m)
	{
	case 0://生成硬石块
		autosprite[nowNum++]= new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect,0); break;
	case 1://软木块
		autosprite[nowNum++] = new soft(x, y, objectwidth, objectheight, 0, disty, &blocksoftImg, winRect, 1); break;
	case 2://跳板
		autosprite[nowNum++] = new flip(x, y, objectwidth, objectheight, 0, disty, &flipImg, winRect, jump, 2); break;
	case 3://硬石块
		autosprite[nowNum++] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
	default://尖刺
		autosprite[nowNum++] = new ci(x, y, objectwidth, objectheight, 0, disty, &ciImg, winRect, 3); break;
	}

}

void createData(person** usr)

{
	int x = rand() % (winWidth - personwidth);
	if (x < 0)x = -x;
	int y = personwidth;
	int dx = distx;
	int dy = fall;
	*usr = new person(x, y, personwidth, personheight, dx, dy, &personImg, winRect,100);

}

void paint()

{
	beginPaint();
	clearDevice();
	int i = 0;
	for (i = 0; i < nowNum; ++i)
	{
		if (autosprite[i])
		{
			autosprite[i]->drawSprite();
		}
	}
	putImageScale(&TopCiImg, 0, 0, winWidth, personheight - 20);
	if (usr)
	{
		usr->drawSprite();
		char str[20] = { NULL };
		sprintf_s(str, "生命值：%d", usr->getHP());
		setTextSize(25);
		setTextColor(RGB(0, 0, 255));
		paintText(125, 10, str);
	}
	char str1[50] = { NULL };
	setTextSize(25);
	setTextColor(RGB(0, 0, 0));
	sprintf_s(str1, "地下%d层", floor);
	paintText(10, 10, str1);

	char str2[50] = {"祝老师和助教新年快乐，身体健康"};
	setTextSize(25);
	setTextColor(RGB(255,0,0));//设置文字的颜色
	paintText(300, 10, str2);
	
	
	
	
	endPaint();

}
void keyEvent(int key, int event)

{

	if (event != KEY_DOWN)return;
	switch (key)
	{
	    case VK_LEFT:
		for (int i = 0; i < nowNum; ++i)
		{
			if (autosprite[i])
			{								
					int c = usr->collision(autosprite[i]->getRoll());
					if (c == 3 || c == 4)
						;
					else usr->move(key);				
			}
		}
		break;
		case VK_RIGHT:
			for (int i = 0; i < nowNum; ++i)
			{
				if (autosprite[i])
				{								
						int c = usr->collision(autosprite[i]->getRoll());
						if (c == 1 || c == 2)
							;
						else usr->move(key);
					
				}
			}
			break;
	}
	if (usr->getHP() <= 0 || floor >= 100 || usr->getY() >= winHeight + personheight)//通过键盘控制显示游戏结束界面
	{
		stopSound(bgm);
		cancelTimer(3);
		cancelTimer(1);
		cancelTimer(2);
		beginPaint();
		char str3[50]{ NULL };
		if (usr->getHP() <= 0 || usr->getY() >= winHeight + personheight)
		{
			sprintf_s(str3, "再接再厉，加油！");
			playSound(lose, 1);
		}
		else
		{
			sprintf_s(str3, "你就是成电优秀学子!");
			playSound(win, 1);
		}
		setTextSize(60);
		paintText(150, 250, str3);
		endPaint();
	}
	else
	paint();

}
void initialization()
{
	beginPaint();

	do
	{
		int x = rand() % (winWidth - objectwidth);
		int y = 100 * (nowNum + 1);
		if (x < 0)x = -x;
		int m = rand() % ciNum;//根据ciNum随机生成不同种类的物块

		switch (m)
		{
		case 0://生成硬石块
			autosprite[nowNum++] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
		case 1://软木块
			autosprite[nowNum++] = new soft(x, y, objectwidth, objectheight, 0, disty, &blocksoftImg, winRect, 1); break;
		case 2://跳板
			autosprite[nowNum++] = new flip(x, y, objectwidth, objectheight, 0, disty, &flipImg, winRect, jump, 2); break;
		case 3://硬石块
			autosprite[nowNum++] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
		default://尖刺
			autosprite[nowNum++] = new ci(x, y, objectwidth, objectheight, 0, disty, &ciImg, winRect, 3); break;
		}
		
		endPaint();
	} while (autosprite[nowNum-1]->getY() < winHeight + 200);
}