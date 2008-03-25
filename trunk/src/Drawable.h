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

#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

class Drawable
{
	public:

		virtual ~Drawable() {}

		// draw the primitive
		virtual void draw(GLenum mode, int active = 1) = 0;
		// select the ith element which has the given type
		virtual void select(unsigned i, int type) = 0;
		// select ith element if inside the given circle
		virtual void circleSelect(unsigned i, int type, int xc, int yc, float r) = 0;
};

#endif

