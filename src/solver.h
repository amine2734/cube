
#ifndef	_SOLVER_H
#define	_SOLVER_H

#include "kocicube.h"
#include "kocimovt.h"
#include "pruningt.h"

class Solver
{
	public:
		Solver(void);

		void InitializeTables(void);
		int  Solve(KociembaCube& scrambledCube);
		
		enum
		{
			NOT_FOUND,
			FOUND,
			OPTIMUM_FOUND,
			ABORT
		};

		void PrintSolution(void);
		bool stopped;

	private:
		enum { Huge = 10000 };

		int Solve2(KociembaCube& cube);
		int Search1(int twist, int flip, int choice, int depth);
		int Search2(
			int cornerPermutation,
			int nonMiddleSliceEdgePermutation,
			int middleSliceEdgePermutation,
			int depth);

		int Phase1Cost(int twist, int flip, int choice);
		int Phase2Cost(
			int cornerPermutation,
			int nonMiddleSliceEdgePermutation,
			int middleSliceEdgePermutation);

		inline int Disallowed(int move, int* solutionMoves, int depth);

		int TranslateMove(int move, int power, int phase2);
		
		int nodes, nodes2;
		int threshold1, threshold2;
		int newThreshold1, newThreshold2;

		int solutionMoves1[32], solutionMoves2[32];
		int solutionPowers1[32], solutionPowers2[32];
		int solutionLength1, solutionLength2;
		int minSolutionLength;

		KociembaCube cube;

		TwistMoveTable twistMoveTable;
		FlipMoveTable flipMoveTable;
		ChoiceMoveTable choiceMoveTable;
		CornerPermutationMoveTable cornerPermutationMoveTable;
		NonMiddleSliceEdgePermutationMoveTable
				nonMiddleSliceEdgePermutationMoveTable;
		MiddleSliceEdgePermutationMoveTable
				middleSliceEdgePermutationMoveTable;
		
		PruningTable TwistAndFlipPruningTable;
		PruningTable TwistAndChoicePruningTable;
		PruningTable FlipAndChoicePruningTable;
		PruningTable CornerAndSlicePruningTable;
		PruningTable EdgeAndSlicePruningTable;
};

#endif	// _SOLVER_H
