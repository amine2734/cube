
#include "solver.h"
#include "gui.h"
#include "animator.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

extern Animator* AnimCube;
extern MainDlg* Window;
extern QApplication* Application;


Solver::Solver(void)
	: twistMoveTable(cube),
	  flipMoveTable(cube),
	  choiceMoveTable(cube),
	  cornerPermutationMoveTable(cube),
	  nonMiddleSliceEdgePermutationMoveTable(cube),
	  middleSliceEdgePermutationMoveTable(cube),
	
	  TwistAndFlipPruningTable(
		twistMoveTable, flipMoveTable,
		cube.Twist(), cube.Flip()),
	  TwistAndChoicePruningTable(
		twistMoveTable, choiceMoveTable,
		cube.Twist(), cube.Choice()),
	  FlipAndChoicePruningTable(
		flipMoveTable, choiceMoveTable,
		cube.Flip(), cube.Choice()),
	  CornerAndSlicePruningTable(
		cornerPermutationMoveTable,
		middleSliceEdgePermutationMoveTable,
		cube.CornerPermutation(),
		cube.MiddleSliceEdgePermutation()),
	  EdgeAndSlicePruningTable(
		nonMiddleSliceEdgePermutationMoveTable,
		middleSliceEdgePermutationMoveTable,
		cube.NonMiddleSliceEdgePermutation(),
		cube.MiddleSliceEdgePermutation())
{
	minSolutionLength = Huge;
}

inline void NotifyGUI(const char *str)
{
	Window->SetInfo("Инициализация таблицы _%s_", str);
	Window->ProgressStep(1);
	Application->processEvents();
}

void Solver::InitializeTables(void)
{
	Window->ProgressInit(11);

	mkdir("Tables", 0755);
	
	NotifyGUI("Twist");
	twistMoveTable.Initialize("Tables/Twist.mtb");

	NotifyGUI("Flip");
	flipMoveTable.Initialize("Tables/Flip.mtb");

	NotifyGUI("Choice");
	choiceMoveTable.Initialize("Tables/Choice.mtb");

	NotifyGUI("CrnrPerm");
	cornerPermutationMoveTable.Initialize("Tables/CrnrPerm.mtb");

	NotifyGUI("EdgePerm");
	nonMiddleSliceEdgePermutationMoveTable.Initialize("Tables/EdgePerm.mtb");

	NotifyGUI("SlicPerm");
	middleSliceEdgePermutationMoveTable.Initialize("Tables/SlicPerm.mtb");

	NotifyGUI("TwstFlip");
	TwistAndFlipPruningTable.Initialize("Tables/TwstFlip.ptb");

	NotifyGUI("TwstChce");
	TwistAndChoicePruningTable.Initialize("Tables/TwstChce.ptb");

	NotifyGUI("FlipChce");
	FlipAndChoicePruningTable.Initialize("Tables/FlipChce.ptb");

	NotifyGUI("CrnrSlic");
	CornerAndSlicePruningTable.Initialize("Tables/CrnrSlic.ptb");

	NotifyGUI("EdgeSlic");
	EdgeAndSlicePruningTable.Initialize("Tables/EdgeSlic.ptb");
}

int Solver::Solve(KociembaCube& scrambledCube)
{
	int iteration = 1;
	int result = NOT_FOUND;

	stopped = false;
	Window->ProgressInit(0);
	
	cube = scrambledCube;

	threshold1 = Phase1Cost(cube.Twist(), cube.Flip(), cube.Choice());

	nodes = 1;
	solutionLength1 = 0;

	do
	{
		Window->SetInfo("Обработка глубины _%d / %d_",
				iteration, nodes);

		newThreshold1 = Huge;

		result = Search1(cube.Twist(), cube.Flip(), cube.Choice(), 0);

		if(stopped)
		{
			Window->SetInfo("_Готово_");
			return ABORT;
		}

		threshold1 = newThreshold1;
		iteration++;

		Application->processEvents();
	} while (result == NOT_FOUND);

	Window->SetInfo("_Найдено оптимальное решение_");
	return result;
}

