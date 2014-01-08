
#ifndef	_FACECUBE_H
#define	_FACECUBE_H

#include "cube.h"

#define FacesToCorner(face1, face2, face3) (((face1*6)+face2)*6+face3)
#define FacesToEdge(face1, face2) (face1*6+face2)

class FaceletCube
{
	public:
		enum Face { U, D, L, R, F, B };

		enum
		{
			VALID,
			INVALID_MARKER,
			INVALID_FACELETCOUNT,
			DUPLICATE_CENTER_MARKING,
			INVALID_CORNER_MARKINGS,
			INVALID_CORNER_PARITY,
			INVALID_EDGE_MARKINGS,
			INVALID_EDGE_PARITY,
			INVALID_TOTAL_PARITY,
			NumberOfErrors
		};

		FaceletCube(void);

		int Validate(Cube& cube);
		int FaceNameToOffset(char faceName);
		const char* ErrorText(unsigned int error);

		char faceletMarkings[6*9];

	private:
		enum Corner
		{
			URF = FacesToCorner(U,R,F),
			RFU = FacesToCorner(R,F,U),
			FUR = FacesToCorner(F,U,R),

			UFL = FacesToCorner(U,F,L),
			FLU = FacesToCorner(F,L,U),
			LUF = FacesToCorner(L,U,F),

			ULB = FacesToCorner(U,L,B),
			LBU = FacesToCorner(L,B,U),
			BUL = FacesToCorner(B,U,L),

			UBR = FacesToCorner(U,B,R),
			BRU = FacesToCorner(B,R,U),
			RUB = FacesToCorner(R,U,B),

			DFR = FacesToCorner(D,F,R),
			FRD = FacesToCorner(F,R,D),
			RDF = FacesToCorner(R,D,F),

			DLF = FacesToCorner(D,L,F),
			LFD = FacesToCorner(L,F,D),
			FDL = FacesToCorner(F,D,L),

			DBL = FacesToCorner(D,B,L),
			BLD = FacesToCorner(B,L,D),
			LDB = FacesToCorner(L,D,B),

			DRB = FacesToCorner(D,R,B),
			RBD = FacesToCorner(R,B,D),
			BDR = FacesToCorner(B,D,R)
		};

		enum Edge
		{
			UF = FacesToEdge(U,F),
			FU = FacesToEdge(F,U),

			UL = FacesToEdge(U,L),
			LU = FacesToEdge(L,U),

			UB = FacesToEdge(U,B),
			BU = FacesToEdge(B,U),
			
			UR = FacesToEdge(U,R),
			RU = FacesToEdge(R,U),
			
			DF = FacesToEdge(D,F),
			FD = FacesToEdge(F,D),
			
			DL = FacesToEdge(D,L),
			LD = FacesToEdge(L,D),
			
			DB = FacesToEdge(D,B),
			BD = FacesToEdge(B,D),
			
			DR = FacesToEdge(D,R),
			RD = FacesToEdge(R,D),
			
			RF = FacesToEdge(R,F),
			FR = FacesToEdge(F,R),
			
			LF = FacesToEdge(L,F),
			FL = FacesToEdge(F,L),
			
			LB = FacesToEdge(L,B),
			BL = FacesToEdge(B,L),
			
			RB = FacesToEdge(R,B),
			BR = FacesToEdge(B,R)
		};

		int ValidateCenters(void);
		int ValidateFacelets(void);
		int ValidateCorners(void);
		int ValidateEdges(void);
		int EdgePermutationParity(void);
		int CornerPermutationParity(void);
		int PermutationParity(int* permutations, int numberOfCubies);
		int FaceletOffsetToMarking(int offset);

		char faceMarkings[6+1];

		static int cornerFacelets[Cube::NumberOfCornerCubies][3];
		static Corner cornerMap[Cube::NumberOfCornerCubies*3];

		static int edgeFacelets[Cube::NumberOfEdgeCubies][2];
		static Edge edgeMap[Cube::NumberOfEdgeCubies*2];

		int cornerCubiePermutations[Cube::NumberOfCornerCubies];
		int cornerCubieOrientations[Cube::NumberOfCornerCubies];

		int edgeCubiePermutations[Cube::NumberOfEdgeCubies];
		int edgeCubieOrientations[Cube::NumberOfEdgeCubies];

		static const char* errorText[NumberOfErrors];
};

#endif // _FACECUBE_H
