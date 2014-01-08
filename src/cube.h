
#ifndef	_CUBE_H
#define	_CUBE_H

#include "combinat.h"

class Cube
{
	public:
		enum EdgeCubie
		{
			
			UF =  0, UL =  1, UB =  2, UR =  3,
			FU =  0, LU =  1, BU =  2, RU =  3,

			DF =  4, DL =  5, DB =  6, DR =  7,
			FD =  4, LD =  5, BD =  6, RD =  7,
			
			RF =  8, FL =  9, LB = 10, BR = 11,
			FR =  8, LF =  9, BL = 10, RB = 11,

			FirstEdgeCubie = UF,
			LastEdgeCubie = BR,
			FirstMiddleSliceEdgeCubie = RF,
			LastMiddleSliceEdgeCubie = BR,
			NumberOfEdgeCubies = LastEdgeCubie+1
		};

		enum CornerCubie
		{
			URF = 0, UFL = 1, ULB = 2, UBR = 3,
			RFU = 0, FLU = 1, LBU = 2, BRU = 3,
			FUR = 0, LUF = 1, BUL = 2, RUB = 3,

			DFR = 4, DLF = 5, DBL = 6, DRB = 7,
			FRD = 4, LFD = 5, BLD = 6, RBD = 7,
			RDF = 4, FDL = 5, LDB = 6, BDR = 7,

			FirstCornerCubie = URF,
			LastCornerCubie = DRB,
			NumberOfCornerCubies = LastCornerCubie+1
		};

		enum Cubie
		{
			InvalidCubie = LastEdgeCubie+1
		};

		enum Twists
		{
			NoQuark, Quark, AntiQuark,
			NumberOfTwists = AntiQuark+1
		};	
		
		enum Flips
		{
			NotFlipped, Flipped
		};

		enum Move
		{
			R,  L,  U,  D,  F,  B,
			Ri, Li, Ui, Di, Fi, Bi,
			R2, L2, U2, D2, F2, B2,

			FirstMove = R,
			LastMove = B2,
			NumberOfClockwiseQuarterTurnMoves = B+1,
			NumberOfMoves = LastMove+1
		};
		
		Cube(void);

		virtual void BackToHome(void);
		
		virtual void SetState(
			int* cornerPermutation, int* cornerOrientation,
			int* edgePermutation, int* edgeOrientation);
		
		virtual void ApplyMove(int move);

		inline static int InverseOfMove(int move)
			{ return InverseMoves[move]; }

		inline static int QuarterTurnToHalfTurnMove(int move)
			{ return R2+move%(B+1); }

		inline static int OpposingFace(int move)
			{ return OppositeFaces[move]; };
		
		inline static const char* NameOfMove(int move)
			{ return MoveNames[move]; }

		static int MoveNameToMove(char* moveName, int& move);

	protected:
		void Move_R(void);
		void Move_L(void);
		void Move_U(void);
		void Move_D(void);
		void Move_F(void);
		void Move_B(void);
		void Move_Ri(void);
		void Move_Li(void);
		void Move_Ui(void);
		void Move_Di(void);
		void Move_Fi(void);
		void Move_Bi(void);
		void Move_R2(void);
		void Move_L2(void);
		void Move_U2(void);
		void Move_D2(void);
		void Move_F2(void);
		void Move_B2(void);

		void FourCycle(
				EdgeCubie first,
				EdgeCubie second,
				EdgeCubie third,
				EdgeCubie fourth);

		void FourCycle(
				CornerCubie first,
				CornerCubie second,
				CornerCubie third,
				CornerCubie fourth);

		void CycleFour(int* vector,
				int first,
				int second,
				int third,
				int fourth);

		void Flip(EdgeCubie cubie);

		void ClockwiseTwist(CornerCubie cubie);
		void CounterClockwiseTwist(CornerCubie cubie);

		int CornerCubiePermutations[NumberOfCornerCubies];
		int CornerCubieOrientations[NumberOfCornerCubies];

		int EdgeCubiePermutations[NumberOfEdgeCubies];
		int EdgeCubieOrientations[NumberOfEdgeCubies];

	private:
		static int ClockwiseTwists[NumberOfTwists];
		static int CounterClockwiseTwists[NumberOfTwists];

		static const char* MoveNames[NumberOfMoves];

		static Cube::Move InverseMoves[NumberOfMoves];
		static Cube::Move OppositeFaces[NumberOfMoves];
};

#endif	// _CUBE_H
