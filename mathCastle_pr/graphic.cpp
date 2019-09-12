//Функции для отрисовки интерфейса, движений, графики и т.п.

#include <iostream>		//cout
#include <windows.h>	//SetConsoleTextAttribute(...), HANDLE
#include "menu.h"	//drawFrame(...), gotoxy(...)
#include "graphic.h"

using std::cout;
using std::endl;

//Для сокращения длины кода введем следующее сокращение
typedef unsigned char ubyte;

const ubyte
CLEAR_SPEED = 150,			//Скорость отрисовки "уничтожения" башен и объектов (чем больше значение, тем медленее)
CLEAR_DISTANCE_TOWER = 3,	//Расстояние, на которое половинки башен отдаляются друг от друга
CLEAR_DISTANCE_OBJ = 5;		//Расстояние, на которое половинки объекта оталяются друг от друга

//Процедура смены цвета выводимых символов на charColor, и фона за ними на BACKGR_COLOR
void SetColor(ubyte bckGrColor, ubyte charColor)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);							//Получаем хэндл консоли
	SetConsoleTextAttribute(hCon, (WORD)((bckGrColor << 4) | charColor));	//Смена цвета текста и фона вводимы символов
}

//Отрисовка рамки из круглых символов по заданным координатам
void drawBallFrame(ubyte l_X, ubyte r_X, ubyte up_Y, ubyte dn_Y)
{
	//Получаем код круглых символов
	const ubyte BALL_CODE = 248;
	
	//Отрисовка верхней и нижних горизонтальных граней рамки
	for (ubyte i = l_X; i <= r_X; i++)
	{
		gotoxy(i, up_Y);
		cout << char(BALL_CODE);

		gotoxy(i, dn_Y);
		cout << char(BALL_CODE);
	}

	//Отрисовка правых и левых вертикальных граней рамки
	for (ubyte i = up_Y; i <= dn_Y; i++)
	{
		gotoxy(l_X, i);
		cout << char(BALL_CODE);

		gotoxy(r_X, i);
		cout << char(BALL_CODE);
	}
}

//Удаление видимых символов с выбранной площади ( области )
void ClearArea(ubyte l_x, ubyte r_x, ubyte up_y, ubyte dn_y)
{
	//В выбранной области перебираем каждый символ и заменяем его на пустой символ
	for (ubyte i = l_x; i <= r_x; i++)
	{
		for (ubyte j = up_y; j <= dn_y; j++)
		{
			gotoxy(i, j);
			cout << ' ';
		}
	}
}

//Процедура отрисовки объекта по настройке object
void drawObject(object_options object)
{
	const ubyte
		//Выясняем длину уравнения для подгонки прямоугольника
		LENGTH = object.task.size() + 4,			//Длина объекта будет равна длине уравнение + 2 отступа по бокам и по символу на боковые грани
		//Выставляем ширину прямоугольника ( чтобы уравнение находилось строго по середине )
		WIDTH = 5,
		//Выясняем координатные параметры рисуемого прямоугольника
		L_X = object.x, R_X = L_X + LENGTH,
		UP_Y = object.y, DN_Y = UP_Y + WIDTH,
		//Выясняем координаты записи уравнения ( посередине )
		TASK_LABEL_X = L_X + 2, TASK_LABEL_Y = UP_Y + 2;

	//Смена цвета вводимых символов и фона за ними на цвета объекта
	SetColor(object.bckGrColor, object.color);
	//Отрисовываем рамку объекта
	drawBallFrame(L_X, R_X, UP_Y, DN_Y);

	//Заполняем внутренность объекта пустыми символами ( необходимо, чтобы объект не оставлял след внутри себя при движении )
	for (ubyte i = L_X + 1; i < R_X; i++)
	{
		for (ubyte j = UP_Y + 1; j < DN_Y; j++)
		{
			gotoxy(i, j);
			cout << ' ';
		}
	}

	//Выводим запись уравнение внутри объекта
	gotoxy(TASK_LABEL_X, TASK_LABEL_Y);
	cout << object.task;
}

