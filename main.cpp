#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"

int main(void)
{
	anim &planets = anim::get_ref(); //Получение экземпляра объекта класса anim
	planets << new planet(10, 5.97e+24); //Добавление планеты, аргументами принимает радиус и массу
	planets << new satellite(3,30, 6000); // добавление спутника, аргументами принимает радиус, начальный угол и скорость
	planets << new axis(); //Добавление осей координат
	planets.run(); //Запуск анимации
	return 0;
}