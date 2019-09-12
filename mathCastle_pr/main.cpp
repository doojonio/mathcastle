//����
#include "menu.h"
#include <conio.h> 
#include<iostream>
#include <windows.h>

using namespace std;

//���������� ������� � ������ ����� �������
enum coordConsole
{
	L_X = 1,
	R_X = 118,
	UP_Y = 0,
	DN_Y = 29
};

//������� ������� �������
BOOL ShowConsoleCursor(BOOL bShow)
{
	CONSOLE_CURSOR_INFO cci;
	HANDLE hStdOut;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return FALSE;
	if (!GetConsoleCursorInfo(hStdOut, &cci))
		return FALSE;
	cci.bVisible = bShow;
	if (!SetConsoleCursorInfo(hStdOut, &cci))
		return FALSE;
	return TRUE;
}

//������� ����
int main()
{
	ShowConsoleCursor(false); //�������� ����������� �������
	ubyte ret = 0; //������ �������� ������ ������������
	long int SCORE = 0; //������ ���� ��������� ������� �� play()
	char name[25]; //������ ��� ������

	do {
		system("color 0E");		//��������� ����� ���� � ������
		cursorMenu();			//������ ���� � ������� ����� ������������

		ret = choice();

		switch (ret)
		{
		case 1:
			system("cls"); //������ �������
			drawFrame(L_X, R_X, UP_Y, DN_Y); //����� �����
			art(2, 3, "./player_name.txt"); //����� ��������
			gotoxy(41, 16);
			cout << "Enter your name (up to 24 characters):";
			gotoxy(50, 18);
			cin >> name; //���� ����� ������
			system("cls");

			drawFrame(L_X, R_X, UP_Y, DN_Y);
			drawFrame(3, 116, 22, 28);
			art(5, 24, "./interface.txt");
			gotoxy(27, 26);
			cout << name;

			SCORE = play(); //������� play() ������ ���������� ��������� �����
			
			system("cls");
			art(17, 9, "./gameover.txt"); //����� �������� � ����� ����
			gotoxy(50, 20);
			cout << "YOUR SCORE /// " << name << ": " << SCORE; //����� ���������� ��������� ������� �����
			
			scoreWriter(name, SCORE); //������ ����� ������ � ��������� �� ����� � ini ����

			cin.get();
			cin.get();
			system("cls");
			break;
		case 2:
			scoreboard();
			break;
		case 3:
			help();
			break;
		case 4:
			ret = exit();
			break;
		}
	} while (ret != 0);

	return 0;
}