#include "animation.h"
#include "planet.h"

#pragma warning(disable:4996)

planet::planet(double radius, double mass, vec coord)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	photo_image = auxDIBImageLoad(L"practice.git/images/01.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	this->coord = coord;
	this->radius = radius;
	this->mass = mass;
	this->type = PLANET;
	rect_set(&view.screen, -100.0, -100.0, -100.0, 100.0, 100.0, 100.0);
	rect_set(&view.math, -5 * math_size * mul, -5 * math_size * mul, -5 * math_size * mul, 5 * math_size * mul, 5 * math_size * mul, 5 * math_size * mul);

}  

void planet::draw(vector<object*> stack)
{
	GLUquadricObj *quadObj;
#if USE_SHADERS
	setShaders(shaders, "practice.git/shaders/default_vert_shader.glsl", "practice.git/shaders/default_frag_shader.glsl");
	glUseProgram(shaders);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		photo_image->sizeX,
		photo_image->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		photo_image->data);
	quadObj = gluNewQuadric();
	gluQuadricTexture(quadObj, GL_TRUE);
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	glColor3d(1,1,1);
	glPushMatrix();
	glRotated(66.5, -1, 0, 0);
	glPushMatrix();

	trans = vec_transform(coord, &view.math, &view.screen);

	glTranslatef(trans.x, trans.y, trans.z);
	glRotated(anim::get_time() * 15, 0, 0, 1);
	gluSphere(quadObj, radius, 1000, 1000);
	glPopMatrix();
	glPopMatrix();
	gluDeleteQuadric(quadObj);
	auxSwapBuffers();
}