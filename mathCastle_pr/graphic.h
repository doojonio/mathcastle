#include <string>

using std::string;
typedef unsigned char ubyte;

//Структура настроек для отрисовки объекта
struct object_options
{
	string task;								//Уравнение внутри объекта
	ubyte
		x, y,									//Координаты объекта
		x_edge,									//Координата края(конца) объекта
		bckGrColor, color;						//Цветовая гамма ( цвет фона, цвет символов )
	bool alive;
};

//Структура настроек для отрисовки башни
struct tower_options
{
	ubyte
		x, y,					//Координаты башни
		width, length,			//Размеры башни (ширина, длина)
		hp, max_hp,				//hit points (здоровье текующее/max)
		bckGrColor, color;		//цветовая гамма
};

//Процедура смены цвета выводимых символов на charColor, и фона за ними на BACKGR_COLOR
void SetColor(ubyte bckGrColor, ubyte charColor);

//Процедура отрисовки объекта по настройке object
void drawObject(object_options object);

//Процедура отрисовки башни по настройке tower
void drawTower(tower_options tower);

//Процедура отрисовки "уничтожения" башни по настройке tower 
void clearTower(tower_options tower);

//Процедура отрисовки "уничтожения" объекта по настройке object
void clearObject(object_options object);
