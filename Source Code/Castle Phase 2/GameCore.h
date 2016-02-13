#include <iostream>
#include <fstream>
#include <conio.h>
#include "utility.h"
#include "Queue.h"
#include "PriorityQueueHeap.h"
#include "Read.h"
#include "DLL.h"

//Move Enemies from the Inactive Queue to their Region's Heap
void PopulateActive(Queue* Inactive, Heap *ActiveA, Heap *ActiveB, Heap *ActiveC, Heap *ActiveD, int GameTime);

//Check if Enemy can attack in this Time Step
bool CanAttack(Enemy *E, int GameTime);

//Check if Tower is Destroyed
bool IsTDestroyed(Castle *ct, int Region);

//Update Enemy Properties
void UpdateEnemy(Enemy *CurrentEnemy, Castle *ct, Constants &C, int GameTime);

//Attack Tower in the Region of an Enemy
void AttackTower(Castle *ct, Enemy *CurrentEnemy);

void HitEnemies(Heap *Active, Castle *ct, int GameTime, Constants &C, DLL *Dead);

//Update Region Enemies
int UpdateRegion(Heap * &Active, Castle *ct, int Region, int &N, int GameTime, Constants &C, DLL *Dead, Heap * &ActiveA, Heap * &ActiveB, Heap * &ActiveC, Heap * &ActiveD, int &NA, int &NB, int &NC, int &ND);

//Check if all Towers Destroyed
bool IsAllTowersDestroyed(Castle *ct);

//Check if all Enemies Destroyed
bool IsAllEnemiesDestroyed(Queue *Inactive, Heap *ActiveA, Heap *ActiveB, Heap *ActiveC, Heap *ActiveD);

//Update Enemies
int SimulateBattle(Castle *ct, Queue *Inactive, DLL *Dead, Heap * &ActiveA, Heap * &ActiveB, Heap * &ActiveC, Heap * &ActiveD, int GameTime, Constants &C, int Mode, int &NA, int &NB, int &NC, int &ND, int &DA, int &DB, int &DC, int &DD, int &DAL, int &DBL, int &DCL, int &DDL);