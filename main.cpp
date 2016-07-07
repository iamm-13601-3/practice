#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"

int main(void)
{
	anim &planets = anim::get_ref(); //��������� ���������� ������� ������ anim

	planets << new sphere(10); //���������� �����
	planets << new axis(); //���������� ���� ���������
	planets.run(); //������ ��������

	return 0;
}