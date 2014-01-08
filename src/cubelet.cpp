 
#ifndef _CUBELET_CPP
#define _CUBELET_CPP

#include "cubelet.h"


const GLfloat Cubelet::FaceletMaterial[][4] = {
	{.05, .05, .05, 1.0},	// N = Null (black)
	{1.0, 0.1, 0.1, 1.0},   // R = Red
	{0.2, 1.0, 0.2, 1.0},   // G = Green
	{0.2, 0.2, 1.0, 1.0},   // B = Blue
	{1.0, 1.0, 0.2, 1.0},   // Y = Yellow
	{1.0, 0.3, 0.0, 1.0},   // O = Orange
	{1.0, 1.0, 1.0, 1.0},   // W = White
	{0.4, 0.4, 0.4, 1.0}    // S = Shadow (Gray)
};

GLuint Cubelet::primitives = 0;
const GLfloat Cubelet::Diffuse[]  = { 0.5, 0.5, 0.5, 1.0 };
const GLfloat Cubelet::Specular[] = { .01, .01, .01, 1.0 };

const GLfloat Cubelet::CubeLen = 0.50;
const GLfloat Cubelet::CubeRound = Cubelet::CubeLen - 0.03;
const GLfloat Cubelet::StickerLong = Cubelet::CubeRound - 0.03;
const GLfloat Cubelet::StickerShort = Cubelet::StickerLong - 0.03;
const GLfloat Cubelet::StickerDepth = Cubelet::CubeLen + 0.001;

Cubelet::Cubelet(int cube_id)
{
	id    = cube_id;
	pick  = false;
	
	for (int i = 0; i < 6; ++i)
		face[i] = N;

	pos[0] = 0.0;
	pos[1] = 0.0;
	pos[2] = 0.0;
}


