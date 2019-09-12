#include "menu.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include "game_cycle.h"
#include <stdlib.h>
using namespace std;
typedef unsigned char ubyte; //Для сокращения длины кода введем обозначение


//Коды клавиш в виде переменных
enum keys
{
	esc = 27,
	enter = 13,
	up = 72,
	down = 80,
	lft = 75,
	rght = 77
};

//Координаты верхних и нижних углов консоли
enum coordConsole
{
	L_X = 1,
	R_X = 118,
	UP_Y = 0,
	DN_Y = 29
};

//Коды символов таблицы ASCII
enum charCode
{	/*l - Левый, r - Правый, Corn - угол, Facet - грань*/
	lup_Corn = 201,
	ldn_Corn = 200,
	rup_Corn = 187,
	rdn_Corn = 188,
	hor_Facet = 205,
	vert_Facet = 186
};

//Используемые константы
const string l_cursor = ">>>";
const string r_cursor = "<<<";
const string d_cursor = "   ";
const ubyte y_mI1 = 20, y_mI2 = 22, y_mI3 = 24, y_mI4 = 26; //Координаты по y пунктов меню

//Хранит значение выбора пользователя в меню
ubyte PLAYERCHOICE = 0;

//Перенос курсора к координатам (x,y)
void gotoxy(ubyte X, ubyte Y)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position;
	position.X = X;
	position.Y = Y;
	SetConsoleCursorPosition(hCon, position);
}

//Вывод рамки по заданным координатам
void drawFrame(ubyte l_X, ubyte r_X, ubyte up_Y, ubyte dn_Y)
{
	//Вывожу углы левой стороны
	gotoxy(l_X, up_Y);
	cout << char(lup_Corn);
	gotoxy(l_X, dn_Y);
	cout << char(ldn_Corn);
	//Вывожу верхнюю и нижнюю грани
	for (ubyte i = l_X + 1; i <= r_X - 1; i++)
	{
		gotoxy(i, up_Y);
		cout << char(hor_Facet);
		gotoxy(i, dn_Y);
		cout << char(hor_Facet);
	}
	//Вывожу левую и правую грани
	for (ubyte i = up_Y + 1; i <= dn_Y - 1; i++)
	{
		gotoxy(l_X, i);
		cout << char(vert_Facet);
		gotoxy(r_X, i);
		cout << char(vert_Facet);
	}
	//Вывожу правые углы
	gotoxy(r_X, up_Y);
	cout << char(rup_Corn);
	gotoxy(r_X, dn_Y);
	cout << char(rdn_Corn);
}

//Вывод текста из файла
void art(ubyte x, ubyte y, string name)
//x,y - координаты начала вывода текста, name - путь в формате "./name.txt" 
{
	string art; //Записываю строку в переменную
	ifstream file(name); //Файл из которого читаем
	ubyte i = y; //С какой строчки начинать выводить текст
	while (getline(file, art)) //Пока не достигнут конец файла класть очередную строку в переменную (art)
	{
		gotoxy(x, i); //Смещение курсора на y вниз после записи новой строчки
		cout << art << endl; //Выводим на экран
		i++; //Опускаю на y вниз
	}
	file.close(); //Обязательно закрываем файл что бы не повредить его
}

//Вывод пунктов меню, заставки
void menuItem()
{
	const ubyte x_mI1 = 57, x_mI2 = 54, x_mI3 = 57, x_mI4 = 57; //Координаты по x пунктов меню
	//По y указаны как глобальные переменные, они понадобятся для определения выбора пользователя
	const string mI1 = "PLAY", mI2 = "SCOREBOARD", mI3 = "HELP", mI4 = "EXIT";

	drawFrame(L_X, R_X, UP_Y, DN_Y); //Выводит рамку главного меню
	art(9, 7, "./art.txt");			//Выводит заставку из файла art.txt

	//Выводит пункты меню
	gotoxy(x_mI1, y_mI1);
	cout << mI1;
	gotoxy(x_mI2, y_mI2);
	cout << mI2;
	gotoxy(x_mI3, y_mI3);
	cout << mI3;
	gotoxy(x_mI4, y_mI4);
	cout << mI4;
}


//Функция записи очков в таблицу рекордов (УДАЛИТЬ)
void scoreWriter(char name[25], int SCORE)
{
	char buff[100] = ""; //Хранит массив символов, который был преобразован из int в char
	_itoa_s(SCORE, buff, 100, 10); //Преобразует int в char[]
	//Записывае имя и очки в секцию Players Score в ini файл в директории exe-шника
	WritePrivateProfileString("Players Score", name, buff, ".\\mathCastle_scoreboard.ini");
}

