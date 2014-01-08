
#ifndef	_KOCICUBE_H
#define	_KOCICUBE_H

#include "cube.h"

class KociembaCube : public Cube
{
	public:
		enum
		{
			Twists = 2187,
			Flips = 2048,
			Choices = 495,
			CornerPermutations = 40320,
			NonMiddleSliceEdgePermutations = 40320,
			MiddleSliceEdgePermutations = 24
		};

		int  Twist(void);
		void Twist(int twist);
		int  Flip(void);
		void Flip(int flip);
		int  Choice(void);
		void Choice(int choice);

		int  CornerPermutation(void);
		void CornerPermutation(int ordinal);
		int  NonMiddleSliceEdgePermutation(void);
		void NonMiddleSliceEdgePermutation(int ordinal);
		int  MiddleSliceEdgePermutation(void);
		void MiddleSliceEdgePermutation(int ordinal);

	private:
		enum {
			NumberOfEncodedChoicePermutations = 4096,
			NumberOfChoiceOrdinals = 495 };

		static int IsMiddleSliceEdgeCubie(int cubie);
		static int ChoiceOrdinal(int* choicePermutation);
		static void ChoicePermutation(int choiceOrdinal,
					      int* choicePermutation);
};

#endif // _KOCICUBE_H
