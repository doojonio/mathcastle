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

//Константа для смены цветовой гаммы перед игрой
const char SYS_COLOR[] = "color F0";

//Константа надписи "PRESS ANY KEY FOR START"
const char PAKFS[] = "PRESS ANY KEY FOR START";

//Код символа пули
const unsigned short int BULLET_CODE = 253;

//Коды клавиш
enum keys
{
	esc = 27,															//Код клавиши "ESC"
	enter = 13,															//Код клавиши "ENTER"
	num0 = 48, num1, num2, num3, num4, num5, num6, num7, num8, num9,	//Коды клавиш с цифрами(от 0 до 9)
	minus = 45															//Код клавиши "-"
};

//Коды консольных цветов
enum ConsoleColor
{
	Black = 0,			//Код черного цвета
	Blue = 1,			//Код синего цвета
	Green = 2,			//Код зеленого цвета
	Cyan = 3,			//Код голубого цвета
	Red = 4,			//Код красного цвета
	Magenta = 5,		//Код пурпурного цвета
	Brown = 6,			//Код коричневого цвета
	LightGray = 7,		//Код светло-серого цвета
	DarkGray = 8,		//Код темно-серого цвета
	LightBlue = 9,		//Код светло-синего цвета
	LightGreen = 10,	//Код светло-зеленого цвета
	LightCyan = 11,		//Код светло-голубого цвета
	LightRed = 12,		//Код светло-красного цвета
	LightMagenta = 13,	//Код светло-пурпурного цвета
	Yellow = 14,		//Код желтого цвета
	White = 15			//Код белого цвета
};

//Цветовая гамма игры
enum Game_Color {
	BACKGR_COLOR = White,		//Цвет фона
	TOWER_COLOR = LightBlue,	//Цвет башен
	CORE_COLOR = LightBlue,		//Цвет ядра
	OBJ_COLOR = LightRed,		//Цвет объекта
	LABEL_COLOR = Black,		//Цвет надписи "Press any key for start" и показателя очков
	BOSS_LABEL_COLOR = Magenta,	//Цвет надписи "ATTENTION! BOSS HAS COME!"
	SHOT_TRUE = LightGreen,		//Цвет выстрела правильным ответом
	SHOT_FALSE = LightRed,		//Цвет выстрела неправильным ответом
};

//Настройки местоположения на координатной плоскости
enum Location_Settings
{
	//Х-координаты башен и ядра
	X_T1 = 50,
	X_T2 = X_T1 + 17,
	X_CORE = X_T2 + 20,

	//y-координаты башен и ядра
	Y_T = 6,
	Y_CORE = 3,

	X_OBJ_START = 3,	//Начальная x-координата объекта, с которой он начинает движение
	Y_OBJ = 7,			//y-координата объекта

	//Координаты надписи состояния счета
	X_SCORE = 76,
	Y_SCORE = 26,

	//Координаты надписи "Press any key for start"
	X_LABEL = 60,
	Y_LABEL = 20,

	//Координаты надписи "Attention! Boss is coming!"
	X_BOSS_LABEL = 20,
	Y_BOSS_LABEL = 15,
};

//Настройки размеров и хит-поинтов башен и ядра
enum Main_Options
{
	CORE_HP = 3,		//Здоровье ядра на момент начала игры
	CORE_LENGTH = 25,	//Длина рамки ядра
	CORE_WIDTH = 12,	//Ширина рамки ядра

	TOWER1_HP = 1,		//Здоровье первой башни на момент начала игры
	TOWER2_HP = 2,		//Здоровье второй башни на момент начала игры
	TOWER_LENGTH = 15,	//Длина рамки башен
	TOWER_WIDTH = 7,	//Ширина рамки башен

	//Внимание! Чем больше указано значение, тем медленее будет двигаться объект. И наоборот.
	LINEAR_SPEED = 200,		//Скорость движения линейных уравнений к башням
	QUADRATIC_SPEED = 1000,	//Скорость движения квадратных уравнений к башням
	CUBIC_SPEED = 5000,		//Скорость движения кубических уравнений к башням 
	BULLET_SPEED = 15,		//Скорость движения пули

