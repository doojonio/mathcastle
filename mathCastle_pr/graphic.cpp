//������� ��� ��������� ����������, ��������, ������� � �.�.

#include <iostream>		//cout
#include <windows.h>	//SetConsoleTextAttribute(...), HANDLE
#include "menu.h"	//drawFrame(...), gotoxy(...)
#include "graphic.h"

using std::cout;
using std::endl;

//��� ���������� ����� ���� ������ ��������� ����������
typedef unsigned char ubyte;

const ubyte
CLEAR_SPEED = 150,			//�������� ��������� "�����������" ����� � �������� (��� ������ ��������, ��� ��������)
CLEAR_DISTANCE_TOWER = 3,	//����������, �� ������� ��������� ����� ���������� ���� �� �����
CLEAR_DISTANCE_OBJ = 5;		//����������, �� ������� ��������� ������� ��������� ���� �� �����

//��������� ����� ����� ��������� �������� �� charColor, � ���� �� ���� �� BACKGR_COLOR
void SetColor(ubyte bckGrColor, ubyte charColor)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);							//�������� ����� �������
	SetConsoleTextAttribute(hCon, (WORD)((bckGrColor << 4) | charColor));	//����� ����� ������ � ���� ������� ��������
}

//��������� ����� �� ������� �������� �� �������� �����������
void drawBallFrame(ubyte l_X, ubyte r_X, ubyte up_Y, ubyte dn_Y)
{
	//�������� ��� ������� ��������
	const ubyte BALL_CODE = 248;
	
	//��������� ������� � ������ �������������� ������ �����
	for (ubyte i = l_X; i <= r_X; i++)
	{
		gotoxy(i, up_Y);
		cout << char(BALL_CODE);

		gotoxy(i, dn_Y);
		cout << char(BALL_CODE);
	}

	//��������� ������ � ����� ������������ ������ �����
	for (ubyte i = up_Y; i <= dn_Y; i++)
	{
		gotoxy(l_X, i);
		cout << char(BALL_CODE);

		gotoxy(r_X, i);
		cout << char(BALL_CODE);
	}
}

//�������� ������� �������� � ��������� ������� ( ������� )
void ClearArea(ubyte l_x, ubyte r_x, ubyte up_y, ubyte dn_y)
{
	//� ��������� ������� ���������� ������ ������ � �������� ��� �� ������ ������
	for (ubyte i = l_x; i <= r_x; i++)
	{
		for (ubyte j = up_y; j <= dn_y; j++)
		{
			gotoxy(i, j);
			cout << ' ';
		}
	}
}

//��������� ��������� ������� �� ��������� object
void drawObject(object_options object)
{
	const ubyte
		//�������� ����� ��������� ��� �������� ��������������
		LENGTH = object.task.size() + 4,			//����� ������� ����� ����� ����� ��������� + 2 ������� �� ����� � �� ������� �� ������� �����
		//���������� ������ �������������� ( ����� ��������� ���������� ������ �� �������� )
		WIDTH = 5,
		//�������� ������������ ��������� ��������� ��������������
		L_X = object.x, R_X = L_X + LENGTH,
		UP_Y = object.y, DN_Y = UP_Y + WIDTH,
		//�������� ���������� ������ ��������� ( ���������� )
		TASK_LABEL_X = L_X + 2, TASK_LABEL_Y = UP_Y + 2;

	//����� ����� �������� �������� � ���� �� ���� �� ����� �������
	SetColor(object.bckGrColor, object.color);
	//������������ ����� �������
	drawBallFrame(L_X, R_X, UP_Y, DN_Y);

	//��������� ������������ ������� ������� ��������� ( ����������, ����� ������ �� �������� ���� ������ ���� ��� �������� )
	for (ubyte i = L_X + 1; i < R_X; i++)
	{
		for (ubyte j = UP_Y + 1; j < DN_Y; j++)
		{
			gotoxy(i, j);
			cout << ' ';
		}
	}

	//������� ������ ��������� ������ �������
	gotoxy(TASK_LABEL_X, TASK_LABEL_Y);
	cout << object.task;
}

