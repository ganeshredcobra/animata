/*
 Animata

 Copyright (C) 2007 Peter Nemeth, Gabor Papp, Bence Samu
 Kitchen Budapest, <http://animata.kibu.hu/>

 This file is part of Animata.

 Animata is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Animata is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Animata. If not, see <http://www.gnu.org/licenses/>.

*/

#include "Texture.h"

#include <stdio.h>
#include <math.h>

Texture::Texture(const char *filename, int w, int h, int d, unsigned char* p, int reuseResource)
{
	sWrap = tWrap = GL_CLAMP;

	minFilter = GL_LINEAR_MIPMAP_LINEAR;
	magFilter = GL_LINEAR;

	this->filename = filename;

	width = w;
	height = h;
	depth = d;

	data = (unsigned char*)p;

	x = y = 0.f;
	scale = 1.f;

	if(!reuseResource)
	{
		// required because the data isnt padded at the end of each texel row
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &glResource);
		glBindTexture(GL_TEXTURE_2D, glResource);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, magFilter);
		gluBuild2DMipmaps(GL_TEXTURE_2D, depth, width, height, d == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		glResource = reuseResource;
	}

//	printf("Texture created. name=%s, gl=%i, w=%i, h=%i, d=%i\n",
//			filename, (int)glResource, width, height, depth);
}

Texture::~Texture()
{
//	printf("Texture deleted. name=%s, gl=%i, w=%i, h=%i, d=%i\n",
//			filename, (int)glResource, width, height, depth);

	glDeleteTextures(1, &glResource);
}

/**
 * Creates a clone from this texture. A new texture will be allocated, but it will use this texture's glResource.
 **/
Texture *Texture::clone()
{
	return new Texture(filename, width, height, depth, data, glResource);
}

void Texture::scaleAroundPoint(float s, float ox, float oy)
{
	if (s > MIN_SCALE)
	{
		x -= ox * (s - scale);
		y -= oy * (s - scale);

		scale = s;
	}
}

/**
 * gets the alpha of the texture colour at the given position
 * \param x x-coordinate of texel
 * \param y y-coordinate of texel
 * \return alpha value from 0 to 255
 **/
int Texture::getTexelAlpha(float x, float y)
{
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return 0;
	else if (depth != 4)
		return 255;
	else
		return data[((int)x + ((int)y)*width)*4 + 3];
}

/**
 * approximation of the triangle alpha by recursively subdividing the triangle
 * and summing the alpha of the centroids
 *
 * \param x0 x-coordinate of vertex0
 * \param y0 y-coordinate of vertex0
 * \param x1 x-coordinate of vertex1
 * \param y1 y-coordinate of vertex1
 * \param x2 x-coordinate of vertex2
 * \param y2 y-coordinate of vertex2
 * \param maxIter maximum number of iterations
 * \param iterLevel current iteration level used only internally
 * \return alpha value from 0 to 255
 **/
int Texture::getTriangleAlpha(float x0, float y0, float x1, float y1,
				float x2, float y2, int maxIter /*= 3*/, int iterLevel /*= 1*/)
{
	int alpha;

	if (depth != 4)
		return 255;

	float xs = (x0 + x1 + x2) / 3.0;
	float ys = (y0 + y1 + y2) / 3.0;

	alpha = getTexelAlpha(xs, ys);

	if (iterLevel == maxIter)
		return alpha;

	// subdivide triangle
	alpha += getTriangleAlpha(x0, y0, x1, y1, xs, ys, maxIter, iterLevel + 1) +
		getTriangleAlpha(x0, y0, xs, ys, x2, y2, maxIter, iterLevel + 1) +
		getTriangleAlpha(xs, ys, x1, y1, x2, y2, maxIter, iterLevel + 1);

	if (iterLevel == 1)
	{
		// calculate number of triangles checked in total -
		// sum of a geometric progression
		int n = (int)((pow(3, maxIter) - 1) / (3 - 1));
		// average alpha
		alpha = alpha / n;
	}

	return alpha;
}

void Texture::draw(int mouseOver)
{
	glBindTexture(GL_TEXTURE_2D, glResource);

	if(mouseOver)
	{
		glColor3f(1.f, 1.f, 0.f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
			glVertex2f(viewx - BORDER, viewy + BORDER);
			glVertex2f(viewx + width * scale + BORDER, viewy + BORDER);
			glVertex2f(viewx + width * scale + BORDER, (viewy - height * scale - BORDER));
			glVertex2f(viewx - BORDER, (viewy - height * scale - BORDER));
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.f, 1.f, 1.f);
	}

	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2f(viewx, viewy);
		glTexCoord2f(1.f, 0.f); glVertex2f(viewx + (float)width * scale, viewy);
		glTexCoord2f(1.f, 1.f); glVertex2f(viewx + (float)width * scale, (viewy - (float)height * scale));
		glTexCoord2f(0.f, 1.f); glVertex2f(viewx, (viewy - (float)height * scale));
	glEnd();

/*
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// mirror over y, because drawing occures in orthographic mode
	// (0, 0) is the top-left corner of the window, but bottom-left corner in opengl

	if(mouseOver)
	{
		glColor3f(1.f, 1.f, 0.f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
			glVertex2f(x - BORDER, viewport[3] - y - BORDER);
			glVertex2f(x + width * scale + BORDER, viewport[3] - y - BORDER);
			glVertex2f(x + width * scale + BORDER, viewport[3] - (y + height * scale + BORDER));
			glVertex2f(x - BORDER, viewport[3] - (y + height * scale + BORDER));
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.f, 1.f, 1.f);
	}

	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2f(x, viewport[3] - y);
		glTexCoord2f(1.f, 0.f); glVertex2f(x + (float)width * scale, viewport[3] - y);
		glTexCoord2f(1.f, 1.f); glVertex2f(x + (float)width * scale, viewport[3] - (y + (float)height * scale));
		glTexCoord2f(0.f, 1.f); glVertex2f(x, viewport[3] - (y + (float)height * scale));
	glEnd();
*/
}

