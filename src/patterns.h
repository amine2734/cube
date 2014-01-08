
#ifndef _PATTERNS_H
#define _PATTERNS_H

const char *Animator::PatternMoves[] = {
/*1*/	"F2B2R2L2U2D2",
/*2*/	"R2L2U B L2D'F B2R L'F'B R D F2L'U'",
/*3*/	"F U F R L2B D'R D2L D'B R2L F U F ",
/*4*/	"F L F U'R U F2L2U'L'B D'B'L2U ",
/*5*/	"B2L2R2D B2F2L2R2D2U'F2L'D U'B F'D2U2L R'U'",
/*6*/	"U F B'L2U2L2F'B U2L2U ",
/*7*/	"F2R'B'U R'L F'L F'B D'R B L2",
/*8*/	"U D'R L'F B'U D'",
/*9*/	"B2D2L R'D2B2L R'",
/*0*/	"D'U B D'L'R F D'B'D'U L ",
/*1*/	"F D F'D2L'B'U L D R U L'F'U L U2",
/*2*/	"L U F2R L'U2B'U D B2L F B'R'L F'R ",
/*3*/	"U F2U'B'U2B U'F2U'R'U2B'U2B R ",
/*4*/	"B'L U'B D B'U B D'B'L'B ",
/*5*/	"F B'U F U F U L B L2B'U F'L U L'B "
};
	
		
const char *Animator::PatternNames[] = {
/*1*/	"Шахматы 1",
/*2*/	"Шахматы 2",
/*3*/	"Полосы",
/*4*/	"Куб в кубе",
/*5*/	"Глобус",
/*6*/	"Крест",
/*7*/	"Змея",
/*8*/	"Точки",
/*9*/	"Буквы \"Т\"",
/*0*/	"Буквы \"П\"",
/*1*/	"Кольца",
/*2*/	"Флаг",
/*3*/	"Рыбки",
/*4*/	"Уголки",
/*5*/	"Диагональ"
};

const int   Animator::PatternNum = 15;

#endif // _PATTERNS_H
