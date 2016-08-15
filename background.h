#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED
#pragma once

#include "animation.h"

class background : public object
{
public:
	AUX_RGBImageRec *texture1;
	double size;
	background()
	{
	type = BACKGROUND;
	load_texture("images/space 3 compressed.bmp");
	size = 150;
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

		for (unsigned int i = 0; i < stack.size(); i++) //Расширение фона при подлете спутника к нему
			if (stack[i]->type = SATELLITE)
				if (((satellite*)stack[i])->r.b.x > size - 20 ||
					((satellite*)stack[i])->r.b.y > size - 20 ||
					((satellite*)stack[i])->r.b.z > size - 20)
					size += 0.5;

		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);

		glTexCoord2f(1.0 / 2, 2.0 / 3);
		glVertex3f(size, size, size);
		glTexCoord2f(3.0 / 4, 2.0 / 3);
		glVertex3f(size, size, -size);
		glTexCoord2f(3.0 / 4, 1.0 / 3);
		glVertex3f(size, -size, -size);
		glTexCoord2f(1.0 / 2, 1.0 / 3);
		glVertex3f(size, -size, size);

		glTexCoord2f(1.0 / 2, 2.0 / 3);
		glVertex3f(size, size, size);
		glTexCoord2f(1.0 / 2, 1.0 / 3);
		glVertex3f(size, -size, size);
		glTexCoord2f(1.0 / 4, 1.0 / 3);
		glVertex3f(-size, -size, size);
		glTexCoord2f(1.0 / 4, 2.0 / 3);
		glVertex3f(-size, size, size);

		glTexCoord2f(3.0 / 4, 2.0 / 3);
		glVertex3f(size, size, -size);
		glTexCoord2f(3.0 / 4, 1.0 / 3);
		glVertex3f(size, -size, -size);
		glTexCoord2f(1.0, 1.0 / 3);
		glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0, 2.0 / 3);
		glVertex3f(-size, size, -size);

		glTexCoord2f(1.0 / 2, 1.0 / 3);
		glVertex3f(size, -size, size);
		glTexCoord2f(1.0 / 2, 0.0);
		glVertex3f(size, -size, -size);
		glTexCoord2f(1.0 / 4, 0.0);
		glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0 / 4, 1.0 / 3);
		glVertex3f(-size, -size, size);

		glTexCoord2f(1.0 / 2, 2.0 / 3);
		glVertex3f(size, size, size);
		glTexCoord2f(1.0 / 2, 1.0);
		glVertex3f(size, size, -size);
		glTexCoord2f(1.0 / 4, 1.0);
		glVertex3f(-size, size, -size);
		glTexCoord2f(1.0 / 4, 2.0 / 3);
		glVertex3f(-size, size, size);

		glTexCoord2f(1.0 / 4, 2.0 / 3);
		glVertex3f(-size, size, size);
		glTexCoord2f(1.0 / 4, 1.0 / 3);
		glVertex3f(-size, -size, size);
		glTexCoord2f(0.0, 1.0 / 3);
		glVertex3f(-size, -size, -size);
		glTexCoord2f(0.0, 2.0 / 3);
		glVertex3f(-size, size, -size);
		glEnd();	
	}
	
};
#endif
