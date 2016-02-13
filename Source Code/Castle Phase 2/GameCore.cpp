#include "GameCore.h"

//Print Enemy Data
void PrintEnemyData(Enemy *Enemy)
{
	cout << endl;
	cout << "ID: " << Enemy->ID << "    ";
	//cout << "Time: " << Enemy->EnemyData.Time_Step << "    ";
	cout << "Type: " << Enemy->Type << "    ";
	cout << "Health: " << Enemy->Health << "    ";
	cout << "Hit Power: " << Enemy->Hit_Power << "    ";
	cout << "Distance: " << Enemy->Distance;
	cout << "Priority: " << Enemy->Priority;
	cout << endl;
}

//Insert Enemies into Region Heaps
void PopulateActive(Queue* Inactive, Heap* ActiveA, Heap *ActiveB, Heap *ActiveC, Heap *ActiveD, int GameTime)
{
	//Populate Active Enemy Queue
	while (Inactive->Head != NULL && Inactive->Head->EnemyData.Time_Step == GameTime)
	{
		//Increment Number of Alive Enemies in each region
		if (Inactive->Head->EnemyData.Region == 1)
		{
			InsertEnemy(ActiveA, Inactive->Head->EnemyData);
			Dequeue(Inactive);
		}
		else if (Inactive->Head->EnemyData.Region == 2)
		{
			InsertEnemy(ActiveB, Inactive->Head->EnemyData);
			Dequeue(Inactive);
		}
		else if (Inactive->Head->EnemyData.Region == 3)
		{
			InsertEnemy(ActiveC, Inactive->Head->EnemyData);
			Dequeue(Inactive);
		}
		else if (Inactive->Head->EnemyData.Region == 4)
		{
			InsertEnemy(ActiveD, Inactive->Head->EnemyData);
			Dequeue(Inactive);
		}
	}
}

//Check if Enemy can attack in this Time Step
bool CanAttack(Enemy *E, int GameTime)
{
	if (GameTime%E->Hit_Period == E->Time_Step%E->Hit_Period)
		return true;

	return false;
}

//Check if Tower is Destroyed
bool IsTDestroyed(Castle *ct, int Region)
{
	if (ct->Towers[Region - 1].THealth <= 0)
		return true;

	return false;
}

//Check if Enemy Can Move
bool CanMove(Enemy *CurrentEnemy, Castle *ct)
{
	int NewDistance = CurrentEnemy->Distance - CurrentEnemy->Speed;
	if (NewDistance > ct->Towers[CurrentEnemy->Region - 1].LastUnpaved && NewDistance > 2)
		return true;

	return false;
}

//Update Enemy Properties
void UpdateEnemy(Enemy *CurrentEnemy, Castle *ct, Constants &C, int GameTime)
{
	//Update Enemy Distance if Can
	if (CanMove(CurrentEnemy, ct))
		CurrentEnemy->Distance -= CurrentEnemy->Speed;
	else if ((ct->Towers[CurrentEnemy->Region - 1].LastUnpaved + 1) > 2)
		CurrentEnemy->Distance = ct->Towers[CurrentEnemy->Region - 1].LastUnpaved + 1;
	
	//Update Enemy Damage to Tower
	CurrentEnemy->Damage_ET = (1.0 / CurrentEnemy->Distance)*CurrentEnemy->Hit_Power;

	//Calculate Enemy Time Remaining to Hit
	CurrentEnemy->TimeRemainingToAttack = CurrentEnemy->LastAttacked + CurrentEnemy->Hit_Period - GameTime;

	//Update Enemy Priority
	CurrentEnemy->Priority = (1.0 / CurrentEnemy->Distance)*CurrentEnemy->Hit_Power*(1.0 / (CurrentEnemy->TimeRemainingToAttack + 1))*C.C1
		+ (CurrentEnemy->Health*C.C2)
		+ (CurrentEnemy->Type*C.C3);
}

void AttackTower(Castle *ct, Enemy *CurrentEnemy)
{
	ct->Towers[CurrentEnemy->Region - 1].THealth -= CurrentEnemy->Damage_ET;
}

void PaveLand(Castle *ct, Enemy *CurrentEnemy)
{
	int NewUnpaved = CurrentEnemy->Distance - (int)CurrentEnemy->Hit_Power - 1;
	if (NewUnpaved < ct->Towers[CurrentEnemy->Region - 1].LastUnpaved)
	{
		if (NewUnpaved >= 2)
		{
			ct->Towers[CurrentEnemy->Region - 1].LastUnpaved = NewUnpaved;
		}
		else
		{
			ct->Towers[CurrentEnemy->Region - 1].LastUnpaved = 2;
		}
		CurrentEnemy->Distance = ct->Towers[CurrentEnemy->Region - 1].LastUnpaved + 1;
	}
}

