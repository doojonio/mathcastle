#include "menu.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include "game_cycle.h"
#include <stdlib.h>
using namespace std;
typedef unsigned char ubyte; //��� ���������� ����� ���� ������ �����������


//���� ������ � ���� ����������
enum keys
{
	esc = 27,
	enter = 13,
	up = 72,
	down = 80,
	lft = 75,
	rght = 77
};

//���������� ������� � ������ ����� �������
enum coordConsole
{
	L_X = 1,
	R_X = 118,
	UP_Y = 0,
	DN_Y = 29
};

//���� �������� ������� ASCII
enum charCode
{	/*l - �����, r - ������, Corn - ����, Facet - �����*/
	lup_Corn = 201,
	ldn_Corn = 200,
	rup_Corn = 187,
	rdn_Corn = 188,
	hor_Facet = 205,
	vert_Facet = 186
};

//������������ ���������
const string l_cursor = ">>>";
const string r_cursor = "<<<";
const string d_cursor = "   ";
const ubyte y_mI1 = 20, y_mI2 = 22, y_mI3 = 24, y_mI4 = 26; //���������� �� y ������� ����

//������ �������� ������ ������������ � ����
ubyte PLAYERCHOICE = 0;

//������� ������� � ����������� (x,y)
void gotoxy(ubyte X, ubyte Y)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position;
	position.X = X;
	position.Y = Y;
	SetConsoleCursorPosition(hCon, position);
}

//����� ����� �� �������� �����������
void drawFrame(ubyte l_X, ubyte r_X, ubyte up_Y, ubyte dn_Y)
{
	//������ ���� ����� �������
	gotoxy(l_X, up_Y);
	cout << char(lup_Corn);
	gotoxy(l_X, dn_Y);
	cout << char(ldn_Corn);
	//������ ������� � ������ �����
	for (ubyte i = l_X + 1; i <= r_X - 1; i++)
	{
		gotoxy(i, up_Y);
		cout << char(hor_Facet);
		gotoxy(i, dn_Y);
		cout << char(hor_Facet);
	}
	//������ ����� � ������ �����
	for (ubyte i = up_Y + 1; i <= dn_Y - 1; i++)
	{
		gotoxy(l_X, i);
		cout << char(vert_Facet);
		gotoxy(r_X, i);
		cout << char(vert_Facet);
	}
	//������ ������ ����
	gotoxy(r_X, up_Y);
	cout << char(rup_Corn);
	gotoxy(r_X, dn_Y);
	cout << char(rdn_Corn);
}

//����� ������ �� �����
void art(ubyte x, ubyte y, string name)
//x,y - ���������� ������ ������ ������, name - ���� � ������� "./name.txt" 
{
	string art; //��������� ������ � ����������
	ifstream file(name); //���� �� �������� ������
	ubyte i = y; //� ����� ������� �������� �������� �����
	while (getline(file, art)) //���� �� ��������� ����� ����� ������ ��������� ������ � ���������� (art)
	{
		gotoxy(x, i); //�������� ������� �� y ���� ����� ������ ����� �������
		cout << art << endl; //������� �� �����
		i++; //������� �� y ����
	}
	file.close(); //����������� ��������� ���� ��� �� �� ��������� ���
}

//����� ������� ����, ��������
void menuItem()
{
	const ubyte x_mI1 = 57, x_mI2 = 54, x_mI3 = 57, x_mI4 = 57; //���������� �� x ������� ����
	//�� y ������� ��� ���������� ����������, ��� ����������� ��� ����������� ������ ������������
	const string mI1 = "PLAY", mI2 = "SCOREBOARD", mI3 = "HELP", mI4 = "EXIT";

	drawFrame(L_X, R_X, UP_Y, DN_Y); //������� ����� �������� ����
	art(9, 7, "./art.txt");			//������� �������� �� ����� art.txt

	//������� ������ ����
	gotoxy(x_mI1, y_mI1);
	cout << mI1;
	gotoxy(x_mI2, y_mI2);
	cout << mI2;
	gotoxy(x_mI3, y_mI3);
	cout << mI3;
	gotoxy(x_mI4, y_mI4);
	cout << mI4;
}


//������� ������ ����� � ������� �������� (�������)
void scoreWriter(char name[25], int SCORE)
{
	char buff[100] = ""; //������ ������ ��������, ������� ��� ������������ �� int � char
	_itoa_s(SCORE, buff, 100, 10); //����������� int � char[]
	//��������� ��� � ���� � ������ Players Score � ini ���� � ���������� exe-�����
	WritePrivateProfileString("Players Score", name, buff, ".\\mathCastle_scoreboard.ini");
}

//������� �����������
void scoreboard()
{
	system("cls"); //������  �������
	drawFrame(L_X, R_X, UP_Y, DN_Y); //����� �����
	drawFrame(29, 93, 9, 28); //����� ��� ������� �����
	art(32, 1, "./scoreboard.txt"); //��������� �������� ������� �����
	art(35, 10, "./mathCastle_scoreboard.ini");

	gotoxy(96, 28); //������ ��������� � ���, ��� ��������� �����
	cout << "To exit press ESCAPE";

	ubyte ent = 0;		//��� ������� �������
	while (ent != esc)	//��� ������� ESC ������� �� �����
	{
		ent = _getch();
	}

	system("cls"); //������ �������
}

