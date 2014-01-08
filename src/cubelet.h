 
#ifndef _CUBELET_H
#define _CUBELET_H

#include <GL/gl.h>

class Cubelet
{
	public:
		enum {N, R, G, B, Y, O, W, S};
		enum {UF, DF, LF, RF, FF, BF};
		
		Cubelet(int cube_id);
		
		void Render();
		void SetPosition(GLfloat x, GLfloat y, GLfloat z);
		void SetColor(int f, int c);

		bool pick;
		int  face[6];

	protected:
		GLuint InitPrimitives();

	private:
		GLfloat pos[3];
		GLfloat id;
		static GLuint primitives;
		static const GLfloat FaceletMaterial[8][4];
		static const GLfloat Diffuse[4];
		static const GLfloat Specular[4];
		static const GLfloat CubeLen;
		static const GLfloat CubeRound;
		static const GLfloat StickerLong;
		static const GLfloat StickerShort;
		static const GLfloat StickerDepth;
};

#endif // _CUBELET_H
