#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"

int main(void)
{
	anim &planets = anim::get_ref(); //��������� ���������� ������� ������ anim
	planets << new planet(10, 5.97e+24); //���������� �������, ����������� ��������� ������ � �����
	planets << new satellite(3,30, 6000); // ���������� ��������, ����������� ��������� ������, ��������� ���� � ��������
	planets << new axis(); //���������� ���� ���������
	planets.run(); //������ ��������
	return 0;
}