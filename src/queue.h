 
#include <cstring>

#define MOVE(x, y) ((x) << 3) + (y) + 2
#define FACE(x) ((x) >> 3)
#define NUM(y) (( (y) & 7) - 2 )

template <class T, int N>
class Queue
{
	public:
		struct QData
		{
			int    priority;
			T      data;
			QData* next;
			QData* prev;
		};
		
		Queue();
		~Queue();

		void Add(T* obj, int prior);
		void Add(T  obj);
		void Begin();
		void Begin(int to);
		T*   Get();
		T    Next();
		void Delete();
		void Reset();
		int  Item();
		int  Count();
		void MoveBack();
		int  GetLength() { return (Work) ? Length : 0; }
		void Block() { Work = !Work; }

	private:
		bool   Work;
		int    Length;
		int    CurPos;
		int    AimPos;
		int    MoveWay;
		QData* Current;
		QData  Root;
		QData* Table[N];
};
