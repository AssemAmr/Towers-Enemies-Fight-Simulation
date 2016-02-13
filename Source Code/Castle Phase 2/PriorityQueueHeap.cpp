#include "GameCore.h"

Heap * CreateHeap(int Size)
{
	Heap *H = new Heap;
	H->HeapE = new Enemy*[Size];
	H->Count = 0;
	return H;
}

/*
void InsertEnemy(Heap *H, Enemy EnemyData)
{
	 If we take enemy is value
	
	//Create new Enemy and Insert at end of Heap
	H->HeapE[H->Count] = new Enemy;
	H->HeapE[H->Count]->Distance = EnemyData.Distance;
	H->HeapE[H->Count]->Health = EnemyData.Health;
	H->HeapE[H->Count]->Hit_Period = EnemyData.Hit_Period;
	H->HeapE[H->Count]->Hit_Power = EnemyData.Hit_Power;
	H->HeapE[H->Count]->ID = EnemyData.ID;
	H->HeapE[H->Count]->K = EnemyData.K;
	H->HeapE[H->Count]->Priority = EnemyData.Priority;
	H->HeapE[H->Count]->Region = EnemyData.Region;
	H->HeapE[H->Count]->Speed = EnemyData.Speed;
	H->HeapE[H->Count]->Time_Step = EnemyData.Time_Step;
	H->HeapE[H->Count]->Type = EnemyData.Type;
	
	
	H->Count++;

	//Move New Enemy to its right position
	int EPos = H->Count - 1;
	while (true)
	{
		if (EPos == 0)
			break;
		int ParentPos = (EPos - 1) / 2;
		if (H->HeapE[EPos]->Priority > H->HeapE[ParentPos]->Priority)
		{
			Enemy *Temp = H->HeapE[EPos];
			H->HeapE[EPos] = H->HeapE[ParentPos];
			H->HeapE[ParentPos] = Temp;
			EPos = ParentPos;
		}
		else
			break;
	}
}
*/

void InsertEnemy(Heap *H, Enemy EnemyData)
{
	//Create New Enemy
	Enemy *NewEnemy = new Enemy;
	NewEnemy->Damage_ET = EnemyData.Damage_ET;
	NewEnemy->Damage_TE = EnemyData.Damage_TE;
	NewEnemy->Distance = EnemyData.Distance;
	NewEnemy->Fight_Delay = EnemyData.Fight_Delay;
	NewEnemy->Fight_Time = EnemyData.Fight_Time;
	NewEnemy->Health = EnemyData.Health;
	NewEnemy->Hit_Period = EnemyData.Hit_Period;
	NewEnemy->Hit_Power = EnemyData.Hit_Power;
	NewEnemy->ID = EnemyData.ID;
	NewEnemy->K = EnemyData.K;
	NewEnemy->Kill_Delay = EnemyData.K;
	NewEnemy->LastAttacked = EnemyData.LastAttacked;
	NewEnemy->Priority = EnemyData.Priority;
	NewEnemy->Region = EnemyData.Region;
	NewEnemy->Speed = EnemyData.Speed;
	NewEnemy->TimeRemainingToAttack = EnemyData.TimeRemainingToAttack;
	NewEnemy->Time_First_Shot = EnemyData.Time_First_Shot;
	NewEnemy->Time_Killed = EnemyData.Time_Killed;
	NewEnemy->Time_Step = EnemyData.Time_Step;
	NewEnemy->Type = EnemyData.Type;

	//Get position of Insertion
	int EPos = H->Count;
	while (EPos > 0 && EnemyData.Priority > H->HeapE[(EPos - 1) / 2]->Priority)
	{
		H->HeapE[EPos] = H->HeapE[(EPos - 1) / 2];
		EPos = (EPos - 1) / 2;
	}

	//Insert Enemy & Increment Heap Count
	H->HeapE[EPos] = NewEnemy;
	H->Count++;
}

//Overloaded version that takes pointer to Enemy as a parameter
void InsertEnemy(Heap *H, Enemy *EnemyData)
{
	//Get position of Insertion
	int EPos = H->Count;
	while (EPos > 0 && EnemyData->Priority > H->HeapE[(EPos - 1) / 2]->Priority)
	{
		H->HeapE[EPos] = H->HeapE[(EPos - 1) / 2];
		EPos = (EPos - 1) / 2;
	}

	//Insert Enemy & Increment Heap Count5
	H->HeapE[EPos] = EnemyData;
	H->Count++;
}

void Heapify(Heap *H, int EnemyPosition)
{
	int EPos = EnemyPosition;
	while (1)
	{
		int Child1 = 2 * EPos + 1;
		int Child2 = 2 * EPos + 2;
		
		if (Child1 >= H->Count)
			break;

		int BiggestChild;
		if (Child2 >= H->Count)
			BiggestChild = Child1;
		else if (H->HeapE[Child1]->Priority >= H->HeapE[Child2]->Priority)
			BiggestChild = Child1;
		else
			BiggestChild = Child2;

		if (H->HeapE[EPos]->Priority >= H->HeapE[BiggestChild]->Priority)
			break;

		Enemy *Temp = H->HeapE[EPos];
		H->HeapE[EPos] = H->HeapE[BiggestChild];
		H->HeapE[BiggestChild] = Temp;

		EPos = BiggestChild;
	}
}

Enemy * RemoveEnemy(Heap *H)
{
	//Get Highest Priority Enemy
	Enemy *EHigestPriority = H->HeapE[0];
	
	//Move Enemy at End of of the Heap to the beginning
	H->HeapE[0] = H->HeapE[H->Count - 1];
	H->HeapE[H->Count - 1] = NULL;
	H->Count--;

	//Heapify the Heap
	Heapify(H, 0);

	//Decrement Count

	return EHigestPriority;
}

void MergeHeaps(Heap * &H1, Heap * &H2, int &N1 ,int &N2, int Region)
{
	int NewSize = N1 + N2;
	N1 = 0;
	N2 = NewSize;
	Heap *NewHeap = CreateHeap(NewSize);
	while (H1->Count>0)
	{
		Enemy *Temp = RemoveEnemy(H1);
		Temp->Region = Region;
		InsertEnemy(NewHeap, Temp);
	}
	while (H2->Count>0)
	{
		Enemy *Temp = RemoveEnemy(H2);
		InsertEnemy(NewHeap, Temp);
	}
	delete H2->HeapE;
	delete H2;
	H2 = NewHeap;
}

void DestroyHeap(Heap * &H)
{
	
	for (int i = 0; i < H->Count; i++)
	{
		delete H->HeapE[i];
	}
	delete H->HeapE;
	delete H;
	H = NULL;
}