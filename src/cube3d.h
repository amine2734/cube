 
#ifndef _CUBE3D_H
#define _CUBE3D_H

#include "cubelet.h"
#include "facemap.h"

#define GetCubelet(x, y, z) Map[(z)*9 + (y)*3 + (x)]

class Cube3D
{
	public:
		enum Axis {X, Y, Z};
		
		Cube3D();
		~Cube3D();
		
		void Render();
		void Picking(bool state);
		void Pick(int n, int f, int* xyz);
		void SetColors();
		void AnimateMove(char m, int rot);
		void AnimateStick(char m, int rot);
		void AnimateStop(char m, int rot);

		GLfloat    angles[3];
		FaceletMap FaceMap;

	protected:
		bool IsAnimated(Cubelet* c);
		
	private:
		Cubelet*   Map[27];

		int        TransCount;
		float      TransRot;
		Axis       TransAxis;
		Cubelet*   Transform[18];
		
};

#endif // _CUBE3D_H
