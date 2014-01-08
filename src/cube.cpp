
#include "cube.h"
#include <cstring>

Cube::Cube(void)
{
	BackToHome();
}

void Cube::BackToHome(void)
{
	int cubie;
	for (cubie = FirstEdgeCubie; cubie <= LastEdgeCubie; cubie++)
	{
		EdgeCubiePermutations[cubie] = cubie;
		EdgeCubieOrientations[cubie] = NotFlipped;
	}
	for (cubie = FirstCornerCubie; cubie <= LastCornerCubie; cubie++)
	{
		CornerCubiePermutations[cubie] = cubie;
		CornerCubieOrientations[cubie] = NoQuark;
	}
}

void Cube::SetState(
		int* cornerPermutation, int* cornerOrientation,
		int* edgePermutation, int* edgeOrientation)
{
	int cubie;
	for (cubie = FirstEdgeCubie; cubie <= LastEdgeCubie; cubie++)
	{
		EdgeCubiePermutations[cubie] = edgePermutation[cubie];
		EdgeCubieOrientations[cubie] = edgeOrientation[cubie];
	}
	for (cubie = FirstCornerCubie; cubie <= LastCornerCubie; cubie++)
	{
		CornerCubiePermutations[cubie] = cornerPermutation[cubie];
		CornerCubieOrientations[cubie] = cornerOrientation[cubie];
	}
}
	
void Cube::ApplyMove(int move)
{
	switch (move)
	{
	case Cube::R:	Move_R();	break;
	case Cube::U:	Move_U();	break;
	case Cube::L:	Move_L();	break;
	case Cube::D:	Move_D();	break;
	case Cube::F:	Move_F();	break;
	case Cube::B:	Move_B();	break;
	case Cube::Ri:	Move_Ri();	break;
	case Cube::Li:	Move_Li();	break;
	case Cube::Ui:	Move_Ui();	break;
	case Cube::Di:	Move_Di();	break;
	case Cube::Fi:	Move_Fi();	break;
	case Cube::Bi:	Move_Bi();	break;
	case Cube::R2:	Move_R2();	break;
	case Cube::L2:	Move_L2();	break;
	case Cube::U2:	Move_U2();	break;
	case Cube::D2:	Move_D2();	break;
	case Cube::F2:	Move_F2();	break;
	case Cube::B2:	Move_B2();	break;
	}
}

int Cube::MoveNameToMove(char* moveName, int& move)
{
	int moveIndex;
	int found = 0;

	for (moveIndex = 0; moveIndex < NumberOfMoves; moveIndex++)
	{
		if (!strcmp(moveName, MoveNames[moveIndex]))
		{
			move = moveIndex;
			found = 1;
			break;
		}
	}
	return found;
}

void Cube::Move_R(void)
{
	FourCycle(URF, UBR, DRB, DFR);

	ClockwiseTwist       (URF);
	CounterClockwiseTwist(UBR);
	ClockwiseTwist       (DRB);
	CounterClockwiseTwist(DFR);

	FourCycle(UR, BR, DR, RF);

	Flip(UR); Flip(BR); Flip(DR); Flip(RF);
}

void Cube::Move_L(void)
{
	FourCycle(ULB, UFL, DLF, DBL);

	ClockwiseTwist       (ULB);
	CounterClockwiseTwist(UFL);
	ClockwiseTwist       (DLF);
	CounterClockwiseTwist(DBL);

	FourCycle(UL, FL, DL, LB);

	Flip(UL); Flip(FL); Flip(DL); Flip(LB);
}

void Cube::Move_U(void)
{
	FourCycle(ULB, UBR, URF, UFL);
	FourCycle(UB, UR, UF, UL);
}

void Cube::Move_D(void)
{
	FourCycle(DLF, DFR, DRB, DBL);
	FourCycle(DF, DR, DB, DL);
}

void Cube::Move_F(void)
{
	FourCycle(UFL, URF, DFR, DLF);

	ClockwiseTwist       (UFL);
	CounterClockwiseTwist(URF);
	ClockwiseTwist       (DFR);
	CounterClockwiseTwist(DLF);

	FourCycle(UF, RF, DF, FL);
}

void Cube::Move_B(void)
{
	FourCycle(UBR, ULB, DBL, DRB);

	ClockwiseTwist       (UBR);
	CounterClockwiseTwist(ULB);
	ClockwiseTwist       (DBL);
	CounterClockwiseTwist(DRB);

	FourCycle(UB, LB, DB, BR);

}

