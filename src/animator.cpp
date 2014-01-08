
#include <iostream>

#include "animator.h"
#include "gui.h"

#include "kocicube.h"
#include "facecube.h"
#include "patterns.h"

#include <cmath>

extern MainDlg *Window;
extern QApplication *Application;

#define abs(a) (((a) < 0) ? -(a) : (a))


Animator::Animator()
{
	Cube = new Cube3D;
	KSolver = 0;
	srand(time(0));
}

Animator::~Animator()
{
	Pipe.Reset();
	delete Cube;

	if(KSolver)
		delete KSolver;
}

void Animator::GLInit()
{
	static GLfloat diffuse0[]  = { 0.9, 0.9, 0.9, 1.0 };
	static GLfloat position0[] = { 0.0, 1.0, 0.0, 0.0 };

	static GLfloat diffuse1[]  = { 0.7, 0.7, 0.7, 1.0 };
	static GLfloat position1[] = { -1.0, -1.0, 2.0, 0.0 };

	static GLfloat diffuse2[]  = { 0.7, 0.7, 0.7, 1.0 };
	static GLfloat position2[] = { 1.0, -1.0, 2.0, 0.0 };
	
	static GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	static GLfloat local_view[] = {0.0};

	glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv (GL_LIGHT0, GL_POSITION, position0);

	glLightfv (GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv (GL_LIGHT1, GL_POSITION, position1);

	glLightfv (GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv (GL_LIGHT2, GL_POSITION, position2);
	
	glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
	glEnable (GL_LIGHT2);
	glEnable (GL_DEPTH_TEST);

	glClearColor (1.0, 1.0, 1.0, 1.0);
	glClearDepth (1.0);

	PipeAdd(DO_IDLE,  20, 20, MainDlg::ALL, false);
	PipeAdd(ROT_TO,   20, 30.0, 30.0, -2.0);
	PipeAdd(DO_IDLE,  20, -1, MainDlg::NONE, false);
}

void Animator::GLResize(int w, int h)
{
	GLfloat aspect;
	glViewport( 0, 0, (GLint)w, (GLint)h );

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	
	if (w > h)
	{
		aspect = (float) w / (float) h;
		glFrustum (-aspect, aspect, -1.0, 1.0, 5.0, 60.0);
	}
	else
	{
		aspect = (float) h / (float) w;
		glFrustum (-1.0, 1.0, -aspect, aspect, 5.0, 60.0);
	}

	glMatrixMode (GL_MODELVIEW);
}

void Animator::PipeAdd(int type, int priority,
		       GLfloat p1, GLfloat p2, GLfloat p3)
{
	PipeData t;
	
	t.type = type;
	t.p1 = p1;
	t.p2 = p2;
	t.p3 = p3;

	Pipe.Add(&t, priority);
}

void Animator::Solve()
{
	FaceletCube faceletCube;
	KociembaCube KCube;

	Cube->FaceMap.MapToString(faceletCube.faceletMarkings);

	if(faceletCube.faceletMarkings[0] == '\0')
	{
		Window->SetInfo("_Раскрасьте куб!_");
		return;
	}

	int status;
	if ((status = faceletCube.Validate(KCube)) != FaceletCube::VALID)
	{
		Window->SetInfo( faceletCube.ErrorText(status) );
		return;
	}

	Window->block = MainDlg::MOVE;
	
	if(KSolver) delete KSolver;
	
	KSolver = new Solver;
	KSolver->InitializeTables();
	KSolver->Solve(KCube);

	while(Pipe.GetLength()) Application->processEvents();
	
	PipeAdd(DO_IDLE,   20, 40, MainDlg::MOVE, false);
	PipeAdd(ADD_MOVES, 15, MoveList.GetLength(), true, false);
	PipeAdd(DO_IDLE,   10, 50, MainDlg::ALL, false);
	PipeAdd(ROT_TO,    10, 360.0*4.0+30.0, 360.0*4.0+30.0, -2.0);

	while(Pipe.GetLength()) Application->processEvents();

	MoveList.Begin(0);
	while(MoveList.Next())
		;

	Cube->FaceMap.StringToMap(Cube->FaceMap.SavedState);
	Cube->SetColors();
	Window->ListNext(1);

	PipeAdd(DO_IDLE,   10, -1, MainDlg::NONE, false);
}

void Animator::StopSolver()
{
	if(KSolver)
		KSolver->stopped = true;
}

void Animator::Shuffle(int count)
{
	char mtable[] = {'F', 'B', 'L', 'R', 'U', 'D'};
	int  ntable[] = {-1, 1, 2};
	char m, prev = '\0';
	
	MoveList.Reset();
	
	for(int i = 0; i < count; ++i)
	{
		do
			m = mtable[ rand()%6 ];
		while(m == prev);

		int move = MOVE(m, ntable[ rand()%3 ]);
		MoveList.Add(move);
		prev = m;
	}

	Window->SetInfo("_Перемешиваю..._");
	PipeAdd(ADD_MOVES, 15, count, false, false);
	PipeAdd(DO_IDLE, 11, -1, SAVE_STATE, false);
}

void Animator::DoPattern(int index)
{
	if(index >= PatternNum)
		return;

	StopSolver();
	MoveList.Reset();
	Cube->FaceMap.Reset();
	Cube->SetColors();
	Window->ListClear();
	Window->ListAdd(0);
	Window->SetInfo("Выбран узор _%s_", PatternNames[index]);
	
	const char *pattern = PatternMoves[index];
	int count = 0;

	while(*pattern)
	{
		int move, n = 0;
		
		switch(pattern[1])
		{
			case ' ':  n =  1; break;
			case '\'': n = -1; break;
			case '2':  n =  2; break;
		}
		
		move = MOVE(pattern[0], n);
		MoveList.Add(move);
		Window->ListAdd(move);
		
		pattern += 2;
		count++;
	}
	
	Window->SetMovesNum(count);
}

void Animator::ProcessPipe()
{
	PipeData *t;

	Pipe.Begin();
	while ((t = Pipe.Get()))
	{
		int code = 0;
		switch (t->type)
		{
			case ROT_TO:    code = RotateTo(t);    break;
			case ROT_SLICE: code = RotateSlice(t); break;
			case DO_MOVE:   code = DoMove(t);      break;
			case ADD_MOVES: code = AddMoves(t);    break;
			case SET_COLOR: code = SetColor(t);    break;
			case DO_IDLE:   code = DoIdle(t);      break;
		}
		
		if (code == -1) break;
		if (code ==  0) Pipe.Delete();
	}
	
	Cube->Render();
	
}

GLfloat Animator::CalculateOffset(GLfloat *from, GLfloat to, GLfloat k)
{
	const GLfloat offset = 0.2;
	bool  pos_offset = *from < to;
	
	if(*from == to) return 0.0;

	*from += (pos_offset) ? k*offset : -k*offset;

	if(( pos_offset && *from > to ) ||
	   (!pos_offset && *from < to))
	{
		*from = to;
		return 0.0;
	}

	return abs(*from - to);
}

inline void ConvertAngle(GLfloat &a)
{
	while(a <= -360.0) a += 360.0;
	while(a >=  360.0) a -= 360.0;
}

int Animator::RotateTo(PipeData* param)
{
	static GLfloat dx, dy;
	GLfloat k = (dx + 1) / (dy + 1);
	GLfloat p = 1 / k;
	GLfloat t = abs(param->p3);

	if(Window->block == MainDlg::BAN) return 0;
	
	if(dx > dy){ p *= p; k = 1; }
	if(dy > dx){ k *= k; p = 1; }
	
	dx = CalculateOffset(&Cube->angles[1], param->p1, t*k);
	dy = CalculateOffset(&Cube->angles[2], param->p2, t*p);

	if(dx == 0.0 && dy == 0.0)
	{
		ConvertAngle(Cube->angles[1]);
		ConvertAngle(Cube->angles[2]);
		return 0;
	}
	
	if(t != 1.0)
		t += 0.5;

	param->p3 = (param->p3 < 0) ? -t : t;
	return (param->p3 < 0) ? -1 : 1;
}

int Animator::RotateSlice(PipeData* param)
{
	if(Window->block == MainDlg::BAN) return 0;
	
	GLfloat da = CalculateOffset(&Cube->angles[0], param->p1, param->p2+=0.5);
	return (da == 0.0) ? 0 : -1;
}

int Animator::DoMove(PipeData* param)
{
	if(param->p1)
	{
		Cube->AnimateStick(param->p2, param->p3);

		if(param->p1 > 0)
			Window->ListNext(param->p1);
	}
	else
	{
		Window->ProgressStep(1);
		Cube->AnimateStop(param->p2, param->p3);
		Cube->angles[0] = 0;
	}
	return 0;
}

inline int Axis(char m)
{
	switch (m)
	{
		case 'U': case 'E': case 'D': return 0;
		case 'F': case 'W': case 'B': return 1;
		case 'L': case 'M': case 'R': return 2;
	}
	return -1;
}

const int Animator::Coeff[] = {
	 8, 12, 13, 12, 12,
	13, 14, 16, 19, 20,
	20, 19,	19, 18, 15,
	12, 15, 12, 11, 10,
	11, 10, 13, 10, 14
};

inline GLfloat Animator::GetC(int t, int s)
{
	if(s < 0) return 50;
	if(t > 24) t = 24;
	return Coeff[t-1];
}

int Animator::AddMoves(PipeData* param)
{
	if(!param->p3)
	{
		MoveList.Begin(param->p1);
		PipeAdd(DO_IDLE, 14, -1, MainDlg::MOVE, false);
		PipeAdd(DO_IDLE, 10, -1, MainDlg::NONE, false);

		int count = MoveList.Count();
		Window->ProgressInit(count);

		if(!param->p2) param->p2 = -1;
		if( count < 4) param->p2 =  0;
		
		param->p3++;
	}

	if(Pipe.GetLength() > 30)
		return 1;

	int  m1 = MoveList.Next();
	char f1 = FACE(m1);
	int  n1 = NUM(m1);

	if(!m1)
		return 0;

	int  ii = (param->p2 >= 0) ? MoveList.Item() + 1 : -5;
	int  m2 = MoveList.Next();
	char f2 = FACE(m2);
	int  n2 = NUM(m2);

	if(n1 == -2) n1 = 2;
	if(n2 == -2) n2 = 2;
	
	int  d1 = 90 * abs(n1);
	int  a1 = n1;
	
	int one_move = 1;
	
	if(param->p2 && Axis(f1) == Axis(f2) && f1 != f2)
	{
		int t  = min(abs(n1), abs(n2));
		int s1 = (n1 <  0) ? -1 : 1;
		int s2 = (n2 <  0) ? -1 : 1;
		int a2 = s2*t;
		    a1 = s1*t;
		    d1 = 90 * t;
		
		if(t == 1 && !(rand()%4))
		{
			a1 = -a1;
			a2 = -a2;
			d1 = 270;
		}

		PipeAdd(DO_MOVE,   12, ii++, f1, a1);
		PipeAdd(DO_MOVE,   12, ii,   f2, a2);
		PipeAdd(ROT_SLICE, 12, d1,
			GetC(param->p3, param->p2), false);
		PipeAdd(DO_MOVE,   12, false, f2, s2*t);
		PipeAdd(DO_MOVE,   12, false, f1, s1*t);

		one_move = -1;
		int t1 = abs(n1);
		int t2 = abs(n2);
		if(t1 > t2)
		{
			n1  = a1 = 1;
			d1  = 90;
		}
		else if(t1 < t2)
		{
			f1  = f2;
			n1  = a1 = 1;
			d1  = 90;
		}
		else
			one_move = 0;
	}
	else if(m2)
		MoveList.MoveBack();
	
	if(one_move)
	{
		if(param->p2 && abs(n1) == 1 && !(rand()%4))
		{
			a1 = -n1;
			d1 = 270;
		}
		if(one_move == -1)
			Window->ProgressIncTotal();

		PipeAdd(Animator::DO_MOVE,   12, ii,  f1, a1);
		PipeAdd(Animator::ROT_SLICE, 12, d1,
			GetC(param->p3, param->p2), false);
		PipeAdd(Animator::DO_MOVE,   12, false, f1, n1);
	}

	param->p3++;
	return 1;
}

int Animator::SetColor(PipeData* param)
{
	GLint viewport[4];
	GLubyte fPixel[3];
	
	Cube->Picking(true);
	Cube->Render();
	
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels( param->p1, viewport[3] - param->p2, 1, 1,
		      GL_RGB, GL_UNSIGNED_BYTE, fPixel );

	if(fPixel[2] == 123)
	{
		int a[3];
		Cube->Pick(fPixel[0], fPixel[1], a);

		if(param->p3 < 0)
		{
			int dir = param->p3;
			param->p3 = Cube->FaceMap.Color(a[0], a[1], a[2]);
			
			if(dir == -1) param->p3++;
			else          param->p3--;

			if(param->p3 > 6) param->p3 = 1;
			if(param->p3 < 1) param->p3 = 6;
		}
		
		Cube->FaceMap.Color(a[0], a[1], a[2]) = param->p3;
		Cube->SetColors();
		Window->SetInfo( Cube->FaceMap.Validate() );
	}

	Cube->Picking(false);
	Cube->FaceMap.SaveState();
	return 0;
}

int Animator::DoIdle(PipeData* param)
{
	if(param->p1-- > 0)
		return -1;
	
	if(param->p2 == SAVE_STATE)
	{
		if(Pipe.GetLength() > 1)
			return 1;
		
		Cube->FaceMap.SaveState();
		return 0;
	}
					
	if(Window->block == MainDlg::BAN)
		return 0;
	
	Window->block = param->p2;
	return 0;
}