//����� � �������
void help()
{
	system("cls"); //������ ������� �� ���������� ��������
	drawFrame(L_X, R_X, UP_Y, DN_Y);  //������ �����

									  //������ ����� ����������
	art(2, 2, "./help.txt");

	gotoxy(96, 28); //������ ��������� � ���, ��� ��������� �����
	cout << "To exit press ESCAPE";

	ubyte ent = 0;		//��� ������� �������
	while (ent != esc)	//��� ������� ESC ������� �� �����
	{
		ent = _getch();
	}

	system("cls");		//������� �������
}

//����� �� ���������
bool exit()
{
	drawFrame(94, 116, 22, 28); //������ ����� ������ ������� � �������

	//���������� ������� � ��������� ������
	ubyte x_QUE = 99, y_QUE = 24, x_ans1 = 101, x_ans2 = 110, y_ans = 26;
	string question = "ARE YOU SURE?", ans1 = "Yes", ans2 = "No"; //���������� � ������� ������� � ���������� ������

	gotoxy(x_QUE, y_QUE);
	cout << question;
	gotoxy(x_ans1, y_ans);
	cout << ans1;
	gotoxy(x_ans2, y_ans);
	cout << ans2;

	ubyte ent = 0;  //������ ��� ������� �������
	ubyte x = 97, y = 26;	   //���������� ����� ������ ��������� ������
	const ubyte lx_edge = 97, rx_edge = 106; //���������� �� x ��������� ������ ��� ����������� �������
	const ubyte move = 9; //���������� �������� ����� ��������� �������
	//������� ������� ����� ������ ��������� ������
	gotoxy(x, y);
	cout << l_cursor;

	while (ent != enter) //����������� ������� � exit()
	{
		ent = _getch(); //��� ������� �������
		switch (ent)    //�������� �������� ��� ������� ����� ����� � ������
		{
		case rght:
		{
			if (x < rx_edge) //����� ������� �� ����� ������ �����������
			{
				gotoxy(x, y);
				cout << d_cursor;
				gotoxy(x = x + move, y);
				cout << l_cursor;
			}
			break;
		}
		case lft:
		{
			if (x > lx_edge) //����� ������� �� ����� ����� �����������
			{
				gotoxy(x, y);
				cout << d_cursor;
				gotoxy(x = x - move, y);
				cout << l_cursor;
			}
			break;
		}
		}
	}

	if (x == lx_edge)   //���� ������ ����� Yes, �� ���������� �������� 0
		return 0;
	else
	{
		system("cls");//������� �������
		return 1; //����� ���������� 1
	}
}


//����������� ������� ����
void cursorMenu()
{
	menuItem();

	ubyte x1 = 49, x2 = 66, y = 20; //���������� ��������� ��������� >>> <<<
	ubyte yup_edge = 20, ydn_edge = 26; //������� � ������ ������� �� y
	ubyte move = 2; //���������� ������� ����� �������� ���� +1

	gotoxy(x1, y);
	cout << l_cursor;					//������ ������� ����� �� ������
	gotoxy(x2, y);
	cout << r_cursor;					//������ ������� ������ �� ������

	ubyte ent = 0;					//������ ��� ������� �������
	while (ent != enter)				//�����������, ���� �� ����� ����� Enter
	{
		ent = _getch(); //��� ������� �������

		//�������� �������� ��� ������ ����� � ����
		switch (ent)
		{
		case up: //�� ������� ����
			if (y > yup_edge) //����� �� ����� �� ����� ������� ���� ���� �����������
			{
				gotoxy(x1, y);
				cout << d_cursor;
				gotoxy(x2, y);
				cout << d_cursor;  //������ ���������� �������
				gotoxy(x1, y = y - move); //��������� ������ �� ������� �������� ������ ����
				cout << l_cursor;
				gotoxy(x2, y);
				cout << r_cursor;  //����� �����
			}
			break;
		case down: //�� ������� ����
			if (y < ydn_edge) // ����� �� ����� �� ����� ������� ���� ���� �����������
			{
				gotoxy(x1, y);
				cout << d_cursor;
				gotoxy(x2, y);
				cout << d_cursor;  //������ ���������� �������
				gotoxy(x1, y = y + move); //��������� ������ �� ������� ������� ������ ����
				cout << l_cursor;
				gotoxy(x2, y);
				cout << r_cursor;  //����� �����
				break;
			}
		}
	}

	PLAYERCHOICE = y; //��������� � PLAYERCHOICE ���������� �� y
}

//����������� ������ ������������
ubyte choice()
{
	switch (PLAYERCHOICE)
	{
	case y_mI1: //PLAY
		return 1;
		break;
	case y_mI2: //SCOREBOARD
		return 2;
		break;
	case y_mI3: //HELP
		return 3;
		break;
	case y_mI4: //EXIT
		return 4;
		break;
	}
}