GLuint Cubelet::InitPrimitives()
{
	if (primitives)
		return primitives;

	GLuint list = glGenLists (7);

	if (!list)
		return 0;

	glNewList (list, GL_COMPILE);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, Diffuse);
		glMaterialfv (GL_FRONT, GL_SPECULAR, Specular);
		
		glBegin (GL_QUADS);
		glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, FaceletMaterial[N]);
		
		/* Put sticker here */
		
		glNormal3f (0.00, 0.00, 1.00);
		glVertex3f (-CubeRound, -CubeRound, CubeLen);
		glVertex3f (CubeRound, -CubeRound, CubeLen);
		glVertex3f (CubeRound, CubeRound, CubeLen);
		glVertex3f (-CubeRound, CubeRound, CubeLen);
		glNormal3f (0.00, 0.00, -1.00);
		glVertex3f (-CubeRound, CubeRound, -CubeLen);
		glVertex3f (CubeRound, CubeRound, -CubeLen);
		glVertex3f (CubeRound, -CubeRound, -CubeLen);
		glVertex3f (-CubeRound, -CubeRound, -CubeLen);
		glNormal3f (-1.00, 0.00, 0.00);
		glVertex3f (-CubeLen, -CubeRound, CubeRound);
		glVertex3f (-CubeLen, CubeRound, CubeRound);
		glVertex3f (-CubeLen, CubeRound, -CubeRound);
		glVertex3f (-CubeLen, -CubeRound, -CubeRound);
		glNormal3f (1.00, 0.00, 0.00);
		glVertex3f (CubeLen, -CubeRound, -CubeRound);
		glVertex3f (CubeLen, CubeRound, -CubeRound);
		glVertex3f (CubeLen, CubeRound, CubeRound);
		glVertex3f (CubeLen, -CubeRound, CubeRound);
		glNormal3f (0.00, -1.00, 0.00);
		glVertex3f (CubeRound, -CubeLen, -CubeRound);
		glVertex3f (CubeRound, -CubeLen, CubeRound);
		glVertex3f (-CubeRound, -CubeLen, CubeRound);
		glVertex3f (-CubeRound, -CubeLen, -CubeRound);
		glNormal3f (0.00, 1.00, 0.00);
		glVertex3f (-CubeRound, CubeLen, -CubeRound);
		glVertex3f (-CubeRound, CubeLen, CubeRound);
		glVertex3f (CubeRound, CubeLen, CubeRound);
		glVertex3f (CubeRound, CubeLen, -CubeRound);

		/* Edges of cubit */
  
		glNormal3f (-1.00, -1.00, 0.00);
		glVertex3f (-CubeRound, -CubeLen, -CubeRound);
		glVertex3f (-CubeRound, -CubeLen, CubeRound);
		glVertex3f (-CubeLen, -CubeRound, CubeRound);
		glVertex3f (-CubeLen, -CubeRound, -CubeRound);
		glNormal3f (1.00, 1.00, 0.00);
		glVertex3f (CubeRound, CubeLen, -CubeRound);
		glVertex3f (CubeRound, CubeLen, CubeRound);
		glVertex3f (CubeLen, CubeRound, CubeRound);
		glVertex3f (CubeLen, CubeRound, -CubeRound);
		glNormal3f (-1.00, 1.00, 0.00);
		glVertex3f (-CubeLen, CubeRound, -CubeRound);
		glVertex3f (-CubeLen, CubeRound, CubeRound);
		glVertex3f (-CubeRound, CubeLen, CubeRound);
		glVertex3f (-CubeRound, CubeLen, -CubeRound);
		glNormal3f (1.00, -1.00, 0.00);
		glVertex3f (CubeLen, -CubeRound, -CubeRound);
		glVertex3f (CubeLen, -CubeRound, CubeRound);
		glVertex3f (CubeRound, -CubeLen, CubeRound);
		glVertex3f (CubeRound, -CubeLen, -CubeRound);
		glNormal3f (0.00, -1.00, -1.00);
		glVertex3f (-CubeRound, -CubeRound, -CubeLen);
		glVertex3f (CubeRound, -CubeRound, -CubeLen);
		glVertex3f (CubeRound, -CubeLen, -CubeRound);
		glVertex3f (-CubeRound, -CubeLen, -CubeRound);
		glNormal3f (0.00, 1.00, 1.00);
		glVertex3f (-CubeRound, CubeRound, CubeLen);
		glVertex3f (CubeRound, CubeRound, CubeLen);
		glVertex3f (CubeRound, CubeLen, CubeRound);
		glVertex3f (-CubeRound, CubeLen, CubeRound);
		glNormal3f (0.00, -1.00, 1.00);
		glVertex3f (-CubeRound, -CubeLen, CubeRound);
		glVertex3f (CubeRound, -CubeLen, CubeRound);
		glVertex3f (CubeRound, -CubeRound, CubeLen);
		glVertex3f (-CubeRound, -CubeRound, CubeLen);
		glNormal3f (0.00, 1.00, -1.00);
		glVertex3f (-CubeRound, CubeLen, -CubeRound);
		glVertex3f (CubeRound, CubeLen, -CubeRound);
		glVertex3f (CubeRound, CubeRound, -CubeLen);
		glVertex3f (-CubeRound, CubeRound, -CubeLen);
		glNormal3f (-1.00, 0.00, -1.00);
		glVertex3f (-CubeLen, -CubeRound, -CubeRound);
		glVertex3f (-CubeLen, CubeRound, -CubeRound);
		glVertex3f (-CubeRound, CubeRound, -CubeLen);
		glVertex3f (-CubeRound, -CubeRound, -CubeLen);
		glNormal3f (1.00, 0.00, 1.00);
		glVertex3f (CubeLen, -CubeRound, CubeRound);
		glVertex3f (CubeLen, CubeRound, CubeRound);
		glVertex3f (CubeRound, CubeRound, CubeLen);
		glVertex3f (CubeRound, -CubeRound, CubeLen);
		glNormal3f (1.00, 0.00, -1.00);
		glVertex3f (CubeRound, -CubeRound, -CubeLen);
		glVertex3f (CubeRound, CubeRound, -CubeLen);
		glVertex3f (CubeLen, CubeRound, -CubeRound);
		glVertex3f (CubeLen, -CubeRound, -CubeRound);
		glNormal3f (-1.00, 0.00, 1.00);
		glVertex3f (-CubeRound, -CubeRound, CubeLen);
		glVertex3f (-CubeRound, CubeRound, CubeLen);
		glVertex3f (-CubeLen, CubeRound, CubeRound);
		glVertex3f (-CubeLen, -CubeRound, CubeRound);
		glEnd();

		/* Corners of cubit */
		
		glBegin (GL_TRIANGLES);
		glNormal3f (1.00, 1.00, 1.00);
		glVertex3f (CubeRound, CubeRound, CubeLen);
		glVertex3f (CubeLen, CubeRound, CubeRound);
		glVertex3f (CubeRound, CubeLen, CubeRound);
		glNormal3f (-1.00, -1.00, -1.00);
		glVertex3f (-CubeRound, -CubeLen, -CubeRound);
		glVertex3f (-CubeLen, -CubeRound, -CubeRound);
		glVertex3f (-CubeRound, -CubeRound, -CubeLen);
		glNormal3f (-1.00, 1.00, 1.00);
		glVertex3f (-CubeRound, CubeRound, CubeLen);
		glVertex3f (-CubeRound, CubeLen, CubeRound);
		glVertex3f (-CubeLen, CubeRound, CubeRound);
		glNormal3f (1.00, -1.00, -1.00);
		glVertex3f (CubeLen, -CubeRound, -CubeRound);
		glVertex3f (CubeRound, -CubeLen, -CubeRound);
		glVertex3f (CubeRound, -CubeRound, -CubeLen);
		glNormal3f (1.00, -1.00, 1.00);
		glVertex3f (CubeRound, -CubeRound, CubeLen);
		glVertex3f (CubeRound, -CubeLen, CubeRound);
		glVertex3f (CubeLen, -CubeRound, CubeRound);
		glNormal3f (-1.00, 1.00, -1.00);
		glVertex3f (-CubeLen, CubeRound, -CubeRound);
		glVertex3f (-CubeRound, CubeLen, -CubeRound);
		glVertex3f (-CubeRound, CubeRound, -CubeLen);
		glNormal3f (-1.00, -1.00, 1.00);
		glVertex3f (-CubeRound, -CubeRound, CubeLen);
		glVertex3f (-CubeLen, -CubeRound, CubeRound);
		glVertex3f (-CubeRound, -CubeLen, CubeRound);
		glNormal3f (1.00, 1.00, -1.00);
		glVertex3f (CubeLen, CubeRound, -CubeRound);
		glVertex3f (CubeRound, CubeRound, -CubeLen);
		glVertex3f (CubeRound, CubeLen, -CubeRound);
		glEnd();
	glEndList();

	glNewList (list + 1, GL_COMPILE);
		glBegin (GL_POLYGON);
		glNormal3f (0.00, 1.00, 0.00);
		glVertex3f (-StickerLong, StickerDepth, -StickerShort);
		glVertex3f (-StickerLong, StickerDepth, StickerShort);
		glVertex3f (-StickerShort, StickerDepth, StickerLong);
		glVertex3f (StickerShort, StickerDepth, StickerLong);
		glVertex3f (StickerLong, StickerDepth, StickerShort);
		glVertex3f (StickerLong, StickerDepth, -StickerShort);
		glVertex3f (StickerShort, StickerDepth, -StickerLong);
		glVertex3f (-StickerShort, StickerDepth, -StickerLong);
		glEnd();

	glEndList();

	glNewList (list + 2, GL_COMPILE);
		glBegin (GL_POLYGON);
		glNormal3f (0.00, -1.00, 0.00);
		glVertex3f (StickerLong, -StickerDepth, -StickerShort);
		glVertex3f (StickerLong, -StickerDepth, StickerShort);
		glVertex3f (StickerShort, -StickerDepth, StickerLong);
		glVertex3f (-StickerShort, -StickerDepth, StickerLong);
		glVertex3f (-StickerLong, -StickerDepth, StickerShort);
		glVertex3f (-StickerLong, -StickerDepth, -StickerShort);
		glVertex3f (-StickerShort, -StickerDepth, -StickerLong);
		glVertex3f (StickerShort, -StickerDepth, -StickerLong);
		glEnd();
	glEndList();

	glNewList (list + 3, GL_COMPILE);
		glBegin (GL_POLYGON);
		glNormal3f (-1.00, 0.00, 0.00);
		glVertex3f (-StickerDepth, -StickerShort, StickerLong);
		glVertex3f (-StickerDepth, StickerShort, StickerLong);
		glVertex3f (-StickerDepth, StickerLong, StickerShort);
		glVertex3f (-StickerDepth, StickerLong, -StickerShort);
		glVertex3f (-StickerDepth, StickerShort, -StickerLong);
		glVertex3f (-StickerDepth, -StickerShort, -StickerLong);
		glVertex3f (-StickerDepth, -StickerLong, -StickerShort);
		glVertex3f (-StickerDepth, -StickerLong, StickerShort);
		glEnd();
	glEndList();

	glNewList (list + 4, GL_COMPILE);
		glBegin (GL_POLYGON);
		glNormal3f (1.00, 0.00, 0.00);
		glVertex3f (StickerDepth, -StickerShort, -StickerLong);
		glVertex3f (StickerDepth, StickerShort, -StickerLong);
		glVertex3f (StickerDepth, StickerLong, -StickerShort);
		glVertex3f (StickerDepth, StickerLong, StickerShort);
		glVertex3f (StickerDepth, StickerShort, StickerLong);
		glVertex3f (StickerDepth, -StickerShort, StickerLong);
		glVertex3f (StickerDepth, -StickerLong, StickerShort);
		glVertex3f (StickerDepth, -StickerLong, -StickerShort);
		glEnd();

	glEndList();

	glNewList (list + 5, GL_COMPILE);
		glBegin (GL_POLYGON);
		glNormal3f (0.00, 0.00, 1.00);
		glVertex3f (-StickerShort, -StickerLong, StickerDepth);
		glVertex3f (StickerShort, -StickerLong, StickerDepth);
		glVertex3f (StickerLong, -StickerShort, StickerDepth);
		glVertex3f (StickerLong, StickerShort, StickerDepth);
		glVertex3f (StickerShort, StickerLong, StickerDepth);
		glVertex3f (-StickerShort, StickerLong, StickerDepth);
		glVertex3f (-StickerLong, StickerShort, StickerDepth);
		glVertex3f (-StickerLong, -StickerShort, StickerDepth);
		glEnd();
	glEndList();

	glNewList (list + 6, GL_COMPILE);
		glBegin (GL_POLYGON);
		glNormal3f (0.00, 0.00, -1.00);
		glVertex3f (-StickerShort, StickerLong, -StickerDepth);
		glVertex3f (StickerShort, StickerLong, -StickerDepth);
		glVertex3f (StickerLong, StickerShort, -StickerDepth);
		glVertex3f (StickerLong, -StickerShort, -StickerDepth);
		glVertex3f (StickerShort, -StickerLong, -StickerDepth);
		glVertex3f (-StickerShort, -StickerLong, -StickerDepth);
		glVertex3f (-StickerLong, -StickerShort, -StickerDepth);
		glVertex3f (-StickerLong, StickerShort, -StickerDepth);
		glEnd();
	glEndList();
	
	return list;
}

void Cubelet::Render()
{
	glPushMatrix();
	glTranslatef (pos[0], pos[1], pos[2]);

	if (!primitives)
	{
		primitives = InitPrimitives();
		if (!primitives)
			return;
	}

	if (!pick)
	{
		glCallList (primitives);
		for(int i = UF; i <= BF; ++i)
		{
			glMaterialfv(GL_FRONT_AND_BACK,
				        GL_DIFFUSE, FaceletMaterial[face[i]]);
			glCallList (primitives + i + 1);
		}
	}
	else
	{
		glColor3b(0, 0, 0);
		glCallList (primitives);
		for(int i = UF; i <= BF; ++i)
		{
			glColor3ub(id, i, 123);
			glCallList (primitives + i + 1);
		}
	}

	glPopMatrix();
}
	
void Cubelet::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}

void Cubelet::SetColor(int f, int c)
{
	face[f] = c;
}

#endif // _CUBELET_CPP
