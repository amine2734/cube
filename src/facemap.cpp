
#ifndef _FACEMAP_CPP
#define _FACEMAP_CPP

#include "facemap.h"
#include "cubelet.h"
#include "facecube.h"

#include <iostream>


int& FaceletMap::Color(int x, int y, int z)
{
	return Map[x][y][z];
}

void FaceletMap::Reset()
{
	for (int i = 0; i <= 4; i++)
		for (int j = 0; j <= 4; j++)
			for (int k = 0; k <= 4; k++)
				if (i != 2 || j != 2 || k != 2)
					Map[i][j][k] = 0;

	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
		{
			Map[i][4][j] = Cubelet::Y;
			Map[i][j][0] = Cubelet::R;
			Map[0][i][j] = Cubelet::G;
			Map[i][j][4] = Cubelet::O;
			Map[4][i][j] = Cubelet::B;
			Map[i][0][j] = Cubelet::W;
		}
}

void FaceletMap::ResetToGray()
{
	for (int i = 0; i <= 4; i++)
		for (int j = 0; j <= 4; j++)
			for (int k = 0; k <= 4; k++)
				if (i != 2 || j != 2 || k != 2)
					Map[i][j][k] = 0;

	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
	{
		Map[i][4][j] = Cubelet::S;
		Map[i][j][0] = Cubelet::S;
		Map[0][i][j] = Cubelet::S;
		Map[i][j][4] = Cubelet::S;
		Map[4][i][j] = Cubelet::S;
		Map[i][0][j] = Cubelet::S;
	}
}

bool FaceletMap::IsSolved()
{
	int c[7];
	
	c[1] = Map[2][4][2];
	c[2] = Map[2][2][0];
	c[3] = Map[0][2][2];
	c[4] = Map[2][2][4];
	c[5] = Map[4][2][2];
	c[6] = Map[2][0][2];
	
	for (int i = 1; i <= 6; ++i)
	{
		int d = 0;
		for (int j = 1; j <= 6; ++j)
			if (c[i] == j)
				d++;

		if (d != 1)
			return false;
	}
	
	for (int i = 1; i < 4; ++i)
		for (int j = 1; j < 4; ++j)
			if(Map[i][4][j] != c[1] ||
			   Map[i][j][0] != c[2] ||
			   Map[0][i][j] != c[3] ||
			   Map[i][j][4] != c[4] ||
			   Map[4][i][j] != c[5] ||
			   Map[i][0][j] != c[6])
				return false;
	
	return true;
}

void FaceletMap::Rotate(char m, int rot)
{
	switch (m)
	{
		case 'U': (rot > 0) ? UL() : UR(); break;
		case 'D': (rot > 0) ? DR() : DL(); break;
		case 'F': (rot > 0) ? FC() : FA(); break;
		case 'B': (rot > 0) ? BA() : BC(); break;
		case 'L': (rot > 0) ? LD() : LU(); break;
		case 'R': (rot > 0) ? RU() : RD(); break;
	}
}

void FaceletMap::MapToString(char *str)
{
	int i = -1;
	for(int z = 0; z <= 2; ++z)
		for(int x = 0; x <= 2; ++x)
	{
		str[++i] = Map[x+1][4][3-z];
		str[9+i] = Map[x+1][0][z+1];
	}
		
	i += 9;
	for(int y = 0; y <= 2; ++y)
		for(int z = 0; z <= 2; ++z)
	{
		str[++i] = Map[0][3-y][3-z];
		str[9+i] = Map[4][3-y][z+1];
	}

	i += 9;
	for(int y = 0; y <= 2; ++y)
		for(int x = 0; x <= 2; ++x)
	{
		str[++i] = Map[x+1][3-y][0];
		str[9+i] = Map[3-x][3-y][4];
	}

	for(int j = 0; j < i+10; ++j)
		switch(str[j])
		{
			case 1:  str[j] = 'R';  break;
			case 2:  str[j] = 'G';  break;
			case 3:  str[j] = 'B';  break;
			case 4:  str[j] = 'Y';  break;
			case 5:  str[j] = 'O';  break;
			case 6:  str[j] = 'W';  break;
			default: str[0] = '\0'; break;
		}

	str[54] = '\0';
}

void FaceletMap::SaveState()
{
	MapToString(SavedState);
}

void FaceletMap::StringToMap(const char *str)
{
	int i = -1;
	int temp[54];

	for(int j = 0; j < 54; ++j)
		switch(str[j])
		{
			case 'R':  temp[j] = 1;  break;
			case 'G':  temp[j] = 2;  break;
			case 'B':  temp[j] = 3;  break;
			case 'Y':  temp[j] = 4;  break;
			case 'O':  temp[j] = 5;  break;
			case 'W':  temp[j] = 6;  break;
			default:		 return;
		}
		
	for(int z = 0; z <= 2; ++z)
		for(int x = 0; x <= 2; ++x)
		{
			Map[x+1][4][3-z] = temp[++i];
			Map[x+1][0][z+1] = temp[9+i];
		}
		
	i += 9;
	for(int y = 0; y <= 2; ++y)
		for(int z = 0; z <= 2; ++z)
		{
			Map[0][3-y][3-z] = temp[++i];
			Map[4][3-y][z+1] = temp[9+i];
		}

	i += 9;
	for(int y = 0; y <= 2; ++y)
		for(int x = 0; x <= 2; ++x)
		{
			Map[x+1][3-y][0] = temp[++i];
			Map[3-x][3-y][4] = temp[9+i];
		}
}

