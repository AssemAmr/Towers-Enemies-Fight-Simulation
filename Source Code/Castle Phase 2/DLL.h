#include <iostream>

using namespace std;

struct DLLNode
{
	Enemy *EnemyData;
	DLLNode *Next;
	DLLNode *Pre;
};

struct DLL
{
	DLLNode *Head;
	DLLNode *Rear;
	int Count;
};

DLL* CreateDLL();

DLLNode* GetNode(DLLNode* ptr1, DLLNode*ptr2);

void InsertEndDLL(DLL * &DLL, Enemy *E);

void RemoveDLL(DLLNode *N, DLL * &DLL);

void PrintFile(DLL * &D, Castle *ct, int Result, int OriginalNumber);

void DestroyDLL(DLL * &DLL);