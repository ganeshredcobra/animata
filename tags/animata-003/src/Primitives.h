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

#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include "Bone.h"
#include "Joint.h"

#define fillColor(r, g, b, a) \
{ \
	fillColorR = (r)/255.0f; \
	fillColorG = (g)/255.0f; \
	fillColorB = (b)/255.0f; \
	fillColorA = (a)/255.0f; \
	doFill = true; \
}

#define strokeColor(r, g, b, a) \
{ \
	strokeColorR = (r)/255.0f; \
	strokeColorG = (g)/255.0f; \
	strokeColorB = (b)/255.0f; \
	strokeColorA = (a)/255.0f; \
	doStroke = true; \
}

void drawBone(Bone *b, int mouseOver, int active);
void drawBoneWhileConnecting(float x1, float y1, float x2, float y2);

void drawJoint(Joint *j, int mouseOver, int active);

void drawVertex(Vertex *v, int mouseOver, int active);
void drawVertexAttached(Vertex *v);

void drawFace(Face *face, int mouseOver = 0, int active = 1);
void drawFaceWhileConnecting(float x1, float y1, float x2, float y2);

void drawSelectionBox(float x1, float y1, float x2, float y2);
void drawSelectionCircle(float x, float y, float r);

void fill(bool b);
void stroke(bool b);
void strokeWeight(float w);

void drawLine(float x1, float y1, float x2, float y2);
void drawCircle(float x, float y, float r);
void drawRect(float x, float y, float size);
void drawTriangle(float x1, float y1, float x2, float y2,float x3, float y3);

#endif

