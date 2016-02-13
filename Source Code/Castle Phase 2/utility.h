#include <iostream>
#include <windows.h>

using namespace std;


#define CmdWidth		150
#define CmdHeight		50
#define CastleWidth		30
#define CastleLength	20
#define CastleXStrt		(CmdWidth/2-(CastleWidth/2))
#define CastleYStrt		(CmdHeight/2-(CastleLength/2))
#define TowerWidth      7
#define TowerLength     3


// Some Datatypes definations
struct Tower
{
	int TW; //Tower width
	int TL; //Tower Height
	double THealth;
	int TN;
	double TFPower;
	int LastUnpaved;
	int THealthSaver;
};

struct Castle
{
	//starting x,y
	int Xstrt;
	int Ystrt;
	int W;	//width
	int L;  //Height
	Tower Towers[4];
};

struct Enemy
{

	int Type;
	int Time_Step;
	double Health;
	double Hit_Power;
	int Hit_Period;
	int ID;
	int Distance;
	int Region;
	double Priority;
	double Damage_ET;
	double Damage_TE;
	int K;
	int LastAttacked;
	int TimeRemainingToAttack;
	int Time_First_Shot;
	int Time_Killed;
	int Fight_Delay;
	int Kill_Delay;
	int Fight_Time;
	//Bonus
	int Speed;

};

struct Constants
{
	double C1;
	double C2;
	double C3;
};


/*A function to set the position of cursor on the screen*/
void gotoxy(int x, int y);
/*A function to set the command window lenght and height for the game specification*/
void SetWindow();
/*A function to color the cmd text*/
void color(int thecolor);
/*A function to partition the castle into regions (A,B,C,D)*/
void DrawRegions(const Castle & C);
/*A function to draw the castle and the towers*/
void DrawCastle(const Castle & C,int SimulationTime);
/*A function to draw a single enemy using its distance from the castle*/
void DrawEnemy(const Enemy& E, int Ypos=0);
/*A function to draw a list of enemies exist in all regions and ensure there is no overflow in the drawing*/
void DrawEnemies(Enemy* enemies[],int size);
/*Student use this function to print his/her message*/
void PrintMsg(char*msg);

