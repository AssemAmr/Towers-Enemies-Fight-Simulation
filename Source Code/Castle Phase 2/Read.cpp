#include "GameCore.h"

using namespace std;


int GetEnemiesNumber(int &NA, int &NB, int &NC, int &ND)
{
	ifstream Input("input.txt");
	int N = 0;
	double t;
	char r;
	Input >> t >> t >> t;
	Input >> t >> t >> t;
	Input >> t;
	while (t != -1)
	{
		////Read in temp variable
		Input >> t;
		Input >> t;
		Input >> t;
		Input >> t;
		Input >> t;
		Input >> r;
		Input >> t;
		Input >> t;
		N++;
		if (r == 'A')
			NA++;
		else if (r == 'B')
			NB++;
		else if (r == 'C')
			NC++;
		else
			ND++;
	}
	Input.close();
	return N;
}

bool Read(Queue* Inactive, Castle* c, Constants &C)
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Check if Queue and Castle exist
	if (c == NULL || Inactive == NULL)
	{
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Read from file
	ifstream Input("input.txt");
	///////////////////////////////////////////////////////////////////////////////////////////////////////	




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Declare variables for Tower
	int _THealth, _TN, _TFPower;
	///////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Read and Save Tower Data
	////Read Tower Data
	Input >> _THealth >> _TN >> _TFPower;
	////Save Tower Data
	//////Save Health
	c->Towers[0].THealth = c->Towers[1].THealth = c->Towers[2].THealth = c->Towers[3].THealth = _THealth;
	c->Towers[0].THealthSaver = c->Towers[1].THealthSaver = c->Towers[2].THealthSaver = c->Towers[3].THealthSaver = _THealth;
	//////Save Number of enemies hit
	c->Towers[0].TN = c->Towers[1].TN = c->Towers[2].TN = c->Towers[3].TN = _TN;
	//////Save Tower Fire Power
	c->Towers[0].TFPower = c->Towers[1].TFPower = c->Towers[2].TFPower = c->Towers[3].TFPower = _TFPower;
	//////Update rest of tower data
	c->Towers[0].TL = c->Towers[1].TL = c->Towers[2].TL = c->Towers[3].TL = TowerLength; //Preprocessing defined constant
	c->Towers[0].TW = c->Towers[1].TW = c->Towers[2].TW = c->Towers[3].TW = TowerWidth; //Preprocessing defined constant
	c->Towers[0].LastUnpaved = c->Towers[1].LastUnpaved = c->Towers[2].LastUnpaved = c->Towers[3].LastUnpaved = 30;
	///////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Read values of Constants " passed by reference "
	Input >> C.C1 >> C.C2 >> C.C3;
	///////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Read and Save enemy data
	Enemy TempEnemy;
	char R; // To read Region as Character
	TempEnemy.Priority = -1; //Set Priority initially for all Enemies
	TempEnemy.Distance = 60; //Set Distance initially for all Enemies
	TempEnemy.Time_First_Shot = -1;
	TempEnemy.Time_Killed = -1;
	TempEnemy.Fight_Delay = -1;
	TempEnemy.Kill_Delay = -1;
	TempEnemy.Fight_Time = -1;
	TempEnemy.Damage_ET = 0;
	TempEnemy.Damage_TE = 0;
	Input >> TempEnemy.ID; //Read Enemy ID for the first time
	while (TempEnemy.ID != -1)
	{
		////Read in temp variable
		Input >> TempEnemy.Time_Step;
		Input >> TempEnemy.Health;
		Input >> TempEnemy.Hit_Power;
		Input >> TempEnemy.Hit_Period;
		Input >> TempEnemy.Type;

		////Last Attackd initialized as Time_Step
		TempEnemy.LastAttacked = TempEnemy.Time_Step;
		////To read as a character
		Input >> R;
		TempEnemy.Region = R - 'A' + 1;
		////

		Input >> TempEnemy.Speed; //Bonus

		if (TempEnemy.Type == 3)
			TempEnemy.K = 2;
		else
			TempEnemy.K = 1;

		//Add Enemy Node to Inactive
		Enqueue(Inactive, TempEnemy);

		Input >> TempEnemy.ID;
	}


	Input.close();
	if (Inactive->Head == NULL)
		return false;
	return true;
}