#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"
#include "background.h"

int main(void)
{
	anim &planets = anim::get_ref(); //��������� ���������� ������� ������ anim

	planets << new planet( //���������� �������, ����������� ��������� ������, ����� � ����������
		20, 
		6e+23, 
		vec{0, 0, 0});	
	planets << new satellite( //���������� ��������, ����������� ��������� ������, ��������� ���������, �������� � �����
		1.5, 
		vec{0, 2e7, 0},
		vec{15e3, 5900, 0},
		100); 
//	planets << new axis(); //���������� ���� ���������
	planets << new background();
	Sleep(2000); //�������� ������������ ���������� ��� �������� �������
	planets.run(); //������ ��������

	return 0;
}