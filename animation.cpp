#include "background.h"

anim anim::instance; //��������� ������ anim

anim::anim(void) //���������� ������ anim
{
	FreeConsole(); //���������� ������ �������


	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //����������� ��������� ������
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL"); //�������� ����

	glEnable(GL_LIGHTING); //���������
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D); //��������� �������

	glutDisplayFunc(anim::display); //����� ������� ��������� ������� � ���������
	glutReshapeFunc(anim::reshape);
	glutKeyboardFunc(anim::keyboard);
	glutMouseFunc(anim::mouse);
	glutIdleFunc(anim::idle);
	glutFullScreen(); //��������� ������� ������, ��� ������������� ��� ������� ����� ���������

	QueryPerformanceFrequency(&instance.frequency); //���������� ���������� ������� � �������
	QueryPerformanceCounter(&instance.start_time);
}

void anim::display(void) //����� ������� ���������
{
	vec pos(50, 40, 50), //��������� ������� �����������
		dir(0, 0, 0),
		up(0, 1, 0);
	GLfloat light1_diffuse[] = {1.0, 1.0, 1.0};
	GLfloat light1_position[] = {
		100.0 /** sin(get_time())*/,
		100.0 /** cos(get_time())*/,
		0, 
		1.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //������� �������
	set_camera(pos, dir, up);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glPushMatrix();
	glRotated(get_time() * 5, 0, 1, 0);
	for (unsigned int i = 0; i < instance.stack.size(); i++) //����� ������� ��������� ��� ������� �������
		instance.stack[i]->draw(instance.stack);
	glPopMatrix();
	glFinish();
	glutSwapBuffers();
}

void anim::set_camera(vec pos, vec dir, vec up) //������� ����� ������ � ����������
{
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,
		dir.x, dir.y, dir.z,
		up.x, up.y, up.z);
}

void anim::reshape(int width, int height) //������� ��������� ������� ������ � ������ ��������� 
{
	double size = 300 * 1.8;

	for (unsigned int i = 0; i < instance.stack.size(); i++)
		if (instance.stack[i]->type == BACKGROUND)
			size = ((background*)(instance.stack[i]))->size * 4 + 20;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (double) width / height, 1, size);
	glMatrixMode(GL_MODELVIEW);
}

void anim::keyboard(unsigned char key, int x, int y) //��������� ������� �� ����������
{
	if (key == 27) //���� esc, �� �����
		exit(0);
	for (unsigned int i = 0; i < instance.stack.size(); i++) //����� ������������ ���� ��������
		instance.stack[i]->keyboard(key, x, y);
}

void anim::mouse(int button, int state, int mouse_x, int mouse_y) //���������� � ������
{
	for (unsigned int i = 0; i < instance.stack.size(); i++)
		instance.stack[i]->mouse(button, state, mouse_x, mouse_y);
}

void anim::idle(void) //��������� � ������ ���������� ��������
{
	LARGE_INTEGER tmp_time;

	QueryPerformanceCounter(&tmp_time);
	instance.time = (tmp_time.QuadPart - instance.start_time.QuadPart) * 1.0 / instance.frequency.QuadPart;
	glutPostRedisplay();
}

double anim::get_time(void) //��������� ������� � ������ ������ ��������� � ��������
{
	return instance.time;
}