void HitEnemies(Heap *Active, Castle *ct, int GameTime, Constants &C, DLL *Dead, int &DA, int &DB, int &DC, int &DD, int &DAL, int &DBL, int &DCL, int &DDL)
{
	if (Active->Count == 0)
		return;
	//Get Highest Priority Enemy

	Enemy *CurrentEnemy = RemoveEnemy(Active);

	//Update Tower Damage to Enemy
	CurrentEnemy->Damage_TE = (1.0 / CurrentEnemy->Distance)*ct->Towers[CurrentEnemy->Region - 1].TFPower*(1.0 / CurrentEnemy->K);

	//Update Enemy Health
	CurrentEnemy->Health -= CurrentEnemy->Damage_TE;

	if (CurrentEnemy->Time_First_Shot == -1)
	{
		CurrentEnemy->Time_First_Shot = GameTime;
		CurrentEnemy->Fight_Delay = CurrentEnemy->Time_First_Shot - CurrentEnemy->Time_Step;
	}

	//Check if Enemy is still Alive
	if (CurrentEnemy->Health > 0)
	{
		//Update Enemy Priority
		CurrentEnemy->Priority = (1.0 / CurrentEnemy->Distance)*CurrentEnemy->Hit_Power*(1.0 / (CurrentEnemy->TimeRemainingToAttack + 1))*C.C1
			+ (CurrentEnemy->Health*C.C2)
			+ (CurrentEnemy->Type*C.C3);

		//Insert it Again
		InsertEnemy(Active, CurrentEnemy);
	}
	else //If Enemy Died
	{
		CurrentEnemy->Time_Killed = GameTime;
		CurrentEnemy->Kill_Delay = CurrentEnemy->Time_Killed - CurrentEnemy->Time_First_Shot;
		CurrentEnemy->Fight_Time = CurrentEnemy->Time_Killed - CurrentEnemy->Time_Step;
		if (CurrentEnemy->Region == 1)
		{
			DA++;
			DAL++;
		}
		else if (CurrentEnemy->Region == 2)
		{
			DB++;
			DBL++;
		}
		else if (CurrentEnemy->Region == 3)
		{
			DC++;
			DCL++;
		}
		else if (CurrentEnemy->Region == 4)
		{
			DD++;
			DDL++;
		}
		InsertEndDLL(Dead, CurrentEnemy);
	}
}

int GetNextRegion(Castle *ct, int Region)
{
	if (ct->Towers[Region % 4].THealth > 0)
		return (Region % 4) + 1;
	else if (ct->Towers[(Region + 1) % 4].THealth > 0)
		return (Region + 1) % 4 + 1;
	else if (ct->Towers[(Region + 2) % 4].THealth > 0)
		return (Region + 2) % 4 + 1;
	return -1;
}


int MoveRegion(Heap * &Active, int Region, int &N, Castle *ct, Heap * &ActiveA, Heap * &ActiveB, Heap * &ActiveC, Heap * &ActiveD, int &NA, int &NB, int &NC, int &ND)
{
	int NewRegion = GetNextRegion(ct, Region);
	int OldCount = Active->Count;
	if (NewRegion == -1)
	{
		return -1;
	}
	else if (NewRegion == 1)
	{
		MergeHeaps(Active, ActiveA, N, NA, 1);
	}
	else if (NewRegion == 2)
	{
		MergeHeaps(Active, ActiveB, N, NB, 2);
	}
	else if (NewRegion == 3)
	{
		MergeHeaps(Active, ActiveC, N, NC, 3);
	}
	else if (NewRegion == 4)
	{
		MergeHeaps(Active, ActiveD, N, ND, 4);
	}
	return 1;
}

//Update Region Enemies
int UpdateRegion(Heap * &Active, Castle *ct, int Region, int &N, int GameTime, Constants &C, DLL *Dead, Heap * &ActiveA, Heap * &ActiveB, Heap * &ActiveC, Heap * &ActiveD, int &NA, int &NB, int &NC, int &ND, int &DA, int &DB, int &DC, int &DD, int &DAL, int &DBL, int &DCL, int &DDL)
{
	//Check if tower destroyed then move Enemies to next region if available
	if (IsTDestroyed(ct, Region)&& Active->Count!=0)
	{
		return MoveRegion(Active, Region, N, ct, ActiveA, ActiveB, ActiveC, ActiveD, NA, NB, NC, ND);
	}

	//Update Enemies and Hit Tower
	for (int i = 0; i < Active->Count; i++)
	{
		Enemy *CurrentEnemy = Active->HeapE[i];

		//Update Enemy Properties
		UpdateEnemy(CurrentEnemy, ct, C, GameTime);

		//Check if enemy can Attack or Pave then let it do it
		if (CanAttack(CurrentEnemy, GameTime))
		{
			if (CurrentEnemy->Type == 1)
				PaveLand(ct, CurrentEnemy);
			else
				AttackTower(ct, CurrentEnemy);
		}
	}

	//Let Tower Hit Enemies
	for (int i = 0; i < ct->Towers[0].TN; i++)
	{
		HitEnemies(Active, ct, GameTime, C, Dead, DA, DB, DC, DD, DAL, DBL, DCL, DDL);
	}
	return 1;
}

