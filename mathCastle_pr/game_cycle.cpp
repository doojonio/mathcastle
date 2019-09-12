#include <iostream>		//cout
#include <Windows.h>	//Sleep()
#include <conio.h>		//_getch()
#include <cstdlib>		//rand(), srand()
#include <ctime>		//time()
#include "graphic.h"	//all_func
#include "generation.h"	//all_func
#include "menu.h"		//gotoxy(x, y)
#include "game_cycle.h"	


using std::cout;

typedef unsigned char ubyte;

//��������� ��� ����� �������� ����� ����� �����
const char SYS_COLOR[] = "color F0";

//��������� ������� "PRESS ANY KEY FOR START"
const char PAKFS[] = "PRESS ANY KEY FOR START";

//��� ������� ����
const unsigned short int BULLET_CODE = 253;

//���� ������
enum keys
{
	esc = 27,															//��� ������� "ESC"
	enter = 13,															//��� ������� "ENTER"
	num0 = 48, num1, num2, num3, num4, num5, num6, num7, num8, num9,	//���� ������ � �������(�� 0 �� 9)
	minus = 45															//��� ������� "-"
};

//���� ���������� ������
enum ConsoleColor
{
	Black = 0,			//��� ������� �����
	Blue = 1,			//��� ������ �����
	Green = 2,			//��� �������� �����
	Cyan = 3,			//��� �������� �����
	Red = 4,			//��� �������� �����
	Magenta = 5,		//��� ���������� �����
	Brown = 6,			//��� ����������� �����
	LightGray = 7,		//��� ������-������ �����
	DarkGray = 8,		//��� �����-������ �����
	LightBlue = 9,		//��� ������-������ �����
	LightGreen = 10,	//��� ������-�������� �����
	LightCyan = 11,		//��� ������-�������� �����
	LightRed = 12,		//��� ������-�������� �����
	LightMagenta = 13,	//��� ������-���������� �����
	Yellow = 14,		//��� ������� �����
	White = 15			//��� ������ �����
};

//�������� ����� ����
enum Game_Color {
	BACKGR_COLOR = White,		//���� ����
	TOWER_COLOR = LightBlue,	//���� �����
	CORE_COLOR = LightBlue,		//���� ����
	OBJ_COLOR = LightRed,		//���� �������
	LABEL_COLOR = Black,		//���� ������� "Press any key for start" � ���������� �����
	BOSS_LABEL_COLOR = Magenta,	//���� ������� "ATTENTION! BOSS HAS COME!"
	SHOT_TRUE = LightGreen,		//���� �������� ���������� �������
	SHOT_FALSE = LightRed,		//���� �������� ������������ �������
};

//��������� �������������� �� ������������ ���������
enum Location_Settings
{
	//�-���������� ����� � ����
	X_T1 = 50,
	X_T2 = X_T1 + 17,
	X_CORE = X_T2 + 20,

	//y-���������� ����� � ����
	Y_T = 6,
	Y_CORE = 3,

	X_OBJ_START = 3,	//��������� x-���������� �������, � ������� �� �������� ��������
	Y_OBJ = 7,			//y-���������� �������

	//���������� ������� ��������� �����
	X_SCORE = 76,
	Y_SCORE = 26,

	//���������� ������� "Press any key for start"
	X_LABEL = 60,
	Y_LABEL = 20,

	//���������� ������� "Attention! Boss is coming!"
	X_BOSS_LABEL = 20,
	Y_BOSS_LABEL = 15,
};

//��������� �������� � ���-������� ����� � ����
enum Main_Options
{
	CORE_HP = 3,		//�������� ���� �� ������ ������ ����
	CORE_LENGTH = 25,	//����� ����� ����
	CORE_WIDTH = 12,	//������ ����� ����

	TOWER1_HP = 1,		//�������� ������ ����� �� ������ ������ ����
	TOWER2_HP = 2,		//�������� ������ ����� �� ������ ������ ����
	TOWER_LENGTH = 15,	//����� ����� �����
	TOWER_WIDTH = 7,	//������ ����� �����

	//��������! ��� ������ ������� ��������, ��� �������� ����� ��������� ������. � ��������.
	LINEAR_SPEED = 200,		//�������� �������� �������� ��������� � ������
	QUADRATIC_SPEED = 1000,	//�������� �������� ���������� ��������� � ������
	CUBIC_SPEED = 5000,		//�������� �������� ���������� ��������� � ������ 
	BULLET_SPEED = 15,		//�������� �������� ����

