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

#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

#if defined(__APPLE__)
	#include <OPENGL/gl.h>
	#include <OPENGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <Fl/Fl.h>
#include <Fl/Fl_Gl_Window.h>
#include "Camera.h"
#include "Layer.h"
#include "animata.h"

class Playback : public Fl_Gl_Window
{
	private:

		Camera		*camera;
		Layer		*rootLayer;

		bool		fullscreen;
		int			ox, oy, ow, oh;

		void		*glContext;

	public:

		static const unsigned RENDER_PLAYBACK = GL_SELECT + 1;

		Playback(int x, int y, int w, int h, const char* l = NULL);
		~Playback();

		void draw();
		int handle(int);

		void show();
		void hide();

		inline Camera *getCamera() { return camera; }

		inline void setRootLayer(Layer *r) { rootLayer = r; }
};

#endif

