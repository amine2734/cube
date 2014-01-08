 
#include "cube3d.h"
#include <iostream>

#define abs(a) (((a) < 0) ? -(a) : (a))


Cube3D::Cube3D()
{
	
	for(int i = 0; i < 27; ++i)
		Map[i] = new Cubelet(i);
	
	const GLfloat a = 1.0;
	const GLfloat b = -1.0;
	
	Map[ 0]->SetPosition(b, a, a);
	Map[ 1]->SetPosition(0.0, a, a);
	Map[ 2]->SetPosition(a, a, a);
	Map[ 3]->SetPosition(b, 0.0, a);
	Map[ 4]->SetPosition(0.0, 0.0, a);
	Map[ 5]->SetPosition(a, 0.0, a);
	Map[ 6]->SetPosition(b, b, a);
	Map[ 7]->SetPosition(0.0, b, a);
	Map[ 8]->SetPosition(a, b, a);

	Map[ 9]->SetPosition(b, a, 0.0);
	Map[10]->SetPosition(0.0, a, 0.0);
	Map[11]->SetPosition(a, a, 0.0);
	Map[12]->SetPosition(b, 0.0, 0.0);
	Map[13]->SetPosition(0.0, 0.0, 0.0);
	Map[14]->SetPosition(a, 0.0, 0.0);
	Map[15]->SetPosition(b, b, 0.0);
	Map[16]->SetPosition(0.0, b, 0.0);
	Map[17]->SetPosition(a, b, 0.0);

	Map[18]->SetPosition(b, a, b);
	Map[19]->SetPosition(0.0, a, b);
	Map[20]->SetPosition(a, a, b);
	Map[21]->SetPosition(b, 0.0, b);
	Map[22]->SetPosition(0.0, 0.0, b);
	Map[23]->SetPosition(a, 0.0, b);
	Map[24]->SetPosition(b, b, b);
	Map[25]->SetPosition(0.0, b, b);
	Map[26]->SetPosition(a, b, b);

	FaceMap.Reset();
	FaceMap.SaveState();
	SetColors();

	TransCount = 0;

	angles[0] =    0.0;
	angles[1] =   10.0;
	angles[2] = -180.0;

}

Cube3D::~Cube3D()
{
	for(int i = 0; i < 27; ++i)
		delete Map[i];
}

void Cube3D::SetColors()
{
	for(int y = 0; y <= 2; ++y)
	  for(int x = 0; x <= 2; ++x)
	  {
		GetCubelet(x, 2-y, 0)->face[Cubelet::FF] = FaceMap.Map[x+1][y+1][0];
		GetCubelet(x, 2-y, 2)->face[Cubelet::BF] = FaceMap.Map[x+1][y+1][4];
	  }

	for(int z = 0; z <= 2; ++z)
	  for(int x = 0; x <= 2; ++x)
	  {
		GetCubelet(x, 0, z)->face[Cubelet::UF] = FaceMap.Map[x+1][4][z+1];
		GetCubelet(x, 2, z)->face[Cubelet::DF] = FaceMap.Map[x+1][0][z+1];
	  }

	for(int z = 0; z <= 2; ++z)
	  for(int y = 0; y <= 2; ++y)
	  {
		GetCubelet(0, 2-y, z)->face[Cubelet::LF] = FaceMap.Map[0][y+1][z+1];
		GetCubelet(2, 2-y, z)->face[Cubelet::RF] = FaceMap.Map[4][y+1][z+1];
	  }
}

void Cube3D::Picking(bool state)
{
	if(state)
	{
		glDisable(GL_LIGHTING);
		for(int i = 0; i < 27; ++i)
			Map[i]->pick = true;
	}
	else
	{
		glEnable(GL_LIGHTING);
		for(int i = 0; i < 27; ++i)
			Map[i]->pick = false;
	}
}

