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

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#if defined(__APPLE__)
#include <OPENGL/gl.h>
#include <OPENGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Vector2D.h"

#define MIN_SCALE 0.1f

class Texture
{
	private:
		unsigned char	*data;
		int				depth;

		float			scale;

		GLuint			glResource;

		int				sWrap;
		int				tWrap;

		int				minFilter;
		int				magFilter;

		int getTexelAlpha(float x, float y);

		const char		*filename;

	public:

		static const int BORDER = 0;

		float		x;
		float		y;

		float		viewx;
		float		viewy;

		int			width;
		int			height;

		Texture(const char *filename, int w, int h, int d, unsigned char* d, int reuseResource = 0);
		~Texture();

		/// draws the texture
		void draw(int mouseOver = 0);

		/// clones this texture to a new texture which uses the old glResource
		Texture *clone();

		inline unsigned char* getData() { return data; }

		/// return texture scaling value
		inline float getScale(void) { return scale; }
		/// sets texture scaling value
		inline void setScale(float s) { scale = s; }
		void scaleAroundPoint(float s, float ox, float oy);

		inline int getWidth() { return width; }
		inline int getHeight() { return height; }
		inline GLuint getGlResource() { return glResource; }

		int getTriangleAlpha(float x0, float y0, float x1, float y1,
				float x2, float y2, int maxIter = 3, int iterLevel = 1);

		/// returns texture filename
		inline const char *getFilename(void) { return filename; }

};

#endif

