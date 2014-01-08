
#ifndef	_MOVETABL_H
#define	_MOVETABL_H

#include "cube.h"
#include <fstream>

using namespace std;

class MoveTable
{
	public:
		MoveTable(Cube& cube, int tableSize, int phase2=0);

		virtual ~MoveTable();

		virtual int* operator[](int index);

		virtual void Initialize(const char* fileName);
		virtual int SizeOf(void) { return TableSize; }

	protected:
		virtual int OrdinalFromCubeState(void) = 0;
		virtual void OrdinalToCubeState(int ordinal) = 0;

	private:
		void Generate(void);
		void Save(const char* fileName);
		void Load(ifstream& infile);

		Cube& TheCube;
		int TableSize;
		int Phase2;
		int (*Table)[Cube::NumberOfClockwiseQuarterTurnMoves];
};

#endif	// _MOVETABL_H
