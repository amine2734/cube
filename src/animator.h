 
#ifndef _ANIMATOR_H
#define _ANIMATOR_H

#include <cstdlib>
#include <ctime>

#include "cube3d.h"
#include "queue.cpp"
#include "solver.h"

class Animator
{
	public:
		enum   Type  {ROT_TO,
			      ROT_SLICE, DO_IDLE,
			      DO_MOVE,   ADD_MOVES,
			      SET_COLOR, SAVE_STATE};
		
		struct PipeData
		{
			int type;
			GLfloat p1, p2, p3;
		};

		Animator();
		~Animator();
		
		void GLInit();
		void GLResize(int w, int h);
		
		void PipeAdd(int type, int priority,
			     GLfloat p1, GLfloat p2, GLfloat p3);
		void ProcessPipe();
		void Solve();
		void StopSolver();
		void DoPattern(int index);
		void Shuffle(int count);

		Cube3D *Cube;
		Queue<int, 50> MoveList;
		Queue<PipeData, 40> Pipe;

		static const char *PatternMoves[];
		static const char *PatternNames[];
		static const int   PatternNum;

	protected:

		GLfloat GetC(int t, int s);
		GLfloat CalculateOffset(GLfloat *from, GLfloat to, GLfloat k);
		int RotateTo(PipeData* param);
		int RotateSlice(PipeData* param);
		int ScaleCube(PipeData* param);
		int DoMove(PipeData* param);
		int AddMoves(PipeData* param);
		int SetColor(PipeData* param);
		int DoIdle(PipeData* param);

	private:
		Solver *KSolver;
		
		static const int Coeff[26];
};


#endif // _ANIMATOR_H
