
#ifndef _FACEMAP_H
#define _FACEMAP_H

class FaceletMap
{
	public:
  		FaceletMap() { Reset(); }

		void Reset();
		void ResetToGray();
		void Rotate(char m, int rot);
		void SaveState();
		bool IsSolved();
		void MapToString(char *str);
		void StringToMap(const char *str);
		void RenderScreen();
		int& Color(int x, int y, int z);
		const char* Validate();
		
		void UL();
		void UR();
		void DL();
		void DR();
		void LU();
		void LD();
		void RU();
		void RD();
		void FC();
		void FA();
		void BC();
		void BA();

		int  Map[5][5][5];
		char SavedState[55];

	protected:
		void FillTemp();
		
		bool XML(int a);
		bool XMR(int a);
		bool XMU(int a);
		bool XMD(int a);
		bool XMC(int a);
		bool XMA(int a);

	private:
  		int  Temp[5][5][5];
};


#endif // _FACEMAP_H
