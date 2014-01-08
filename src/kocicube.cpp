
#include "kocicube.h"
#include "combinat.h"


int KociembaCube::Twist(void)
{
	int corner;
	int twist = 0;
	for (corner = FirstCornerCubie; corner < LastCornerCubie; corner++)
		twist = twist*3 + CornerCubieOrientations[corner];

	return twist;
}

void KociembaCube::Twist(int twist)
{
	int corner;
	int paritySum = 0;
	for (corner =  LastCornerCubie-1; corner >= FirstCornerCubie; corner--)
	{
		paritySum += (CornerCubieOrientations[corner] = twist%3);
		twist /= 3;
	}

	CornerCubieOrientations[LastCornerCubie] = (3-paritySum%3)%3;
	return;
}

int KociembaCube::Flip(void)
{
	int edge;
	int flip = 0;
	for (edge = FirstEdgeCubie; edge < LastEdgeCubie; edge++)
		flip = flip*2 + EdgeCubieOrientations[edge];

	return flip;
}

void KociembaCube::Flip(int flip)
{
	int edge;
	int paritySum = 0;
	for (edge =  LastEdgeCubie-1; edge >= FirstEdgeCubie; edge--)
	{
		paritySum += (EdgeCubieOrientations[edge] = flip%2);
		flip /= 2;
	}

	EdgeCubieOrientations[LastEdgeCubie] = paritySum%2;
	return;
}

int KociembaCube::Choice(void)
{
	int choicePermutation[4];
	int edge;
	int i = 0;

	for (edge =  FirstEdgeCubie; edge <= LastEdgeCubie; edge++)
	{
		if (IsMiddleSliceEdgeCubie(EdgeCubiePermutations[edge]))
			choicePermutation[i++] = edge;
	}
	return ChoiceOrdinal(choicePermutation);
}

void KociembaCube::Choice(int choice)
{
	ChoicePermutation(choice, EdgeCubiePermutations);
	return;
}


int KociembaCube::CornerPermutation(void)
{
	return PermutationToOrdinal(CornerCubiePermutations, NumberOfCornerCubies);
}

void KociembaCube::CornerPermutation(int ordinal)
{
	OrdinalToPermutation(ordinal, CornerCubiePermutations,
			      NumberOfCornerCubies, FirstCornerCubie);
	return;
}

int KociembaCube::NonMiddleSliceEdgePermutation(void)
{
	return PermutationToOrdinal(EdgeCubiePermutations, 8);
}


void KociembaCube::NonMiddleSliceEdgePermutation(int ordinal)
{
	OrdinalToPermutation(ordinal, EdgeCubiePermutations, 8, FirstEdgeCubie);
	return;
}

int KociembaCube::MiddleSliceEdgePermutation(void)
{
	return PermutationToOrdinal(
		&EdgeCubiePermutations[FirstMiddleSliceEdgeCubie], 4);
}

void KociembaCube::MiddleSliceEdgePermutation(int ordinal)
{
	OrdinalToPermutation(ordinal,
			     &EdgeCubiePermutations[FirstMiddleSliceEdgeCubie],
			     4, FirstMiddleSliceEdgeCubie);
	return;
}

int KociembaCube::IsMiddleSliceEdgeCubie(int cubie)
{
	return cubie >= FirstMiddleSliceEdgeCubie &&
			 cubie <= LastMiddleSliceEdgeCubie;
}

int KociembaCube::ChoiceOrdinal(int* choicePermutation)
{
	int edgeMarkVector[NumberOfEdgeCubies];
	int edgesRemaining = 4;
	int ordinal = 0;
	int edge;

	for (edge = 0; edge < NumberOfEdgeCubies; edge++)
		edgeMarkVector[edge] = 0;
	for (edge = 0; edge < 4; edge++)
		edgeMarkVector[choicePermutation[edge]] = 1;

	edge = 0;
	while (edgesRemaining > 0)
	{
		if (edgeMarkVector[edge++])
			edgesRemaining--;
		else
			ordinal += NChooseM(12-edge, edgesRemaining-1);
	}
	return ordinal;
}

void KociembaCube::ChoicePermutation(int choiceOrdinal, int* choicePermutation)
{
	int edge;
	int digit = 0;
	int combinations;

	for (edge = 0; edge < NumberOfEdgeCubies; edge++)
		choicePermutation[edge] = InvalidCubie;

	for (edge = 0; edge < 4; edge++)
	{
		for (;;)
		{
			combinations=NChooseM(12-1-digit++, 4-1-edge);
			if (choiceOrdinal >= combinations)
				choiceOrdinal -= combinations;
			else
				break;
		}
		choicePermutation[digit-1] = FirstMiddleSliceEdgeCubie+edge;
	}
}
