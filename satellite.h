#ifndef SATELLITE_H_INCLUDED
#define SATELLITE_H_INCLUDED
#pragma once

#include "animation.h"
#include "vec_math.h"
#include "sphere.h"
#include "planet.h"


class satellite : public sphere
{
private:
	float a, b, c;
	GLUquadricObj* disk;
	AUX_RGBImageRec *texture1, *texture2;
	GLuint texture[2];
public:
	double w, *planet_rad, *planet_mass, self_mass;
	viewport_t view;
	timer_t timer;
	object_t obj;
	vec r0, a0, start_velocity, start_pos, *planet_pos;
	vec k1;
	vec *trajectory;
	double* dist;
	object_t obj_work;
	bool alive;
	rect_t r;
	int counter, size;
	unsigned int planet_count;
	void load_texture(LPCSTR name1, LPCSTR name2)
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
	satellite(double radius, vec start_pos, vec start_velocity, double mass) : sphere(radius)
	{
		counter = 0;
		type = SATELLITE;
		this->start_velocity = start_velocity;
		this->start_pos = start_pos;
		this->self_mass = mass;
		size = 0;
		planet_count = 0;
		alive = true;
		obj.a = { 0, 0, 0 };
		load_texture("practice.git/images/image2.bmp", "practice.git/images/image4.bmp"); //«агрузка текстуры
	}

	void draw(vector<object*> stack)
	{
		//инилиазизаци€ переменных(только 1 раз)
		if (counter == 0)
		{
			counter = 1;
			while (((planet*)stack[planet_count++])->type == PLANET)
			{
				planet_pos = (vec*)realloc(planet_pos,planet_count * sizeof(vec));
				planet_mass = (double*)realloc(planet_mass, planet_count * sizeof(double));
				planet_rad = (double*)realloc(planet_rad, planet_count * sizeof(double));
				planet_pos[planet_count - 1] = ((planet*)stack[planet_count - 1])->coord;
				planet_rad[planet_count - 1] = mul * ((planet*)stack[planet_count - 1])->radius;
				planet_mass[planet_count - 1] = ((planet*)stack[planet_count - 1])->mass;
				//задание экранных и математических координат
				rect_set(&view.screen, -100.0, -100.0, -100.0, 100.0,  100.0, 100.0);
				rect_set(&view.math, -5 * math_size * mul, -5 * math_size * mul, -5 * math_size * mul, 5 * math_size * mul, 5 * math_size * mul, 5 * math_size * mul);
				//начальное положение,скорость, ускорение и угол
				k1.x = 0;
				k1.y = 0;
				k1 = vec_transform(k1, &view.math, &view.screen);

				obj.t = 0.0;
				obj.r = start_pos;

				obj.v.x = start_velocity.x;
				obj.v.y = start_velocity.y;
				obj.v.z = start_velocity.z;

				//таймер
				timer.start = real_timer_start;
				timer.step = real_timer_step;

				timer.start(&timer);
				//расчЄт рассто€ни€ между двум€ телами
				dist = (double*)realloc(dist, planet_count * sizeof(double));
				dist[planet_count - 1] = range(obj.r, planet_pos[planet_count - 1]);
			}
			planet_count--;
		}

		glColor3d(color.x, color.y, color.z);
		unsigned int i = 0;
		for (i = 0; i < planet_count; i++)
		{
			if (dist[i] < planet_rad[i])
				alive = false;
		}
		if (alive)
		{
			size++;
			void* temp = realloc(trajectory, size * sizeof(vec));
			if (temp != NULL)
				trajectory = (vec*)temp;
			else
				exit(MEMORY_ERROR);
			obj_work = obj;
			obj.a = { 0, 0, 0 };
			object_t t;
			for (i = 0; i < planet_count; i++)
			{
				//obj.a = vec_add(obj.a, force(((planet*)stack[i])->coord, obj.r, ((planet*)stack[i])->mass, self_mass));
				runge_kutta_step(&obj, timer.step(&timer), planet_mass[i], self_mass);
			}
			//	obj.a = force(((planet*)stack[0])->coord, obj.r, ((planet*)stack[0])->mass, self_mass);
			//runge_kutta_step(&obj, timer.step(&timer), planet_mass[0] , self_mass);//¬ызов функции, указатель на которую мы получили при вызове
			//euler_step(&obj, timer.step(&timer));
			rect_set(&r, obj_work.r.x, obj_work.r.y,obj_work.r.z, obj.r.x, obj.r.y, obj.r.z);//«аполнение координат вектора
			r.a = vec_transform(r.a, &view.math, &view.screen);//ѕеревод кооринат точки в экранные
			r.b = vec_transform(r.b, &view.math, &view.screen);
			glPushMatrix();//запоминаем матрицу
			glTranslatef(r.b.x, r.b.y, r.b.z);//делаем преобразование
			
			double k = 10, 
				k1 = 150, k2 = k1 * k, k3 = 30, 
				k4 = 1.0 / 7, k5 = k4 * k, k6 = k3,
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
		for (i = 0; i < planet_count; i++)
		{
			dist[i] = range(obj.r, planet_pos[i]);
		}
		//r1 = obj.r.x * obj.r.x + obj.r.y * obj.r.y + obj.r.z * obj.r.z;
		//r1 = sqrt(r1);
#if USE_SHADERS
		setShaders(shaders, "practice.git/shaders/default_vert_shader.glsl", "practice.git/shaders/default_frag_shader.glsl");
		glUseProgram(shaders);
#endif
	}
};
#endif