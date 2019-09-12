//Меню
#include "menu.h"
#include <conio.h> 
#include<iostream>
#include <windows.h>

using namespace std;

//Координаты верхних и нижних углов консоли
enum coordConsole
{
	L_X = 1,
	R_X = 118,
	UP_Y = 0,
	DN_Y = 29
};

//Функция курсора консоли
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

//Главное меню
int main()
{
	ShowConsoleCursor(false); //Выключаю отображение курсора
	ubyte ret = 0; //Хранит резльтат выбора пользователя
	long int SCORE = 0; //Хранит очки набранные игроком из play()
	char name[25]; //Хранит имя игрока

	do {
		system("color 0E");		//Изменение цвета фона и шрифта
		cursorMenu();			//Вывожу меню и получаю выбор пользователя

		ret = choice();

		switch (ret)
		{
		case 1:
			system("cls"); //Очищаю консоль
			drawFrame(L_X, R_X, UP_Y, DN_Y); //Вывод рамки
			art(2, 3, "./player_name.txt"); //Вывод заставки
			gotoxy(41, 16);
			cout << "Enter your name (up to 24 characters):";
			gotoxy(50, 18);
			cin >> name; //Ввод имени игрока
			system("cls");

			drawFrame(L_X, R_X, UP_Y, DN_Y);
			drawFrame(3, 116, 22, 28);
			art(5, 24, "./interface.txt");
			gotoxy(27, 26);
			cout << name;

			SCORE = play(); //Функция play() вернет количество набранных очков
			
			system("cls");
			art(17, 9, "./gameover.txt"); //Вывод заставки о конце игры
			gotoxy(50, 20);
			cout << "YOUR SCORE /// " << name << ": " << SCORE; //Вывод количество набранных игроком очков
			
			scoreWriter(name, SCORE); //Запись имени игрока и набранных им очков в ini файл

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