#ifndef SATELLITE_H_INCLUDED
#define SATELLITE_H_INCLUDED
#pragma once

#include "animation.h"
#include "vec_math.h"
#include "sphere.h"
#include "planet.h"

#define mul (6371e3 / 10)
#define math_size 15

class satellite : public sphere
{
private:
	float a, b, c;
//	int i;
	GLUquadricObj* disk;
	AUX_RGBImageRec *texture1, *texture2;
	GLuint texture[2];
public:
	double w, V1, start_angle, rad_planet, velocity, mass;
	viewport_t view;
	timer_t timer;
	object_t obj;
	vec r0, a0;
	vec k1;
	vec *trajectory;
	double r1;
	object_t obj_work;
	rect_t r;
	int counter, size;
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
				rect_set(&view.math, -5 * math_size * mul, -5 * math_size * mul, -5 * math_size * mul, 5 * math_size * mul, 5 * math_size * mul, 5 * math_size * mul);
				//начальное положение,скорость, ускорение и угол
				k1.x = 0;
				k1.y = 0;
				k1 = vec_transform(k1, &view.math, &view.screen);
				//V1 = sqrt(G * mass / (2 * rad_planet));
				V1 = velocity;
				r0.x = 0;
				r0.y = 2 * math_size * mul;
				a0.x = 0;
				a0.y = -G * mass / (math_size * mul * math_size * mul);

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
			
			double k = 10, 
				k1 = 150, k2 = k1 * k, k3 = 30, 
				k4 = 1.0 / 10, k5 = k4 * k, k6 = k3,
				r_vec;

			glLineWidth(3);
				
			glPushMatrix();
			
			r_vec = sqrt(obj.v.x * obj.v.x + obj.v.y * obj.v.y + obj.v.z * obj.v.z) / k1;
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(obj.v.x / k1, obj.v.y / k1, obj.v.z / k1);
			glEnd();
			glTranslatef(obj.v.x / k1, obj.v.y / k1, obj.v.z / k1);
			glBegin(GL_LINE_STRIP);
			glVertex3f(-obj.v.x / k2 + r_vec / k3, -obj.v.y / k2 + r_vec / k3, -obj.v.z / k2 + r_vec / k3);
			glVertex3f(0, 0, 0);
			glVertex3f(-obj.v.x / k2 - r_vec / k3, -obj.v.y / k2 - r_vec / k3, -obj.v.z / k2 - r_vec / k3);
			glEnd();
			glPopMatrix();

			glPushMatrix();

			r_vec = sqrt(obj.a.x * obj.a.x + obj.a.y * obj.a.y + obj.a.z * obj.a.z) / k4;
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(obj.a.x / k4, obj.a.y / k4, obj.a.z / k4);
			glEnd();
			glTranslatef(obj.a.x / k4, obj.a.y / k4, obj.a.z / k4);
			glBegin(GL_LINE_STRIP);
			glVertex3f(-obj.a.x / k5 + r_vec / k6, -obj.a.y / k5 + r_vec / k6, -obj.a.z / k5 + r_vec / k6);
			glVertex3f(0, 0, 0);
			glVertex3f(-obj.a.x / k5 - r_vec / k6, -obj.a.y / k5 - r_vec / k6, -obj.a.z / k5 - r_vec / k6);
			glEnd();
			glPopMatrix();

			glLineWidth(2);

			glColor3f(0.87, 1.0, 0.65);
			glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
			glutSolidCone(radius, radius + (radius + 1), 20, 20);
			disk = gluNewQuadric();
			gluDisk(disk, 0, radius, 20, 20);

			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glColor3f(1.0, 1.0, 0.56);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - (radius * 4), radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(radius - (radius * 4), -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(radius - (radius * 4), -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(radius - (radius * 4), radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - (radius * 4), radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(radius - (radius * 4), -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), -radius, radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), radius, radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - (radius * 4), radius, -radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(radius - (radius * 4), -radius, -radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - (radius * 4), -radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - (radius * 4), -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(radius - (radius * 4), -radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(radius - (radius * 4), radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - (radius * 4), radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(radius - (radius * 4), radius, -radius);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(3 * radius - (radius * 4), radius, radius);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(3 * radius - (radius * 4), -radius, radius);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), -radius, -radius);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(3 * radius - (radius * 4), radius, -radius);
			glEnd();

			glColor3f(1.0, 1.0, 1.0);//рисуем солнечные панели
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, texture2->sizeX, texture2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2->data);

			for (int i = 0; i < 6; i++)
			{
				if (i == 0)
				{
					a = 1;
					b = 3;
					c = 0;
				}
				else
				{
					c = b;
					a = b + 0.5;
					b = a + 2;
				}
				glBegin(GL_LINES);

				glVertex3f(3 * radius - (radius * 4), 0.0, radius + c);
				glVertex3f(3 * radius - (radius * 4), 0.0, radius + a);

				glVertex3f(3 * radius - (radius * 4), 0.0, -radius - c);
				glVertex3f(3 * radius - (radius * 4), 0.0, -radius - a);
				glEnd();

				glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); //«адание координат текстуры
				glVertex3f(3 * radius - (radius * 4), radius - 5, radius + a);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(3 * radius - (radius * 4), -radius + 5, radius + a);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(3 * radius - (radius * 4), -radius + 5, radius + b);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(3 * radius - (radius * 4), radius - 5, radius + b);

				glTexCoord2f(0.0, 0.0);
				glVertex3f(3 * radius - (radius * 4), radius - 5, -radius - a);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(3 * radius - (radius * 4), -radius + 5, -radius - a);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(3 * radius - (radius * 4), -radius + 5, -radius - b);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(3 * radius - (radius * 4), radius - 5, -radius - b);
				glEnd();
			}

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