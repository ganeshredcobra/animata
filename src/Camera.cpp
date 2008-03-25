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

#include "Camera.h"
#include <math.h>
#include <stdio.h>

#if defined(__APPLE__)
	#include <OPENGL/gl.h>
	#include <OPENGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

Camera::Camera()
{
	target.set(0.f, 0.f, 0.f);
	orientation.set(0.f, 0.f, 0.f);
	upvector.set(0.f, -1.f, 0.f);

	distance = 150.f;
	fov = 90.f;

	width = height = 0;
	pictureWidth = pictureHeight = 0;

	zNear = 0.1f;
	zFar = 1000.f;

	init = false;

	parent = NULL;
}

/**
 * sets the projection parameters by defining the width and height of the view at distance
 * repositions the target if needed
 * /param w width of the view
 * /param h height of the view
 **/
void Camera::setSize(int w, int h)
{
	width = w;
	height = h;

	pictureWidth = w;
	pictureHeight = h;

	if(!init)
	{
		target.x = width  / 2;
		target.y = height / 2;

		init = true;
	}

	setAspect(width, height);

	if(parent)
	{
		int pw = parent->getWidth();
		int ph = parent->getHeight();

		setAspect(pw, ph);

		if(width / aspect > height)
		{
			pictureHeight = height;
			pictureWidth = (int)(pictureHeight * aspect);
		}
		else
		{
			pictureWidth = width;
			pictureHeight = (int)(pictureWidth / aspect);
		}

	}

	setupPerspective();
}

void Camera::setAspect(int w, int h)
{
	aspect = h > 0 ? (double)w / (double)h : 1.0;

	double radtheta, degtheta;
	radtheta = 2.0 * atan2(h/2, distance);
	degtheta = (180.0 * radtheta) / M_PI;

	fov = degtheta;
}

/**
 * sets up the projection matrix by perspective transformation
 * also the vertical fov is stored after it is computed from the dimension of the view
 * in the beginning the projection matrix is cleared
**/
void Camera::setupPerspective()
{
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(fov, aspect, zNear, zFar);

	setupViewport();
}

/**
 * sets up the projection matrix by orthographic transformation
 * also the vertical fov is stored after it is computed from the dimension of the view
 * in the beginning the projection matrix is cleared
**/
void Camera::setupOrtho()
{
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
//	glOrtho(0, width, 0, height, -1, 1);
	glOrtho((width - pictureWidth) / 2, width + (width - pictureWidth) / 2, (height - pictureHeight) / 2, height + (height - pictureHeight) / 2, -1, 1);

	setupViewport();
}

/**
 * sets up the projection matrix used by picking functions
 * this is done via multiplying the a special picking matrix onto the current projection matrix
 * /param x x coordinate of picking position
 * /param y y coordinate of picking position
 * /param radius the radius around the given point where picking can occure
 **/
void Camera::setupPickingProjection(int x, int y, int radius)
{
	// get the boundaries of actual viewport
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), (GLdouble)radius, (GLdouble)radius, viewport);

//	glOrtho(0, width, 0, height, -1, 1);
	glOrtho((width - pictureWidth) / 2, width + (width - pictureWidth) / 2, (height - pictureHeight) / 2, height + (height - pictureHeight) / 2, -1, 1);
}

/**
 * sets up the modelview matrix by lookAt, based on the camera properties
 * in the beginning the modelview matrix is cleared
 **/
void Camera::setupModelView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(target.x, target.y, -distance,
			  target.x, target.y, 0,
			  upvector.x, upvector.y, upvector.z);
}

void Camera::setupViewport()
{
//	glViewport(0, 0, pictureWidth, pictureHeight);
	glViewport((width - pictureWidth) / 2, (height - pictureHeight) / 2, pictureWidth, pictureHeight);
}