const char* FaceletMap::Validate()
{
	int status;
	FaceletCube faceletCube;
	Cube testCube;

	MapToString(faceletCube.faceletMarkings);

	if(faceletCube.faceletMarkings[0] == '\0')
		return 0;
	
	if ((status = faceletCube.Validate(testCube)) != FaceletCube::VALID)
		return faceletCube.ErrorText(status);

	return 0;
}

void FaceletMap::FillTemp()
{
	for (int i = 0; i <= 4; i++)
	  for (int j = 0; j <= 4; j++)
	    for (int k = 0; k <= 4; k++)
		Temp[i][j][k] = Map[i][j][k];
}

bool FaceletMap::XML(int a)
{
	if (a < 1 || a > 3) return false;
	FillTemp();
	
	for (int i = 1; i <= 3; i++)
	{
		if (a == 1)
			for (int j = 1; j <= 3; j++)
				Map[i][0][j] = Temp[4-j][0][i];
		
		else if (a == 3)
			for (int j = 1; j <= 3; j++)
				Map[i][4][j] = Temp[4-j][4][i];
		
		Map[i][a][0] = Temp[4][a][i];
		Map[i][a][4] = Temp[0][a][i];
		Map[0][a][i] = Temp[4-i][a][0];
		Map[4][a][i] = Temp[4-i][a][4];
	}
	
	if (a == 1) {
		Map[2][1][2] = Temp[2][1][2];
		Map[2][1][2]--;
		if (Map[2][1][2] < 0) Map[2][1][2] += 4;
	}
	else if (a == 3)
	{
		Map[2][3][2] = Temp[2][3][2];
		Map[2][3][2]++;
		if (Map[2][3][2] > 3) Map[2][3][2] -= 4;
	}
	else
	{
		Map[2][a][1] = Temp[3][a][2];
		Map[2][a][3] = Temp[1][a][2];
		Map[1][a][2] = Temp[2][a][1];
		Map[3][a][2] = Temp[2][a][3];
	}
	return true;
}

bool FaceletMap::XMR(int a)
{
	if (a < 1 || a > 3) return false;
	FillTemp();
	
	for (int i = 1; i <= 3; i++)
	{
		if (a == 1)
			for (int j = 1; j <= 3; j++)
				Map[i][0][j] = Temp[j][0][4-i];
		
		else if (a == 3)
			for (int j = 1; j <= 3; j++)
				Map[i][4][j] = Temp[j][4][4-i];
		
		Map[i][a][0] = Temp[0][a][4-i];
		Map[i][a][4] = Temp[4][a][4-i];
		Map[0][a][i] = Temp[i][a][4];
		Map[4][a][i] = Temp[i][a][0];
	}
	
	if (a == 1)
	{
		Map[2][1][2] = Temp[2][1][2];
		Map[2][1][2]++;
		if (Map[2][1][2] > 3) Map[2][1][2] -= 4;
	}
	else if (a == 3)
	{

		Map[2][3][2] = Temp[2][3][2];
		Map[2][3][2]--;
		if (Map[2][3][2] < 0) Map[2][3][2] += 4;

	}
	else
	{
		Map[2][a][1] = Temp[1][a][2];
		Map[2][a][3] = Temp[3][a][2];
		Map[1][a][2] = Temp[2][a][3];
		Map[3][a][2] = Temp[2][a][1];
	}
	return true;
}
 
bool FaceletMap::XMU(int a)
{
	if (a < 1 || a > 3) return false;
	FillTemp();
	
	for (int i = 1; i <= 3; i++)
	{
		if (a == 1)
			for (int j = 1; j <= 3; j++)
				Map[0][i][j] = Temp[0][j][4-i];
		
		if (a == 3)
			for (int j = 1; j <= 3; j++)
				Map[4][i][j] = Temp[4][j][4-i];
		
		Map[a][i][0] = Temp[a][0][4-i];
		Map[a][i][4] = Temp[a][4][4-i];
		Map[a][0][i] = Temp[a][i][4];
		Map[a][4][i] = Temp[a][i][0];
	}
	
	if (a == 1)
	{
		Map[1][2][2] = Temp[1][2][2];
		Map[1][2][2]--;
		if (Map[1][2][2] < 0) Map[1][2][2] += 4;
	}
	else if (a == 3)
	{

		Map[3][2][2] = Temp[3][2][2];
		Map[3][2][2]++;
		if (Map[3][2][2] > 3) Map[3][2][2] -= 4;
	}
	else
	{
		Map[a][2][1] = Temp[a][1][2];
		Map[a][2][3] = Temp[a][3][2];
		Map[a][1][2] = Temp[a][2][3];
		Map[a][3][2] = Temp[a][2][1];
	}
	return true;
}
 
