#include <cstdlib>			//rand, srand, _itoa_S
#include <ctime>			//time(NULL)
#include <math.h>			//pow(num, exp);
#include <string>			//string, to_string
#include "generation.h"

using std::string;

struct viewEquation
{
	char *signB, //знак числа b
		*signC, //знак числа с
		*signD, //знак числа d
		//присутствие или отсутвие "*" перед х (в квадратных - свободный член)
		*signCx,
		//присутствие или отсутвие "*" перед х^2 (в квадратных - перед х, в линейных - свободный член)
		*signBx,
		//присутствие или отсутвие "*" перед х^3 (в квадратных - перед х^2, в линейных - перед х)
		*signKx,
		*meanK, //"=" для случая k != 1
		//значения для d, c, b и k при d!=0, с!=0, b != 0 и k != 1 cоответственно
		*meaningD, *meaningC, *meaningB, *meaningK;
};


//Функция генерации линейного уравнения, возвращает структуру результатов генерации с одним корнем
gen_result generateLinear()
{
	char stringB[2] = "", //данные переменные нужны для перевода k и b из типа 
		stringK[2] = ""; //int в тип char*

	viewEquation view1;

	int k, //коэффициет при х
		b, //свободный член
		x, //неизвестное
		y, //значение уравнения
		sgn; //отвечает за присваивание знака х
	srand(static_cast<unsigned int>(time(NULL)));

	k = rand() % 9 + 1;
	if (k == 1)
	{
		view1.meanK = "";
		view1.meaningK = "=";
		view1.signKx = "x";
	}
	else//k!=1
	{
		view1.meanK = "=";
		_itoa_s(k, stringK, 10); //перевод k из типа int в тип char*
		view1.meaningK = stringK;
		view1.signKx = "*x";

	}
	b = rand() % 10;
	if (b == 0)
	{
		view1.meaningB = "";
		view1.signB = "";
	}

	else //b>0
	{
		_itoa_s(b, stringB, 10); //перевод b из типа int в тип char*
		view1.meaningB = stringB;
		view1.signB = "+";
	}
	x = rand() % 10;
	sgn = rand() % 2 + 1;

	if (sgn % 2 == 0)	//yсловие, присваивающее знак переменной х
	{
		x = x * (-1);
	}
	y = k * x + b;

	string y_str = std::to_string(y);

	gen_result result;

	result.task = y_str + view1.meanK + view1.meaningK + view1.signKx + view1.signB + view1.meaningB;

	result.number = 1;

	result.roots[0] = x;



	return result;
}

//Функция генерации квадратного уравнения, возвращает структуру результатов генерации с 2-мя корнями
gen_result generateQuadratic()
{
	char stringKQ[5] = "", //три данных переменных нужны для перевода k, b и с из типа
		stringBQ[5] = "", //int в тип char*
		stringCQ[5] = "";


	viewEquation view2;

	int k, b, //коэффициенты при х^2 и x соответственно
		c, //свободный член
		x1, x2, //корни
		x, //неизвестное
		y, //значение уравнения 
		signRoots, //отвечает за вариации знаков корней
		valueX; //отвечает за условие присваивания значения x

	srand(static_cast<unsigned int>(time(NULL)));

	k = rand() % 9 + 1;
	if (k == 1)
	{
		view2.meanK = "";
		view2.meaningK = "=";
		view2.signKx = "x^2";

	}
	else //k!=1
	{
		view2.meanK = "=";
		_itoa_s(k, stringKQ, 10); //перевод k из типа int в тип char*
		view2.meaningK = stringKQ;
		view2.signKx = "*x^2";
	}

	x1 = rand() % 10;
	x2 = rand() % 10;
	signRoots = rand() % 4 + 1;
	valueX = rand() % 2 + 1;


	switch (signRoots) //вариации знаков корней
	{
	case 1:
		x1 = x1;
		x2 = x2;
		break;

	case 2:
		x1 = -x1;
		x2 = x2;
		break;

	case 3:
		x1 = x1;
		x2 = -x2;
		break;

	case 4:
		x1 = -x1;
		x2 = -x2;
		break;
	}
	if (valueX == 1) //условие, необходимое для присваивания значения х
		x = x1;

	else
		x = x2;



	b = (-1)*(x1 + x2)*k;
	if (b < 0)
	{
		_itoa_s(b, stringBQ, 10); //перевод b из типа int в тип char*
		view2.meaningB = stringBQ;
		view2.signB = "";
		view2.signBx = "*x";
	}
	else if (b == 0)
	{
		view2.meaningB = "";
		view2.signB = "";
		view2.signBx = "";
	}
	else if (b == 1)
	{
		view2.meaningB = "";
		view2.signB = "+";
		view2.signBx = "x";
	}
	else //b>0
	{
		_itoa_s(b, stringBQ, 10);
		view2.meaningB = stringBQ;
		view2.signB = "+";
		view2.signBx = "*x";
	}

	c = x1 * x2*k;
	if (c < 0)
	{
		_itoa_s(c, stringCQ, 10); //перевод c из типа int в тип char*
		view2.meaningC = stringCQ;
		view2.signC = "";
	}

	else if (c == 0)
	{
		view2.meaningC = "";
		view2.signC = "";
	}
	else //с>0
	{
		_itoa_s(c, stringCQ, 10);
		view2.meaningC = stringCQ;
		view2.signC = "+";
	}
	y = k * pow(x, 2) + b * x + c;

	string y_str = std::to_string(y);

	gen_result result;

	result.task =
		y_str +
		view2.meanK + view2.meaningK + view2.signKx +
		view2.signB + view2.meaningB + view2.signBx +
		view2.signC + view2.meaningC;

	result.number = 2;

	result.roots[0] = x1;
	result.roots[1] = x2;

	return result;
}

