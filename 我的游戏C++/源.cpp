#include"acllib.h"
#include "object.h"
#include<time.h>
#include<stdio.h>
#include "person.h"
int floor = 0/*��ʼ��*/, life = 100/*��ʼ����ֵ*/;
int fall = 1/*�����½��ٶȺ���ľ���������*/, disty = 2/*ľ�������߶�*/, distx = 3/*���������ƶ��ٶ�*/;
int ciNum = 6/*���Ƽ�̳��ָ���*/;
int nowNum = 0, createtimerInterval = 1500, movetimerInterval = 20;
roll winRect;
const int winWidth = 800, winHeight = 600;//��Ϸ���ڴ�С
const int maxNum = 8/*�������ľ��*/;
const int personwidth = 25, personheight = 40; //�����С
const int personid = 3, objectcreat = 2, objectmove = 1; //timerID
const int objectwidth = 170, objectheight = 25, jump = 30;//ľ���С�͵����ϵ��߶�
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
void timerEvent(int id);//����Interva�ĳ���Ϊ��ʱ�����
void keyEvent(int key, int event);//���̿������������ƶ�
void paint();
void loadimg();//�����زģ�ͼƬ��������
void createData(object** autosprite);
void createData(person** usr);
void initialization();
int Setup()
{
	winRect.x = DEFAULT; winRect.y = DEFAULT; winRect.w = winWidth; winRect.h = winHeight;
	srand((unsigned)time(NULL));//���������
	initWindow("������ɵ�ѧ�Ӿ���100��", DEFAULT, DEFAULT, winWidth, winHeight);
	loadimg();//�����ز�
	initialization();
	createData(&usr);//����Ϳ�ʼľ��ĳ�ʼλ��
	registerTimerEvent(timerEvent);
	startTimer(personid, 0);//������ʱ���Զ������ƶ�
	startTimer(objectcreat, createtimerInterval);//����ڵײ�����ľ��
	startTimer(objectmove, movetimerInterval);//ľ����ʱ���Զ������ƶ�
	registerKeyboardEvent(keyEvent);//���̿� �����������ƶ�
	paint();//��ͼ
	playSound(bgm, 1);//��������
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
					if (floor >= 100)//�ɹ�����100��
					{
						stopSound(bgm);
						cancelTimer(0);
						cancelTimer(1);
						cancelTimer(2);
					}
					if (floor % 30 == 0)//���Ų������ӣ����ɼ�̸��ʱ��
					{
						ciNum++;
					}
					if (floor % 20 == 0)//���Ų������ӣ�ľ�������ٶȼӿ�
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


					int m = rand() % ciNum;//����ciNum������ɲ�ͬ��������

					switch (m)
					{
					case 0://����Ӳʯ��
						autosprite[i] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
					case 1://��ľ��
						autosprite[i] = new soft(x, y, objectwidth, objectheight, 0, disty, &blocksoftImg, winRect, 1); break;
					case 2://����
						autosprite[i] = new flip(x, y, objectwidth, objectheight, 0, disty, &flipImg, winRect, jump, 2); break;
					case 3://Ӳʯ��
						autosprite[i] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
					default://���
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
	if (usr->getHP() <= 0 || floor >= 100 || usr->getY() >= winHeight + personheight)//ͨ�����̿�����ʾ��Ϸ��������
	{
		stopSound(bgm);
		cancelTimer(3);
		cancelTimer(1);
		cancelTimer(2);
		beginPaint();
		char str3[50]{ NULL };
		if (usr->getHP() <= 0 || usr->getY() >= winHeight + personheight)
		{
			sprintf_s(str3, "�ٽ����������ͣ�");
			playSound(lose, 1);
		}
		else
		{
			sprintf_s(str3, "����ǳɵ�����ѧ��!");
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

	int m = rand() % ciNum;//����ciNum������ɲ�ͬ��������

	switch (m)
	{
	case 0://����Ӳʯ��
		autosprite[nowNum++]= new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect,0); break;
	case 1://��ľ��
		autosprite[nowNum++] = new soft(x, y, objectwidth, objectheight, 0, disty, &blocksoftImg, winRect, 1); break;
	case 2://����
		autosprite[nowNum++] = new flip(x, y, objectwidth, objectheight, 0, disty, &flipImg, winRect, jump, 2); break;
	case 3://Ӳʯ��
		autosprite[nowNum++] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
	default://���
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
		sprintf_s(str, "����ֵ��%d", usr->getHP());
		setTextSize(25);
		setTextColor(RGB(0, 0, 255));
		paintText(125, 10, str);
	}
	char str1[50] = { NULL };
	setTextSize(25);
	setTextColor(RGB(0, 0, 0));
	sprintf_s(str1, "����%d��", floor);
	paintText(10, 10, str1);

	char str2[50] = {"ף��ʦ������������֣����彡��"};
	setTextSize(25);
	setTextColor(RGB(255,0,0));//�������ֵ���ɫ
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
	if (usr->getHP() <= 0 || floor >= 100 || usr->getY() >= winHeight + personheight)//ͨ�����̿�����ʾ��Ϸ��������
	{
		stopSound(bgm);
		cancelTimer(3);
		cancelTimer(1);
		cancelTimer(2);
		beginPaint();
		char str3[50]{ NULL };
		if (usr->getHP() <= 0 || usr->getY() >= winHeight + personheight)
		{
			sprintf_s(str3, "�ٽ����������ͣ�");
			playSound(lose, 1);
		}
		else
		{
			sprintf_s(str3, "����ǳɵ�����ѧ��!");
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
		int m = rand() % ciNum;//����ciNum������ɲ�ͬ��������

		switch (m)
		{
		case 0://����Ӳʯ��
			autosprite[nowNum++] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
		case 1://��ľ��
			autosprite[nowNum++] = new soft(x, y, objectwidth, objectheight, 0, disty, &blocksoftImg, winRect, 1); break;
		case 2://����
			autosprite[nowNum++] = new flip(x, y, objectwidth, objectheight, 0, disty, &flipImg, winRect, jump, 2); break;
		case 3://Ӳʯ��
			autosprite[nowNum++] = new tough(x, y, objectwidth, objectheight, 0, disty, &blockImg, winRect, 0); break;
		default://���
			autosprite[nowNum++] = new ci(x, y, objectwidth, objectheight, 0, disty, &ciImg, winRect, 3); break;
		}
		
		endPaint();
	} while (autosprite[nowNum-1]->getY() < winHeight + 200);
}