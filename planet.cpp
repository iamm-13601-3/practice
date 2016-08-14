#include "animation.h"
#include "planet.h"

#pragma warning(disable:4996)


planet planet::instance; 

AUX_RGBImageRec* photo_image;

planet::planet(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	photo_image = auxDIBImageLoad (L"images/01.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

planet::planet(double radius, double mass)
{
	this->radius = radius;
	this->mass = mass;
	this->type = PLANET;
}  

void planet::draw(vector<object*> stack)
{
	GLUquadricObj *quadObj;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
	glRotated(9, 0, 0, 0);
	gluSphere(quadObj, radius, 1000, 1000);
	glPopMatrix();
	gluDeleteQuadric(quadObj);
	auxSwapBuffers();
}