int Solver::Search1(int twist, int flip, int choice, int depth)
{
	int cost, totalCost;
	int move;
	int power;
	int twist2, flip2, choice2;
	int result;

	cost = Phase1Cost(twist, flip, choice);

	if (cost == 0)
	{
		solutionLength1 = depth;
		
		KociembaCube phase2Cube = cube;
		for (int i = 0; i < solutionLength1; i++)
		{
			for (power = 0; power < solutionPowers1[i]; power++)
				phase2Cube.ApplyMove(solutionMoves1[i]);
		}

		(void)Solve2(phase2Cube);
		if(stopped) return ABORT;
	}

	totalCost = depth + cost;

	if (totalCost <= threshold1)
	{
		if (depth >= minSolutionLength-1)
			return OPTIMUM_FOUND;
		
		for (move = Cube::R; move <= Cube::B; move++)
		{
			if (Disallowed(move, solutionMoves1, depth)) continue;
				
			twist2  = twist;
			flip2   = flip;
			choice2 = choice;

			solutionMoves1[depth] = move;
			for (power = 1; power < 4; power++)
			{
				solutionPowers1[depth] = power;
				twist2  = twistMoveTable[twist2][move];
				flip2   = flipMoveTable[flip2][move];
				choice2 = choiceMoveTable[choice2][move];

				if ((result =
					Search1(twist2, flip2, choice2, depth+1)))
						return result;
			}
		}
	}
	else
	{
		if (totalCost < newThreshold1)
			newThreshold1 = totalCost;
	}
	return NOT_FOUND;
}

int Solver::Solve2(KociembaCube& cube)
{
	int iteration = 1;
	int result = NOT_FOUND;


	threshold2 = Phase2Cost(
		cube.CornerPermutation(),
		cube.NonMiddleSliceEdgePermutation(),
		cube.MiddleSliceEdgePermutation());

	nodes++;
	nodes2 = 1;
	solutionLength2 = 0;
	
	do
	{
		newThreshold2 = Huge;

		result = Search2(
			cube.CornerPermutation(),
			cube.NonMiddleSliceEdgePermutation(),
			cube.MiddleSliceEdgePermutation(), 0);

		threshold2 = newThreshold2;
		
		iteration++;
	} while (result == NOT_FOUND);

	Application->processEvents();

	return result;
}

int Solver::Search2(
	int cornerPermutation,
	int nonMiddleSliceEdgePermutation,
	int middleSliceEdgePermutation,
	int depth)
{
	int cost, totalCost;
	int move;
	int power, powerLimit;
	int cornerPermutation2;
	int nonMiddleSliceEdgePermutation2;
	int middleSliceEdgePermutation2;
	int result;

	cost = Phase2Cost(
		cornerPermutation,
		nonMiddleSliceEdgePermutation,
		middleSliceEdgePermutation);

	if (cost == 0)
	{
		solutionLength2 = depth;
		if (solutionLength1 + solutionLength2 < minSolutionLength)
			minSolutionLength = solutionLength1 + solutionLength2;
		PrintSolution();
		return FOUND;
	}

	if (cost < 2)
		Window->ProgressStep(1);

	
	totalCost = depth + cost;

	if (totalCost <= threshold2)
	{
		if (solutionLength1 + depth >= minSolutionLength-1) return ABORT;

		for (move = Cube::R; move <= Cube::B; move++)
		{
		  if (Disallowed(move, solutionMoves2, depth)) continue;

		  cornerPermutation2 = cornerPermutation;
		  nonMiddleSliceEdgePermutation2 = nonMiddleSliceEdgePermutation;
		  middleSliceEdgePermutation2 = middleSliceEdgePermutation;

		  solutionMoves2[depth] = move;
		  powerLimit = 4;
		  if (move != Cube::U && move != Cube::D) powerLimit=2;

		  for (power = 1; power < powerLimit; power++)
		  {
		    cornerPermutation2 =
		        cornerPermutationMoveTable
				    [cornerPermutation2][move];
		    
		    nonMiddleSliceEdgePermutation2 =
		        nonMiddleSliceEdgePermutationMoveTable
				    [nonMiddleSliceEdgePermutation2][move];
		    
		    middleSliceEdgePermutation2 =
		        middleSliceEdgePermutationMoveTable
				    [middleSliceEdgePermutation2][move];

		    solutionPowers2[depth] = power;

		    nodes2++;

		    if ((result = Search2(
				cornerPermutation2,
				nonMiddleSliceEdgePermutation2,
				middleSliceEdgePermutation2, depth+1)))
				return result;
		  }
		}
	}
	else
	{
		if (totalCost < newThreshold2)
			newThreshold2 = totalCost;
	}
	return NOT_FOUND;
}

