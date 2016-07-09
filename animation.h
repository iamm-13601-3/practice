#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED
#pragma once

#include <cstdlib> 
#include <GL/glut.h>
#include <windows.h>
#include <vector>

//тип обьекта
typedef enum {
			  PLANET,
			  SATELLITE,
			  STAR,
			  BACKGROUND,
}SELESTIAL_BODY_TYPE;

using namespace std;

class vec //Класс вектор
{
public:
	double x, y, z;
	vec() :x(0), y(0), z(0)
	{
	}
	vec(double x, double y, double z) : x(x), y(y), z(z)
	{
	}
};

class object //Класс всех объектов
{
public:
	vec color; //Цвет в rgb
	SELESTIAL_BODY_TYPE type;
	virtual void draw(vector<object*> stack) //Виртуальный метод рисования, конкретный для каждого типа объектов
	{
	}
	object()
	{
		color = vec(1, 1, 1); //Цвет по умолчанию
	}
	virtual void keyboard(unsigned char key, int x, int y) //Виртуальный метод обработки нажатий клавиатуры
	{
	}
	virtual void mouse(int button, int state, int mouse_x, int mouse_y) //Виртуальный метод обработки движений мыши
	{
	}
};

class anim //Класс, который управляет всей анимацией
{
private:
	static void reshape(int w, int h); //Конкретные методы обработки событий и рисования
	static void display(void);
	static void idle(void);
	static void keyboard(unsigned char key, int x, int y);
	static void mouse(int button, int state, int mouse_x, int mouse_y);
	static anim instance; //Единственный экземпляр класса
	LARGE_INTEGER frequency, start_time; //Переменные времени
	anim();
public:
	vector<object*> stack; //Вектор, в котором хранятся все объекты
	double time;
	static double get_time(void);
	static void set_camera(vec pos, vec dir, vec up);

	void run(void) //Запуск цикл обслуживания сообщений
	{
		glutMainLoop();
	}

	static anim &get_ref(void) //Доступ к экзепляру класса
	{
		return instance;
	}

	anim & operator << (object *obj) //Перегрузка оператора <<, для более удобного добавления
	{
		stack.push_back(obj);
		return *this;
	}
};

#endif