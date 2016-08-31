#include "background.h"

anim* instance;

anim::anim(void) //Констуктор класса anim
{
#ifdef NDEBUG
	FreeConsole(); //Отключение вызова консоли
#endif

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //Стандартная настройка вывода
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL"); //Создание окна

	glEnable(GL_LIGHTING); //Освещение
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D); //Включение текстур

	glutDisplayFunc(instance->display); //Вызов функций обработки событий и рисования
	glutReshapeFunc(instance->reshape);
	glutKeyboardFunc(instance->keyboard);
	glutMouseFunc(instance->mouse_button);
	glutMotionFunc(instance->mouse_move);
	glutIdleFunc(instance->idle);
	glutFullScreen(); //Включение полного экрана, при необходимости для отладки можно отключить
#if USE_SHADERS
	glewInit();
#endif
	QueryPerformanceFrequency(&frequency); //Сохранение начального времени и частоты
	QueryPerformanceCounter(&start_time);

	GetCursorPos(&start_pos);
}

void anim::display(void) //Общая функция рисования
{
	vec pos(50, 40, 50), //Установка позиции наблюдателя
		dir(0, 0, 0),
		up(0, 1, 0);
	GLfloat light1_diffuse[] = {1.0, 1.0, 1.0};
	GLfloat light1_position[] = {
		100.0 * sin(instance->get_time()),
		0,
		-100.0 * cos(get_time()),
		1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Очистка буферов
	set_camera(pos, dir, up);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glPushMatrix();
	glRotated(instance->angle_y, 0, 1, 0);
	glRotated(instance->angle_x, 1, 0, -1);
//	glRotated(get_time() * 5, 0, 1, 0);
	for (unsigned int i = 0; i < instance->stack.size(); i++) //Вызов функций рисования для каждого объекта
		instance->stack[i]->draw(instance->stack);
	glPopMatrix();
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
	double size = 300 * 1.8;

	for (unsigned int i = 0; i < instance->stack.size(); i++)
		if (instance->stack[i]->type == BACKGROUND)
			size = ((background*)(instance->stack[i]))->size * 4 + 20;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (double) width / height, 1, size);
	glMatrixMode(GL_MODELVIEW);
}

void anim::keyboard(unsigned char key, int x, int y) //Обработка нажатий на клавиатуру
{
	if (key == 27) //Если esc, то выход
		exit(0);
	for (unsigned int i = 0; i < instance->stack.size(); i++) //Вызов обработчиков всех объектов
		instance->stack[i]->keyboard(key, x, y);
}

void anim::mouse_button(int button, int state, int mouse_x, int mouse_y)
{
	for (unsigned int i = 0; i < instance->stack.size(); i++)
		instance->stack[i]->mouse_button(button, state, mouse_x, mouse_y);
}

void anim::mouse_move(int mouse_x, int mouse_y)
{
	instance->angle_y = mouse_x - instance->start_pos.x;
//	instance.angle_x = mouse_y / 3;
	for (unsigned int i = 0; i < instance->stack.size(); i++)
		instance->stack[i]->mouse_move(mouse_x, mouse_y);
}

void anim::idle(void) //Обработка в случае остутствия действий
{
	LARGE_INTEGER tmp_time;

	QueryPerformanceCounter(&tmp_time);
	instance->time = (tmp_time.QuadPart - instance->start_time.QuadPart) * 1.0 / instance->frequency.QuadPart;
	glutPostRedisplay();
}

double anim::get_time(void) //Получение времени с начала работы программы в секундах
{
	return instance->time;
}

anim** anim::get_ref(void)
{
	return &instance;
}