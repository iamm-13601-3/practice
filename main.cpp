#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"

int main(void)
{
	anim &planets = anim::get_ref(); //Получение экземпляра объекта класса anim

	planets << new sphere(10); //Добавление сферы
	planets << new axis(); //Добавление осей координат
	planets.run(); //Запуск анимации

	return 0;
}