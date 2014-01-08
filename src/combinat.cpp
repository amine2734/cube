
#ifndef	_COMBINAT_CPP
#define	_COMBINAT_CPP

#include "combinat.h"
#include <cstring>


int NChooseM(int N, int M)
{
	int NoverMfact = N;

	int Mfact = 1;

	int Result = 1;
	if (N < M) return 0;
	if (M > N/2) M = N-M;
	while (NoverMfact > M)
	{
		Result *= NoverMfact--;
		Result /= Mfact++;
	}
	return Result;
}

int PermutationToOrdinal(int* vector, int n)
{
	int Ordinal = 0;
	int Vector[12];
	int Limit;
	int i;
	int Coeff_i = 0;
	int Temp;

	CopyVector(vector, Vector, n);
	
	for (Limit = n-1; Limit > 0; Limit--)
	{
		Temp = -1;
		for (i = 0; i <= Limit; i++)
		{
			if (Vector[i] > Temp)
			{
				Temp = Vector[i];
				Coeff_i = i;
			}
		}
		Ordinal = Ordinal*(Limit+1)+Coeff_i;

		Temp            = Vector[Limit];
		Vector[Limit]   = Vector[Coeff_i];
		Vector[Coeff_i] = Temp;
	}
	return Ordinal;
}

void OrdinalToPermutation(int Ordinal, int* vector, int n, int offset)
{
	int i;
	int Coeff_i;
	int Temp;
	for (i = 0; i < n; i++)
		vector[i] = i+offset;

	for (i = 1; i < n; i++)
	{

		Coeff_i   = Ordinal % (i+1);
		Ordinal   /= (i+1);

		Temp            = vector[i];
		vector[i]       = vector[Coeff_i];
		vector[Coeff_i] = Temp;
	}
}

void CopyVector(int* fromVector, int* toVector, int n)
{
	memcpy(toVector, fromVector, n*sizeof(int));
}

#endif	// _COMBINAT_CPP
