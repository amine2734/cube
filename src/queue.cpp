
#ifndef _QUEUE_CPP
#define _QUEUE_CPP
 
#include "queue.h"
#include <iostream>

template <class T, int N>
Queue<T, N>::Queue()
{
	Current = 0;
	Length = CurPos = AimPos = 0;
	MoveWay = 1;
	Work = true;
	
	Root.next = Root.prev = &Root;
	Root.priority = 1;

	for(int i = 0; i < N; ++i)
	{
		Table[i] = new QData;
		Table[i]->priority = 0;
	}
}

template <class T, int N>
Queue<T, N>::~Queue()
{
	Reset();
	for(int i = 0; i < N; ++i)
		delete Table[i];
}

template <class T, int N>
void Queue<T, N>::Reset()
{
	CurPos = AimPos = 0;
	Begin();
	while (Get())
		Delete();
}

template <class T, int N>
void Queue<T, N>::Add(T* obj, int prior)
{
	int    found = -1;
	QData  *t, *next;
	
	for (int i = 0; i < N; ++i)
		if ( ! (Table[i]->priority & 1) )
		{
			found = i;
			break;
		}

	if (found != -1)
	{
		t = Table[found];
		t->priority = 1;
	}
	else
	{
		t = new QData;
		t->priority = 0;
	}

	t->priority += ++prior << 1;
	memcpy(&t->data, obj, sizeof(T));

	next = Root.next;
	while (next != &Root && (next->priority >> 1) < prior)
		next = next->next;

	t->next = next;
	t->prev = next->prev;
	t->prev->next = t;
	next->prev = t;

	Length++;
}

template <class T, int N>
void Queue<T, N>::Add(T obj)
{
	Add(&obj, 1);
}

template <class T, int N>
void Queue<T, N>::Begin()
{
	Current = &Root;
}

template <class T, int N>
void Queue<T, N>::Begin(int to)
{
	AimPos = (to > Length || to < 1) ? 0 : to;
	MoveWay = (AimPos > CurPos) ? 1 : -1;
	if(!CurPos)
		Current = Root.prev;
}

template <class T, int N>
void Queue<T, N>::Delete()
{
	QData* del = Current;
	Current = del->next;
	Current->prev = del->prev;
	del->prev->next = Current;

	if (del->priority & 1)
	{
		for (int i = 0; i < N; ++i)
			if (Table[i] == del)
			{
				del->priority = 0;
				break;
			}
	}
	else
		delete del;

	Length--;
}

template <class T, int N>
T* Queue<T, N>::Get()
{
	if (Current->prev != &Root)
	{
		Current = Current->prev;
		return &Current->data;
	}
	else
		return 0;
}

template <class T, int N>
T Queue<T, N>::Next()
{
	if(AimPos > CurPos)
	{
		Current = Current->prev;
		CurPos++;
		return Current->next->data;
	}
	else if(AimPos < CurPos)
	{
		Current = Current->next;
		CurPos--;
		
		int conv = (Current->data & 7) - 2;
		return ((Current->data >> 3) << 3) - conv + 2;
	}
	else
		return 0;
}

template <class T, int N>
int Queue<T, N>::Item()
{
	return CurPos;
}

template <class T, int N>
int Queue<T, N>::Count()
{
	return abs(AimPos - CurPos);
}

template <class T, int N>
void Queue<T, N>::MoveBack()
{
	if(MoveWay == 1)
	{
		Current = Current->next;
		CurPos--;
	}
	else
	{
		Current = Current->prev;
		CurPos++;
	}
}

#endif // _QUEUE_CPP
