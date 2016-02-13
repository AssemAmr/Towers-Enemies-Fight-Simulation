//Define Node Strucutres
struct Node
{
	Enemy EnemyData;
	Node *Next;
};

//Define Queue Structure
struct Queue
{
	Node *Head;
	Node *Rear;
	int Count;
};

Queue* CreateQueue();
bool Enqueue(Queue* L, Enemy EnemyData);
bool Dequeue(Queue* L);
bool DestroyQueue(Queue * & L);
void PrepareDraw(Queue* Active, Enemy **E);