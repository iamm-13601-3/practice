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
	GLUquadricObj* disk;
	AUX_RGBImageRec *texture1, *texture2;
	GLuint texture[2];
	virtual void load_texture(LPCSTR name1, LPCSTR name2)
	{
		texture1 = auxDIBImageLoadA(name1);
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);

		texture2 = auxDIBImageLoadA(name2);
		glGenTextures(2, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture2->sizeX, texture2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2->data);
	}
	satellite(double radius, double start_angle, double start_velocity) : sphere(radius)
	{
		counter = 0;
		type = SATELLITE;
		this->start_angle = start_angle;
		velocity = start_velocity;
		size = 0;
		load_texture("images/image2.bmp", "images/image4.bmp"); //«агрузка текстуры
	}

	void draw(vector<object*> stack)
	{
		//инилиазизаци€ переменных(только 1 раз)
		if (counter == 0)
		{
			counter++;
			if (((planet*)stack[0])->type == PLANET)
			{
				rad_planet = mul * ((planet*)stack[0])->radius;
				mass = ((planet*)stack[0])->mass;
				//задание экранных и математических координат
				rect_set(&view.screen, -100.0, -100.0, -100.0, 100.0,  100.0, 100.0);
				rect_set(&view.math, -5 * rad_planet, -5 * rad_planet, -5 * rad_planet, 5 * rad_planet, 5 * rad_planet, 5 * rad_planet);
				//начальное положение,скорость, ускорение и угол
				k1.x = 0;
				k1.y = 0;
				k1 = vec_transform(k1, &view.math, &view.screen);
				//V1 = sqrt(G * mass / (2 * rad_planet));
				V1 = velocity;
				r0.x = 0;
				r0.y = 2 * rad_planet;
				a0.x = 0;
				a0.y = -G * mass / (rad_planet * rad_planet);

				obj.t = 0.0;
				obj.r = r0;
				//временный костыль, пока нет системы задани€ углов и проекций скоростей
				V1 *= 5;
				V1 /= 6;
				obj.v.x = V1 * cos(deg_to_rad(start_angle));
				obj.v.y = V1 * sin(deg_to_rad(start_angle));
				obj.v.z = V1 * sin(deg_to_rad(start_angle));
				obj.a = a0;
				//таймер
				timer.start = real_timer_start;
				timer.step = real_timer_step;

				timer.start(&timer);
				//расчЄт рассто€ни€ между двум€ телами
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
			runge_kutta_step(&obj, timer.step(&timer), mass);//¬ызов функции, указатель на которую мы получили при вызове
			rect_set(&r, obj_work.r.x, obj_work.r.y,obj_work.r.z, obj.r.x, obj.r.y, obj.r.z);//«аполнение координат вектора
			r.a = vec_transform(r.a, &view.math, &view.screen);//ѕеревод кооринат точки в экранные
			r.b = vec_transform(r.b, &view.math, &view.screen);
			glPushMatrix();//запоминаем матрицу
			glTranslatef(r.b.x, r.b.y, r.b.z);//делаем преобразование
			glColor3f(0.87, 1.0, 0.65);
			glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
			glutSolidCone(radius, 8, 20, 20);
			disk = gluNewQuadric();
			gluDisk(disk, 0, radius, 20, 20);

			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glColor3f(1.0, 1.0, 0.56);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - 11.5, radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(radius - 11.5, -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(radius - 11.5, -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(radius - 11.5, radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - 11.5, radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(radius - 11.5, -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius, radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius, radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - 11.5, radius, -radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(radius - 11.5, -radius, -radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - 11.5, -radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(radius - 11.5, -radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - 11.5, radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(radius - 11.5, radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(3 * radius - 11.5, radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius, -radius);
			glEnd();

			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_LINES);

			glVertex3f(3 * radius - 11.5, 0.0, radius);
			glVertex3f(3 * radius - 11.5, 0.0, radius + 1);

			glVertex3f(3 * radius - 11.5, 0.0, -radius);
			glVertex3f(3 * radius - 11.5, 0.0, -radius - 1);

			glVertex3f(3 * radius - 11.5, radius, 0.0);
			glVertex3f(3 * radius - 11.5, radius + 1, 0.0);

			glVertex3f(3 * radius - 11.5, -radius, 0.0);
			glVertex3f(3 * radius - 11.5, -radius - 1, 0.0);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, texture2->sizeX, texture2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2->data);

			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); //«адание координат текстуры
			glVertex3f(3 * radius - 11.5, radius - 2, radius + 1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, -radius + 2, radius + 1);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius + 2, radius + 7);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius - 2, radius + 7);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(3 * radius - 11.5, radius - 2, -radius - 1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, -radius + 2, -radius - 1);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius + 2, -radius - 7);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius - 2, -radius - 7);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(3 * radius - 11.5, radius + 1, radius - 2);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, radius + 1, -radius + 2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius + 7, -radius + 2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, radius + 7, radius - 2);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(3 * radius - 11.5, -radius - 1, radius - 2);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - 11.5, -radius - 1, -radius + 2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius - 7, -radius + 2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - 11.5, -radius - 7, radius - 2);
			glEnd();

			glPopMatrix();//возвращаем матрицу на место
			trajectory[size - 1] = { r.b.x, r.b.y, r.b.z };
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < size; i++)
			{
				glVertex3f(trajectory[i].x, trajectory[i].y, trajectory[i].z);
			}
			glEnd();
		}
		r1 = obj.r.x * obj.r.x + obj.r.y * obj.r.y + obj.r.z * obj.r.z;
		r1 = sqrt(r1);
	}
};
#endif