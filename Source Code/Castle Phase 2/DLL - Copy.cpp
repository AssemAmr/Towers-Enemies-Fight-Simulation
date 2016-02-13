#include "GameCore.h"

DLL* CreateDLL()
{
	DLL *D = new DLL;
	if (!D)
		return D;
	D->Count = 0;
	D->Head = NULL;
	D->Rear = NULL;
	return D;
}

DLLNode* GetNode(DLLNode* ptr1, DLLNode*ptr2)
{
	if (ptr1->Next == NULL || ptr2 == NULL || ptr1->EnemyData->Time_Killed  <  ptr2->EnemyData->Time_Killed)
		return ptr1;

	if (ptr1->EnemyData->Fight_Delay < ptr2->EnemyData->Fight_Delay)
		return GetNode(ptr1, ptr2->Next);

	return GetNode(ptr2, ptr2->Next);
}

void InsertEndDLL(DLL * &DLL, Enemy *E)
{
	DLL->Count++;
	DLLNode * NewNode = new DLLNode;
	NewNode->EnemyData = E;
	NewNode->Pre = DLL->Rear;
	NewNode->Next = NULL;

	if (DLL->Head == NULL)
	{
		DLL->Head = DLL->Rear = NewNode;
	}
	else
	{
		DLL->Rear->Next = NewNode;
		DLL->Rear = NewNode;
	}
}

void RemoveDLL(DLLNode *N, DLL * &DLL)
{
	if (DLL->Count == 0)
		return;

	if (DLL->Count == 1)
	{
		DLL->Head = DLL->Rear = NULL;
		DLL->Count--;
		delete N;
		return;
	}

	DLL->Count--;

	if (N->Pre == NULL)
	{
		N->Next->Pre = NULL;
		DLL->Head = N->Next;
	}
	else if (N->Next == NULL)
	{
		N->Pre->Next = NULL;
		DLL->Rear = N->Pre;
	}
	else
	{
		N->Pre->Next = N->Next;
		N->Next->Pre = N->Pre;
	}

	delete N;
}

void PrintFile(DLL * &D, Castle *ct, int Result, int OriginalNumber)
{
	int DeadCount = D->Count;
	double TotalFD = 0;
	double TotalKD = 0;
	while (D->Count != 0)
	{
		DLLNode *Temp = GetNode(D->Head, D->Head->Next);

		TotalFD += Temp->EnemyData->Fight_Delay;
		TotalKD += Temp->EnemyData->Kill_Delay;

		ofstream Out("Out.txt", ifstream::app);
		Out << Temp->EnemyData->Time_Killed << "  ";
		Out << Temp->EnemyData->ID << "  ";
		Out << Temp->EnemyData->Fight_Delay << "  ";
		Out << Temp->EnemyData->Kill_Delay << "  ";
		Out << Temp->EnemyData->Fight_Time << endl;
		Out.close();
		RemoveDLL(Temp, D);
	}
	ofstream Out("Out.txt", ifstream::app);
	for (int i = 0; i < 4; i++)
	{
		Out << ct->Towers[i].THealthSaver - ct->Towers[i].THealth << "   ";
	}
	Out << endl;
	for (int i = 0; i < 4; i++)
	{
		Out << ct->Towers[i].LastUnpaved << "   ";
	}
	Out << endl;
	if (Result == 1)
	{
		Out << "WIN!" << endl;
		Out << "Total Number of Enemies: " << OriginalNumber << endl;
		Out << "Average Fight Delay: " << TotalFD / OriginalNumber << endl;
		Out << "Average Kill Delay: " << TotalKD / OriginalNumber << endl;
	}
	else
	{
		Out << "LOSE!" << endl;
		Out << "Number of Killed Enemies: " << DeadCount << endl;
		Out << "Number of Alive Enemies: " << OriginalNumber - DeadCount<<endl;
		Out << "Average Fight Delay: " << TotalFD / DeadCount << endl;
		Out << "Average Kill Delay: " << TotalKD / DeadCount << endl;
	}
	Out.close();
}

void DestroyDLL(DLL * &DLL)
{
	while (DLL->Head != NULL)
	{
		RemoveDLL(DLL->Head, DLL);
	}
	delete DLL;
	DLL = NULL;
}