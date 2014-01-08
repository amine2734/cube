
#include "facecube.h"
#include <cstring>

FaceletCube::FaceletCube(void)
{
	faceMarkings[6] = '\0';
}

int FaceletCube::Validate(Cube& cube)
{
	int status;
	
	if ((status = ValidateCenters()) != VALID)
		return status;

	if ((status = ValidateFacelets()) != VALID)
		return status;
	
	if ((status = ValidateCorners()) != VALID)
		return status;

	if ((status = ValidateEdges()) != VALID)
		return status;

	if (EdgePermutationParity() != CornerPermutationParity())
		return INVALID_TOTAL_PARITY;

	cube.SetState(cornerCubiePermutations, cornerCubieOrientations,
		edgeCubiePermutations, edgeCubieOrientations);

	return status;
}

int FaceletCube::ValidateCenters(void)
{
	int face;
	int faceName;

	for (face = 0; face < 6; face++)
		 faceMarkings[face] = '\0';
	
	for (face = 0; face < 6; face++)
	{
		for (faceName = 0; faceName < 6; faceName++)
		{
			if (faceMarkings[faceName] == faceletMarkings[face*9+4])
				return DUPLICATE_CENTER_MARKING;
		}
		faceMarkings[face] = faceletMarkings[face*9+4];
	}
	
	return VALID;
}

int FaceletCube::ValidateFacelets(void)
{
	int face;
	int facelet;
	int facelets[6];

	for (face = 0; face < 6; face++)
		facelets[face] = 0;
	
	for (facelet = 0; facelet < 6*9; facelet++)
	{
		if ((face = FaceletOffsetToMarking(facelet)) < 0)
			return INVALID_MARKER;

		facelets[face]++;
	}

	for (face = 0; face < 6; face++)
	{
		if (facelets[face] != 9)
			return INVALID_FACELETCOUNT;
	}
	
	return VALID;
}

int FaceletCube::ValidateCorners(void)
{
	int cubicle;
	int corner, cornerLocation;
	int cornerParity = 0;
	int cubie;

	for (cubicle = 0; cubicle < Cube::NumberOfCornerCubies; cubicle++)
	{
		cubie = FacesToCorner(
			FaceletOffsetToMarking(cornerFacelets[cubicle][0]),
			FaceletOffsetToMarking(cornerFacelets[cubicle][1]),
			FaceletOffsetToMarking(cornerFacelets[cubicle][2]));

		cornerLocation = -1;

		for (corner = 0; corner < Cube::NumberOfCornerCubies*3; corner++)
		{
			if (cubie == cornerMap[corner])
			{
				cornerLocation = corner;
				cornerCubiePermutations[cubicle] = corner/3;
				cornerCubieOrientations[cubicle] = corner%3;
				cornerParity += cornerCubieOrientations[cubicle];
				break;
			}
		}

		if (cornerLocation == -1)
			return INVALID_CORNER_MARKINGS;
	}
	
	if (cornerParity%3 != 0)
		return INVALID_CORNER_PARITY;
	
	return VALID;
}

int FaceletCube::ValidateEdges(void)
{
	int cubicle;
	int edge, edgeLocation;
	int edgeParity = 0;
	int cubie;

	for (cubicle = 0; cubicle < Cube::NumberOfEdgeCubies; cubicle++)
	{
		cubie = FacesToEdge(
			FaceletOffsetToMarking(edgeFacelets[cubicle][0]),
			FaceletOffsetToMarking(edgeFacelets[cubicle][1]));

		edgeLocation = -1;
		
		for (edge = 0; edge < Cube::NumberOfEdgeCubies*2; edge++)
		{
			if (cubie == edgeMap[edge])
			{
				edgeLocation = edge;
				edgeCubiePermutations[cubicle] = edge/2;
				edgeCubieOrientations[cubicle] = edge%2;
				edgeParity += edgeCubieOrientations[cubicle];
				break;
			}
		}
		
		if (edgeLocation == -1)
			return INVALID_EDGE_MARKINGS;
	}

	if (edgeParity%2 != 0)
		return INVALID_EDGE_PARITY;

	return VALID;
}


int FaceletCube::EdgePermutationParity()
{
	return PermutationParity(edgeCubiePermutations,
				  Cube::NumberOfEdgeCubies);
}


int FaceletCube::CornerPermutationParity()
{
	return PermutationParity(cornerCubiePermutations,
				  Cube::NumberOfCornerCubies);
}


int FaceletCube::PermutationParity(int* permutations, int numberOfCubies)
{
	int p, q;
	int permutationParity = 0;

	for (p = 0; p < numberOfCubies-1; p++)
	{
		for (q = p+1; q < numberOfCubies; q++)
		{
			if (permutations[p] > permutations[q])
				permutationParity++;
		}
	}
	return permutationParity%2;
}


int FaceletCube::FaceNameToOffset(char faceName)
{
	static const char* faceNames = "UDLRFB";
	char *name;
	
	if (!(name = strchr(faceNames, faceName)))
		return -1;
	return name-faceNames;
}

int FaceletCube::FaceletOffsetToMarking(int offset)
{
	char* name;
	if (!(name = strchr(faceMarkings, faceletMarkings[offset])))
		return -1;
	return name-faceMarkings;
}


int FaceletCube::cornerFacelets[Cube::NumberOfCornerCubies][3] = {
	{ 8,  27, 38 },		// URF
	{ 6,  36, 20 },		// UFL
	{ 0,  18, 47 },		// ULB
	{ 2,  45, 29 },		// UBR
	{ 11, 44, 33 },		// DFR
	{  9, 26, 42 },		// DLF
	{ 15, 53, 24 },		// DBL
	{ 17, 35, 51 } };	// DRB


FaceletCube::Corner FaceletCube::cornerMap[Cube::NumberOfCornerCubies*3] = {
//   0    1    2
	URF, RFU, FUR,
	UFL, FLU, LUF,
	ULB, LBU, BUL,
	UBR, BRU, RUB,
	DFR, FRD, RDF,
	DLF, LFD, FDL,
	DBL, BLD, LDB,
	DRB, RBD, BDR };


int FaceletCube::edgeFacelets[Cube::NumberOfEdgeCubies][2] = {
	{  7, 37 },	// UF
	{  3, 19 },	// UL
	{  1, 46 },	// UB
	{  5, 28 },	// UR
	{ 10, 43 },	// DF
	{ 12, 25 },	// DL
	{ 16, 52 },	// DB
	{ 14, 34 },	// DR
	{ 30, 41 },	// RF
	{ 23, 39 },	// LF
	{ 21, 50 },	// LB
	{ 32, 48 } };	// RB


FaceletCube::Edge FaceletCube::edgeMap[Cube::NumberOfEdgeCubies*2] = {
//   0   1
	UF, FU,
	UL, LU,
	UB, BU,
	UR, RU,
	DF, FD,
	DL, LD,
	DB, BD,
	DR, RD,
	RF, FR,
	LF, FL,
	LB, BL,
	RB, BR };


const char* FaceletCube::ErrorText(unsigned int error)
{
	if (error >= NumberOfErrors) error = 0;
	return errorText[error];
}

const char* FaceletCube::errorText[NumberOfErrors] = {
	"",
	"_Несоответствие центров_",
	"_Несоответствие граней_",
	"_Дублирование центров_",
	"_Несоответствие углов_",
	"_Ошибка четности углов_",
	"_Несоответствие вершин_",
	"_Ошибка четности вершин_",
	"_Ошибка четности куба_" };
