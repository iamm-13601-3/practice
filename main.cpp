#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"
#include "background.h"

int main(void)
{
	FreeConsole(); //Отключение вызова консоли

	anim &planets = anim::get_ref(); //Получение экземпляра объекта класса anim
	vec coord = { 0 , 0, 0  };// начальные координаты для планеты
	vec velocity = { 17e3, 0 , 0 };//начальная скорость для спутника
	vec pos = { 0 , 2e7  , 0 };//начальное положение спутника
	planets << new planet(20, 6e+23, coord); //Добавление планеты, аргументами принимает радиус,  массу и координаты
	//coord = { 2e7, 0, 0 };	
	//planets << new planet(20, 3e+23, coord);
	// добавление спутника, аргументами принимает радиус, начальное положение, скорость и массу
	planets << new satellite( 1.5, pos, velocity, 100); 
	velocity = { 0, 0, 17e3 };
	planets << new satellite(1.5, pos, velocity, 100);

//	planets << new axis(); //Добавление осей координат
	planets << new background();
	Sleep(2000); //Ожидание освобождения процессора для плавного запуска
	planets.run(); //Запуск анимации

	return 0;
}