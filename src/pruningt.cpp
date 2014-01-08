
#include "pruningt.h"
#include "movetabl.h"
#include <fstream>

using namespace std;


PruningTable::PruningTable(MoveTable& moveTable1, MoveTable& moveTable2,
	int homeOrdinal1, int homeOrdinal2)
	: MoveTable1(moveTable1),
	  MoveTable2(moveTable2),
	  HomeOrdinal1(homeOrdinal1),
	  HomeOrdinal2(homeOrdinal2)
{
	MoveTable1Size = MoveTable1.SizeOf();
	MoveTable2Size = MoveTable2.SizeOf();
	TableSize = MoveTable1Size*MoveTable2Size;

	AllocationSize = ((TableSize+7)/8)*4;
	Table = new unsigned char[AllocationSize];
}

void PruningTable::Initialize(const char* fileName)
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

PruningTable::~PruningTable()
{
	delete [] Table;
}


void PruningTable::Generate(void)
{
	unsigned int depth = 0;
	int numberOfNodes;
	int ordinal1, ordinal2;
	int index, index2;
	int move;
	int power;

	for (index = 0; index < TableSize; index++)
		SetValue(index, Empty);

	SetValue(MoveTableIndicesToPruningTableIndex(HomeOrdinal1, HomeOrdinal2),
		depth);
	numberOfNodes = 1;

	while (numberOfNodes < TableSize)
	{
	  for (index = 0; index < TableSize; index++)
	  {
	    if (GetValue(index) == depth)
	    {
	      for (move = Cube::R; move <= Cube::B; move++)
	      {
		PruningTableIndexToMoveTableIndices(index, ordinal1, ordinal2);
		for (power = 1; power < 4; power++)
		{
			ordinal1 = MoveTable1[ordinal1][move];
			ordinal2 = MoveTable2[ordinal2][move];
			index2 = MoveTableIndicesToPruningTableIndex(
						ordinal1, ordinal2);

			if (GetValue(index2) == Empty)
			{
				SetValue(index2, depth+1);
				numberOfNodes++;
			}
		}
	      }
	    }
	  }
	  depth++;
	}
}

void PruningTable::PruningTableIndexToMoveTableIndices(
	int index, int& ordinal1, int& ordinal2)
{
	ordinal1 = index/MoveTable2Size;
	ordinal2 = index%MoveTable2Size;
	return;
}

int PruningTable::MoveTableIndicesToPruningTableIndex(
	int ordinal1, int ordinal2)
{
	return ordinal1*MoveTable2Size+ordinal2;
}

unsigned int PruningTable::OffsetToEntryMask[2] = { Empty<<0,  Empty<<4 };
unsigned int PruningTable::OffsetToShiftCount[2] = { 0, 4 };
	
unsigned int PruningTable::GetValue(int index)
{
	int offset = index%2;
	return (Table[index/2]&OffsetToEntryMask[offset])>>
			OffsetToShiftCount[offset];
}

void PruningTable::SetValue(int index, unsigned int value)
{
	int i = index/2;
	int offset = index%2;
	Table[i] = (Table[i] & ~OffsetToEntryMask[offset]) |
		(value<<OffsetToShiftCount[offset]);
}

void PruningTable::Save(const char* fileName)
{
	ofstream outfile(fileName, ios::out|ios::binary);
	outfile.write((char*)Table, AllocationSize);
}

void PruningTable::Load(ifstream& infile)
{
	infile.read((char*)Table, AllocationSize);
}