bool FaceletMap::XMD(int a)
{
	if (a < 1 || a > 3) return false;
	FillTemp();
	
	for (int i = 1; i <= 3; i++)
	{
		if (a == 1)
			for (int j = 1; j <= 3; j++)
				Map[0][i][j] = Temp[0][4-j][i];
		
		if (a == 3)
			for (int j = 1; j <= 3; j++)
				Map[4][i][j] = Temp[4][4-j][i];
		
		Map[a][i][0] = Temp[a][4][i];
		Map[a][i][4] = Temp[a][0][i];
		Map[a][0][i] = Temp[a][4-i][0];
		Map[a][4][i] = Temp[a][4-i][4];
	}
	
	if (a == 1)
	{

		Map[1][2][2] = Temp[1][2][2];
		Map[1][2][2]++;
		if (Map[1][2][2] > 3) Map[1][2][2] -= 4;
	}
	else if (a == 3)
	{
		Map[3][2][2] = Temp[3][2][2];
		Map[3][2][2]--;
		if (Map[3][2][2] < 0) Map[3][2][2] += 4;

	}
	else
	{
		Map[a][2][1] = Temp[a][3][2];
		Map[a][2][3] = Temp[a][1][2];
		Map[a][1][2] = Temp[a][2][1];
		Map[a][3][2] = Temp[a][2][3];
	}
	return true;
}
 
bool FaceletMap::XMC(int a)
{
	if (a < 1 || a > 3) return false;
	FillTemp();
	
	for (int i = 1; i <= 3; i++)
	{
		if (a == 1)
			for (int j = 1; j <= 3; j++)
				Map[i][j][0] = Temp[4-j][i][0];
		
		if (a == 3)
			for (int j = 1; j <= 3; j++)
				Map[i][j][4] = Temp[4-j][i][4];
		
		Map[i][0][a] = Temp[4][i][a];
		Map[i][4][a] = Temp[0][i][a];
		Map[0][i][a] = Temp[4-i][0][a];
		Map[4][i][a] = Temp[4-i][4][a];
	}
	
	if (a == 1)
	{
		Map[2][2][1] = Temp[2][2][1];
		Map[2][2][1]++;
		if (Map[2][2][1] > 3) Map[2][2][1] -= 4;
	}
	else if (a == 3)
	{
		Map[2][2][3] = Temp[2][2][3];
		Map[2][2][3]--;
		if (Map[2][2][3] < 0) Map[2][2][3] += 4;
	}
	else
	{
		Map[2][1][a] = Temp[3][2][a];
		Map[2][3][a] = Temp[1][2][a];
		Map[1][2][a] = Temp[2][1][a];
		Map[3][2][a] = Temp[2][3][a];
	}
	return true;
}
 
bool FaceletMap::XMA(int a)
{
	if (a < 1 || a > 3) return false;
	FillTemp();
	
	for (int i = 1; i <= 3; i++)
	{
		if (a == 1)
			for (int j = 1; j <= 3; j++)
				Map[i][j][0] = Temp[j][4-i][0];
		
		if (a == 3)
			for (int j = 1; j <= 3; j++)
				Map[i][j][4] = Temp[j][4-i][4];
		
		Map[i][0][a] = Temp[0][4-i][a];
		Map[i][4][a] = Temp[4][4-i][a];
		Map[0][i][a] = Temp[i][4][a];
		Map[4][i][a] = Temp[i][0][a];
	}
	if (a == 1)
	{
		Map[2][2][1] = Temp[2][2][1];
		Map[2][2][1]--;
		if (Map[2][2][1] < 0) Map[2][2][1] += 4;
	}
	else if (a == 3)
	{
		Map[2][2][3] = Temp[2][2][3];
		Map[2][2][3]++;
		if (Map[2][2][3] > 3) Map[2][2][3] -= 4;
	}
	else
	{
		Map[2][1][a] = Temp[1][2][a];
		Map[2][3][a] = Temp[3][2][a];
		Map[1][2][a] = Temp[2][3][a];
		Map[3][2][a] = Temp[2][1][a];
	}
	return true;
}
 
void FaceletMap::UL() { XML(3); } // rotate top left
void FaceletMap::UR() { XMR(3); } // rotate top right
void FaceletMap::DL() { XML(1); } // rotate bottom left
void FaceletMap::DR() { XMR(1); } // rotate bottom right
void FaceletMap::LU() { XMU(1); } // rotate left up
void FaceletMap::LD() { XMD(1); } // rotate left down
void FaceletMap::RU() { XMU(3); } // rotate right up
void FaceletMap::RD() { XMD(3); } // rotate right down
void FaceletMap::FC() { XMC(1); } // rotate front clockwise
void FaceletMap::FA() { XMA(1); } // rotate front anticlockwise
void FaceletMap::BC() { XMC(3); } // rotate back clockwise (looking from front)
void FaceletMap::BA() { XMA(3); } // rotate back anticlockwise (looking from front)

#endif // _FACEMAP_CPP
