#ifndef SATELLITE_H_INCLUDED
#define SATELLITE_H_INCLUDED
#pragma once
//test change
#include "animation.h"
#include "vec_math.h"
#include "sphere.h"
#include "planet.h"

#define mul (6371e3 / 10)

class satellite : public sphere
{
public:
	double w, V1, start_angle, rad_planet, velocity, mass;
	viewport_t view;
	timer_t timer;
	object_t obj;
	vec r0, a0;
	vec k1;
	vec * trajectory;
	double r1;
	object_t obj_work;
	rect_t r;
	int counter, size;

	satellite(double radius, double start_angle, double start_velocity) : sphere(radius)
	{
		counter = 0;
		type = SATELLITE;
		this->start_angle = start_angle;
		velocity = start_velocity;
		size = 0;
	}

	void draw(vector<object*> stack)
	{
		//инилиазизация переменных(только 1 раз)
		if (counter == 0)
		{
			counter++;
			if (((planet*)stack[0])->type == PLANET)
			{
				rad_planet = mul * ((planet*)stack[0])->radius;
				mass = ((planet*)stack[0])->mass;
				//задание экранных и математических координат
				rect_set(&view.screen, -100.0, -100.0, 100.0, 100.0);
				rect_set(&view.math, -5 * rad_planet, 5 * rad_planet, 5 * rad_planet, -5 * rad_planet);
				//начальное положение,скорость, ускорение и угол
				k1.x = 0;
				k1.y = 0;
				k1 = vec_transform(k1, &view.math, &view.screen);
				//V1 = sqrt(G * mass / (2 * rad_planet));
				V1 =velocity;
				r0.x = 0;
				r0.y = 2 * rad_planet;
				a0.x = 0;
				a0.y = -G * mass / (rad_planet * rad_planet);

				obj.t = 0.0;
				obj.r = r0;
				obj.v.x = V1 * cos(deg_to_rad(start_angle));
				obj.v.y = V1 * sin(deg_to_rad(start_angle));
				obj.a = a0;
				//таймер
				timer.start = real_timer_start;
				timer.step = real_timer_step;

				timer.start(&timer);
				//расчёт расстояния между двумя телами
				r1 = obj.r.x * obj.r.x + obj.r.y * obj.r.y;
				r1 = sqrt(r1); 
			}
		}
		glColor3d(color.x, color.y, color.z);
		if (r1 >= rad_planet)
		{
			size++;
			void* temp = realloc(trajectory, size * sizeof(vec));
			if (temp != NULL)
				trajectory = (vec*)temp;
			else
				exit(MEMORY_ERROR);
			obj_work = obj;
			runge_kutta_step(&obj, timer.step(&timer), mass);//Вызов функции, указатель на которую мы получили при вызове
			rect_set(&r, obj_work.r.x, obj_work.r.y, obj.r.x, obj.r.y);//Заполнение координат вектора
			r.a = vec_transform(r.a, &view.math, &view.screen);//Перевод кооринат точки в экранные
			r.b = vec_transform(r.b, &view.math, &view.screen);
			glPushMatrix();//запоминаем матрицу
			glTranslatef(r.b.x, r.b.y, 0);//делаем преобразование
			glColor3f(1.0, 0.30, 0.0);
			glutSolidSphere(radius, 20, 20);

			glColor3f(0.089, 1.0, 0.0);
			glBegin(GL_QUADS);
			glVertex3f(radius - 2.5, radius, radius);
			glVertex3f(radius - 2.5, -radius, radius);
			glVertex3f(radius - 2.5, -radius, -radius);
			glVertex3f(radius - 2.5, radius, -radius);

			glVertex3f(radius - 2.5, radius, radius);
			glVertex3f(radius - 2.5, -radius, radius);
			glVertex3f(3 * radius - 2.5, -radius, radius);
			glVertex3f(3 * radius - 2.5, radius, radius);

			glVertex3f(radius - 2.5, radius, -radius);
			glVertex3f(radius - 2.5, -radius, -radius);
			glVertex3f(3 * radius - 2.5, -radius, -radius);
			glVertex3f(3 * radius - 2.5, radius, -radius);

			glVertex3f(radius - 2.5, -radius, radius);
			glVertex3f(3 * radius - 2.5, -radius, radius);
			glVertex3f(3 * radius - 2.5, -radius, -radius);
			glVertex3f(radius - 2.5, -radius, -radius);

			glVertex3f(radius - 2.5, radius, radius);
			glVertex3f(3 * radius - 2.5, radius, radius);
			glVertex3f(3 * radius - 2.5, radius, -radius);
			glVertex3f(radius - 2.5, radius, -radius);

			glVertex3f(3 * radius - 2.5, radius, radius);
			glVertex3f(3 * radius - 2.5, -radius, radius);
			glVertex3f(3 * radius - 2.5, -radius, -radius);
			glVertex3f(3 * radius - 2.5, radius, -radius);
			glEnd();

			glColor3f(0.89, 0.75, 0.0);
			glBegin(GL_LINES);

			glVertex3f(3 * radius - 2.5, 0.0, radius);
			glVertex3f(3 * radius - 2.5, 0.0, radius + 1);

			glVertex3f(3 * radius - 2.5, 0.0, -radius);
			glVertex3f(3 * radius - 2.5, 0.0, -radius - 1);

			glVertex3f(3 * radius - 2.5, radius, 0.0);
			glVertex3f(3 * radius - 2.5, radius + 1, 0.0);

			glVertex3f(3 * radius - 2.5, -radius, 0.0);
			glVertex3f(3 * radius - 2.5, -radius - 1, 0.0);
			glEnd();

			glBegin(GL_QUADS);
			glVertex3f(3 * radius - 2.5, radius - 2, radius + 1);
			glVertex3f(3 * radius - 2.5, -radius + 2, radius + 1);
			glVertex3f(3 * radius - 2.5, -radius + 2, radius + 7);
			glVertex3f(3 * radius - 2.5, radius - 2, radius + 7);

			glVertex3f(3 * radius - 2.5, radius - 2, -radius - 1);
			glVertex3f(3 * radius - 2.5, -radius + 2, -radius - 1);
			glVertex3f(3 * radius - 2.5, -radius + 2, -radius - 7);
			glVertex3f(3 * radius - 2.5, radius - 2, -radius - 7);

			glVertex3f(3 * radius - 2.5, radius + 1, radius - 2);
			glVertex3f(3 * radius - 2.5, radius + 1, -radius + 2);
			glVertex3f(3 * radius - 2.5, radius + 7, -radius + 2);
			glVertex3f(3 * radius - 2.5, radius + 7, radius - 2);

			glVertex3f(3 * radius - 2.5, -radius - 1, radius - 2);
			glVertex3f(3 * radius - 2.5, -radius - 1, -radius + 2);
			glVertex3f(3 * radius - 2.5, -radius - 7, -radius + 2);
			glVertex3f(3 * radius - 2.5, -radius - 7, radius - 2);
			glEnd();
			glTranslatef(r.b.x , r.b.y ,0);//делаем преобразование
			glutSolidSphere(radius, 100, 100);//рисование
			glPopMatrix();//возвращаем матрицу на место
			trajectory[size - 1] = { r.b.x, r.b.y, 0 };
			glBegin(GL_LINE_STRIP);
				for (int i = 0; i < size; i++)
				{	
					glVertex3f(trajectory[i].x, trajectory[i].y, trajectory[i].z);
				}
			glEnd();
		}
		r1 = obj.r.x * obj.r.x + obj.r.y * obj.r.y;
		r1 = sqrt(r1);
	}
};
#endif