void Cube3D::Pick(int n, int f, int* xyz)
{
	int x, y, z;

	z  = n / 9 + 1;
	n %= 9;
	y  = n / 3 + 1;
	x  = n % 3 + 1;

	switch(f)
	{
		case Cubelet::UF: y = 4; break;
		case Cubelet::DF: y = 0; break;
		case Cubelet::FF: y = 4 - y; z = 0; break;
		case Cubelet::BF: y = 4 - y; z = 4; break;
		case Cubelet::RF: y = 4 - y; x = 4; break;
		case Cubelet::LF: y = 4 - y; x = 0; break;
	}
	
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

void Cube3D::Render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity ();
	glTranslatef(0.0, 0.0, -20.0);

	glRotatef(angles[1], 1.0, 0.0, 0.0);
	glRotatef(angles[2], 0.0, 1.0, 0.0);

	for (int z = 0; z <= 2; ++z)
	  for (int y = 0; y <= 2; ++y)
	    for (int x = 0; x <= 2; ++x)
	    {
		Cubelet* c = GetCubelet(x, y, z);
		if(!IsAnimated(c)) c->Render();
	    }

	if(TransCount)
	{
		GLfloat angle[3] = {(TransAxis == X) ? 1.0 : 0.0,
				    (TransAxis == Y) ? 1.0 : 0.0,
				    (TransAxis == Z) ? 1.0 : 0.0};

		GLfloat step[2] = {(    TransRot  < 0) ?  1.0 : -1.0,
				   (abs(TransRot) < 1) ? -1.0 :  1.0};

		step[1] = -step[0] + step[0]*step[1];
		
		glPushMatrix();
		
		glRotatef(angles[0]*step[0], angle[0], angle[1], angle[2]);
		for(int i = 0; i < 9; ++i)
			Transform[i]->Render();
		
		if(TransCount == 2)
		{
			glRotatef(angles[0]*step[1], angle[0], angle[1], angle[2]);
			for(int i = 9; i < 18; ++i)
				Transform[i]->Render();
		}

		glPopMatrix();
	}
	
}

void Cube3D::AnimateMove(char m, int rot)
{
	int x1, y1, z1;
	int x2, y2, z2;

	x1 = y1 = z1 = 0;
	x2 = y2 = z2 = 2;
	
	TransRot = rot;
	TransCount = 1;
	
	switch (m)
	{
		case 'U': y1 = y2 = 0; TransAxis = Y; rot =  1; break;
		case 'D': y1 = y2 = 2; TransAxis = Y; rot = -1; break;
		case 'F': z1 = z2 = 0; TransAxis = Z; rot =  1; break;
		case 'B': z1 = z2 = 2; TransAxis = Z; rot = -1; break;
		case 'L': x1 = x2 = 0; TransAxis = X; rot = -1; break;
		case 'R': x1 = x2 = 2; TransAxis = X; rot =  1; break;
	}

	TransRot *= rot;

	int k = 0;
	for (int z = z1; z <= z2; ++z)
	  for (int y = y1; y <= y2; ++y)
	    for (int x = x1; x <= x2; ++x)
		Transform[k++] = GetCubelet(x, y, z);
}

void Cube3D::AnimateStick(char m, int rot)
{
	int   c_axis;
	float c_rot;
	
	if(!TransCount)
		return AnimateMove(m, rot);
	
	for(int i = 0; i < 9; ++i)
		Transform[i+9] = Transform[i];

	c_axis = TransAxis;
	c_rot  = TransRot;

	AnimateMove(m, rot);

	if(c_rot * TransRot < 0)
		TransRot += c_rot/2;

	TransCount = 2;
}

void Cube3D::AnimateStop(char m, int rot)
{
	TransCount = 0;

	while(rot)
	{
		FaceMap.Rotate(m, rot);
		rot += (rot < 0) ? 1 : -1;
	}
	
	SetColors();

}

bool Cube3D::IsAnimated(Cubelet* c)
{
	for (int i = 0; i < TransCount*9; ++i)
		if (Transform[i] == c)
			return true;
	return false;
}
