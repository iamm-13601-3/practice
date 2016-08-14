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
	planets << new planet(10, 5.97e+24); //���������� �������, ����������� ��������� ������ � �����
	planets << new satellite(1.5, 30, 6000); // ���������� ��������, ����������� ��������� ������, ��������� ���� � ��������
	planets << new satellite(1.5, 30, 6300);
//	planets << new axis(); //���������� ���� ���������
	planets << new background();
	Sleep(2000); //�������� ������������ ���������� ��� �������� �������
	planets.run(); //������ ��������

	return 0;
}