//��������� ��������� ����� �� ��������� tower
void drawTower(tower_options tower)
{
	//�������� ���������� �� ���������� ��� ���������� ��������� �����
	const ubyte
		L_X = tower.x, R_X = L_X + tower.length,
		UP_Y = tower.y, DN_Y = UP_Y + tower.width;
	//�������� ������ � ������� � ������������ �������� ����� ��� ��������� ���� ����������� ������ �����
	const unsigned int
		CURR_HP = static_cast<unsigned int>(tower.hp),
		MAX_HP = static_cast<unsigned int>(tower.max_hp);

	//����� ����� �������� �������� �� ����� �����
	SetColor(tower.bckGrColor, tower.color);
	//��������� ����� �����
	drawFrame(L_X, R_X, UP_Y, DN_Y);
	//����� ���������� �������� ������ �����
	gotoxy(L_X + tower.length / 2, UP_Y + tower.width / 2);
	cout << CURR_HP << '/' << MAX_HP << "  ";
}

//��������� ��������� "�����������" ����� �� ��������� tower 
void clearTower(tower_options tower)
{
	//����� ����� �������� �������� �� ����� �����
	SetColor(tower.bckGrColor, tower.color);

	//�������� ����������� ���������� ��� ��������� ����� ������� �������� �����
	ubyte
		up_l_x = tower.x, up_r_x = up_l_x + tower.length,
		up_up_y = tower.y, up_dn_y = up_up_y + tower.width / 2;
	//�� �� ����� �������� ��� ��������� ����� ������ �������� �����
	ubyte
		dn_l_x = up_l_x, dn_r_x = up_l_x + tower.length,
		dn_up_y = up_up_y + tower.width / 2, dn_dn_y = up_up_y + tower.width;
	//��������� ����������, ������ ���������� ����� �����������
	ubyte distance = 0;

	//���� ��������� "�����������", ���� ��������� ����� ����������� �� ��������� ������������ ��������
	while (distance != CLEAR_DISTANCE_TOWER)
	{	
		//��������� ����� ������� ���������
		drawFrame(up_l_x, up_r_x, up_up_y, up_dn_y);
		//����� �� ������� ������ ����� �, ���� ��������� ��� ���������� ���� �� �����, ������� �����, 
		//����������� ��������� ������� ��������
		for (ubyte i = up_l_x; i <= up_r_x; i++)
		{
			gotoxy(i, up_dn_y);
			cout << ' ';
			
			//���� �������� ���������� ���, �� ������� ����, ����������� ��������� ������� ��������
			if (distance != 0)
			{
				gotoxy(i, up_dn_y + 1);
				cout << ' ';
			}

			//������� ����, ����������� ��� �������� ������� ������ (������ ��������)
			if (i != up_r_x && i != up_l_x)		//����� ������� ������� ��� ����, ����� �� ��������� ������� ������� ������
			{
				gotoxy(i, up_up_y + 1);
				cout << ' ';
			}
		}
		//��������� ������ �������� �����
		drawFrame(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
		//����� �� ������� ������� ����� �, ���� ��������� ��� ����������, ������� �����
		//����������� ��������� ������ ��������
		for (ubyte i = dn_l_x; i <= dn_r_x; i++)
		{
			gotoxy(i, dn_up_y);
			cout << ' ';

			//���� �������� ����������, ������� ����������� ������ ��������� ���� ��� ��������
			if (distance != 0)
			{
				gotoxy(i, dn_up_y);
				cout << ' ';
			}

			//������� ����, ����������� ������ ������ ��� �������� ������ ��������
			if (i != dn_r_x && i != dn_l_x)	//����� ������� ��� ����, ����� �� ������� ������� ������� ������
			{
				gotoxy(i, dn_dn_y - 1);
				cout << ' ';
			}
		}

		Sleep(CLEAR_SPEED);

		//������� ��������� ��������� �� 1 �����
		up_up_y--;	
		up_dn_y--;

		//������ �������� ��������� �� 1 ����
		dn_up_y++;
		dn_dn_y++;

		//��������������, ��������� ����� ���� ����������
		distance++;
	}

	//� ����� ������� ������� ����� (������� ��������� � ������)
	ClearArea(up_l_x, up_r_x, up_up_y, up_dn_y);
	ClearArea(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
}

//��������� ��������� "�����������" ������� �� ��������� object
void clearObject(object_options object)
{
	//�������� ���� ���� � �������� �������� �� ����� �������
	SetColor(object.bckGrColor, object.color);

	//�������� ����� � ������ ������� ��� �������� ��������������
	const ubyte
		LENGTH = object.task.size() + 4,	//����� ������� ����� ����� ����� ��������� + 2 ������� �� ����� � �� ������� �� ������� �����
		WIDTH = 5;	

	//�������� �� ������ ����������� ���������� ��� ��������� ������� �������� �������
	ubyte
		up_l_x = object.x - 1, up_r_x = up_l_x + LENGTH,
		up_up_y = object.y, up_dn_y = up_up_y + WIDTH / 2;
	//�������� �� ������ ����������� ���������� ��� ��������� ������ �������� �������
	ubyte
		dn_l_x = up_l_x, dn_r_x = up_r_x,
		dn_up_y = up_up_y + WIDTH / 2, dn_dn_y = up_up_y + WIDTH;
	//������� ����������, ������ ���������� ����� ����������
	ubyte distance = 0;
	
	//���� ��������� "�����������", ���� ��������� ����� ����������� �� ��������� ������������ ��������
	while (distance != CLEAR_DISTANCE_OBJ)
	{
		//��������� ����� ������� ��������
		drawBallFrame(up_l_x, up_r_x, up_up_y, up_dn_y);
		//����� �� ������� ������ �����, � �����, ���� ��������� ���������� ���� �� �����, ��
		//������� �����, ����������� ��������� ��������
		for (ubyte i = up_l_x; i <= up_r_x; i++)
		{
			//������� ������ �����
			gotoxy(i, up_dn_y);
			cout << ' ';

			//���� ��������� ���������� ���� �� �����, ������� ������ ����, ����������� ������� ���������
			if (distance != 0)
			{
				gotoxy(i, up_dn_y + 1);
				cout << ' ';
			}

			//������� ����, ����������� ��� �������� �������� ������� ������ ( ������ ������� )
			if (i != up_r_x && i != up_l_x)
			{
				gotoxy(i, up_up_y + 1);
				cout << ' ';
			}
		}

		//��������� ������ �������� �������
		drawBallFrame(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
		//����� �� ������� ������� ����� �������� �, ���� ��������� ���������� ���, ��
		//������� �����, ����������� ������ ���������
		for (ubyte i = dn_l_x; i <= dn_r_x; i++)
		{
			//������� ������� ����� ������ ��������
			gotoxy(i, dn_up_y);
			cout << ' ';

			//���� �������� ���������� ���� �� �����, �� ������� ���� ������ �������� ������
			if (distance != 0)
			{
				gotoxy(i, dn_up_y - 1);
				cout << ' ';
			}

			//������� ���� ������ ��������, ����������� ��� �������� ������ ����� (������)
			if (i != dn_r_x && i != dn_l_x)
			{
				gotoxy(i, dn_dn_y - 1);
				cout << ' ';
			}
		}

		Sleep(CLEAR_SPEED);

		//������� �������� ���������� �����
		up_up_y--;
		up_dn_y--;

		//������ ����
		dn_up_y++;
		dn_dn_y++;

		//��������������, ��������� ����� ���� �������������
		distance++;
	}

	//������� ������� ������� � ���� (������� � ������ ��������)
	ClearArea(up_l_x, up_r_x, up_up_y, up_dn_y);
	ClearArea(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
}