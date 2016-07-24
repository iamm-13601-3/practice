#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED
#pragma once
#define MEMORY_ERROR 1

#include <cstdlib> 
#include <GL/glut.h>
#include <GL/glaux.h>
#include <windows.h>
#include <vector>

//��� �������
typedef enum {
	PLANET,
	SATELLITE,
	STAR,
	BACKGROUND,
}SELESTIAL_BODY_TYPE;

using namespace std;

class vec //����� ������
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

class object //����� ���� ��������
{
public:
	vec color; //���� � rgb
	SELESTIAL_BODY_TYPE type;
	GLuint texture[2];
	virtual void draw(vector<object*> stack) //����������� ����� ���������, ���������� ��� ������� ���� ��������
	{
	}
	object()
	{
		color = vec(1, 1, 1); //���� �� ���������
	}
	virtual void keyboard(unsigned char key, int x, int y) //����������� ����� ��������� ������� ����������
	{
	}
	virtual void mouse(int button, int state, int mouse_x, int mouse_y) //����������� ����� ��������� �������� ����
	{
	}
	virtual void load_texture(LPCSTR name)
	{
		AUX_RGBImageRec *texture1 = auxDIBImageLoadA(name);
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
	}
};

class anim //�����, ������� ��������� ���� ���������
{
private:
	static void reshape(int w, int h); //���������� ������ ��������� ������� � ���������
	static void display(void);
	static void idle(void);
	static void keyboard(unsigned char key, int x, int y);
	static void mouse(int button, int state, int mouse_x, int mouse_y);
	static anim instance; //������������ ��������� ������
	LARGE_INTEGER frequency, start_time; //���������� �������
	anim();
public:
	vector<object*> stack; //������, � ������� �������� ��� �������
	double time;
	static double get_time(void);
	static void set_camera(vec pos, vec dir, vec up);

	void run(void) //������ ���� ������������ ���������
	{
		glutMainLoop();
	}

	static anim &get_ref(void) //������ � ��������� ������
	{
		return instance;
	}

	anim & operator << (object *obj) //���������� ��������� <<, ��� ����� �������� ����������
	{
		stack.push_back(obj);
		return *this;
	}
};

#endif