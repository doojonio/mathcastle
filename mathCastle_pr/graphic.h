#include <string>

using std::string;
typedef unsigned char ubyte;

//��������� �������� ��� ��������� �������
struct object_options
{
	string task;								//��������� ������ �������
	ubyte
		x, y,									//���������� �������
		x_edge,									//���������� ����(�����) �������
		bckGrColor, color;						//�������� ����� ( ���� ����, ���� �������� )
	bool alive;
};

//��������� �������� ��� ��������� �����
struct tower_options
{
	ubyte
		x, y,					//���������� �����
		width, length,			//������� ����� (������, �����)
		hp, max_hp,				//hit points (�������� ��������/max)
		bckGrColor, color;		//�������� �����
};

//��������� ����� ����� ��������� �������� �� charColor, � ���� �� ���� �� BACKGR_COLOR
void SetColor(ubyte bckGrColor, ubyte charColor);

//��������� ��������� ������� �� ��������� object
void drawObject(object_options object);

//��������� ��������� ����� �� ��������� tower
void drawTower(tower_options tower);

//��������� ��������� "�����������" ����� �� ��������� tower 
void clearTower(tower_options tower);

//��������� ��������� "�����������" ������� �� ��������� object
void clearObject(object_options object);