//Процедура отрисовки башни по настройке tower
void drawTower(tower_options tower)
{
	//Выясняем координаты из полученных для дальнейшей отрисовки рамки
	const ubyte
		L_X = tower.x, R_X = L_X + tower.length,
		UP_Y = tower.y, DN_Y = UP_Y + tower.width;
	//Получаем данные о текущем и максимальном здоровье башни для выведения этих показателей внутри башни
	const unsigned int
		CURR_HP = static_cast<unsigned int>(tower.hp),
		MAX_HP = static_cast<unsigned int>(tower.max_hp);

	//Смена цвета вводимых символов на цвета башни
	SetColor(tower.bckGrColor, tower.color);
	//Отрисовка рамки башни
	drawFrame(L_X, R_X, UP_Y, DN_Y);
	//Вывод показателя здоровья внутри башни
	gotoxy(L_X + tower.length / 2, UP_Y + tower.width / 2);
	cout << CURR_HP << '/' << MAX_HP << "  ";
}

//Процедура отрисовки "уничтожения" башни по настройке tower 
void clearTower(tower_options tower)
{
	//Смена цвета вводимых символов на цвета башни
	SetColor(tower.bckGrColor, tower.color);

	//Выясняем необходимые координаты для отрисовки рамки ВЕРХНЕЙ половины башни
	ubyte
		up_l_x = tower.x, up_r_x = up_l_x + tower.length,
		up_up_y = tower.y, up_dn_y = up_up_y + tower.width / 2;
	//То же самое выясняем для отрисовки рамки НИЖНЕЙ половины башни
	ubyte
		dn_l_x = up_l_x, dn_r_x = up_l_x + tower.length,
		dn_up_y = up_up_y + tower.width / 2, dn_dn_y = up_up_y + tower.width;
	//Объявляем переменную, равную расстоянию между половинками
	ubyte distance = 0;

	//Цикл отрисовки "уничтожения", пока дистанция между половинками не достигнет настроенного значения
	while (distance != CLEAR_DISTANCE_TOWER)
	{	
		//Отрисовка рамки ВЕРХНЕЙ половинки
		drawFrame(up_l_x, up_r_x, up_up_y, up_dn_y);
		//Сразу же стираем НИЖНЮЮ грань и, если половинки уже отдалились друг от друга, стираем следы, 
		//оставленные движением верхней половины
		for (ubyte i = up_l_x; i <= up_r_x; i++)
		{
			gotoxy(i, up_dn_y);
			cout << ' ';
			
			//Если половины отдалились уже, до стираем след, оставленный движением верхней половины
			if (distance != 0)
			{
				gotoxy(i, up_dn_y + 1);
				cout << ' ';
			}

			//Стираем след, оставленный при движении верхней гранью (внутри половины)
			if (i != up_r_x && i != up_l_x)		//такое условие сделано для того, чтобы не стирались символы боковых граней
			{
				gotoxy(i, up_up_y + 1);
				cout << ' ';
			}
		}
		//Отрисовка НИЖНЕЙ половины башни
		drawFrame(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
		//Сразу же стираем ВЕРХНЮЮ грань и, если половинки уже отдалились, стираем следы
		//оставленные движением нижней половины
		for (ubyte i = dn_l_x; i <= dn_r_x; i++)
		{
			gotoxy(i, dn_up_y);
			cout << ' ';

			//Если половины отдалились, стираем оставленный нижней половиной след при движении
			if (distance != 0)
			{
				gotoxy(i, dn_up_y);
				cout << ' ';
			}

			//Стираем след, оставленный нижней гранью при движении нижней половины
			if (i != dn_r_x && i != dn_l_x)	//такое условие для того, чтобы не стереть символы боковых граней
			{
				gotoxy(i, dn_dn_y - 1);
				cout << ' ';
			}
		}

		Sleep(CLEAR_SPEED);

		//Верхняя половинка сдвинется на 1 вверх
		up_up_y--;	
		up_dn_y--;

		//Нижняя половина сдвинется на 1 вниз
		dn_up_y++;
		dn_dn_y++;

		//Соответственно, дистанция между ними увеличится
		distance++;
	}

	//В конце стираем остатки башни (верхнюю половинку и нижнюю)
	ClearArea(up_l_x, up_r_x, up_up_y, up_dn_y);
	ClearArea(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
}

//Процедура отрисовки "уничтожения" объекта по настройке object
void clearObject(object_options object)
{
	//Изменяем цвет фона и вводимых символов на цвета объекта
	SetColor(object.bckGrColor, object.color);

	//Выясняем длину и ширину объекта для подгонки прямоугольника
	const ubyte
		LENGTH = object.task.size() + 4,	//Длина объекта будет равна длине уравнение + 2 отступа по бокам и по символу на боковые грани
		WIDTH = 5;	

	//Выясняем по данным необходимые координаты для отрисовки верхней половины объекта
	ubyte
		up_l_x = object.x - 1, up_r_x = up_l_x + LENGTH,
		up_up_y = object.y, up_dn_y = up_up_y + WIDTH / 2;
	//Выясняем по данным необходимые координаты для отрисовки нижней половины объекта
	ubyte
		dn_l_x = up_l_x, dn_r_x = up_r_x,
		dn_up_y = up_up_y + WIDTH / 2, dn_dn_y = up_up_y + WIDTH;
	//Объявим переменную, равную расстоянию между половинами
	ubyte distance = 0;
	
	//Цикл отрисовки "уничтожения", пока дистанция между половинками не достигнет настроенного значения
	while (distance != CLEAR_DISTANCE_OBJ)
	{
		//Отрисовка рамки верхней половины
		drawBallFrame(up_l_x, up_r_x, up_up_y, up_dn_y);
		//Сразу же стираем нижнюю грань, а затем, если половинки сдвинулись друг от друга, то
		//стираем следы, оставленный движением половины
		for (ubyte i = up_l_x; i <= up_r_x; i++)
		{
			//Стираем НИЖНЮЮ грань
			gotoxy(i, up_dn_y);
			cout << ' ';

			//Если половинки сдвинулись друг от друга, стираем нижний след, оставленный верхней половиной
			if (distance != 0)
			{
				gotoxy(i, up_dn_y + 1);
				cout << ' ';
			}

			//Стираем след, оставленный при движении половины верхней гранью ( внутри который )
			if (i != up_r_x && i != up_l_x)
			{
				gotoxy(i, up_up_y + 1);
				cout << ' ';
			}
		}

		//Отрисовка нижней половины объекта
		drawBallFrame(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
		//Сразу же стираем ВЕРХНЮЮ грань половины и, если половинки сдвинулись уже, то
		//стираем следы, оставленные нижней половиной
		for (ubyte i = dn_l_x; i <= dn_r_x; i++)
		{
			//Стираем верхнюю грань нижней половины
			gotoxy(i, dn_up_y);
			cout << ' ';

			//Если половины сдвинулись друг от друга, то стираем след нижней половины сверху
			if (distance != 0)
			{
				gotoxy(i, dn_up_y - 1);
				cout << ' ';
			}

			//Стираем след нижней половины, оставленный при движении нижней грани (внутри)
			if (i != dn_r_x && i != dn_l_x)
			{
				gotoxy(i, dn_dn_y - 1);
				cout << ' ';
			}
		}

		Sleep(CLEAR_SPEED);

		//Верхняя половина сдвигается вверх
		up_up_y--;
		up_dn_y--;

		//Нижняя вниз
		dn_up_y++;
		dn_dn_y++;

		//Соответственно, дистанция между ними увеличивается
		distance++;
	}

	//Стираем остатки объекта с поля (верхняя и нижняя половины)
	ClearArea(up_l_x, up_r_x, up_up_y, up_dn_y);
	ClearArea(dn_l_x, dn_r_x, dn_up_y, dn_dn_y);
}