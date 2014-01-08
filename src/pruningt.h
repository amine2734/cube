
#ifndef	_PRUNINGT_H
#define	_PRUNINGT_H

#include "movetabl.h"

class PruningTable
{
	public:
		PruningTable(MoveTable& moveTable1, MoveTable& moveTable2,
			int homeOrdinal1, int homeOrdinal2);

		~PruningTable();

		void Initialize(const char* fileName);
		
		void PruningTableIndexToMoveTableIndices(int index,
				int& ordinal1, int& ordinal2);
		
		int MoveTableIndicesToPruningTableIndex(int ordinal1,
				int ordinal2);
		
		unsigned int GetValue(int index);
		void SetValue(int index, unsigned int value);
		int SizeOf(void) { return TableSize; }

	private:
		enum { Empty = 0x0f };

		void Generate(void);
		void Save(const char* fileName);
		void Load(ifstream& infile);
		
		MoveTable& MoveTable1;
		MoveTable& MoveTable2;
		int HomeOrdinal1;
		int HomeOrdinal2;
		int MoveTable1Size;
		int MoveTable2Size;

		int TableSize;
		int AllocationSize;
		unsigned char (*Table);

		static unsigned int OffsetToEntryMask[2];
		static unsigned int OffsetToShiftCount[2];
};

#endif	// _PRUNINGT_H