	LINEAR_POINTS = 10,		//Очки, получаемые за решение линейного уравнения
	QUADRATIC_POINTS = 12,	//Очки, получаемые за один верно введенный корень квадратного уравнения 
	CUBIC_POINTS = 33,		//Очки, получаемые за один верно введенный корень кубического уравнения
	WRONG_POINTS = -10,		//Очки, "получаемые" за неверно введенный ответ

	FIRST_LEVEL = 50,		//Количество очков, необходимое набрать для 1 этапа (до квадратных)
	SECOND_LEVEL = 150,		//Количество очков, необходимое набрать для 2 этапа (до кубического и последующего рандома)

	LINEAR_PERCENT = 60,										//Процент выпадения линейного уравнения
	QUADRATIC_PERCENT = 30,										//Процент выпадения квадратного уравнения
	CUBIC_PERCENT = 100 - LINEAR_PERCENT - QUADRATIC_PERCENT,	//Процент выпадения кубического уравнения
};

void shoot(bool ans, tower_options tower, ubyte xEdgeObj);
void LinearToTower(tower_options tower, ubyte *ptr_hp, int*ptr_score, ubyte *ptr_keystroke);
void QuadraticToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke);
void CubicToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke);

//Функция игрового цикла. Возвращает количество очков
int play()
{
	//Объявляем структурные переменные башенных опций(для 2-х башен и ядра)
	tower_options t1, t2, core;

	//Инициализируем показатели 1 башни
	t1.bckGrColor = BACKGR_COLOR;
	t1.color = TOWER_COLOR;
	t1.length = TOWER_LENGTH;
	t1.width = TOWER_WIDTH;

	//Проинициализированные показатели первой башни будут такими же и у второй башни
	t2 = t1;

	//Инициализурем координаты 1-й башни
	t1.x = X_T1;
	t1.y = Y_T;
	//Инициализируем здоровье первой башни
	t1.hp = TOWER1_HP;
	t1.max_hp = t1.hp;

	//Инициализируем координаты 2-й башни
	t2.x = X_T2;
	t2.y = Y_T;
	//Инициализируем здоровье второй башни
	t2.hp = TOWER2_HP;
	t2.max_hp = t2.hp;

	//Инициализируем показатели ядра
	core.bckGrColor = BACKGR_COLOR;
	core.color = TOWER_COLOR;
	core.hp = CORE_HP;
	core.max_hp = core.hp;
	core.length = CORE_LENGTH;
	core.width = CORE_WIDTH;
	core.x = X_CORE;
	core.y = Y_CORE;

	//Объявляем указатель на здоровье некоторой башни или ядра для изменения их хит-поинтов вне используемых функций
	ubyte *ptr_hp;

	//Изменяем цвет фона на белый, цвет символов на черный
	system(SYS_COLOR);

	//Готовим поле битвы (отрисовываем две башни и ядро)
	drawTower(t1);
	drawTower(t2);
	drawTower(core);


	int score = 0;						//Объявим переменную, в которой будем хранить состояние счета
	int *ptr_score = &score;			//Объявим указатель на переменную счета для изменения её вне используемых функций

	ubyte keystroke;					//Объяим переменную, в которой будем заносить код нажатой клавиши	
	ubyte *ptr_keystroke = &keystroke;	//Объявим указатель на переменную нажатия клавиш для изменения её вне используемых функций

	//Выводим надпись "Press any key for start"
	gotoxy(X_LABEL, Y_LABEL);
	SetColor(BACKGR_COLOR, LABEL_COLOR);
	cout << PAKFS;
	_getch();				//Ожидание нажатия клавиши
	
	//После нажатия стираем надпись PAKFS
	for (ubyte i = X_LABEL; i < X_LABEL + 30; i++)
	{
		gotoxy(i, Y_LABEL);
		cout << ' ';
	}
	
	/*
	Здесь начинается полный игровой цикл.
	Сначала пользователю предлгается решать линейные уравнения, пока он не наберет некоторое настроенное очков.
	Затем ему предлагается решить 2 квадратных уравнения, и далее, с помощью линейных уравнений набрать настроенное
	количество очков для второго этапа.
	После этого предлагается решить одно кубическое уравнения.
	И после того, как пользователь решил или не решил кубическое уравнение, начинается рандомная генерация со следующей
	вероятностью:
				60% - линейное уравнение
				31% - квадратное уравнение
				9%	- кубическое уравнение 
	*/

	//Цикл для первого этапа - пока не наберет нужное количество очков, не нажмет esc или не проиграет (здоровья ядра станет равно нулю)
	while (score < FIRST_LEVEL && keystroke != esc && core.hp != 0) 
	{
		//Если первая башня жива, то линейное уравнение направляем на неё
		if (t1.hp != 0)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t1.hp;	
			LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
		}
		//Если не жива, то проверяем здоровье второй башни и, если вторая башня жива направляем на неё
		else if (t2.hp != 0)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t2.hp;
			LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
		}
		//Если обе башни не живы, то направляем объеко на ядро
		//Оно должно быть живо, иначе бы мы в цикл даже не вошли
		else
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &core.hp;
			LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
		}
	}

	//После первого этапа предложим пользователю решить 2 квадратных уравнения, счередованных с 1 линейным
	for (ubyte i = 0; i < 3; i++)
	{
		//Если первая башня жива, то направляем на неё линейное или квадратно уравнение ( в зависимости от i )
		if (t1.hp != 0 && keystroke != esc)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t1.hp;
			//Теперь, в зависимости от i, выбираем уравнение, которое пойдет на башню
			//Чтобы между квадратными было линейное, мы, если i = 0 и 2 направим квадратное,
			//а если i = 1 (между 0 и 2), направим линейное.
			//Таким образом, между двумя квадратными будет одно линейное уравнение
			if (i == 1)
			{
				LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
			else
			{
				QuadraticToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		//Если первая башня мертва, но вторая башня жива, то направляем на неё уравнение
		else if (t2.hp != 0 && keystroke != esc)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t2.hp;
			//По такому же принципу, как с 1 башней, выбираем, какое уравнение полетит на башню
			if (i == 1)
			{
				LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
			else
			{
				QuadraticToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		//Если обе башни мертвы, то направляем на ядро, если оно живое
		else if (core.hp != 0 && keystroke != esc)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &core.hp;
			//Так же выбираем уравнение, которое полетит на ядро
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

	//Второй этап - это решение линейных уравнений, пока пользователь не наберет настроенное количество очков
	//и не нажмет esc, и не проиграет (ядро не разрушится)
	while (score < SECOND_LEVEL && keystroke != esc && core.hp != 0)
	{
		//Если первая башня жива, то направляем на неё уравнение
		if (t1.hp != 0)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t1.hp;
			LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
		}
		//Иначе, если жива вторая башня, то направляем на неё
		else if (t2.hp != 0)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t2.hp;
			LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
		}
		//Если обе башни мертвы, то направляем на ядро
		//Оно точно живо, иначе бы игра не вошла в этот цикл
		else
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &core.hp;
			LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
		}
	}

	//Когда пользователь набрал настроенное количество очков второго этапа, 
	//ему предлагается решить одно кубическое уравнение
	
	//Если жива первая башня, то направляем кубическое уравнение на него:
	if (t1.hp != 0 && keystroke != esc)
	{
		//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
		ptr_hp = &t1.hp;
		CubicToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
	}
	//Иначе, если жива вторая башня, то направляем на неё
	else if (t2.hp != 0 && keystroke != esc)
	{
		//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
		ptr_hp = &t2.hp;
		CubicToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
	}
	//Если обе башни мертвы, проверяем, живо ли ядро
	//Если живо, то направляем на него уравнение
	else if (core.hp != 0 && keystroke !=esc)
	{
		ptr_hp = &core.hp;
		CubicToTower(core, ptr_hp, ptr_score, ptr_keystroke);
	}

	//Если пользователь пережил "нападение" кубического уравнения,
	//то он фактически прошел контролируемую часть игры.
	//После этого начинается "рандомная" часть игры
	//Уравнения будут генерироваться в случайном порядке с разным процентами выпадения
	
	//Пока игрок не проиграет (здоровье ядра не станет равно нулю) и не нажмет esc
	while (core.hp != 0 && keystroke != esc)
	{
		//Посев зерна рандома, зависимого от текущего времени
		srand(static_cast<unsigned int>(time(NULL)));
		//Получаем случайное число и берем его остаток от деления на 100, затем прибавляем 1
		//Таким образом мы буем получать числа в интервале от 1 до 100
		ubyte randValue = rand() % 100 + 1;
		
		//Если первая башня жива и не нажималась клавиша esc, направляем случайное уравнение на первую башню
		if (t1.hp != 0 && keystroke != esc)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t1.hp;
			//Если полученное случайно число попало в процент выпадения линейного, то направляем линейное на башню
			if (randValue <= LINEAR_PERCENT)
			{
				LinearToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
			//Иначе, если попало в процент выпадения квадратного (допустим от 60 до 90, т.е. 90-60=30%),
			//то генерируем квадратное
			else if (randValue <= LINEAR_PERCENT+QUADRATIC_PERCENT)
			{
				QuadraticToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
			//Если не попало ни в процент линейного, ни в процент квадратного,
			//значит, попало в кубическое. Тогда генерируем кубическое
			else
			{
				CubicToTower(t1, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		//Иначе(если первая башня мертва), то направляем случайное уравнение на вторую башню
		else if (t2.hp != 0 && keystroke != esc)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &t2.hp;
			//Если полученное случайно число попало в процент выпадения линейного, то направляем линейное на башню
			if (randValue <= LINEAR_PERCENT)
			{
				LinearToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
			//Иначе, если попало в процент выпадения квадратного (допустим от 60 до 90, т.е. 90-60=30%),
			//то генерируем квадратное
			else if (randValue <= LINEAR_PERCENT + QUADRATIC_PERCENT)
			{
				QuadraticToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
			//Если не попало ни в процент линейного, ни в процент квадратного,
			//значит, попало в кубическое. Тогда генерируем кубическое
			else
			{
				CubicToTower(t2, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		else if (core.hp != 0 && keystroke != esc)
		{
			//Используем указатель, чтобы в функции play тоже изменялось здоровье башни
			ptr_hp = &core.hp;
			//Если полученное случайно число попало в процент выпадения линейного, то направляем линейное на башню
			if (randValue <= LINEAR_PERCENT)
			{
				LinearToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
			//Иначе, если попало в процент выпадения квадратного (допустим от 60 до 90, т.е. 90-60=30%),
			//то генерируем квадратное
			else if (randValue <= LINEAR_PERCENT + QUADRATIC_PERCENT)
			{
				QuadraticToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
			//Если не попало ни в процент линейного, ни в процент квадратного,
			//значит, попало в кубическое. Тогда генерируем кубическое
			else
			{
				CubicToTower(core, ptr_hp, ptr_score, ptr_keystroke);
			}
		}
		
	}

	//Когда игрок либо проиграл, либо нажал esc, то мы возвращаем количество очков, набранное им
	return score;
}

//Функция выстрела из башни tower с верностью ответа ans,
//координатой края объекта xEdgeObj
void shoot(bool ans, tower_options tower, ubyte xEdgeObj)
{
	//Создаем переменные, в которых будем хранить координаты пули
	ubyte xBullet, yBullet;
	//Задаем место, откуда будет вылетать пуля. Зависит от координат и размеров башни
	xBullet = tower.x - 2;
	yBullet = tower.y + tower.width / 2;

	//Если ответ верный, то стреляем заданным цветов для правильных, иначе стреляем заданным цветом для неправильных.
	if (ans == true)
	{
		SetColor(BACKGR_COLOR, SHOT_TRUE);
	}
	else
	{
		SetColor(BACKGR_COLOR, SHOT_FALSE);
	}
	
	//Цикл движения пули. Движется, пока не достигнет объекта ( его края )
	while (xBullet != xEdgeObj - 2)
	{	
		//Переходим к координатам пули и отрисовываем символ
		gotoxy(xBullet, yBullet);
		cout << char(BULLET_CODE);
		
		//Сдвигаем пулю на 1 влево
		xBullet--;
		
		//Стираем след пули
		gotoxy(xBullet + 2, yBullet);
		cout << ' ';

		Sleep(BULLET_SPEED);
	}
}

//Процедура создания объекта с линейным уравнением и его движения к башне tower,
//К параметрам функции добавлены указатели на здоровье, счет и переменную нажатия кнопки
//для того, чтобы изменения выше перечисленных происходили в вызвавшей функции
void LinearToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke)
{
	//Генерируем линейное уравнение и заносим результат в специально созданную структурную переменную
	gen_result accept = generateLinear();

	//Создаем структурную переменную опций объекта и инициализируем её
	object_options obj;
	obj.task = accept.task;
	obj.x = X_OBJ_START;
	obj.y = Y_OBJ;
	obj.x_edge = obj.x + obj.task.size() + 4;
	obj.bckGrColor = BACKGR_COLOR;
	obj.color = OBJ_COLOR;
	obj.alive = true;

	//Булевая переменная, необходимая для проверки нажатия на минус ( чтобы потом корень умножить на -1 )
	bool minusoid = false;
	
	//Переменная, хранящая в себя ответ пользователя
	short int ans = 11;

	//Цикл движения объекта к башне tower (пока его край не достигнет башни и сам он жив)
	while (obj.x_edge != tower.x && obj.alive)
	{
		//Отрисовка объекта
		drawObject(obj);
		//Сдвигаем его вправо на 1
		obj.x++;
		//Вычисляем x-координату края объекта
		obj.x_edge = obj.x + obj.task.size() + 4;
		//Стираем след, оставляемый объектом
		for (ubyte i = Y_OBJ; i < Y_OBJ + 6; i++)
		{
			gotoxy(obj.x - 2, i);
			cout << ' ';
		}
		
		Sleep(LINEAR_SPEED);

		//Проверка нажатия клавиши
		if (_kbhit())
		{
			//Заносим в keystroke код нажатой клавиши
			*ptr_keystroke = _getch();
			
			//Если была нажата esc, то выходим из данной функции и далее по цепочке из функции play()
			if (*ptr_keystroke == esc)
				return;

			//Вычисляем координаты, где будет выводиться вводимые числа пользователя
			ubyte
				xNum = tower.x + tower.length / 2,
				yNum = tower.y + tower.width / 2 + 1;

			//Переходим к этим координатам и меняем цвет на заданные цвета для надписей
			gotoxy(xNum, yNum);
			SetColor(BACKGR_COLOR, LABEL_COLOR);

			//Команда выяснения введенной пользователем цифры
			//В зависимости от кода клавиши, мы заносим в ответ пользователя число, введенное пользователем
			//Если был нажат минус, то делаем даем об этом знать с помощью bool minusoid, созданной ранее
			//Если был нажат enter, то запускается цепочка команд для проверки верности ответа и последующего выстрела и уничтожения
			//объекта и изменения количества очков
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
				//Если был нажат минус, то умножаем число на минус
				if (minusoid)
				{
					ans *= -1;
				}
				//Проверка верности корня
				//Если верно, то стреляет "правильным ответом", уничтожает объект и прибавляются очки
				if (ans == accept.roots[0])
				{
					shoot(true, tower, obj.x_edge);
					clearObject(obj);
					obj.alive = false;
					*ptr_score += LINEAR_POINTS;
				}
				//Если неверно, то стреляет "неправильным ответом", и отнимает очки
				else
				{
					shoot(false, tower, obj.x_edge);
					*ptr_score += WRONG_POINTS;
				}
				
				//Инициализируем значение переменной ответа на такое, которое получить невозможно. Для подстраховки.
				ans = 11;
				//Вовзращаем исходное значение минусоида.
				minusoid = false;
				
				//Обновление показателя очков

				//Стираем предыдущее значение
				for (ubyte i = X_SCORE; i < X_SCORE + 8; i++)
				{
					gotoxy(i, Y_SCORE);
					cout << ' ';
				}
				//Переходим к координатам надписи счета, меняем цвет на цвет надписей и выводим счет
				gotoxy(X_SCORE, Y_SCORE);
				SetColor(BACKGR_COLOR, LABEL_COLOR);
				cout << *ptr_score;
				break;
			}
			}
		}
	}
	//Если все же объект успел достичь башни, то уничтожаем объект, отнимаем здоровье у башни и отрисовываем башню
	//с новым показателем. Если у башни не осталось здоровья, то уничтожаем её
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

//Процедура создания объекта с квадратным уравнением и его движения к башне tower,
//К параметрам функции добавлены указатели на здоровье, счет и переменную нажатия кнопки
//для того, чтобы изменения выше перечисленных происходили в вызвавшей функции
void QuadraticToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke)
{
	//Генерируем квадратное уравнение и результат записываем в структурную переменную
	gen_result accept = generateQuadratic();

	//Создание структурной переменной объектный опций и её инициализация
	object_options obj;
	obj.task = accept.task;
	obj.x = X_OBJ_START;
	obj.y = Y_OBJ;
	obj.x_edge = obj.x + obj.task.size() + 4;
	obj.bckGrColor = BACKGR_COLOR;
	obj.color = OBJ_COLOR;
	obj.alive = true;

	//Булевая переменная, необходимая для проверки нажатия на минус ( чтобы потом корень умножить на -1 )
	bool minusoid = false;
	//Создаем массив, в котором будем хранить ответы пользователя
	short int ans[2] = { 11, 11 };
	//Создаем переменную, в которой будем хранить НОМЕР ответа пользователя ( чтобы не записать ответ в уже правильно записанную яйчейку )
	ubyte numAns = 0;
	//Создаем булевый массив, чтобы проверять сколько ответов правильно введенно ( объект уничтожиться только, если все ans_check = true )
	bool ans_check[2] = { false, false };

	//Цикл движения объекта к башне tower (пока его край не достигнет башни и сам он жив)
	while (obj.x_edge != tower.x && obj.alive)
	{
		//Отрисовка объекта по настрйоке obj
		drawObject(obj);
		//Сдвигаем объект на 1 вправо
		obj.x++;
		//Высчитываем снова координату края объекта
		obj.x_edge = obj.x + obj.task.size() + 4;

		//Стираем след, оставляемый объектом при движении
		for (ubyte i = Y_OBJ; i < Y_OBJ + 6; i++)
		{
			gotoxy(obj.x - 2, i);
			cout << ' ';
		}
		
		Sleep(QUADRATIC_SPEED);

		//Проверка нажатия клавиши
		if (_kbhit())
		{
			//Записываем в keystroke нажатую клавишу
			*ptr_keystroke = _getch();

			//Если была нажата esc, то выходим из функции (далее выходит из play() по цепочке )
			if (*ptr_keystroke == esc)
				return;

			//Вычисляем координаты записи введенной пользователем цифры
			ubyte
				xNum = tower.x + tower.length / 2,
				yNum = tower.y + tower.width / 2 + 1;

			//Переходим к вычисленным координатам и изменяет цвет на заданный для надписей
			gotoxy(xNum, yNum);
			SetColor(BACKGR_COLOR, LABEL_COLOR);

			//Команда выяснения введенной пользователем цифры
			//В зависимости от кода клавиши, мы заносим в ответ пользователя число, введенное пользователем
			//Если был нажат минус, то делаем даем об этом знать с помощью bool minusoid, созданной ранее
			//Если был нажат enter, то запускается цепочка команд для проверки верности ответа и последующего выстрела и уничтожения
			//объекта и изменения количества очков
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
				//Если был нажат минус, умножаем на -1 ответ
				if (minusoid)
				{
					ans[numAns] *= -1;
				}

				//Если введенный ответ равен одному из правильных корней, то стреляем правильным ответом
				//Даем знать ans_check, что ответ правильный
				//Увеличиваем очки
				if (ans[numAns] == accept.roots[0] || ans[numAns] == accept.roots[1])
				{
					shoot(true, tower, obj.x_edge);
					ans_check[numAns] = true;
					*ptr_score += QUADRATIC_POINTS;
					numAns++;
				}
				//Иначе стреляем неправильным ответом и убавляем очки
				else
				{
					shoot(false, tower, obj.x_edge);
					*ptr_score += WRONG_POINTS;
				}

				//Если пользователь ввел два правильных ответа, то уничтожаем объект и заново инициализируем номер ответа пользователя
				if (ans_check[0] == true && ans_check[1] == true)
				{
					clearObject(obj);
					obj.alive = false;
					numAns = 0;
				}

				//Инициализируем значение переменной ответа на такое, которое получить невозможно. Для подстраховки.
				ans[numAns] = 11;
				//Заново иницилазируем минусоид (чтобы следующийо ответ не был отрицательным)
				minusoid = false;
				//Обновление показателя очков

				//стираем предыдущее значение
				for (ubyte i = X_SCORE; i < X_SCORE + 8; i++)
				{
					gotoxy(i, Y_SCORE);
					cout << ' ';
				}
				//Выводим новое с соответствующим цветом и заданными координатами
				gotoxy(X_SCORE, Y_SCORE);
				SetColor(BACKGR_COLOR, LABEL_COLOR);
				cout << *ptr_score;
				break;
			}
			}
		}
	}
	//Если объект достиг башни, то уничтожаем объект, отнимаем жизни у башни и вырисовываем башню с новым показателем
	//Если у башни не осталось здоровья, то уничтожаем башню
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

//Процедура создания объекта с кубическим уравнением и его движения к башне tower,
//К параметрам функции добавлены указатели на здоровье, счет и переменную нажатия кнопки
//для того, чтобы изменения выше перечисленных происходили в вызвавшей функции
void CubicToTower(tower_options tower, ubyte *ptr_hp, int *ptr_score, ubyte *ptr_keystroke)
{
	//Генерируем кубическим уравнение и результат записываем в структурную переменную
	gen_result accept = generateCubic();

	//Создаем структурную переменную объектных опций и инициализируем её
	object_options obj;
	obj.task = accept.task;
	obj.x = X_OBJ_START;
	obj.y = Y_OBJ;
	obj.x_edge = obj.x + obj.task.size() + 4;
	obj.bckGrColor = BACKGR_COLOR;
	obj.color = OBJ_COLOR;
	obj.alive = true;

	//Булевая переменная, проверяющая, был ли нажат минус
	bool minusoid = false;
	//Создаем массив, в котором будут содержаться пользовательские ответы
	short int ans[3] = { 11, 11, 11 };
	//Переменная, в которой храним номер ответа игрока для того, чтобы не записать ответ в уже записанную яйчейку с правильным ответом
	ubyte numAns = 0;
	//Создаем булевый массив, чтобы проверять сколько ответов правильно введенно ( объект уничтожиться только, если все ans_check = true )
	bool ans_check[3] = { false, false, false };

	cout << "\a";								//Звуковое сопровождения при появлении босса
	gotoxy(X_BOSS_LABEL, Y_BOSS_LABEL);			//Вывод оповещения о том, что босс пришел
	SetColor(BACKGR_COLOR, BOSS_LABEL_COLOR);	//Смена цвета на цвет надписи для Boss'a 
	cout << "ATTENTION! BOSS HAS COME!";		//Вывод записи о боссе

	//Цикл движения объекта к башне tower (пока его край не достигнет башни и сам он жив)
	while (obj.x_edge != tower.x && obj.alive)
	{
		//Отрисовка объекта
		drawObject(obj);
		//Сдвиг объекта на 1 вправо
		obj.x++;
		//Заново вычисляем координату края объекта
		obj.x_edge = obj.x + obj.task.size() + 4;
		//Стираем след, оставляемый объектом
		for (ubyte i = Y_OBJ; i < Y_OBJ + 6; i++)
		{
			gotoxy(obj.x - 2, i);
			cout << ' ';
		}
		Sleep(CUBIC_SPEED);

		//Проверка нажатия клавиши
		if (_kbhit())
		{
			//Заносим в keystroke нажатую клавишу
			*ptr_keystroke = _getch();
			//Если был нажат ESC, то выходим из функции
			if (*ptr_keystroke == esc)
				return;

			//Вычисляем координаты записи вводимых пользователем символов
			ubyte
				xNum = tower.x + tower.length / 2,
				yNum = tower.y + tower.width / 2 + 1;

			//Переходим к вычисленным координатам и меняем цвет на цвет надписей
			gotoxy(xNum, yNum);
			SetColor(BACKGR_COLOR, LABEL_COLOR);
			
			//Команда выяснения введенной пользователем цифры
			//В зависимости от кода клавиши, мы заносим в ответ пользователя число, введенное пользователем
			//Если был нажат минус, то делаем даем об этом знать с помощью bool minusoid, созданной ранее
			//Если был нажат enter, то запускается цепочка команд для проверки верности ответа и последующего выстрела и уничтожения
			//объекта и изменения количества очков
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
				//Если был нажат минус, то умножаем число на -1
				if (minusoid)
				{
					ans[numAns] *= -1;
				}
				
				//Если введенное значение равно одному из правильных корней, то стреляем "правильным" выстрелом
				//Даем ans_check знать о том, что ответ правильный
				//Прибавляем очки
				if (ans[numAns] == accept.roots[0] || ans[numAns] == accept.roots[1] || accept.roots[2])
				{
					shoot(true, tower, obj.x_edge);
					ans_check[numAns] = true;
					*ptr_score += CUBIC_POINTS;
					numAns++;
				}
				//Иначе стреляем неправильным выстрелом и убавляем очки
				else
				{
					shoot(false, tower, obj.x_edge);
					*ptr_score += WRONG_POINTS;
				}
				//Если пользователь ввел все корни верно, то уничтожаем объект и сбрасываем номер ответа
				if (ans_check[0] && ans_check[1] && ans_check[2])
				{
					clearObject(obj);
					obj.alive = false;
					numAns = 0;
				}

				//Инициализируем значение переменной ответа на такое, которое получить невозможно. Для подстраховки.
				ans[numAns] = 11;
				//Сбрасываем значение минусоида для того, чтобы следующий ответ не умножался автоматически на -1
				minusoid = false;
				//Обновление показателя очков

				//Стираем предыдущее значение
				for (ubyte i = X_SCORE; i < X_SCORE + 8; i++)
				{
					gotoxy(i, Y_SCORE);
					cout << ' ';
				}
				//Выводим новое по соответствующим координатам и настроенным цветам
				gotoxy(X_SCORE, Y_SCORE);
				SetColor(BACKGR_COLOR, LABEL_COLOR);
				cout << *ptr_score;
				break;
			}
			}
		}
	}
	//Если объект достиг башни, то уничтожаем его
	//отнимаем здоровье у башни и обновляем её с новым показателем
	//Если у башни здоровья не осталось, то уничтожаем её
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
	
	//Стираем надпись BOSS HAS COME
	for (ubyte i = X_BOSS_LABEL; i < X_BOSS_LABEL + 30; i++)
	{
		gotoxy(i, Y_BOSS_LABEL);
		cout << ' ';
	}
}