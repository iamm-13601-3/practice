#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"
#include "background.h"

int main(void)
{
	FreeConsole(); //���������� ������ �������

	anim &planets = anim::get_ref(); //��������� ���������� ������� ������ anim
	vec coord = { 0 , 0, 0  };// ��������� ���������� ��� �������
	vec velocity = { 17e3, 0 , 0 };//��������� �������� ��� ��������
	vec pos = { 0 , 2e7  , 0 };//��������� ��������� ��������
	planets << new planet(20, 6e+23, coord); //���������� �������, ����������� ��������� ������,  ����� � ����������
	//coord = { 2e7, 0, 0 };	
	//planets << new planet(20, 3e+23, coord);
	// ���������� ��������, ����������� ��������� ������, ��������� ���������, �������� � �����
	planets << new satellite( 1.5, pos, velocity, 100); 
	velocity = { 0, 0, 17e3 };
	planets << new satellite(1.5, pos, velocity, 100);

//	planets << new axis(); //���������� ���� ���������
	planets << new background();
	Sleep(2000); //�������� ������������ ���������� ��� �������� �������
	planets.run(); //������ ��������

	return 0;
}