//Таблица результатов
void scoreboard()
{
	system("cls"); //Очищаю  консоль
	drawFrame(L_X, R_X, UP_Y, DN_Y); //Вывод рамки
	drawFrame(29, 93, 9, 28); //Рамка для таблицы очков
	art(32, 1, "./scoreboard.txt"); //Офигенная заставка таблицы очков
	art(35, 10, "./mathCastle_scoreboard.ini");

	gotoxy(96, 28); //Вывожу сообщение о том, как вернуться назад
	cout << "To exit press ESCAPE";

	ubyte ent = 0;		//Код нажатой клавиши
	while (ent != esc)	//При нажатии ESC выходит из цикла
	{
		ent = _getch();
	}

	system("cls"); //Очищаю консоль
}

//Экран с помощью
void help()
{
	system("cls"); //Очищаю консоль от предыдущих символов
	drawFrame(L_X, R_X, UP_Y, DN_Y);  //Вывожу рамку

									  //Вывожу текст инструкции
	art(2, 2, "./help.txt");

	gotoxy(96, 28); //Вывожу сообщение о том, как вернуться назад
	cout << "To exit press ESCAPE";

	ubyte ent = 0;		//Код нажатой клавиши
	while (ent != esc)	//При нажатии ESC выходит из цикла
	{
		ent = _getch();
	}

	system("cls");		//Очищает консоль
}

//Выход из программы
bool exit()
{
	drawFrame(94, 116, 22, 28); //Вывожу рамку вокруг ответов и вопроса

	//Координаты вопроса и вариантов ответа
	ubyte x_QUE = 99, y_QUE = 24, x_ans1 = 101, x_ans2 = 110, y_ans = 26;
	string question = "ARE YOU SURE?", ans1 = "Yes", ans2 = "No"; //Переменные с текстом вопроса и вариантами ответа

	gotoxy(x_QUE, y_QUE);
	cout << question;
	gotoxy(x_ans1, y_ans);
	cout << ans1;
	gotoxy(x_ans2, y_ans);
	cout << ans2;

	ubyte ent = 0;  //Хранит код нажатой клавиши
	ubyte x = 97, y = 26;	   //Координаты места вывода вариантов ответа
	const ubyte lx_edge = 97, rx_edge = 106; //Координаты по x доступных границ для перемещения курсора
	const ubyte move = 9; //Количество символов между позициями курсора
	//Выводит стрелки перед первым вариантом ответа
	gotoxy(x, y);
	cout << l_cursor;

	while (ent != enter) //Перемещение курсора в exit()
	{
		ent = _getch(); //Код нажатой клавиши
		switch (ent)    //Алгоритм действий при нажатии клавш влево и вправо
		{
		case rght:
		{
			if (x < rx_edge) //Чтобы стрелка не пошла правее положенного
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
			if (x > lx_edge) //Чтобы стрелка не пошла левее положенного
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

	if (x == lx_edge)   //Если выбран пункт Yes, то возвращает значение 0
		return 0;
	else
	{
		system("cls");//Очищает консоль
		return 1; //Иначе возвращает 1
	}
}


//Перемещение курсора меню
void cursorMenu()
{
	menuItem();

	ubyte x1 = 49, x2 = 66, y = 20; //Переменные координат стрелочек >>> <<<
	ubyte yup_edge = 20, ydn_edge = 26; //Верхние и нижние границы по y
	ubyte move = 2; //Количество строчек между пунктами меню +1

	gotoxy(x1, y);
	cout << l_cursor;					//Вывожу стрелки слева от пункта
	gotoxy(x2, y);
	cout << r_cursor;					//Вывожу стрелки справа от пункта

	ubyte ent = 0;					//Хранит код нажатой клавиши
	while (ent != enter)				//Выполняется, пока не будет нажат Enter
	{
		ent = _getch(); //Код нажатой клавиши

		//Алгоритм действия для клавиш вверх и вниз
		switch (ent)
		{
		case up: //На позицию выше
			if (y > yup_edge) //Чтобы не вышел за рамки пунктов меню выше положенного
			{
				gotoxy(x1, y);
				cout << d_cursor;
				gotoxy(x2, y);
				cout << d_cursor;  //Стираю предыдущие стрелки
				gotoxy(x1, y = y - move); //Перемещаю курсор на позицию верхнего пункта меню
				cout << l_cursor;
				gotoxy(x2, y);
				cout << r_cursor;  //Рисую новые
			}
			break;
		case down: //На позицию ниже
			if (y < ydn_edge) // Чтобы не вышел за рамки пунктов меню ниже положенного
			{
				gotoxy(x1, y);
				cout << d_cursor;
				gotoxy(x2, y);
				cout << d_cursor;  //Стираю предыдущие стрелки
				gotoxy(x1, y = y + move); //Перемещаю курсор на позицию нижнего пункта меню
				cout << l_cursor;
				gotoxy(x2, y);
				cout << r_cursor;  //Рисую новые
				break;
			}
		}
	}

	PLAYERCHOICE = y; //Записываю в PLAYERCHOICE координату по y
}

//Возвращение выбора пользователя
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