#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"
#include "background.h"

int main(void)
{
	anim &planets = anim::get_ref(); //Получение экземпляра объекта класса anim

	planets << new planet( //Добавление планеты, аргументами принимает радиус, массу и координаты
		20, 
		6e+23, 
		vec{0, 0, 0});	
	planets << new satellite( //Добавление спутника, аргументами принимает радиус, начальное положение, скорость и массу
		1.5, 
		vec{0, 2e7, 0},
		vec{15e3, 5900, 0},
		100); 
//	planets << new axis(); //Добавление осей координат
	planets << new background();
	Sleep(2000); //Ожидание освобождения процессора для плавного запуска
	planets.run(); //Запуск анимации

	return 0;
}