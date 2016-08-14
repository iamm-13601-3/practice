#include "animation.h"


anim anim::instance; //Экземпляр класса anim

anim::anim(void) //Констуктор класса anim
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //Стандартная настройка вывода
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("OpenGL"); //Создание окна

	glEnable(GL_LIGHTING); //Освещение
	//glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 20.0, 0, 1.0 };
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glEnable(GL_TEXTURE_2D); //Включение текстур

	glutDisplayFunc(anim::display); //Вызов функций обработки событий и рисования
	glutReshapeFunc(anim::reshape);
	glutKeyboardFunc(anim::keyboard);
	glutMouseFunc(anim::mouse);
	glutIdleFunc(anim::idle);
	glutFullScreen(); //Включение полного экрана, при необходимости для отладки можно отключить

	QueryPerformanceFrequency(&instance.frequency); //Сохранение начального времени и частоты
	QueryPerformanceCounter(&instance.start_time);
}

void anim::display(void) //Общая функция рисования
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Очистка буферов
	vec pos(50, 40, 50), //Установка позиции наблюдателя
		dir(0, 0, 0),
		up(0, 1, 0);
	set_camera(pos, dir, up);
	glRotated(get_time() * 5, 0, 1, 0);
	for (unsigned int i = 0; i < instance.stack.size(); i++) //Вызов функций рисования для каждого объекта
	{
		instance.stack[i]->draw(instance.stack);
	}
	glFinish();
	glutSwapBuffers();
}

void anim::set_camera(vec pos, vec dir, vec up) //Задание точки обзора и наблюдения
{
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
		dir.x, dir.y, dir.z,
		up.x, up.y, up.z);
}

void anim::reshape(int width, int height) //Функция установки области вывода и ситемы координат 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(90, (double) width / height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void anim::keyboard(unsigned char key, int x, int y) //Обработка нажатий на клавиатуру
{
	if (key == 27) //Если esc, то выход
		exit(0);
	for (unsigned int i = 0; i < instance.stack.size(); i++) //Вызов обработчиков всех объектов
		instance.stack[i]->keyboard(key, x, y);
}

void anim::mouse(int button, int state, int mouse_x, int mouse_y) //Аналогично с мышкой
{
	for (unsigned int i = 0; i < instance.stack.size(); i++)
		instance.stack[i]->mouse(button, state, mouse_x, mouse_y);
}

void anim::idle(void) //Обработка в случае остутствия действий
{
	LARGE_INTEGER tmp_time;

	QueryPerformanceCounter(&tmp_time);
	instance.time = (tmp_time.QuadPart - instance.start_time.QuadPart) * 1.0 / instance.frequency.QuadPart;
	glutPostRedisplay();
}

double anim::get_time(void) //Получение времени с начала работы программы в секундах
{
	return instance.time;
}