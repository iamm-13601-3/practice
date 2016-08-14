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
	planets << new planet(10, 5.97e+24); //Добавление планеты, аргументами принимает радиус и массу
	planets << new satellite(1.5, 30, 6000); // добавление спутника, аргументами принимает радиус, начальный угол и скорость
	planets << new satellite(1.5, 30, 6300);
//	planets << new axis(); //Добавление осей координат
	planets << new background();
	Sleep(2000); //Ожидание освобождения процессора для плавного запуска
	planets.run(); //Запуск анимации

	return 0;
}