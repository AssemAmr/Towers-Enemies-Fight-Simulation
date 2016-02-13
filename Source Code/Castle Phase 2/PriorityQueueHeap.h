///////////////////////////////
//	       Structs
///////////////////////////////

struct Heap
{
	Enemy **HeapE;
	int Count;
};

Heap * CreateHeap(int Size);
void InsertEnemy(Heap *H, Enemy EnemyData);
void InsertEnemy(Heap *H, Enemy *EnemyData);
Enemy * RemoveEnemy(Heap *H);
void MergeHeaps(Heap * &H1, Heap * &H2 ,int &N1, int &N2, int Region);
void DestroyHeap(Heap * &H);