void Cube::Move_Ri(void)
{
	FourCycle(UBR, URF, DFR, DRB);

	CounterClockwiseTwist(UBR);
	ClockwiseTwist       (URF);
	CounterClockwiseTwist(DFR);
	ClockwiseTwist       (DRB);

	FourCycle(UR, RF, DR, BR);

	Flip(UR); Flip(RF); Flip(DR); Flip(BR);
}

void Cube::Move_Li(void)
{
	FourCycle(UFL, ULB, DBL, DLF);

	CounterClockwiseTwist(UFL);
	ClockwiseTwist       (ULB);
	CounterClockwiseTwist(DBL);
	ClockwiseTwist       (DLF);

	FourCycle(UL, LB, DL, FL);

	Flip(UL); Flip(LB); Flip(DL); Flip(FL);
}

void Cube::Move_Ui(void)
{
	FourCycle(UBR, ULB, UFL, URF);
	FourCycle(UB, UL, UF, UR);
}

void Cube::Move_Di(void)
{
	FourCycle(DFR, DLF, DBL, DRB);
	FourCycle(DF, DL, DB, DR);
}

void Cube::Move_Fi(void)
{
	FourCycle(URF, UFL, DLF, DFR);

	CounterClockwiseTwist(URF);
	ClockwiseTwist       (UFL);
	CounterClockwiseTwist(DLF);
	ClockwiseTwist       (DFR);

	FourCycle(UF, FL, DF, RF);
}

void Cube::Move_Bi(void)
{
	FourCycle(ULB, UBR, DRB, DBL);

	CounterClockwiseTwist(ULB);
	ClockwiseTwist       (UBR);
	CounterClockwiseTwist(DRB);
	ClockwiseTwist       (DBL);

	FourCycle(UB, BR, DB, LB);
}

void Cube::Move_R2(void)
{
	Move_R(); Move_R();
}

void Cube::Move_L2(void)
{
	Move_L(); Move_L();
}

void Cube::Move_U2(void)
{
	Move_U(); Move_U();
}

void Cube::Move_D2(void)
{
	Move_D(); Move_D();
}

void Cube::Move_F2(void)
{
	Move_F(); Move_F();
}

void Cube::Move_B2(void)
{
	Move_B(); Move_B();
}


void Cube::FourCycle(
			EdgeCubie first,
			EdgeCubie second,
			EdgeCubie third,
			EdgeCubie fourth)
{
	CycleFour(EdgeCubiePermutations, first, second, third, fourth);
	CycleFour(EdgeCubieOrientations, first, second, third, fourth);
}


void Cube::FourCycle(
			CornerCubie first,
			CornerCubie second,
			CornerCubie third,
			CornerCubie fourth)
{
	CycleFour(CornerCubiePermutations, first, second, third, fourth);
	CycleFour(CornerCubieOrientations, first, second, third, fourth);
}


void Cube::CycleFour(
		int* vector,
		int first,
		int second,
		int third,
		int fourth)
{
	int temp       = vector[fourth];
	vector[fourth] = vector[third];
	vector[third]  = vector[second];
	vector[second] = vector[first];
	vector[first]  = temp;
}


void Cube::Flip(EdgeCubie cubie)
{
	EdgeCubieOrientations[cubie] ^= 1;
}


void Cube::ClockwiseTwist(CornerCubie cubie)
{
	CornerCubieOrientations[cubie] =
			ClockwiseTwists[CornerCubieOrientations[cubie]];
}

void Cube::CounterClockwiseTwist(CornerCubie cubie)
{
	CornerCubieOrientations[cubie] =
			CounterClockwiseTwists[CornerCubieOrientations[cubie]];
}


Cube::Move Cube::InverseMoves[NumberOfMoves] = {
	Ri, Li, Ui, Di, Fi, Bi,
	R,  L,  U,  D,  F,  B,
	R2, L2, U2, D2, F2, B2 };


Cube::Move Cube::OppositeFaces[NumberOfMoves] = {
	L,  R,  D,  U,  B,  F };
	

const char* Cube::MoveNames[NumberOfMoves] = {
	"R1", "L1", "U1", "D1", "F1", "B1",
	"R0", "L0", "U0", "D0", "F0", "B0",
	"R2", "L2", "U2", "D2", "F2", "B2" };


int Cube::ClockwiseTwists[] = {
	Quark, AntiQuark, NoQuark };
	
int Cube::CounterClockwiseTwists[] = {
	AntiQuark, NoQuark, Quark };
