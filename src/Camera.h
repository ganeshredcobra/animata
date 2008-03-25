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

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3D.h"

/// camera class, manages projection and modelview matrices
class Camera
{
	private:
		Vector3D	target;			///< position of the target
		Vector3D	orientation;	///< orientation of the camera
		Vector3D	upvector;		///< vector defining the up

		float		distance;		///< distance from the target
		float		fov;			///< vertical field of view in degrees

		int			width;			///< width of the camerapicture at distance
		int			height;			///< height of the camerapicture at distance
		double		aspect;

		Camera		*parent;		///< the camera which this will clone, this will show the same picture as the parent
		int			pictureWidth;	///< width of the parent's picture
		int			pictureHeight;	///< height of the parent's picture

		float		zNear;
		float		zFar;

		bool		init;			///< shows if the camera target has been already set

	public:
		Camera();

		void setSize(int w, int h);
		void setAspect(int w, int h);

		inline void setParent(Camera *c) { parent = c; }

		void setupViewport();
		void setupPerspective();
		void setupOrtho();
		void setupPickingProjection(int x, int y, int radius);

		void setupModelView();

		inline Vector3D *getTarget() { return &target; }
		inline int getWidth() { return width; }
		inline int getHeight() { return height; }
		inline float getDistance() { return distance; }
};

#endif