//Функция генерации кубического уравнения, возвращает структуру результатов генерации с 3-мя корнями
gen_result generateCubic()
{
	char stringKC[7] = "", //четыре данных переменных нужны для перевода 
		stringBC[7] = "", //k, b, c и d из типа
		stringCC[7] = "", //int в тип char*
		stringDC[7] = "";


	viewEquation view3;

	int k, b, c, //коэффициенты при x^3, х^2 и x соответственно
		d, //свободный член
		x1, x2, x3, //корни
		x, //неизвестное
		y, //значение уравнения 
		signRoots, //отвечает за вариации знаков корней
		valueX; //отвечает за условие присваивания значения x

	srand(static_cast<unsigned int>(time(NULL)));

	k = rand() % 9 + 1;
	if (k == 1)
	{
		view3.meanK = "";
		view3.meaningK = "=";
		view3.signKx = "x^3";

	}
	else //k!=1
	{
		view3.meanK = "=";
		_itoa_s(k, stringKC, 10); //перевод k из типа int в тип char*
		view3.meaningK = stringKC;
		view3.signKx = "*x^3";
	}

	x1 = rand() % 10;
	x2 = rand() % 10;
	x3 = rand() % 10;
	signRoots = rand() % 8 + 1;
	valueX = rand() % 3 + 1;


	switch (signRoots) //вариации знаков корней
	{
	case 1:
		x1 = x1;
		x2 = x2;
		x3 = x3;
		break;

	case 2:
		x1 = -x1;
		x2 = x2;
		x3 = x3;
		break;

	case 3:
		x1 = x1;
		x2 = -x2;
		x3 = x3;
		break;

	case 4:
		x1 = -x1;
		x2 = -x2;
		x3 = x3;
		break;
	case 5:
		x1 = x1;
		x2 = x2;
		x3 = -x3;
		break;
	case 6:
		x1 = -x1;
		x2 = x2;
		x3 = -x3;
	case 7:
		x1 = x1;
		x2 = -x2;
		x3 = -x3;
		break;
	case 8:
		x1 = -x1;
		x2 = -x2;
		x3 = -x3;
	}
	if (valueX == 1) //условие, необходимое для присваивания значения х
		x = x1;

	else if (valueX == 2)
		x = x2;

	else
		x = x3;

	b = (-1)*(x1 + x2 + x3)*k;
	if (b < 0)
	{
		_itoa_s(b, stringBC, 10); //перевод b из типа int в тип char*
		view3.meaningB = stringBC;
		view3.signB = "";
		view3.signBx = "*x^2";
	}
	else if (b == 0)
	{
		view3.meaningB = "";
		view3.signB = "";
		view3.signBx = "";
	}
	else if (b == 1)
	{
		view3.meaningB = "";
		view3.signB = "+";
		view3.signBx = "x^2";
	}
	else //b>0
	{
		_itoa_s(b, stringBC, 10);
		view3.meaningB = stringBC;
		view3.signB = "+";
		view3.signBx = "*x^2";
	}

	c = (x1*x2 + x2 * x3 + x1 * x3)*k;
	if (c < 0)
	{
		_itoa_s(c, stringCC, 10); //перевод c из типа int в тип char*
		view3.meaningC = stringCC;
		view3.signC = "";
		view3.signCx = "*x";
	}

	else if (c == 0)
	{
		view3.meaningC = "";
		view3.signC = "";
		view3.signCx = "";
	}
	else if (c == 1)
	{
		view3.meaningC = "";
		view3.signC = "+";
		view3.signCx = "x";
	}
	else //с>0
	{
		_itoa_s(c, stringCC, 10);
		view3.meaningC = stringCC;
		view3.signC = "+";
		view3.signCx = "*x";
	}

	d = (-1)*x1*x2*x3*k;
	if (d < 0)
	{
		_itoa_s(d, stringDC, 10); //перевод d из типа int в тип char*
		view3.meaningD = stringDC;
		view3.signD = "";
	}

	else if (d == 0)
	{
		view3.meaningD = "";
		view3.signD = "";
	}
	else //d>0
	{
		_itoa_s(d, stringDC, 10);
		view3.meaningD = stringDC;
		view3.signD = "+";
	}


	y = k * pow(x, 3) + b * pow(x, 2) + c * x + d;
	string y_str = std::to_string(y);

	gen_result result;

	result.task =
		y_str + view3.meanK + view3.meaningK + view3.signKx +
		view3.signB + view3.meaningB + view3.signBx +
		view3.signC + view3.meaningC + view3.signCx +
		view3.signD + view3.meaningD;

	result.number = 3;

	result.roots[0] = x1;
	result.roots[1] = x2;
	result.roots[2] = x3;

	return result;
}

