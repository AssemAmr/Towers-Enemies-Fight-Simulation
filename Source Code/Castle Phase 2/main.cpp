#include "GameCore.h"

int main()
{
	SetWindow();
	Castle ct; // define Castle 
	ct.Xstrt = CastleXStrt;
	ct.Ystrt = CastleYStrt;
	ct.W = CastleWidth;
	ct.L = CastleLength;
	int NA = 0, NB = 0, NC = 0, ND = 0;
	int DA = 0, DB = 0, DC = 0, DD = 0;
	int DAL = 0, DBL = 0, DCL = 0, DDL = 0;
	int EnemiesNumber = GetEnemiesNumber(NA, NB, NC, ND);
	Heap *ActiveA=CreateHeap(NA);
	Heap *ActiveB = CreateHeap(NB);
	Heap *ActiveC = CreateHeap(NC);
	Heap *ActiveD = CreateHeap(ND);
	Queue *Inactive = CreateQueue();
	DLL *Dead = CreateDLL();

	Constants C = { 0, 0, 0 };
	if (Read(Inactive, &ct, C))
	{
		int Mode;
		cout << "Please Choose Game Mode: \n1: Interactive Mode\n2: Step-by-step Mode\n3: Silent Mode\n";
		cin >> Mode;
		int Result = 0;
		for (int GameTime = 1; Result == 0; GameTime++)
		{
			Result = SimulateBattle(&ct, Inactive, Dead, ActiveA, ActiveB, ActiveC, ActiveD, GameTime, C, Mode, NA, NB, NC, ND, DA, DB, DC, DD, DAL, DBL, DCL, DDL);
			if (Mode == 1)
				_getch();
			else if (Mode == 2)
				Sleep(1000);
		}
		PrintFile(Dead, &ct,Result,EnemiesNumber);
	}
	else cout << "Since There is ( no enemy or we cant allocate memory for your enemies ), the game won't start" << endl;
	
	//Destroy ADTs
	DestroyQueue(Inactive);
	DestroyHeap(ActiveA);
	DestroyHeap(ActiveB);
	DestroyHeap(ActiveC);
	DestroyHeap(ActiveD);
	DestroyDLL(Dead);
	system("pause");
	return 0;
}