	LINEAR_POINTS = 10,		//����, ���������� �� ������� ��������� ���������
	QUADRATIC_POINTS = 12,	//����, ���������� �� ���� ����� ��������� ������ ����������� ��������� 
	CUBIC_POINTS = 33,		//����, ���������� �� ���� ����� ��������� ������ ����������� ���������
	WRONG_POINTS = -10,		//����, "����������" �� ������� ��������� �����

	FIRST_LEVEL = 50,		//���������� �����, ����������� ������� ��� 1 ����� (�� ����������)
	SECOND_LEVEL = 150,		//���������� �����, ����������� ������� ��� 2 ����� (�� ����������� � ������������ �������)

	LINEAR_PERCENT = 60,										//������� ��������� ��������� ���������
	QUADRATIC_PERCENT = 30,										//������� ��������� ����������� ���������
	CUBIC_PERCENT = 100 - LINEAR_PERCENT - QUADRATIC_PERCENT,	//������� ��������� ����������� ���������
};

void shoot(bool ans, tower_options tower, ubyte xEdgeObj);
void LinearToTower(tower_options tower, ubyte *ptr_hp, int*ptr_score, ubyte *ptr_keystroke);
void QuadraticToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke);
void CubicToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke);

//������� �������� �����. ���������� ���������� �����
int play()
{
	//��������� ����������� ���������� �������� �����(��� 2-� ����� � ����)
	tower_options t1, t2, core;

	//�������������� ���������� 1 �����
	t1.bckGrColor = BACKGR_COLOR;
	t1.color = TOWER_COLOR;
	t1.length = TOWER_LENGTH;
	t1.width = TOWER_WIDTH;

	//��������������������� ���������� ������ ����� ����� ������ �� � � ������ �����
	t2 = t1;

	//������������� ���������� 1-� �����
	t1.x = X_T1;
	t1.y = Y_T;
	//�������������� �������� ������ �����
	t1.hp = TOWER1_HP;
	t1.max_hp = t1.hp;

	//�������������� ���������� 2-� �����
	t2.x = X_T2;
	t2.y = Y_T;
	//�������������� �������� ������ �����
	t2.hp = TOWER2_HP;
	t2.max_hp = t2.hp;

	//�������������� ���������� ����
	core.bckGrColor = BACKGR_COLOR;
	core.color = TOWER_COLOR;
	core.hp = CORE_HP;
	core.max_hp = core.hp;
	core.length = CORE_LENGTH;
	core.width = CORE_WIDTH;
	core.x = X_CORE;
	core.y = Y_CORE;

	//��������� ��������� �� �������� ��������� ����� ��� ���� ��� ��������� �� ���-������� ��� ������������ �������
	ubyte *ptr_hp;

	//�������� ���� ���� �� �����, ���� �������� �� ������
	system(SYS_COLOR);

	//������� ���� ����� (������������ ��� ����� � ����)
	drawTower(t1);
	drawTower(t2);
	drawTower(core);


	int score = 0;						//������� ����������, � ������� ����� ������� ��������� �����
	int *ptr_score = &score;			//������� ��������� �� ���������� ����� ��� ��������� � ��� ������������ �������

	ubyte keystroke;					//������ ����������, � ������� ����� �������� ��� ������� �������	
	ubyte *ptr_keystroke = &keystroke;	//������� ��������� �� ���������� ������� ������ ��� ��������� � ��� ������������ �������

	//������� ������� "Press any key for start"
	gotoxy(X_LABEL, Y_LABEL);
	SetColor(BACKGR_COLOR, LABEL_COLOR);
	cout << PAKFS;
	_getch();				//�������� ������� �������
	
	//����� ������� ������� ������� PAKFS
	for (ubyte i = X_LABEL; i < X_LABEL + 30; i++)
	{
		gotoxy(i, Y_LABEL);
		cout << ' ';
	}
	
	/*
	����� ���������� ������ ������� ����.
	������� ������������ ����������� ������ �������� ���������, ���� �� �� ������� ��������� ����������� �����.
	����� ��� ������������ ������ 2 ���������� ���������, � �����, � ������� �������� ��������� ������� �����������
	���������� ����� ��� ������� �����.
	����� ����� ������������ ������ ���� ���������� ���������.
	� ����� ����, ��� ������������ ����� ��� �� ����� ���������� ���������, ���������� ��������� ��������� �� ���������
	������������:
				60% - �������� ���������
				31% - ���������� ���������
				9%	- ���������� ��������� 
	*/

	//���� ��� ������� ����� - ���� �� ������� ������ ���������� �����, �� ������ esc ��� �� ��������� (�������� ���� ������ ����� ����)
	while (score < FIRST_LEVEL && keystroke != esc && core.hp != 0) 
	{
		//���� ������ ����� ����, �� �������� ��������� ���������� �� ��
		if (t1.hp != 0)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t1.hp;	
			LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
		}
		//���� �� ����, �� ��������� �������� ������ ����� �, ���� ������ ����� ���� ���������� �� ��
		else if (t2.hp != 0)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t2.hp;
			LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
		}
		//���� ��� ����� �� ����, �� ���������� ������ �� ����
		//��� ������ ���� ����, ����� �� �� � ���� ���� �� �����
		else
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &core.hp;
			LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
		}
	}

	//����� ������� ����� ��������� ������������ ������ 2 ���������� ���������, ������������� � 1 ��������
	for (ubyte i = 0; i < 3; i++)
	{
		//���� ������ ����� ����, �� ���������� �� �� �������� ��� ��������� ��������� ( � ����������� �� i )
		if (t1.hp != 0 && keystroke != esc)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t1.hp;
			//������, � ����������� �� i, �������� ���������, ������� ������ �� �����
			//����� ����� ����������� ���� ��������, ��, ���� i = 0 � 2 �������� ����������,
			//� ���� i = 1 (����� 0 � 2), �������� ��������.
			//����� �������, ����� ����� ����������� ����� ���� �������� ���������
			if (i == 1)
			{
				LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
			else
			{
				QuadraticToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		//���� ������ ����� ������, �� ������ ����� ����, �� ���������� �� �� ���������
		else if (t2.hp != 0 && keystroke != esc)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t2.hp;
			//�� ������ �� ��������, ��� � 1 ������, ��������, ����� ��������� ������� �� �����
			if (i == 1)
			{
				LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
			else
			{
				QuadraticToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		//���� ��� ����� ������, �� ���������� �� ����, ���� ��� �����
		else if (core.hp != 0 && keystroke != esc)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &core.hp;
			//��� �� �������� ���������, ������� ������� �� ����
			if (i == 1)
			{
				LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
			else
			{
				QuadraticToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
	}

	//������ ���� - ��� ������� �������� ���������, ���� ������������ �� ������� ����������� ���������� �����
	//� �� ������ esc, � �� ��������� (���� �� ����������)
	while (score < SECOND_LEVEL && keystroke != esc && core.hp != 0)
	{
		//���� ������ ����� ����, �� ���������� �� �� ���������
		if (t1.hp != 0)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t1.hp;
			LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
		}
		//�����, ���� ���� ������ �����, �� ���������� �� ��
		else if (t2.hp != 0)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t2.hp;
			LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
		}
		//���� ��� ����� ������, �� ���������� �� ����
		//��� ����� ����, ����� �� ���� �� ����� � ���� ����
		else
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &core.hp;
			LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
		}
	}

	//����� ������������ ������ ����������� ���������� ����� ������� �����, 
	//��� ������������ ������ ���� ���������� ���������
	
	//���� ���� ������ �����, �� ���������� ���������� ��������� �� ����:
	if (t1.hp != 0 && keystroke != esc)
	{
		//���������� ���������, ����� � ������� play ���� ���������� �������� �����
		ptr_hp = &t1.hp;
		CubicToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
	}
	//�����, ���� ���� ������ �����, �� ���������� �� ��
	else if (t2.hp != 0 && keystroke != esc)
	{
		//���������� ���������, ����� � ������� play ���� ���������� �������� �����
		ptr_hp = &t2.hp;
		CubicToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
	}
	//���� ��� ����� ������, ���������, ���� �� ����
	//���� ����, �� ���������� �� ���� ���������
	else if (core.hp != 0 && keystroke !=esc)
	{
		ptr_hp = &core.hp;
		CubicToTower(core, ptr_hp, ptr_score, ptr_keystroke);
	}

	//���� ������������ ������� "���������" ����������� ���������,
	//�� �� ���������� ������ �������������� ����� ����.
	//����� ����� ���������� "���������" ����� ����
	//��������� ����� �������������� � ��������� ������� � ������ ���������� ���������
	
	//���� ����� �� ��������� (�������� ���� �� ������ ����� ����) � �� ������ esc
	while (core.hp != 0 && keystroke != esc)
	{
		//����� ����� �������, ���������� �� �������� �������
		srand(static_cast<unsigned int>(time(NULL)));
		//�������� ��������� ����� � ����� ��� ������� �� ������� �� 100, ����� ���������� 1
		//����� ������� �� ���� �������� ����� � ��������� �� 1 �� 100
		ubyte randValue = rand() % 100 + 1;
		
		//���� ������ ����� ���� � �� ���������� ������� esc, ���������� ��������� ��������� �� ������ �����
		if (t1.hp != 0 && keystroke != esc)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t1.hp;
			//���� ���������� �������� ����� ������ � ������� ��������� ���������, �� ���������� �������� �� �����
			if (randValue <= LINEAR_PERCENT)
			{
				LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
			//�����, ���� ������ � ������� ��������� ����������� (�������� �� 60 �� 90, �.�. 90-60=30%),
			//�� ���������� ����������
			else if (randValue <= LINEAR_PERCENT+QUADRATIC_PERCENT)
			{
				QuadraticToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
			//���� �� ������ �� � ������� ���������, �� � ������� �����������,
			//������, ������ � ����������. ����� ���������� ����������
			else
			{
				CubicToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		//�����(���� ������ ����� ������), �� ���������� ��������� ��������� �� ������ �����
		else if (t2.hp != 0 && keystroke != esc)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &t2.hp;
			//���� ���������� �������� ����� ������ � ������� ��������� ���������, �� ���������� �������� �� �����
			if (randValue <= LINEAR_PERCENT)
			{
				LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
			//�����, ���� ������ � ������� ��������� ����������� (�������� �� 60 �� 90, �.�. 90-60=30%),
			//�� ���������� ����������
			else if (randValue <= LINEAR_PERCENT + QUADRATIC_PERCENT)
			{
				QuadraticToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
			//���� �� ������ �� � ������� ���������, �� � ������� �����������,
			//������, ������ � ����������. ����� ���������� ����������
			else
			{
				CubicToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		else if (core.hp != 0 && keystroke != esc)
		{
			//���������� ���������, ����� � ������� play ���� ���������� �������� �����
			ptr_hp = &core.hp;
			//���� ���������� �������� ����� ������ � ������� ��������� ���������, �� ���������� �������� �� �����
			if (randValue <= LINEAR_PERCENT)
			{
				LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
			//�����, ���� ������ � ������� ��������� ����������� (�������� �� 60 �� 90, �.�. 90-60=30%),
			//�� ���������� ����������
			else if (randValue <= LINEAR_PERCENT + QUADRATIC_PERCENT)
			{
				QuadraticToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
			//���� �� ������ �� � ������� ���������, �� � ������� �����������,
			//������, ������ � ����������. ����� ���������� ����������
			else
			{
				CubicToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		
	}

	//����� ����� ���� ��������, ���� ����� esc, �� �� ���������� ���������� �����, ��������� ��
	return score;
}

//������� �������� �� ����� tower � ��������� ������ ans,
//����������� ���� ������� xEdgeObj
void shoot(bool ans, tower_options tower, ubyte xEdgeObj)
{
	//������� ����������, � ������� ����� ������� ���������� ����
	ubyte xBullet, yBullet;
	//������ �����, ������ ����� �������� ����. ������� �� ��������� � �������� �����
	xBullet = tower.x - 2;
	yBullet = tower.y + tower.width / 2;

	//���� ����� ������, �� �������� �������� ������ ��� ����������, ����� �������� �������� ������ ��� ������������.
	if (ans == true)
	{
		SetColor(BACKGR_COLOR, SHOT_TRUE);
	}
	else
	{
		SetColor(BACKGR_COLOR, SHOT_FALSE);
	}
	
	//���� �������� ����. ��������, ���� �� ��������� ������� ( ��� ���� )
	while (xBullet != xEdgeObj - 2)
	{	
		//��������� � ����������� ���� � ������������ ������
		gotoxy(xBullet, yBullet);
		cout << char(BULLET_CODE);
		
		//�������� ���� �� 1 �����
		xBullet--;
		
		//������� ���� ����
		gotoxy(xBullet + 2, yBullet);
		cout << ' ';

		Sleep(BULLET_SPEED);
	}
}

//��������� �������� ������� � �������� ���������� � ��� �������� � ����� tower,
//� ���������� ������� ��������� ��������� �� ��������, ���� � ���������� ������� ������
//��� ����, ����� ��������� ���� ������������� ����������� � ��������� �������
void LinearToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke)
{
	//���������� �������� ��������� � ������� ��������� � ���������� ��������� ����������� ����������
	gen_result accept = generateLinear();

	//������� ����������� ���������� ����� ������� � �������������� �
	object_options obj;
	obj.task = accept.task;
	obj.x = X_OBJ_START;
	obj.y = Y_OBJ;
	obj.x_edge = obj.x + obj.task.size() + 4;
	obj.bckGrColor = BACKGR_COLOR;
	obj.color = OBJ_COLOR;
	obj.alive = true;

	//������� ����������, ����������� ��� �������� ������� �� ����� ( ����� ����� ������ �������� �� -1 )
	bool minusoid = false;
	
	//����������, �������� � ���� ����� ������������
	short int ans = 11;

	//���� �������� ������� � ����� tower (���� ��� ���� �� ��������� ����� � ��� �� ���)
	while (obj.x_edge != tower.x && obj.alive)
	{
		//��������� �������
		drawObject(obj);
		//�������� ��� ������ �� 1
		obj.x++;
		//��������� x-���������� ���� �������
		obj.x_edge = obj.x + obj.task.size() + 4;
		//������� ����, ����������� ��������
		for (ubyte i = Y_OBJ; i < Y_OBJ + 6; i++)
		{
			gotoxy(obj.x - 2, i);
			cout << ' ';
		}
		
		Sleep(LINEAR_SPEED);

		//�������� ������� �������
		if (_kbhit())
		{
			//������� � keystroke ��� ������� �������
			*ptr_keystroke = _getch();
			
			//���� ���� ������ esc, �� ������� �� ������ ������� � ����� �� ������� �� ������� play()
			if (*ptr_keystroke == esc)
				return;

			//��������� ����������, ��� ����� ���������� �������� ����� ������������
			ubyte
				xNum = tower.x + tower.length / 2,
				yNum = tower.y + tower.width / 2 + 1;

			//��������� � ���� ����������� � ������ ���� �� �������� ����� ��� ��������
			gotoxy(xNum, yNum);
			SetColor(BACKGR_COLOR, LABEL_COLOR);

			//������� ��������� ��������� ������������� �����
			//� ����������� �� ���� �������, �� ������� � ����� ������������ �����, ��������� �������������
			//���� ��� ����� �����, �� ������ ���� �� ���� ����� � ������� bool minusoid, ��������� �����
			//���� ��� ����� enter, �� ����������� ������� ������ ��� �������� �������� ������ � ������������ �������� � �����������
			//������� � ��������� ���������� �����
			switch (*ptr_keystroke)
			{
			case num0:
			{
				cout << 0;
				ans = 0;
				break;
			}
			case num1:
			{
				cout << 1;
				ans = 1;
				break;
			}
			case num2:
			{
				cout << 2;
				ans = 2;
				break;
			}
			case num3:
			{
				cout << 3;
				ans = 3;
				break;
			}
			case num4:
			{
				cout << 4;
				ans = 4;
				break;
			}
			case num5:
			{
				cout << 5;
				ans = 5;
				break;
			}
			case num6:
			{
				cout << 6;
				ans = 6;
				break;
			}
			case num7:
			{
				cout << 7;
				ans = 7;
				break;
			}
			case num8:
			{
				cout << 8;
				ans = 8;
				break;
			}
			case num9:
			{
				cout << 9;
				ans = 9;
				break;
			}
			case minus:
			{
				cout << '-';
				xNum++;
				minusoid = true;
				break;
			}
			case enter:
			{
				//���� ��� ����� �����, �� �������� ����� �� �����
				if (minusoid)
				{
					ans *= -1;
				}
				//�������� �������� �����
				//���� �����, �� �������� "���������� �������", ���������� ������ � ������������ ����
				if (ans == accept.roots[0])
				{
					shoot(true, tower, obj.x_edge);
					clearObject(obj);
					obj.alive = false;
					*ptr_score += LINEAR_POINTS;
				}
				//���� �������, �� �������� "������������ �������", � �������� ����
				else
				{
					shoot(false, tower, obj.x_edge);
					*ptr_score += WRONG_POINTS;
				}
				
				//�������������� �������� ���������� ������ �� �����, ������� �������� ����������. ��� ������������.
				ans = 11;
				//���������� �������� �������� ���������.
				minusoid = false;
				
				//���������� ���������� �����

				//������� ���������� ��������
				for (ubyte i = X_SCORE; i < X_SCORE + 8; i++)
				{
					gotoxy(i, Y_SCORE);
					cout << ' ';
				}
				//��������� � ����������� ������� �����, ������ ���� �� ���� �������� � ������� ����
				gotoxy(X_SCORE, Y_SCORE);
				SetColor(BACKGR_COLOR, LABEL_COLOR);
				cout << *ptr_score;
				break;
			}
			}
		}
	}
	//���� ��� �� ������ ����� ������� �����, �� ���������� ������, �������� �������� � ����� � ������������ �����
	//� ����� �����������. ���� � ����� �� �������� ��������, �� ���������� �
	if (obj.x_edge == tower.x)
	{
		clearObject(obj);

		tower.hp--;
		*ptr_hp = tower.hp;

		drawTower(tower);

		if (tower.hp == 0)
		{
			clearTower(tower);
		}
	}
}

//��������� �������� ������� � ���������� ���������� � ��� �������� � ����� tower,
//� ���������� ������� ��������� ��������� �� ��������, ���� � ���������� ������� ������
//��� ����, ����� ��������� ���� ������������� ����������� � ��������� �������
void QuadraticToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke)
{
	//���������� ���������� ��������� � ��������� ���������� � ����������� ����������
	gen_result accept = generateQuadratic();

	//�������� ����������� ���������� ��������� ����� � � �������������
	object_options obj;
	obj.task = accept.task;
	obj.x = X_OBJ_START;
	obj.y = Y_OBJ;
	obj.x_edge = obj.x + obj.task.size() + 4;
	obj.bckGrColor = BACKGR_COLOR;
	obj.color = OBJ_COLOR;
	obj.alive = true;

	//������� ����������, ����������� ��� �������� ������� �� ����� ( ����� ����� ������ �������� �� -1 )
	bool minusoid = false;
	//������� ������, � ������� ����� ������� ������ ������������
	short int ans[2] = { 11, 11 };
	//������� ����������, � ������� ����� ������� ����� ������ ������������ ( ����� �� �������� ����� � ��� ��������� ���������� ������� )
	ubyte numAns = 0;
	//������� ������� ������, ����� ��������� ������� ������� ��������� �������� ( ������ ������������ ������, ���� ��� ans_check = true )
	bool ans_check[2] = { false, false };

	//���� �������� ������� � ����� tower (���� ��� ���� �� ��������� ����� � ��� �� ���)
	while (obj.x_edge != tower.x && obj.alive)
	{
		//��������� ������� �� ��������� obj
		drawObject(obj);
		//�������� ������ �� 1 ������
		obj.x++;
		//����������� ����� ���������� ���� �������
		obj.x_edge = obj.x + obj.task.size() + 4;

		//������� ����, ����������� �������� ��� ��������
		for (ubyte i = Y_OBJ; i < Y_OBJ + 6; i++)
		{
			gotoxy(obj.x - 2, i);
			cout << ' ';
		}
		
		Sleep(QUADRATIC_SPEED);

		//�������� ������� �������
		if (_kbhit())
		{
			//���������� � keystroke ������� �������
			*ptr_keystroke = _getch();

			//���� ���� ������ esc, �� ������� �� ������� (����� ������� �� play() �� ������� )
			if (*ptr_keystroke == esc)
				return;

			//��������� ���������� ������ ��������� ������������� �����
			ubyte
				xNum = tower.x + tower.length / 2,
				yNum = tower.y + tower.width / 2 + 1;

			//��������� � ����������� ����������� � �������� ���� �� �������� ��� ��������
			gotoxy(xNum, yNum);
			SetColor(BACKGR_COLOR, LABEL_COLOR);

			//������� ��������� ��������� ������������� �����
			//� ����������� �� ���� �������, �� ������� � ����� ������������ �����, ��������� �������������
			//���� ��� ����� �����, �� ������ ���� �� ���� ����� � ������� bool minusoid, ��������� �����
			//���� ��� ����� enter, �� ����������� ������� ������ ��� �������� �������� ������ � ������������ �������� � �����������
			//������� � ��������� ���������� �����
			switch (*ptr_keystroke)
			{
			case num0:
			{
				cout << 0;
				ans[numAns] = 0;
				break;
			}
			case num1:
			{
				cout << 1;
				ans[numAns] = 1;
				break;
			}
			case num2:
			{
				cout << 2;
				ans[numAns] = 2;
				break;
			}
			case num3:
			{
				cout << 3;
				ans[numAns] = 3;
				break;
			}
			case num4:
			{
				cout << 4;
				ans[numAns] = 4;
				break;
			}
			case num5:
			{
				cout << 5;
				ans[numAns] = 5;
				break;
			}
			case num6:
			{
				cout << 6;
				ans[numAns] = 6;
				break;
			}
			case num7:
			{
				cout << 7;
				ans[numAns] = 7;
				break;
			}
			case num8:
			{
				cout << 8;
				ans[numAns] = 8;
				break;
			}
			case num9:
			{
				cout << 9;
				ans[numAns] = 9;
				break;
			}
			case minus:
			{
				cout << '-';
				xNum++;
				minusoid = true;
				break;
			}
			case enter:
			{
				//���� ��� ����� �����, �������� �� -1 �����
				if (minusoid)
				{
					ans[numAns] *= -1;
				}

				//���� ��������� ����� ����� ������ �� ���������� ������, �� �������� ���������� �������
				//���� ����� ans_check, ��� ����� ����������
				//����������� ����
				if (ans[numAns] == accept.roots[0] || ans[numAns] == accept.roots[1])
				{
					shoot(true, tower, obj.x_edge);
					ans_check[numAns] = true;
					*ptr_score += QUADRATIC_POINTS;
					numAns++;
				}
				//����� �������� ������������ ������� � �������� ����
				else
				{
					shoot(false, tower, obj.x_edge);
					*ptr_score += WRONG_POINTS;
				}

				//���� ������������ ���� ��� ���������� ������, �� ���������� ������ � ������ �������������� ����� ������ ������������
				if (ans_check[0] == true && ans_check[1] == true)
				{
					clearObject(obj);
					obj.alive = false;
					numAns = 0;
				}

				//�������������� �������� ���������� ������ �� �����, ������� �������� ����������. ��� ������������.
				ans[numAns] = 11;
				//������ ������������� �������� (����� ���������� ����� �� ��� �������������)
				minusoid = false;
				//���������� ���������� �����

				//������� ���������� ��������
				for (ubyte i = X_SCORE; i < X_SCORE + 8; i++)
				{
					gotoxy(i, Y_SCORE);
					cout << ' ';
				}
				//������� ����� � ��������������� ������ � ��������� ������������
				gotoxy(X_SCORE, Y_SCORE);
				SetColor(BACKGR_COLOR, LABEL_COLOR);
				cout << *ptr_score;
				break;
			}
			}
		}
	}
	//���� ������ ������ �����, �� ���������� ������, �������� ����� � ����� � ������������ ����� � ����� �����������
	//���� � ����� �� �������� ��������, �� ���������� �����
	if (obj.x_edge == tower.x)
	{
		clearObject(obj);

		tower.hp --;

		*ptr_hp = tower.hp;

		drawTower(tower);

		if (tower.hp == 0)
		{
			clearTower(tower);
		}
	}
}

//��������� �������� ������� � ���������� ���������� � ��� �������� � ����� tower,
//� ���������� ������� ��������� ��������� �� ��������, ���� � ���������� ������� ������
//��� ����, ����� ��������� ���� ������������� ����������� � ��������� �������
void CubicToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke)
{
	//���������� ���������� ��������� � ��������� ���������� � ����������� ����������
	gen_result accept = generateCubic();

	//������� ����������� ���������� ��������� ����� � �������������� �
	object_options obj;
	obj.task = accept.task;
	obj.x = X_OBJ_START;
	obj.y = Y_OBJ;
	obj.x_edge = obj.x + obj.task.size() + 4;
	obj.bckGrColor = BACKGR_COLOR;
	obj.color = OBJ_COLOR;
	obj.alive = true;

	//������� ����������, �����������, ��� �� ����� �����
	bool minusoid = false;
	//������� ������, � ������� ����� ����������� ���������������� ������
	short int ans[3] = { 11, 11, 11 };
	//����������, � ������� ������ ����� ������ ������ ��� ����, ����� �� �������� ����� � ��� ���������� ������� � ���������� �������
	ubyte numAns = 0;
	//������� ������� ������, ����� ��������� ������� ������� ��������� �������� ( ������ ������������ ������, ���� ��� ans_check = true )
	bool ans_check[3] = { false, false, false };

	cout << "\a";								//�������� ������������� ��� ��������� �����
	gotoxy(X_BOSS_LABEL, Y_BOSS_LABEL);			//����� ���������� � ���, ��� ���� ������
	SetColor(BACKGR_COLOR, BOSS_LABEL_COLOR);	//����� ����� �� ���� ������� ��� Boss'a 
	cout << "ATTENTION! BOSS HAS COME!";		//����� ������ � �����

	//���� �������� ������� � ����� tower (���� ��� ���� �� ��������� ����� � ��� �� ���)
	while (obj.x_edge != tower.x && obj.alive)
	{
		//��������� �������
		drawObject(obj);
		//����� ������� �� 1 ������
		obj.x++;
		//������ ��������� ���������� ���� �������
		obj.x_edge = obj.x + obj.task.size() + 4;
		//������� ����, ����������� ��������
		for (ubyte i = Y_OBJ; i < Y_OBJ + 6; i++)
		{
			gotoxy(obj.x - 2, i);
			cout << ' ';
		}
		Sleep(CUBIC_SPEED);

		//�������� ������� �������
		if (_kbhit())
		{
			//������� � keystroke ������� �������
			*ptr_keystroke = _getch();
			//���� ��� ����� ESC, �� ������� �� �������
			if (*ptr_keystroke == esc)
				return;

			//��������� ���������� ������ �������� ������������� ��������
			ubyte
				xNum = tower.x + tower.length / 2,
				yNum = tower.y + tower.width / 2 + 1;

			//��������� � ����������� ����������� � ������ ���� �� ���� ��������
			gotoxy(xNum, yNum);
			SetColor(BACKGR_COLOR, LABEL_COLOR);
			
			//������� ��������� ��������� ������������� �����
			//� ����������� �� ���� �������, �� ������� � ����� ������������ �����, ��������� �������������
			//���� ��� ����� �����, �� ������ ���� �� ���� ����� � ������� bool minusoid, ��������� �����
			//���� ��� ����� enter, �� ����������� ������� ������ ��� �������� �������� ������ � ������������ �������� � �����������
			//������� � ��������� ���������� �����
			switch (*ptr_keystroke)
			{
			case num0:
			{
				cout << 0;
				ans[numAns] = 0;
				break;
			}
			case num1:
			{
				cout << 1;
				ans[numAns] = 1;
				break;
			}
			case num2:
			{
				cout << 2;
				ans[numAns] = 2;
				break;
			}
			case num3:
			{
				cout << 3;
				ans[numAns] = 3;
				break;
			}
			case num4:
			{
				cout << 4;
				ans[numAns] = 4;
				break;
			}
			case num5:
			{
				cout << 5;
				ans[numAns] = 5;
				break;
			}
			case num6:
			{
				cout << 6;
				ans[numAns] = 6;
				break;
			}
			case num7:
			{
				cout << 7;
				ans[numAns] = 7;
				break;
			}
			case num8:
			{
				cout << 8;
				ans[numAns] = 8;
				break;
			}
			case num9:
			{
				cout << 9;
				ans[numAns] = 9;
				break;
			}
			case minus:
			{
				cout << '-';
				xNum++;
				minusoid = true;
				break;
			}
			case enter:
			{
				//���� ��� ����� �����, �� �������� ����� �� -1
				if (minusoid)
				{
					ans[numAns] *= -1;
				}
				
				//���� ��������� �������� ����� ������ �� ���������� ������, �� �������� "����������" ���������
				//���� ans_check ����� � ���, ��� ����� ����������
				//���������� ����
				if (ans[numAns] == accept.roots[0] || ans[numAns] == accept.roots[1] || accept.roots[2])
				{
					shoot(true, tower, obj.x_edge);
					ans_check[numAns] = true;
					*ptr_score += CUBIC_POINTS;
					numAns++;
				}
				//����� �������� ������������ ��������� � �������� ����
				else
				{
					shoot(false, tower, obj.x_edge);
					*ptr_score += WRONG_POINTS;
				}
				//���� ������������ ���� ��� ����� �����, �� ���������� ������ � ���������� ����� ������
				if (ans_check[0] && ans_check[1] && ans_check[2])
				{
					clearObject(obj);
					obj.alive = false;
					numAns = 0;
				}

				//�������������� �������� ���������� ������ �� �����, ������� �������� ����������. ��� ������������.
				ans[numAns] = 11;
				//���������� �������� ��������� ��� ����, ����� ��������� ����� �� ��������� ������������� �� -1
				minusoid = false;
				//���������� ���������� �����

				//������� ���������� ��������
				for (ubyte i = X_SCORE; i < X_SCORE + 8; i++)
				{
					gotoxy(i, Y_SCORE);
					cout << ' ';
				}
				//������� ����� �� ��������������� ����������� � ����������� ������
				gotoxy(X_SCORE, Y_SCORE);
				SetColor(BACKGR_COLOR, LABEL_COLOR);
				cout << *ptr_score;
				break;
			}
			}
		}
	}
	//���� ������ ������ �����, �� ���������� ���
	//�������� �������� � ����� � ��������� � � ����� �����������
	//���� � ����� �������� �� ��������, �� ���������� �
	if (obj.x_edge == tower.x)
	{
		clearObject(obj);

		tower.hp -= tower.hp;
		*ptr_hp = tower.hp;

		drawTower(tower);

		if (tower.hp == 0)
		{
			clearTower(tower);
		}
	}
	
	//������� ������� BOSS HAS COME
	for (ubyte i = X_BOSS_LABEL; i < X_BOSS_LABEL + 30; i++)
	{
		gotoxy(i, Y_BOSS_LABEL);
		cout << ' ';
	}
}