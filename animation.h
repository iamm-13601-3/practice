#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED
#pragma once

#define MEMORY_ERROR 1

#include <cstdlib> 
#include <GL/glut.h>
#include <GL/glaux.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

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
	GLuint texture[1];
	GLuint shaders;
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
	virtual void load_texture(LPCSTR name)
	{
		AUX_RGBImageRec *texture1 = auxDIBImageLoadA(name);
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
	}/*
	virtual void printInfoLog(GLuint obj)
	{
		int log_size = 0;
		int bytes_written = 0;

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &log_size);
		if (!log_size)
			return;
		char *infoLog = new char[log_size];
		glGetProgramInfoLog(obj, log_size, &bytes_written, infoLog);
		std::cerr << infoLog << std::endl;
		delete[] infoLog;
	}

	virtual bool read_n_compile_shader(const char *filename, GLuint &hdlr, GLenum shaderType)
	{
		std::ifstream is(filename, std::ios::in | std::ios::binary | std::ios::ate);

		if (!is.is_open())
		{
			std::cerr << "Unable to open file " << filename << std::endl;
			return false;
		}

		long size = is.tellg();
		char *buffer = new char[size + 1];

		is.seekg(0, std::ios::beg);
		is.read(buffer, size);
		is.close();
		buffer[size] = 0;

		hdlr = glCreateShader(shaderType);
		glShaderSource(hdlr, 1, (const GLchar**)&buffer, NULL);
		glCompileShader(hdlr);
		std::cerr << "info log for " << filename << std::endl;
		printInfoLog(hdlr);
		delete[] buffer;
		return true;
	}

	virtual void setShaders(GLuint &prog_hdlr, const char *vsfile, const char *fsfile)
	{
		GLuint vert_hdlr, frag_hdlr;

		read_n_compile_shader(vsfile, vert_hdlr, GL_VERTEX_SHADER);
		read_n_compile_shader(fsfile, frag_hdlr, GL_FRAGMENT_SHADER);

		prog_hdlr = glCreateProgram();
		glAttachShader(prog_hdlr, frag_hdlr);
		glAttachShader(prog_hdlr, vert_hdlr);

		glLinkProgram(prog_hdlr);
		std::cerr << "info log for the linked program" << std::endl;
		printInfoLog(prog_hdlr);
	}*/
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
	~anim() //Очистка памяти
	{
		for (unsigned int i = 0; i < stack.size(); i++)
			delete stack[i];
	}
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