int Solver::Phase1Cost(int twist, int flip, int choice)
{
	int cost = TwistAndFlipPruningTable.GetValue(
			twist*flipMoveTable.SizeOf()+flip);
	
	int cost2 = TwistAndChoicePruningTable.GetValue(
			twist*choiceMoveTable.SizeOf()+choice);
	
	if (cost2 > cost) cost = cost2;
	
	cost2 = FlipAndChoicePruningTable.GetValue(
			flip*choiceMoveTable.SizeOf()+choice);
	
	if (cost2 > cost) cost = cost2;
	return cost;
}

int Solver::Phase2Cost(
		int cornerPermutation,
		int nonMiddleSliceEdgePermutation,
		int middleSliceEdgePermutation)
{
	int cost = CornerAndSlicePruningTable.GetValue(
		cornerPermutation*middleSliceEdgePermutationMoveTable.SizeOf()+
			middleSliceEdgePermutation);
	
	int cost2 = EdgeAndSlicePruningTable.GetValue(
		nonMiddleSliceEdgePermutation*
			middleSliceEdgePermutationMoveTable.SizeOf()+
				middleSliceEdgePermutation);
	
	if (cost2 > cost) cost = cost2;
	return cost;
}

int Solver::Disallowed(int move, int* solutionMoves, int depth)
{
	if (depth > 0)
	{
		if (solutionMoves[depth-1] == move)
			return 1;
		if ((move == Cube::F) && solutionMoves[depth-1] == Cube::B)
			return 1;
		if ((move == Cube::R) && solutionMoves[depth-1] == Cube::L)
			return 1;
		if ((move == Cube::U) && solutionMoves[depth-1] == Cube::D)
			return 1;

		if ((depth > 1) && solutionMoves[depth-2] == move &&
			solutionMoves[depth-1] == Cube::OpposingFace(move))
			return 1;
	}
	return 0;
}

inline void AddMove(int index)
{
	int n;
	const char *m = Cube::NameOfMove(index);
	switch(m[1])
	{
		case '0': n = -1; break;
		case '1': n =  1; break;
		case '2': n =  2; break;
	}
	
	int move = MOVE(m[0], n);
	AnimCube->MoveList.Add(move);
	Window->ListAdd(move);
}

void Solver::PrintSolution(void)
{
	AnimCube->MoveList.Reset();
	Window->ListClear();
	Window->ListAdd(0);
	Window->SetMovesNum(solutionLength1 + solutionLength2);
	
	for (int i = 0; i < solutionLength1; i++)
		AddMove(TranslateMove(solutionMoves1[i], solutionPowers1[i], 0));
		
	for (int i = 0; i < solutionLength2; i++)
		AddMove(TranslateMove(solutionMoves2[i], solutionPowers2[i], 1));
}

int Solver::TranslateMove(int move, int power, int phase2)
{
	int translatedMove = move;

	if (phase2 && move != Cube::U && move != Cube::D)
		power = 2;
		
	if (power == 2)
		translatedMove = Cube::QuarterTurnToHalfTurnMove(move);
	else if (power == 3)
		translatedMove = Cube::InverseOfMove(move);

	return translatedMove;
}
