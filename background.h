#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED
#pragma once

#include "animation.h"

class background : public object
{
public:
	AUX_RGBImageRec *texture1;
	background()
	{
	type = BACKGROUND;
	load_texture("images/space.bmp");
	}
	virtual void load_texture(LPCSTR name)
	{
		texture1 = auxDIBImageLoadA(name);
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
	}
	void draw(vector<object*> stack)
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);

		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0, 0.0);
		glVertex3f(150, 150, 150);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(150, 150, -150);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(150, -150, -150);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(150, -150, 150);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(150, 150, 150);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(150, -150, 150);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-150, -150, 150);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-150, 150, 150);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(150, 150, -150);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(150, -150, -150);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-150, -150, -150);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-150, 150, -150);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(150, -150, 150);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(150, -150, -150);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-150, -150, -150);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-150, -150, 150);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(150, 150, 150);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(150, 150, -150);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-150, 150, -150);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-150, 150, 150);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-150, 150, 150);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-150, -150, 150);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-150, -150, -150);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-150, 150, -150);
		glEnd();	
	}
	
};
#endif