//Check if all Towers Destroyed
bool IsAllTowersDestroyed(Castle *ct)
{
	if (ct->Towers[0].TN <= 0 && ct->Towers[1].TN <= 0 && ct->Towers[2].TN <= 0 && ct->Towers[3].TN <= 0)
		return 1;

	return 0;
}

//Check if all Enemies Destroyed
bool IsAllEnemiesDestroyed(Queue *Inactive, Heap *ActiveA, Heap *ActiveB, Heap *ActiveC, Heap *ActiveD)
{
	if (Inactive->Count == 0 && ActiveA->Count == 0 && ActiveB->Count == 0 && ActiveC->Count == 0 && ActiveD->Count == 0)
		return 1;

	return 0;
}

//Update Enemies
int SimulateBattle(Castle *ct, Queue *Inactive, DLL *Dead, Heap * &ActiveA, Heap * &ActiveB, Heap * &ActiveC, Heap * &ActiveD, int GameTime, Constants &C, int Mode, int &NA, int &NB, int &NC, int &ND, int &DA, int &DB, int &DC, int &DD, int &DAL, int &DBL, int &DCL, int &DDL)
{
	PopulateActive(Inactive, ActiveA, ActiveB, ActiveC, ActiveD, GameTime);
	if (Mode != 3)
	{
		DrawCastle(*ct, GameTime);
		DrawEnemies(ActiveA->HeapE, ActiveA->Count);
		DrawEnemies(ActiveB->HeapE, ActiveB->Count);
		DrawEnemies(ActiveC->HeapE, ActiveC->Count);
		DrawEnemies(ActiveD->HeapE, ActiveD->Count);
		printf("Total Current Enemies\t\tA: %d\tB: %d\tC: %d\tD: %d\n", ActiveA->Count, ActiveB->Count, ActiveC->Count, ActiveD->Count);
		printf("Killed Enemies Last Time Step\tA: %d\tB: %d\tC: %d\tD: %d\n", DAL, DBL, DCL, DDL);
		printf("Total Killed Enemies\t\tA: %d\tB: %d\tC: %d\tD: %d\n", DA, DB, DC, DD);
		printf("Unpaved Distance\t\tA: %d\tB: %d\tC: %d\tD: %d\n", ct->Towers[0].LastUnpaved, ct->Towers[1].LastUnpaved, ct->Towers[2].LastUnpaved, ct->Towers[3].LastUnpaved);
	}
	//Check if someone Won
	if (IsAllTowersDestroyed(ct) && IsAllEnemiesDestroyed(Inactive, ActiveA, ActiveB, ActiveC, ActiveD))
	{
		return 2;
	}
	else if (IsAllTowersDestroyed(ct))
	{
		return -1;
	}
	else if (IsAllEnemiesDestroyed(Inactive, ActiveA, ActiveB, ActiveC, ActiveD))
	{
		return 1;
	}
	DAL = DBL = DCL = DDL = 0;//Reset Dead counters for each time step
	//Update Enemies of each Region
	//Region A
	if (UpdateRegion(ActiveA, ct, 1, NA, GameTime, C, Dead, ActiveA, ActiveB, ActiveC, ActiveD, NA, NB, NC, ND, DA, DB, DC, DD, DAL, DBL, DCL, DDL) == -1)
		return -1;
	//Region B
	if (UpdateRegion(ActiveB, ct, 2, NB, GameTime, C, Dead, ActiveA, ActiveB, ActiveC, ActiveD, NA, NB, NC, ND, DA, DB, DC, DD, DAL, DBL, DCL, DDL) == -1)
		return -1;
	//Region C
	if (UpdateRegion(ActiveC, ct, 3, NC, GameTime, C, Dead, ActiveA, ActiveB, ActiveC, ActiveD, NA, NB, NC, ND, DA, DB, DC, DD, DAL, DBL, DCL, DDL) == -1)
		return -1;
	//Region D
	if (UpdateRegion(ActiveD, ct, 4, ND, GameTime, C, Dead, ActiveA, ActiveB, ActiveC, ActiveD, NA, NB, NC, ND, DA, DB, DC, DD, DAL, DBL, DCL, DDL) == -1)
		return -1;

	return 0;
}