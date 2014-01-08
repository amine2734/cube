
#include "movetabl.h"
#include <fstream>

using namespace std;


MoveTable::MoveTable(Cube& cube, int tableSize, int phase2)
	:TheCube(cube),
	 TableSize(tableSize),
	 Phase2(phase2)
{
	Table = (int (*)[Cube::NumberOfClockwiseQuarterTurnMoves])
	        (new int[TableSize*Cube::NumberOfClockwiseQuarterTurnMoves]);
}

MoveTable::~MoveTable()
{
	delete [] Table;
}

void MoveTable::Initialize(const char* fileName)
{
	ifstream infile(fileName, ios::in|ios::binary);
	if (!infile)
	{
		Generate();
		Save(fileName);
	}
	else
		Load(infile);
}

void MoveTable::Generate(void)
{
	int ordinal;
	int move, move2;

	TheCube.BackToHome();

	for (ordinal = 0; ordinal < TableSize; ordinal++)
	{
		OrdinalToCubeState(ordinal);

		for (move = Cube::R; move <= Cube::B; move++)
		{
			move2 = move;
			if (Phase2 && move != Cube::U && move != Cube::D)
				move2 = Cube::QuarterTurnToHalfTurnMove(move);

			TheCube.ApplyMove(move2);
			Table[ordinal][move] = OrdinalFromCubeState();
			TheCube.ApplyMove(Cube::InverseOfMove(move2));
		}
	}
}

int* MoveTable::operator[](int index)
{
	return Table[index];
}

void MoveTable::Save(const char* fileName)
{
	ofstream outfile(fileName, ios::out|ios::binary);
	int index;
	for (index = 0; index < TableSize; index++)
		outfile.write((char*)&Table[index],
			Cube::NumberOfClockwiseQuarterTurnMoves*sizeof(int));
}

void MoveTable::Load(ifstream& infile)
{
	int index;
	for (index = 0; index < TableSize; index++)
		infile.read((char*)&Table[index],
			Cube::NumberOfClockwiseQuarterTurnMoves*sizeof(int));
}
