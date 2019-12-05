// GS3.1 RPG Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Author: Trail Sammarco
//Project: RPG Project
//File Name: GS3.1 RPG Project.cpp
//Purpose: continue makeing the rpg
//explanation: add names and randomly generated health and damage
//maintenance log: 
//9-12-19, wrote introduction,
//9-21-19, wrote naming system and health pool randomization
//9-24-19, wrote damage randomization
//10-7-19, worked on loop branch
//10-9-19, wroked on loop branch
//10-10-19, worked on navigation system
//10-21-19, worked on combat function
//11-4-19, work on combat function
//11-5-19, work on random encounter assignment and navigation with encounters
//11-7-19, structs!
//12-4-19, integrated combat function from seperate testing file

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

enum turn { ship, eng, sci, pil };
enum action { attack, evade, special, escape };

struct roomInfo //room struct will store location and encounter. door state will be stored in the door arrays
{
	string name;
	int encounterID = 0;//npc, enemy, or encounter in this room//is this legal?
};
struct point
{
	int X, Y;
};
struct crew
{
	string name;
	int HP, HPmax, wep, atk;
	bool dead;
	action act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;//damage multiplier
};
struct vessel
{
	string name;
	int HP, HPmax;
	action act;
};
struct party
{
	crew eng, sci, pil;
	vessel ship;
};
struct enemy
{
	string name;
	int HP, HPmax, atk;
	bool dead = true;
	action act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;
	int ID;
};
struct eparty
{
	enemy e1, e2, e3;
	int a;//number of enemies in the party that need to be displayed
	vessel ship;
	int credits;
	int graphicID;
};
//crew and ship stat structs must contain a multiplier value from the scientist weapons
//make a party struct that consists of all crew structs

void displayMap(bool(&VdoorArray)[4][5], bool(&HdoorArray)[5][4], int C, int R);
int _dice(int max);
bool encounter(party &player, int encounterID, int &credits);
int roundUp(int number, int devisor);
void printHbar(int HP, int HPmax, int length);
void printHealthWindow(party& player, eparty& enemy);//player, enemy, number of enemies in eparty
char _getdir();
void killAndTop(party& player, eparty& enemyParty);
bool combat(party& player, int enemyID, int& credits);

void color(int a);
void blocks(int a);
void pixelArtRelay(int id);
void printSpaceGoblin();

void spaceGoblinAI(party& player, eparty& enemyParty, int a, int turnNum);
void spaceGoblinActions(party& player, eparty& enemyParty, int e);

int main()
{
	srand((unsigned)time(NULL));
	printf("Hello!\nMy name is Trail Sammarco, the creator of this game.\nMiyoshi says that introductions should be written last so that's all I'll say about that.\n\n");
	printf("You are in control of a group of 3 cosmonauts.\n");
	printf("After a terrible accident they are stranded in a dangerous region of the galaxy thousands of lightyears from home.\n");
	printf("They will need to fight through the galaxy's most dangerous foes to get them home and find out the truth about what\nhappened in...\n\n");
	_getch();
	printf("Sp_ce\n\n");
	party player;
	printf("your first crew member is the engineer\nWhat is their name?\n");
	getline(cin, player.eng.name);
	printf("your second crew member is the scientist\nWhat is their name?\n");
	getline(cin, player.sci.name);
	printf("the last crew member is the pilot\nWhat is their name?\n");
	getline(cin, player.pil.name);
	printf("generating random health pools...\n");
	player.eng.wep = 0;
	player.sci.wep = 0;
	player.pil.wep = 0;
	int crewMin = 9; //using variables in case I want to add difficulty modes later
	int crewMax = 15;
	player.eng.HP = crewMin + rand() % (crewMax - crewMin + 1) + 3; //engi has more HP, sci has less
	player.sci.HP = crewMin + rand() % (crewMax - crewMin + 1) - 2;
	player.pil.HP = crewMin + rand() % (crewMax - crewMin + 1);
	player.eng.HPmax = player.eng.HP;
	player.eng.dead = false;
	player.sci.HPmax = player.sci.HP;
	player.sci.dead = false;
	player.pil.HPmax = player.pil.HP;
	player.pil.dead = false;
	int shipMin = 15;
	int shipMax = 25;
	player.ship.HP = 15 + rand() % (25 - 15 + 1);
	player.ship.HPmax = player.ship.HP;
	printf("%s the engineer: %i HP\n", player.eng.name.c_str(), player.eng.HP);
	printf("%s the scientist: %i HP\n", player.sci.name.c_str(), player.sci.HP);
	printf("%s the pilot: %i HP\n", player.pil.name.c_str(), player.pil.HP);

	printf("%s, %s, and %s have found themselves stranded on an abandoned asteroid mining station\n", player.eng.name.c_str(), player.sci.name.c_str(), player.pil.name.c_str());
	printf("they find an old mining vesslewith a bit of fuel still in the tank\n");
	printf("what do they name the ship?\n");
	getline(cin, player.ship.name);
	printf("after %s does some repairs, %s is ready to fly\n", player.eng.name.c_str(), player.ship.name.c_str());
	printf("%s has %i HP\n[press any key to continue]\n", player.ship.name.c_str(), player.ship.HP);
	_getch();
	system("cls");
	printf("\nthere are some items left in the lockers that the miners left behind\n");
	printf("you crew scavenges what they can\n");
	char inputc;
	bool exit = false;
	bool b = 0;
	//equipment menue
	while (!exit) 
	{
		if (b == 1)
		{
			system("cls");
		}
		b = 1;
		printf("which character would you like to equip?\n");
		printf("a. %s\n", player.eng.name.c_str());
		printf("b. %s\n", player.sci.name.c_str());
		printf("c. %s\n", player.pil.name.c_str());
		printf("d. done equiping\n");
		cin >> inputc;
		fseek(stdin, 0, SEEK_END);
		switch (inputc) //not sure why i used a switch here but it's too much work to change it
		{
		case 'a':
			/*
			monkey wrench - better attack, worse repair rate
			pipe wrench - better ship repair, worse attack
			grenade pouch - masive damage but no manual repairs (must find oportunities to fix your ship along the way)
			*/
			system("cls");
			while (true)
			{
				system("cls");
				printf("%s finds three items:\n", player.eng.name.c_str());
				printf("a. monkey wrench\n");
				printf("b. pipe wrench\n");
				printf("c. grenade belt\n");
				printf("d. go back\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)//pick monkey wrench
				{
					while (true)//monkey wrench menue
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the monkey wrench\n");
						printf("b. inspect the monkey wrench\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the monkey wrench\n", player.eng.name.c_str());
							player.eng.wep = 1;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("Not too versitile, but it gets the job done.\nA solid impliment, both for fixing and head bashing\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('b' == inputc)//pick pipe wrench
				{
					while (true)
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the pipe wrench\n");
						printf("b. inspect the pipe wrench\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the pipe wrench\n", player.eng.name.c_str());
							player.eng.wep = 2;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A wrench with an adjustable mouth.\nFine tuned for mechanics, not massacre\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('c' == inputc)
				{
					while (true)
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the grenade belt\n");
						printf("b. inspect the grenade belt\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the grenade belt\n", player.eng.name.c_str());
							player.eng.wep = 3;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("High explosives arranged in an easily accessable fasion.\nHopefully the ship won't need any repairs...\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('d' == inputc)
				{
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
				}
				
			}
			//exit = false;//review-----------------------------
			break;
		case 'b':
			/*
			ray gun - high damage, less ship stat multipliers
			data pad - minimal damage, great ship stat multipliers, Lots of healing
			head set - increase crew stat multipliers
			*/
			system("cls");
			while (true)
			{
				system("cls");
				printf("%s finds three items:\n", player.sci.name.c_str());
				printf("a. ray gun\n");
				printf("b. data pad\n");
				printf("c. head set\n");
				printf("d. go back\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					while (true)//ray gun menue
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the ray gun\n");
						printf("b. inspect the ray gun\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the ray gun\n", player.sci.name.c_str());
							player.sci.wep = 1;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A device that emits a solid beam of particles.\nA direct, yet sophistocated solution to a problem.\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('b' == inputc)
				{
					while (true)//data pad menue
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the data pad\n");
						printf("b. inspect the data pad\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the data pad\n", player.sci.name.c_str());
							player.sci.wep = 1;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A flat screen that can be held in the hand, continuously displaying information and diagnostics.\n");
							printf("Great for displaying medical diagnostics.\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('c' == inputc)
				{
					while (true)//head set menue
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the head set\n");
						printf("b. inspect the head set\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the head set\n", player.sci.name.c_str());
							player.sci.wep = 1;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A head set and microphone with matching ear pieces for each crew member.\n");
							printf("Communication is vital to an efficient team.\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('d' == inputc)
				{
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
				}
			}
			break;
		case 'c':
			/*
			goggles - ship buffs ---- and more damage
			trench coat - party buffs ---- and less damage
			Hyperdrive turbo - double jump, recharges after 2 jumps
			*/
			system("cls");
			while (true)
			{
				system("cls");
				printf("%s finds three items:\n", player.eng.name.c_str());
				printf("a. flight goggles\n");
				printf("b. trench coat\n");
				printf("c. Hyperdrive Turbo\n");
				printf("d. go back\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)//pick flight goggles
				{
					while (true)//flight goggles
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the flight goggle\n");
						printf("b. inspect the flight goggles\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the flight goggles\n", player.pil.name.c_str());
							player.pil.wep = 1;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("Protective goggles made for a pilot.\nWearing them makes you feel like a champ, and that's what counts.\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('b' == inputc)//pick trench coat
				{
					while (true)//trench coat menue
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the trench coat\n");
						printf("b. inspect the trench coat\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equip the trench coat\n", player.pil.name.c_str());
							player.pil.wep = 2;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A thick leather coat that goes down to your ankles and makes you feel like a real MBOC.\nShurely you crew mates will be impressed.\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('c' == inputc)//Hyperdrive Turbo
				{
					while (true)//Hyperdrive Turbo
					{
						system("cls");
						printf("would you like to:\n");
						printf("a. equip the Hyperdrive Turbo\n");
						printf("b. inspect the Hyperdrive Turbo\n");
						printf("c. go back\n");
						cin >> inputc;
						fseek(stdin, 0, SEEK_END);
						if ('a' == inputc)
						{
							printf("%s equips the Hyperdrive Turbo\n", player.pil.name.c_str());
							player.pil.wep = 3;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A part that can be equiped on the ship's hyperdrive, letting you make a second jump that recharges after 2 jumps.\n");
							printf("While it doesn't look all that cool, you'll be zip'n around the galaxy real quick.\n");
							printf("[continue]\n");
							_getch();
						}
						else if ('c' == inputc)
						{
							break;
						}
						else
						{
							printf("[invalid input]\n");
							_getch();
						}
					}
				}
				else if ('d' == inputc)
				{
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
				}

			}
			exit = false;
			break;
		case 'd':
			exit = true;
			break;
		default:
			printf("invalid input\n");
			break;
		}
	}
	printf("With the ship fully operational and your party fully equiped, the crew sets off into the great unknown...\n[press any key to continue]");
	_getch();
	system("cls");
	printf("input seed number:");
	int intIn;
	cin >> intIn;
	fseek(stdin, 0, SEEK_END);
	printf("generating map...\n");
	srand(intIn);
	exit = false;
	roomInfo roomArray[5][5];
	bool VdoorArray[4][5];//assigning all doors closed
	for (int c = 0; c < 4; c++)
	{
		for (int r = 0; r < 5; r++)
		{
			VdoorArray[c][r] = 1;
		}
	}
	bool HdoorArray[5][4];
	for (int c = 0; c < 5; c++)
	{
		for (int r = 0; r < 4; r++)
		{
			HdoorArray[c][r] = 1;
		}
	}
	int C = 0;
	int R = 0;
	bool roomVisited[5][5];
	for (int c = 0; c < 5; c++)//assigning all rooms unvisited
	{
		for (int r = 0; r < 5; r++)
		{
			roomVisited[c][r] = 0;
		}
	}
	roomVisited[0][0] = 1;//starting at room (0, 0)
	while (true)
	{
		if (4 == R && 4 == C)//stop when room (4, 4) is reached
		{
			break;
		}
		int random = _dice(2);
		if (1 == random && 4 != C)//go east
		{
			roomVisited[C + 1][R] = 1;//make next room visited
			VdoorArray[C][R] = 0;//opening door
			C++;//move to next room
		}
		else if (2 == random && 4 != R)
		{
			roomVisited[C][R + 1] = 1;//make next room visited
			HdoorArray[C][R] = 0;//opening door
			R++;//move to next room
		}
	}
	bool centralPath[5][5];
	for (int c = 0; c < 5; c++)//copying roomVisited into central path
	{
		for (int r = 0; r < 5; r++)
		{
			centralPath[c][r] = roomVisited[c][r];
		}
	}
	//snaking from central path
	for (int r = 0; r < 5; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if (1 == centralPath[c][r])//use a copy of roomVisited
			{
				C = c;
				R = r;
				exit = false;
				int counter = 0;
				while (!exit)//snaking loop
				{
					counter++;
					if (4 == counter)
					{
						break;
					}
					int adjRooms = 0;//number of adjacent open rooms
					bool unVisN = false;
					bool unVisE = false;
					bool unVisS = false;
					bool unVisW = false;
					if (0 == roomVisited[C][R - 1] && 0 != R)
					{
						adjRooms++;
						unVisN = true;
					}
					if (0 == roomVisited[C + 1][R] && 4 != C)
					{
						adjRooms++;
						unVisE = true;
					}
					if (0 == roomVisited[C][R + 1] && 4 != R)
					{
						adjRooms++;
						unVisS = true;
					}
					if (0 == roomVisited[C - 1][R] && 0 != C)
					{
						adjRooms++;
						unVisW = true;
					}

					if (0 == adjRooms)
					{
						break;
					}

					int random = _dice(adjRooms);

					if (1 == random)
					{
						if (true == unVisN)
						{
							roomVisited[C][R - 1] = 1;//visit next room
							HdoorArray[C][R - 1] = 0;//open door
							R--;//set coords to new room
						}
						else if (true == unVisE)
						{
							roomVisited[C + 1][R] = 1;
							VdoorArray[C][R] = 0;
							C++;
						}
						else if (true == unVisS)
						{
							roomVisited[C][R + 1] = 1;
							HdoorArray[C][R] = 0;
							R++;
						}
						else if (true == unVisW)
						{
							roomVisited[C - 1][R] = 1;
							VdoorArray[C - 1][R] = 0;
							C--;
						}
					}
					else if (2 == random)
					{
						if (true == unVisE)
						{
							roomVisited[C + 1][R] = 1;
							VdoorArray[C][R] = 0;
							C++;
						}
						else if (true == unVisS)
						{
							roomVisited[C][R + 1] = 1;
							HdoorArray[C][R] = 0;
							R++;
						}
						else if (true == unVisW)
						{
							roomVisited[C - 1][R] = 1;
							VdoorArray[C - 1][R] = 0;
							C--;
						}
					}
					else if (3 == random)
					{
						if (true == unVisS)
						{
							roomVisited[C][R + 1] = 1;
							HdoorArray[C][R] = 0;
							R++;
						}
						else if (true == unVisW)
						{
							roomVisited[C - 1][R] = 1;
							VdoorArray[C - 1][R] = 0;
							C--;
						}
					}
					else if (4 == random)//prety sure this is impossible
					{
						roomVisited[C - 1][R] = 1;
						VdoorArray[C - 1][R] = 0;
						C--;
					}
				}
			}
		}
	}
	//opening remaining rooms
	int openedRooms = 0;
	for (int r = 0; r < 5; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if (0 == roomVisited[c][r])
			{
				roomVisited[c][r] = 1;//mark room as visited
				openedRooms++;
				if (0 != r)
				{
					roomVisited[c][r - 1] = 1;//north room
					HdoorArray[c][r - 1] = 0;//north door
				}
				if (4 != c)
				{
					roomVisited[c + 1][r] = 1;//east room
					VdoorArray[c][r] = 0;//east door
				}
				if (4 != r)
				{
					roomVisited[c][r + 1] = 1;//south room
					HdoorArray[c][r] = 0;//south door
				}
				if (0 != c)
				{
					roomVisited[c - 1][r] = 1;//west room
					VdoorArray[c - 1][r] = 0;//west door
				}
			}
		}
	}
	//opening random rooms until 3 rooms have been opened
	for (openedRooms; openedRooms < 3; openedRooms++)
	{
		int c = _dice(5) - 1;
		int r = _dice(5) - 1;
		if (0 != r)
		{
			roomVisited[c][r - 1] = 1;//north room
			HdoorArray[c][r - 1] = 0;//north door
		}
		if (4 != c)
		{
			roomVisited[c + 1][r] = 1;//east room
			VdoorArray[c][r] = 0;//east door
		}
		if (4 != r)
		{
			roomVisited[c][r + 1] = 1;//south room
			HdoorArray[c][r] = 0;//south door
		}
		if (0 != c)
		{
			roomVisited[c - 1][r] = 1;//west room
			VdoorArray[c - 1][r] = 0;//west door
		}
	}
	//filling in 2 x 2 squares
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			if (VdoorArray[c][r] == 0 && HdoorArray[c][r] == 0 && VdoorArray[c][r + 1] == 0 && HdoorArray[c + 1][r] == 0)
			{
				int random = _dice(4);
				switch (random)
				{
				case 1:
					VdoorArray[c][r] = 1;
					break;
				case 2:
					HdoorArray[c][r] = 1;
					break;
				case 3:
					VdoorArray[c][r + 1] = 1;
					break;
				case 4:
					HdoorArray[c + 1][r] = 1;
					break;
				}
			}
		}
	}
	//random name generator and encouter assigner
	int npc1 = 0;//max 1
	int npc2 = 0;//max 1
	int npc3 = 0;//max 1
	int lowDiff = 0;//max 6
	int medDiff = 0;//max 4
	int hrdDiff = 0;//max 3
	int shop = 0;//max 2
	int trove = 0;//max 3
	for (int r = 0; r < 5; r++)
	{
		for (int c = 0; c < 5; c++)
		{
			if ((0 == c && 0 == r) || (0 == c && 4 == r) || (4 == c && 0 == r) || (4 == c && 4 == r))
			{
				roomArray[c][r].name = "Ancient Warp Gate";
				roomArray[c][r].encounterID = 1;
				continue;
			}
			int adjective = _dice(14);
			int noun = _dice(12);
			switch (adjective)
			{
			case 1:
				roomArray[c][r].name = "Soiled";//change to enum later
				break;
			case 2:
				roomArray[c][r].name = "Spatered";
				break;
			case 3:
				roomArray[c][r].name = "Arcane";
				break;
			case 4:
				roomArray[c][r].name = "Soiled";
				break;
			case 5:
				roomArray[c][r].name = "Great";
				break;
			case 6:
				roomArray[c][r].name = "Wasted";
				break;
			case 7:
				roomArray[c][r].name = "Glimering";
				break;
			case 8:
				roomArray[c][r].name = "Colored";
				break;
			case 9:
				roomArray[c][r].name = "Filthy";
				break;
			case 10:
				roomArray[c][r].name = "Daunting";
				break;
			case 11:
				roomArray[c][r].name = "Terrifying";
				break;
			case 12:
				roomArray[c][r].name = "Spectral";
				break;
			case 13:
				roomArray[c][r].name = "Abandoned";
				break;
			case 14:
				roomArray[c][r].name = "Easter";
				break;
			}
			switch (noun)
			{
			case 1:
				roomArray[c][r].name = roomArray[c][r].name + " Wastes";
				break;
			case 2:
				roomArray[c][r].name = roomArray[c][r].name + " Singularity";
				break;
			case 3:
				roomArray[c][r].name = roomArray[c][r].name + " Reaches";
				break;
			case 4:
				roomArray[c][r].name = roomArray[c][r].name + " Rim";
				break;
			case 5:
				roomArray[c][r].name = roomArray[c][r].name + " Reef";
				break;
			case 6:
				roomArray[c][r].name = roomArray[c][r].name + " Breach";
				break;
			case 7:
				roomArray[c][r].name = roomArray[c][r].name + " Row";
				break;
			case 8:
				roomArray[c][r].name = roomArray[c][r].name + " Hole";
				break;
			case 9:
				roomArray[c][r].name = roomArray[c][r].name + " Terminal";
				break;
			case 10:
				roomArray[c][r].name = roomArray[c][r].name + " Sink";
				break;
			case 11:
				roomArray[c][r].name = roomArray[c][r].name + " Station";
				break;
			case 12:
				roomArray[c][r].name = roomArray[c][r].name + " Egg";
				break;
			}
			while (true)
			{
				int rand = _dice(8);
				if (npc1 != 1 && rand == 1)
				{
					roomArray[c][r].encounterID = 6;
					npc1++;
					break;
				}
				if (npc2 != 1 && rand == 2)
				{
					roomArray[c][r].encounterID = 7;
					npc2++;
					break;
				}
				if (npc3 != 1 && rand == 3)
				{
					roomArray[c][r].encounterID = 8;
					npc3++;
					break;
				}
				if (lowDiff != 6 && rand == 4)
				{
					roomArray[c][r].encounterID = 2;
					lowDiff++;
					break;
				}
				if (medDiff != 4 && rand == 5)
				{
					roomArray[c][r].encounterID = 3;
					medDiff++;
					break;
				}
				if (hrdDiff != 3 && rand == 6)
				{
					roomArray[c][r].encounterID = 4;
					hrdDiff++;
					break;
				}
				if (shop != 2 && rand == 7)
				{
					roomArray[c][r].encounterID = 5;
					shop++;
					break;
				}
				if (trove != 3 && rand == 8)
				{
					roomArray[c][r].encounterID = 9;
					trove++;
					break;
				}
			}
		}
	}
	exit = false;
	int credits = 0;
	int playerC = 4;
	int playerR = 4;
	//navigation loop
	while (!exit)
	{
		system("cls");
		
		printf("you arive in sector (%i, %i), %s\n", playerC + 1, playerR + 1, roomArray[playerC][playerR].name.c_str());
		bool victory = encounter(player, roomArray[playerC][playerR].encounterID, credits);
		if (!victory)//game over
		{
			break;
		}
		while (true)//menue after encounter
		{
			system("cls");
			printf("you arive in sector (%i, %i), %s\n", playerC + 1, playerR + 1, roomArray[playerC][playerR].name.c_str());
			printf("you can jump to sectors:\n");
			printf("n. ");
			if (0 == HdoorArray[playerC][playerR - 1] && 0 != playerR)
			{
				printf("(%i, %i), %s\n", playerC + (1), playerR - 1 + (1), roomArray[playerC][playerR - 1].name.c_str());
			}
			else
			{
				printf("[no warp node]\n");
			}
			printf("e. ");
			if (0 == VdoorArray[playerC][playerR] && 4 != playerC)
			{
				printf("(%i, %i), %s\n", playerC + 1 + (1), playerR + (1), roomArray[playerC + 1][playerR].name.c_str());
			}
			else
			{
				printf("[no warp node]\n");
			}
			printf("s. ");
			if (0 == HdoorArray[playerC][playerR] && 4 != playerR)//somethings wrong, i can feel it...
			{
				printf("(%i, %i), %s\n", playerC + (1), playerR + 1 + (1), roomArray[playerC][playerR + 1].name.c_str());
			}
			else
			{
				printf("[no warp node]\n");
			}
			printf("w. ");
			if (0 == VdoorArray[playerC - 1][playerR] && 0 != playerC)
			{
				printf("(%i, %i), %s\n", playerC - 1 + (1), playerR + (1), roomArray[playerC - 1][playerR].name.c_str());
			}
			else
			{
				printf("[no warp node]\n");
			}
			printf("m. view map\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('m' == inputc)
			{
				displayMap(VdoorArray, HdoorArray, playerC, playerR);
				_getch();
			}
			else if (0 == HdoorArray[playerC][playerR - 1] && 'n' == inputc)
			{
				playerR--;
				break;
			}
			else if (0 == VdoorArray[playerC][playerR] && 'e' == inputc)
			{
				playerC++;
				break;
			}
			else if (0 == HdoorArray[playerC][playerR] && 's' == inputc)
			{
				playerR++;
				break;
			}
			else if (0 == VdoorArray[playerC - 1][playerR] && 'w' == inputc)
			{
				playerC--;
				break;
			}
			else
			{
				printf("[NAVIGATION ERROR]\n");
				_getch();
			}
		}
	}
	//print scores
	return 0;
}

void displayMap(bool(&VdoorArray)[4][5], bool(&HdoorArray)[5][4], int C, int R)
{
	int a = 1;
	printf("%c%c%c%c%c%c%c%c%c%c%c\n", 201, 205, 203, 205, 203, 205, 203, 205, 203, 205, 187);
	for (int r = 0; r < 5; r++)
	{
		a++;
		printf("%c", 186);
		for (int c = 0; c < 4; c++)
		{
			if (c == C && r == R)//display player location
			{
				printf("X");
			}
			else
			{
				printf(" ");
			}
			if (VdoorArray[c][r] == 1)
			{
				printf("%c", 186);
			}
			else
			{
				printf(" ");
			}
		}
		if (4 == C && r == R)//display player location
		{
			printf("X");
		}
		else
		{
			printf(" ");
		}
		printf("%c\n", 186);
		if (a <= 5)
		{
			printf("%c", 204);
			for (int c = 0; c < 4; c++)
			{
				if (HdoorArray[c][r] == 1)
				{
					printf("%c%c", 205, 206);
				}
				else
				{
					printf(" %c", 206);
				}
			}
			if (HdoorArray[4][r] == 1)
			{
				printf("%c%c\n", 205, 185);
			}
			else
			{
				printf(" %c\n", 185);
			}
		}
		else
		{
			printf("%c%c%c%c%c%c%c%c%c%c%c\n", 200, 205, 202, 205, 202, 205, 202, 205, 202, 205, 188);
		}
	}
}

int _dice(int max)//outputs a random number from 1 to max
{
	int rn;
	rn = 1 + rand() % (max);
	return rn;
}
int roundUp(int number, int devisor)
{
	int ans = number / devisor;
	if (number % devisor != 0)
	{
		ans++;
	}
	return ans;
}
char _getdir()
{
	//returns u d l r
	//e = error
	int c = 0;

	c = _getch();
	//cout << c;
	if (224 == c)//arrow keys
	{
		switch (_getch())
		{
		case 72:
			return 'u';
		case 75:
			return 'l';
		case 77:
			return 'r';
		case 80:
			return 'd';
		}
	}
	else//regular keys
	{
		if ('w' == c)
		{
			return 'u';
		}
		else if ('s' == c)
		{
			return 'd';
		}
		else if ('a' == c)
		{
			return 'l';
		}
		else if ('d' == c)
		{
			return 'r';
		}
		else if (13 == c)//enter key
		{
			return 13;
		}
		else
		{
			return 'e';
		}
	}
}

bool encounter(party &player, int encounterID, int &credits)//this is why we use structs
{
	if (1 == encounterID)//starting sector
	{
		printf("as your party leaves the mining station you see an ancient warp gate, long lost to time, shrinking in the distance\n");
		printf("while this region of the galaxy isn't necessarily the most civilized of places, there are still enough warp nodes\nuntouched to get from sector to sector\n");
		_getch();
		return 1;
	}
	else if (2 == encounterID)//low difficulty enemies
	{
		switch (_dice(3))
		{
		case 1:
			combat(player, 1, credits);
			break;
		case 2:
			combat(player, 1, credits);
			break;
		case 3:
			combat(player, 1, credits);
			break;
		}
		//combat function(level, id)
		return 1;
	}
	else if (3 == encounterID)//medium difficulty enemies
	{
		switch (_dice(2))
		{
		case 1:
			combat(player, 1, credits);
			break;
		case 2:
			combat(player, 1, credits);
			break;
		}
		return 1;
	}
	else if (4 == encounterID)//hard difficulty enemies
	{
		cout << "hard difficulty enemy";
		//This will get filled out when i use structs in goal sheet 7
		_getch();
		return 1;
	}
	else if (5 == encounterID)//shop
	{
		cout << "shop";
		//This will get filled out when i use structs in goal sheet 7
		_getch();
		return 1;
	}
	else if (6 == encounterID)//npc 1
	{
		cout << "first NPC";
		//This will get filled out when i use structs in goal sheet 7
		_getch();
		return 1;
	}
	else if (7 == encounterID)//npc 2
	{
		cout << "second NPC\n";
		//This will get filled out when i use structs in goal sheet 7
		_getch();
		return 1;
	}
	else if (8 == encounterID)//npc 3
	{
		cout << "third NPC\n";
		//This will get filled out when i use structs in goal sheet 7
		_getch();
		return 1;
	}
	else if (9 == encounterID)//treasure trove
	{
		cout << "treasure\n";
		//This will get filled out when i use structs in goal sheet 7
		_getch();
		return 1;
	}
}


//combat related functions
bool combat(party& player, int enemyID, int &credits)
{
	system("cls");
	bool DEBUG = false;
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	player.eng.atkMulti = 1.0;
	if (1 == player.eng.wep)
	{
		player.eng.atk = 8;
	}
	else if (2 == player.eng.wep)
	{
		player.eng.atk = 9;
	}
	else if (3 == player.eng.wep)
	{
		player.eng.atk = 10;
	}
	player.sci.atkMulti = 1.0;
	if (1 == player.sci.wep)
	{
		player.sci.atk = 7;
	}
	else if (2 == player.sci.wep)
	{
		player.sci.atk = 8;
	}
	else if (3 == player.sci.wep)
	{
		player.sci.atk = 9;
	}
	player.pil.atkMulti = 1.0;
	if (1 == player.pil.wep)
	{
		player.pil.atk = 6;
	}
	else if (2 == player.pil.wep)
	{
		player.pil.atk = 7;
	}
	else if (3 == player.pil.wep)
	{
		player.pil.atk = 8;
	}
	eparty spaceGoblin, ooze, spiderDrone;//easy enemies
	eparty zombie, rockMan;//medium enemies
	eparty worm;//hard enemy
	spaceGoblin.e1.name = "Space Goblin Tim";
	spaceGoblin.e1.atk = 3;
	spaceGoblin.e1.atkMulti = 1.0;
	spaceGoblin.e1.HP = 9;
	spaceGoblin.e1.HPmax = 9;
	spaceGoblin.e1.dead = false;
	spaceGoblin.e2.name = "Space Goblin Tom";
	spaceGoblin.e2.atk = 3;
	spaceGoblin.e2.atkMulti = 1.0;
	spaceGoblin.e2.HP = 9;
	spaceGoblin.e2.HPmax = 9;
	spaceGoblin.e2.dead = false;
	spaceGoblin.e3.name = "Space Goblin Tam";
	spaceGoblin.e3.atk = 3;
	spaceGoblin.e3.atkMulti = 1.0;
	spaceGoblin.e3.HP = 9;
	spaceGoblin.e3.HPmax = 9;
	spaceGoblin.e3.dead = false;
	spaceGoblin.ship.HP = 20;
	spaceGoblin.ship.HPmax = 20;
	spaceGoblin.ship.name = "Goblin Junker";
	spaceGoblin.graphicID = 1;
	spaceGoblin.a = 3;
	eparty enemyParty;
	switch (enemyID)
	{
	case 1:
		enemyParty = spaceGoblin;
		break;
	}
	turn pTurn = eng;
	int turnNum = 0;
	while (true)//combat loop
	{
		SetConsoleTextAttribute(color, 15);
		turnNum++;
		system("cls");
		string currentChar;//name of whichever character's turn it is
		pTurn = eng;
		printHealthWindow(player, enemyParty);
		bool healthReprint = false;
		//get player input
		while (!healthReprint)
		{
			switch (pTurn)
			{
			case eng:
				currentChar = player.eng.name;
				break;
			case sci:
				currentChar = player.sci.name;
				break;
			case pil:
				currentChar = player.pil.name;
				break;
			case ship:
				currentChar = player.ship.name;
			}
			coord.X = 0;
			coord.Y = 23;
			SetConsoleCursorPosition(standard, coord);
			SetConsoleTextAttribute(color, 15);
			printf("%c", 218);//upper left corner
			printf("%c%c%c", 196, 196, 196);
			for (int a = 0; a < 61; a++)
			{
				if (a < currentChar.length())
				{
					cout << currentChar[a];
				}
				else
				{
					printf("%c", 196);
				}
			}
			printf("%c\n", 191);//upper right corner
			printf("%c", 179);
			for (int a = 0; a < 64; a++)
			{
				cout << " ";
			}
			printf("%c\n", 179);
			printf("%c", 179);
			cout << "ATTACK                          EVADE                           ";
			printf("%c\n", 179);
			printf("%c", 179);
			for (int a = 0; a < 64; a++) { cout << " "; }
			printf("%c\n", 179);
			printf("%c", 179);
			cout << "SPECIAL                         ESCAPE                          ";
			printf("%c\n", 179);
			printf("%c", 192);//lower left corner
			for (int a = 0; a < 64; a++) { printf("%c", 196); }
			printf("%c\n", 217);//lower right corner
			point menu;
			menu.X = 1;
			menu.Y = 1;
			point menuPast = menu;
			bool menuReprint = false;
			//menu navigation
			while (!menuReprint)
			{
				//color menu position
				if (1 == menu.X && 1 == menu.Y)
				{
					coord.X = 1;
					coord.Y = 25;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << "ATTACK";
				}
				else if (1 == menu.X && 2 == menu.Y)
				{
					coord.X = 1;
					coord.Y = 27;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << "SPECIAL";
				}
				else if (2 == menu.X && 1 == menu.Y)
				{
					coord.X = 33;
					coord.Y = 25;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << "EVADE";
				}
				else if (2 == menu.X && 2 == menu.Y)
				{
					coord.X = 33;
					coord.Y = 27;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << "ESCAPE";
				}
				//color past position
				if (menu.X != menuPast.X || menu.Y != menuPast.Y)
				{
					if (1 == menuPast.X && 1 == menuPast.Y)
					{
						coord.X = 1;
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << "ATTACK";
					}
					else if (1 == menuPast.X && 2 == menuPast.Y)
					{
						coord.X = 1;
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << "SPECIAL";
					}
					else if (2 == menuPast.X && 1 == menuPast.Y)
					{
						coord.X = 33;
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << "EVADE";
					}
					else if (2 == menuPast.X && 2 == menuPast.Y)
					{
						coord.X = 33;
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << "ESCAPE";
					}
				}
				coord.X = 0;
				coord.Y = 29;
				SetConsoleCursorPosition(standard, coord);
				menuPast = menu;
				//get new position
				char inputc = _getdir();
				if ('u' == inputc)
				{
					if (2 == menu.Y)
					{
						menu.Y = 1;
					}
				}
				else if ('r' == inputc)
				{
					if (1 == menu.X)
					{
						menu.X = 2;
					}
				}
				else if ('d' == inputc)
				{
					if (1 == menu.Y)
					{
						menu.Y = 2;
					}
				}
				else if ('l' == inputc)
				{
					if (2 == menu.X)
					{
						menu.X = 1;
					}
				}
				//record inputs
				else if (13 == inputc)
				{
					if (1 == menu.X && 1 == menu.Y)//ATTACK
					{
						coord.X = 0;
						coord.Y = 23;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						printf("%c", 218);//upper left corner
						printf("%c%c%c", 196, 196, 196);
						for (int a = 0; a < 61; a++)
						{
							if (a < currentChar.length())
							{
								cout << currentChar[a];
							}
							else if ((currentChar.length() + 1) < a && a < currentChar.length() + 8)// if character length plus one
							{
								string temp = "ATTACK";
								cout << temp[a - (currentChar.length() + 2)];
							}
							else
							{
								printf("%c", 196);
							}
						}
						printf("%c\n", 191);//upper right corner
						printf("%c", 179);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						printf("%c\n", 179);
						printf("%c", 179);
						if (1 <= enemyParty.a)//will always run, but whatever
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.e1.name.length())
								{
									cout << enemyParty.e1.name[a];
								}
								else
								{
									cout << " ";
								}
							}
						}
						else//will never run, but whatever
						{
							for (int a = 0; a < 32; a++)
							{
								cout << " ";
							}
						}
						if (2 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.e2.name.length())
								{
									cout << enemyParty.e2.name[a];
								}
								else
								{
									cout << " ";
								}
							}
						}
						else
						{
							for (int a = 0; a < 32; a++)
							{
								cout << " ";
							}
						}
						printf("%c\n", 179);
						printf("%c", 179);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						printf("%c\n", 179);
						printf("%c", 179);
						if (3 == enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.e3.name.length())
								{
									cout << enemyParty.e3.name[a];
								}
								else
								{
									cout << " ";
								}
							}
						}
						else
						{
							for (int a = 0; a < 32; a++)
							{
								cout << " ";
							}
						}
						cout << "BACK";
						for (int a = 0; a < 28; a++)
						{
							cout << " ";
						}
						//menu navigation
						bool attackReprint = false;
						while (!attackReprint)
						{
							//color menu position
							if (1 == menu.X && 1 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.e1.name;
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 27;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.e3.name;
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.e2.name;
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 27;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << "BACK";
							}
							//color past position
							if (menu.X != menuPast.X || menu.Y != menuPast.Y)
							{
								if (1 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.e1.name;
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 27;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.e3.name;
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.e2.name;
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 27;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << "BACK";
								}
							}
							coord.X = 0;
							coord.Y = 29;
							SetConsoleCursorPosition(standard, coord);
							menuPast = menu;
							//get new position
							char inputc = _getdir();
							if ('u' == inputc)
							{
								if (2 == menu.Y)
								{
									menu.Y = 1;
								}
							}
							else if ('r' == inputc)
							{
								if (1 == menu.X)
								{
									menu.X = 2;
								}
							}
							else if ('d' == inputc)
							{
								if (1 == menu.Y)
								{
									menu.Y = 2;
								}
							}
							else if ('l' == inputc)
							{
								if (2 == menu.X)
								{
									menu.X = 1;
								}
							}
							//record inputs
							else if (13 == inputc)
							{
								if (1 == menu.X && 1 == menu.Y)//e1
								{
									if (eng == pTurn)
									{
										player.eng.act = attack;
										player.eng.target = 1;
									}
									else if (sci == pTurn)
									{
										player.sci.act = attack;
										player.sci.target = 1;
									}
									else if (pil == pTurn)
									{
										player.pil.act = attack;
										player.pil.target = 1;
									}
									attackReprint = true;
								}
								else if (1 == menu.X && 2 == menu.Y)//e3
								{
									if (eng == pTurn)
									{
										player.eng.act = attack;
										player.eng.target = 3;
									}
									else if (sci == pTurn)
									{
										player.sci.act = attack;
										player.sci.target = 3;
									}
									else if (pil == pTurn)
									{
										player.pil.act = attack;
										player.pil.target = 3;
									}
									attackReprint = true;
								}
								else if (2 == menu.X && 1 == menu.Y)//e2
								{
									if (eng == pTurn)
									{
										player.eng.act = attack;
										player.eng.target = 2;
									}
									else if (sci == pTurn)
									{
										player.sci.act = attack;
										player.sci.target = 2;
									}
									else if (pil == pTurn)
									{
										player.pil.act = attack;
										player.pil.target = 2;
									}
									attackReprint = true;
								}
								else if (2 == menu.X && 2 == menu.Y)//BACK
								{
									menuReprint = true;
									attackReprint = true;
								}
							}
						}
					}
					else if (1 == menu.X && 2 == menu.Y)//SPECIAL
					{
						coord.X = 0;
						coord.Y = 23;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						printf("%c", 218);//upper left corner
						printf("%c%c%c", 196, 196, 196);
						for (int a = 0; a < 61; a++)
						{
							if (a < currentChar.length())
							{
								cout << currentChar[a];
							}
							else if ((currentChar.length() + 1) < a && a < currentChar.length() + 9)// if character length plus one
							{
								string temp = "SPECIAL";
								cout << temp[a - (currentChar.length() + 2)];
							}
							else
							{
								printf("%c", 196);
							}
						}
						printf("%c\n\n", 191);//upper right corner
						printf("%c", 179);
						/*
						+----engineer--------------+
						|scientist        pilot    |
						|ship             BACK     |
						+--------------------------+
						+----scientist-------------+
						|engineer         pilot    |
						|ship             BACK     |
						+--------------------------+
						+----pilot-----------------+
						|engineer         scientist|
						|ship             BACK     |
						+--------------------------+
						*/
						string firstMember, secondMember, shipName;
						if (eng == pTurn)
						{
							firstMember = player.sci.name;
							secondMember = player.pil.name;
						}
						else if (sci == pTurn)
						{
							firstMember = player.eng.name;
							secondMember = player.pil.name;
						}
						else if (pil == pTurn)
						{
							firstMember = player.eng.name;
							secondMember = player.sci.name;
						}
						shipName = player.ship.name;
						for (int a = 0; a < 64; a++)
						{
							if (a <= firstMember.length())
							{
								cout << firstMember[a];
							}
							else if (a - 32 <= secondMember.length())
							{
								cout << secondMember[a - 32];
							}
							else
							{
								cout << " ";
							}
						}
						printf("%c\n\n%c", 179, 179);
						for (int a = 0; a < 64; a++)
						{
							if (a < shipName.length())
							{
								cout << shipName[a];
							}
							else if (a == 32)
							{
								cout << "BACK";
								a = 35;
							}
							else
							{
								cout << " ";
							}
						}
						//menu navigation
						bool attackReprint = false;
						menu.X = 1;
						menu.Y = 1;
						while (!attackReprint)
						{
							//color menu position
							if (1 == menu.X && 1 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << firstMember;
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 27;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << shipName;
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << secondMember;
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 27;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << "BACK";
							}
							//color past position
							if (menu.X != menuPast.X || menu.Y != menuPast.Y)
							{
								if (1 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << firstMember;
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 27;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << shipName;
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << secondMember;
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 27;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << "BACK";
								}
							}
							coord.X = 0;
							coord.Y = 29;
							SetConsoleCursorPosition(standard, coord);
							menuPast = menu;
							//get new position
							char inputc = _getdir();
							if ('u' == inputc)
							{
								if (2 == menu.Y)
								{
									menu.Y = 1;
								}
							}
							else if ('r' == inputc)
							{
								if (1 == menu.X)
								{
									menu.X = 2;
								}
							}
							else if ('d' == inputc)
							{
								if (1 == menu.Y)
								{
									menu.Y = 2;
								}
							}
							else if ('l' == inputc)
							{
								if (2 == menu.X)
								{
									menu.X = 1;
								}
							}
							//record inputs
							else if (13 == inputc)
							{
								if (1 == menu.X && 1 == menu.Y)//e1
								{
									if (eng == pTurn)
									{
										player.eng.act = special;
										player.eng.target = 2;
									}
									else if (sci == pTurn)
									{
										player.sci.act = special;
										player.sci.target = 1;
									}
									else if (pil == pTurn)
									{
										player.pil.act = special;
										player.pil.target = 1;
									}
									attackReprint = true;
								}
								else if (1 == menu.X && 2 == menu.Y)//e3
								{
									if (eng == pTurn)
									{
										player.eng.act = special;
										player.eng.target = 4;
									}
									else if (sci == pTurn)
									{
										player.sci.act = special;
										player.sci.target = 4;
									}
									else if (pil == pTurn)
									{
										player.pil.act = special;
										player.pil.target = 4;
									}
									attackReprint = true;
								}
								else if (2 == menu.X && 1 == menu.Y)//e2
								{
									if (eng == pTurn)
									{
										player.eng.act = special;
										player.eng.target = 3;
									}
									else if (sci == pTurn)
									{
										player.sci.act = special;
										player.sci.target = 3;
									}
									else if (pil == pTurn)
									{
										player.pil.act = special;
										player.pil.target = 2;
									}
									attackReprint = true;
								}
								else if (2 == menu.X && 2 == menu.Y)//BACK
								{
									menuReprint = true;
									attackReprint = true;
								}
							}
						}
					}
					else if (2 == menu.X && 1 == menu.Y)//EVADE
					{
						coord.X = 0;
						coord.Y = 23;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						printf("%c", 218);//upper left corner
						printf("%c%c%c", 196, 196, 196);
						for (int a = 0; a < 61; a++)
						{
							if (a < currentChar.length())
							{
								cout << currentChar[a];
							}
							else if ((currentChar.length() + 1) < a && a < currentChar.length() + 7)// if character length plus one
							{
								string temp = "EVADE";
								cout << temp[a - (currentChar.length() + 2)];
							}
							else
							{
								printf("%c", 196);
							}
						}
						printf("%c\n", 191);//upper right corner
						printf("%c", 179);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						printf("%c\n", 179);
						printf("%c", 179);
						if (1 <= enemyParty.a)//will always run, but whatever
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.e1.name.length())
								{
									cout << enemyParty.e1.name[a];
								}
								else
								{
									cout << " ";
								}
							}
						}
						else//will never run, but whatever
						{
							for (int a = 0; a < 32; a++)
							{
								cout << " ";
							}
						}
						if (2 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.e2.name.length())
								{
									cout << enemyParty.e2.name[a];
								}
								else
								{
									cout << " ";
								}
							}
						}
						else
						{
							for (int a = 0; a < 32; a++)
							{
								cout << " ";
							}
						}
						printf("%c\n", 179);
						printf("%c", 179);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						printf("%c\n", 179);
						printf("%c", 179);
						if (3 == enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.e3.name.length())
								{
									cout << enemyParty.e3.name[a];
								}
								else
								{
									cout << " ";
								}
							}
						}
						else
						{
							for (int a = 0; a < 32; a++)
							{
								cout << " ";
							}
						}
						cout << "BACK";
						for (int a = 0; a < 28; a++)
						{
							cout << " ";
						}
						//menu navigation
						bool goBack = false;
						while (!goBack)
						{
							//color menu position
							if (1 == menu.X && 1 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.e1.name;
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 27;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.e3.name;
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.e2.name;
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 27;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << "BACK";
							}
							//color past position
							if (menu.X != menuPast.X || menu.Y != menuPast.Y)
							{
								if (1 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.e1.name;
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 27;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.e3.name;
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.e2.name;
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 27;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << "BACK";
								}
							}
							coord.X = 0;
							coord.Y = 29;
							SetConsoleCursorPosition(standard, coord);
							menuPast = menu;
							//get new position
							char inputc = _getdir();
							if ('u' == inputc)
							{
								if (2 == menu.Y)
								{
									menu.Y = 1;
								}
							}
							else if ('r' == inputc)
							{
								if (1 == menu.X)
								{
									menu.X = 2;
								}
							}
							else if ('d' == inputc)
							{
								if (1 == menu.Y)
								{
									menu.Y = 2;
								}
							}
							else if ('l' == inputc)
							{
								if (2 == menu.X)
								{
									menu.X = 1;
								}
							}
							//record inputs
							else if (13 == inputc)
							{
								if (1 == menu.X && 1 == menu.Y)//e1
								{
									if (eng == pTurn)
									{
										player.eng.act = evade;
										player.eng.target = 1;
									}
									else if (sci == pTurn)
									{
										player.sci.act = evade;
										player.sci.target = 1;
									}
									else if (pil == pTurn)
									{
										player.pil.act = evade;
										player.pil.target = 1;
									}
									goBack = true;//aka attackReprint
								}
								else if (1 == menu.X && 2 == menu.Y)//e3
								{
									if (eng == pTurn)
									{
										player.eng.act = evade;
										player.eng.target = 3;
									}
									else if (sci == pTurn)
									{
										player.sci.act = evade;
										player.sci.target = 3;
									}
									else if (pil == pTurn)
									{
										player.pil.act = evade;
										player.pil.target = 3;
									}
									goBack = true;
								}
								else if (2 == menu.X && 1 == menu.Y)//e2
								{
									if (eng == pTurn)
									{
										player.eng.act = evade;
										player.eng.target = 2;
									}
									else if (sci == pTurn)
									{
										player.sci.act = evade;
										player.sci.target = 2;
									}
									else if (pil == pTurn)
									{
										player.pil.act = evade;
										player.pil.target = 2;
									}
									goBack = true;
								}
								else if (2 == menu.X && 2 == menu.Y)//BACK
								{
									menuReprint = true;
									goBack = true;
								}
							}
						}
					}
					else if (2 == menu.X && 2 == menu.Y)//ESCAPE
					{
						if (eng == pTurn) { player.eng.act = escape; }
						else if (sci == pTurn) { player.sci.act = escape; }
						else if (pil == pTurn) { player.pil.act = escape; }
						else if (ship == pTurn) { player.ship.act = escape; }
					}
					//used by menus for BACK option
					if (menuReprint)
					{
						menuReprint = false;
						break;
					}
					if (pTurn != pil)
					{
						menuReprint = true;
						switch (pTurn)
						{
						case eng:
							pTurn = sci;
							break;
						case sci:
							pTurn = pil;
						}
					}
					else
					{
						healthReprint = true;
					}
					menuReprint = true;
				}
			}
		}
		//randomize enemy input ------- ENEMY AI GOES HERE
		//enemies don't escape
		switch (enemyParty.e1.ID)
		{
		case 1:
			spaceGoblinAI(player, enemyParty, 1, turnNum);
			break;
		}
		switch (enemyParty.e2.ID)
		{
		case 1:
			spaceGoblinAI(player, enemyParty, 1, turnNum);
			break;
		}
		switch (enemyParty.e3.ID)
		{
		case 1:
			spaceGoblinAI(player, enemyParty, 1, turnNum);
			break;
		}
		if (DEBUG)//artificial enemy actions
		{
			//enemyParty.e1.act = special;
			//enemyParty.e2.act = special;
			//enemyParty.e3.act = special;
		}
		if (DEBUG)
		{
			SetConsoleTextAttribute(color, 15);
			cout << "e1: action " << enemyParty.e1.act << " target " << enemyParty.e1.target << "\n";
			cout << "e2: action " << enemyParty.e2.act << " target " << enemyParty.e2.target << "\n";
			cout << "e3: action " << enemyParty.e3.act << " target " << enemyParty.e3.target << "\n";
			_getch();
		}
		//player actions
		turn a = eng;
		bool exit = false;
		while (!exit)
		{
			SetConsoleTextAttribute(standard, 15);
			coord.X = 0;
			coord.Y = 29;
			SetConsoleCursorPosition(standard, coord);
			for (int a = 0; a < 80; a++)
			{
				cout << " ";
			}
			coord.X = 0;
			coord.Y = 29;
			SetConsoleCursorPosition(standard, coord);
			int engTempAtk = (float)player.eng.atk * player.eng.atkMulti;
			int sciTempAtk = (float)player.sci.atk * player.sci.atkMulti;
			int pilTempAtk = (float)player.pil.atk * player.pil.atkMulti;
			if (eng == a)//engineeer
			{
				if (attack == player.eng.act)//attacks
				{
					if (1 == player.eng.target)//enemy 1
					{
						if (evade == enemyParty.e1.act && 1 == enemyParty.e1.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e1.name << " fails to dodge " << player.eng.name << "'s attack for " << engTempAtk << " damage";
								enemyParty.e1.HP -= engTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e1.name << " dodges " << player.eng.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//25% chance to miss
							{
								cout << player.eng.name << " attacks " << enemyParty.e1.name << " but misses";
							}
							else//75% chance to not miss
							{
								cout << player.eng.name << " attacks " << enemyParty.e1.name << " for " << engTempAtk << " damage";
								enemyParty.e1.HP -= engTempAtk;
							}
						}
					}
					else if (2 == player.eng.target)
					{
						if (evade == enemyParty.e2.act && 1 == enemyParty.e2.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e2.name << " fails to dodge " << player.eng.name << "'s attack for " << engTempAtk << " damage";
								enemyParty.e2.HP -= engTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e2.name << " dodges " << player.eng.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.eng.name << " attacks " << enemyParty.e2.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.eng.name << " attacks " << enemyParty.e2.name << " for " << engTempAtk << " damage";
								enemyParty.e2.HP -= engTempAtk;
							}
						}
					}
					else if (3 == player.eng.target)
					{
						if (evade == enemyParty.e3.act && 1 == enemyParty.e3.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e3.name << " fails to dodge " << player.eng.name << "'s attack for " << engTempAtk << " damage";
								enemyParty.e3.HP -= engTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e3.name << " dodges " << player.eng.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.eng.name << " attacks " << enemyParty.e3.name << " but misses";
							}
							else//75% chance to hit
							{
								cout << player.eng.name << " attacks " << enemyParty.e3.name << " for " << engTempAtk << " damage";
								enemyParty.e3.HP -= engTempAtk;
							}
						}
					}
				}
				else if (special == player.eng.act)
				{
					if (2 == player.eng.target)
					{
						cout << player.eng.name << " attends to " << player.sci.name << "'s wounds for 1 point of damage";
						player.sci.HP += 1;
					}
					else if (3 == player.eng.target)
					{
						cout << player.eng.name << " attends to " << player.pil.name << "'s wounds for 1 point of damage";
						player.pil.HP += 1;
					}
					else if (4 == player.eng.target)
					{
						if (1 == player.eng.wep)//less ship repair
						{
							cout << player.eng.name << " repairs the ship for 6 health";
							player.ship.HP += 6;
						}
						else if (2 == player.eng.wep)//more ship repair
						{
							cout << player.eng.name << " repairs the ship for 10 health";
							player.ship.HP += 10;
						}
						else if (3 == player.eng.wep)//no ship repair
						{
							cout << "without a tool " << player.eng.name << " is unable to repair the ship";
						}
					}
				}
			}
			else if (sci == a)//scientist
			{
				if (attack == player.sci.act)//attacks
				{
					if (1 == player.sci.target)//enemy 1
					{
						if (evade == enemyParty.e1.act && 2 == enemyParty.e1.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e1.name << " fails to dodge " << player.sci.name << "'s attack for " << sciTempAtk << " damage";
								enemyParty.e1.HP -= sciTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e1.name << " dodges " << player.sci.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.sci.name << " attacks " << enemyParty.e1.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.sci.name << " attacks " << enemyParty.e1.name << " for " << sciTempAtk << " damage";
								enemyParty.e1.HP -= sciTempAtk;
							}
						}
					}
					else if (2 == player.sci.target)
					{
						if (evade == enemyParty.e2.act && 2 == enemyParty.e2.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e2.name << " fails to dodge " << player.sci.name << "'s attack for " << sciTempAtk << " damage";
								enemyParty.e2.HP -= sciTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e2.name << " dodges " << player.sci.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.sci.name << " attacks " << enemyParty.e2.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.sci.name << " attacks " << enemyParty.e2.name << " for " << sciTempAtk << " damage";
								enemyParty.e2.HP -= sciTempAtk;
							}
						}
					}
					else if (3 == player.sci.target)
					{
						if (evade == enemyParty.e3.act && 2 == enemyParty.e3.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e3.name << " fails to dodge " << player.sci.name << "'s attack for " << sciTempAtk << " damage";
								enemyParty.e3.HP -= sciTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e3.name << " dodges " << player.sci.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.sci.name << " attacks " << enemyParty.e3.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.sci.name << " attacks " << enemyParty.e3.name << " for " << sciTempAtk << " damage";
								enemyParty.e3.HP -= sciTempAtk;
							}
						}
					}
				}
				else if (special == player.sci.act)//uses special
				{
					if (1 == player.sci.target)//on engineer
					{
						if (1 == player.sci.wep)//heals
						{
							cout << player.sci.name << " attends to " << player.eng.name << "'s wounds for 3 points of damage";
							player.eng.HP += 3;
						}
						else if (2 == player.sci.wep)
						{
							cout << player.sci.name << " heals " << player.eng.name << " for 7 points of damage and optimizes their stats by 10%";
							player.eng.HP += 7;
							player.eng.atkMulti *= 1.1;
						}
						else if (3 == player.sci.wep)
						{
							cout << player.sci.name << " optimizes " << player.eng.name << "'s stats by 15%";
							player.eng.atkMulti *= 1.15;
						}
					}
					else if (3 == player.sci.target)
					{
						if (1 == player.sci.wep)//heals
						{
							cout << player.sci.name << " attends to " << player.pil.name << "'s wounds for 3 points of damage";
							player.pil.HP += 3;
						}
						else if (2 == player.sci.wep)
						{
							cout << player.sci.name << " heals " << player.pil.name << " for 7 points of damage and optimizes their stats by 10%";
							player.pil.HP += 7;
							player.pil.atkMulti *= 1.1;
						}
						else if (3 == player.sci.wep)
						{
							cout << player.sci.name << " optimizes " << player.pil.name << "'s stats by 15%";
							player.pil.atkMulti *= 1.15;
						}
					}
					else if (4 == player.sci.target)
					{
						if (1 == player.sci.wep)//less ship repair
						{
							cout << "Sammarco is bad at game design and didn't plan for anything to happen here";
						}
						else if (2 == player.sci.wep)//more ship repair
						{
							cout << "Sammarco is bad at game design and didn't plan for anything to happen here";
						}
						else if (3 == player.sci.wep)//no ship repair
						{
							cout << "Sammarco is bad at game design and didn't plan for anything to happen here";
						}
					}
				}
			}
			else if (pil == a)//pilot
			{
				if (attack == player.pil.act)//attacks
				{
					if (1 == player.pil.target)//enemy 1
					{
						if (evade == enemyParty.e1.act && 2 == enemyParty.e1.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e1.name << " fails to dodge " << player.pil.name << "'s attack for " << pilTempAtk << " damage";
								enemyParty.e1.HP -= pilTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e1.name << " dodges " << player.pil.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.pil.name << " attacks " << enemyParty.e1.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.pil.name << " attacks " << enemyParty.e1.name << " for " << pilTempAtk << " damage";
								enemyParty.e1.HP -= pilTempAtk;
							}
						}
					}
					else if (2 == player.pil.target)
					{
						if (evade == enemyParty.e2.act && 2 == enemyParty.e2.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e2.name << " fails to dodge " << player.pil.name << "'s attack for " << pilTempAtk << " damage";
								enemyParty.e2.HP -= pilTempAtk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e2.name << " dodges " << player.pil.name << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.pil.name << " attacks " << enemyParty.e2.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.pil.name << " attacks " << enemyParty.e2.name << " for " << pilTempAtk << " damage";
								enemyParty.e2.HP -= pilTempAtk;
							}
						}
					}
					else if (3 == player.pil.target)
					{
						if (evade == enemyParty.e3.act && 2 == enemyParty.e3.target)//enemy evades
						{
							if (1 == _dice(4))//25% chance to not dodge
							{
								cout << enemyParty.e3.name << " fails to dodge " << player.pil.name << "'s attack for " << pilTempAtk << " damage";
								enemyParty.e3.HP -= player.pil.atk;
							}
							else//75% chance to dodge
							{
								cout << enemyParty.e3.name << " dodges " << pilTempAtk << "'s attack";
							}
						}
						else//enemy doesn't evade
						{
							if (1 == _dice(4))//10% chance to miss
							{
								cout << player.pil.name << " attacks " << enemyParty.e3.name << " but misses";
							}
							else//75% chance to dodge
							{
								cout << player.pil.name << " attacks " << enemyParty.e3.name << " for " << pilTempAtk << " damage";
								enemyParty.e3.HP -= pilTempAtk;
							}
						}
					}
				}
				else if (special == player.sci.act)//uses special
				{
					if (1 == player.pil.target)//on engineer
					{
						if (1 == player.pil.wep)//heals
						{
							cout << player.pil.name << " attends to " << player.eng.name << "'s wounds for 4 points of damage";
							player.eng.HP += 4;
						}
						else if (2 == player.pil.wep)
						{
							cout << player.pil.name << " optimizes the party's stats by 3%";
							player.eng.atkMulti *= 1.04;
							player.sci.atkMulti *= 1.03;
							player.pil.atkMulti *= 1.03;
						}
						else if (3 == player.pil.wep)
						{
							cout << player.pil.name << " attends to " << player.eng.name << "'s wounds for 1 points of damage";
							player.eng.HP += 1;
						}
					}
					else if (3 == player.pil.target)//on scientist
					{
						if (1 == player.pil.wep)//heals
						{
							cout << player.pil.name << " attends to " << player.sci.name << "'s wounds for 4 points of damage";
							player.sci.HP += 4;
						}
						else if (2 == player.pil.wep)
						{
							cout << player.pil.name << " optimizes the party's stats by 3%";
							player.eng.atkMulti *= 1.03;
							player.sci.atkMulti *= 1.04;
							player.pil.atkMulti *= 1.03;
						}
						else if (3 == player.pil.wep)
						{
							cout << player.pil.name << " attends to " << player.sci.name << "'s wounds for 1 points of damage";
							player.sci.HP += 1;
						}
					}
					else if (4 == player.pil.target)
					{
						if (1 == player.sci.wep)//less ship repair
						{
							cout << "Sammarco is bad at game design and didn't plan for anything to happen here";
						}
						else if (2 == player.sci.wep)//more ship repair
						{
							cout << "Sammarco is bad at game design and didn't plan for anything to happen here";
						}
						else if (3 == player.sci.wep)//no ship repair
						{
							cout << "Sammarco is bad at game design and didn't plan for anything to happen here";
						}
					}
				}
			}
			if (eng == a && player.eng.act != evade)
			{
				_getch();
			}
			else if (sci == a && player.sci.act != evade)
			{
				_getch();
			}
			else if (pil == a && player.pil.act != evade)
			{
				_getch();
			}
			switch (a)
			{
			case eng:
				a = sci;
				break;
			case sci:
				a = pil;
				break;
			case pil:
				exit = true;
			}
		}
		killAndTop(player, enemyParty);
		//enemy actions
		switch (enemyParty.e1.ID)
		{
		case 1:
			spaceGoblinActions(player, enemyParty, 1);
			break;
		}
		switch (enemyParty.e2.ID)
		{
		case 1:
			spaceGoblinActions(player, enemyParty, 2);
			break;
		}
		switch (enemyParty.e3.ID)
		{
			spaceGoblinActions(player, enemyParty, 3);
			break;
		}
		killAndTop(player, enemyParty);
		if (enemyParty.e1.dead && enemyParty.e2.dead && enemyParty.e3.dead)
		{
			//victory animation
			//also change "victory" bool
			SetConsoleTextAttribute(standard, 15);
			system("cls");
			coord.X = 32;
			coord.Y = 15;
			SetConsoleCursorPosition(standard, coord);
			cout << "victory";
			for (int a = 0; a < 6; a++)
			{
				cout << "\n";
			}
			_getch();
			return 1;
		}
		else if (player.eng.dead || player.sci.dead || player.pil.dead)
		{
			//failure
			//also change victory bool
			system("cls");
			coord.X = 25;
			coord.Y = 15;
			SetConsoleCursorPosition(standard, coord);
			cout << "a party member has died";
			coord.X = 25;
			coord.Y = 16;
			SetConsoleCursorPosition(standard, coord);
			cout << "your mission has failed";
			for (int a = 0; a < 6; a++)
			{
				cout << "\n";
			}
			_getch();
			return 0;
		}
	}
}

void printHbar(int HP, int HPmax, int length)
{
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	int Hbar = (float)HP / (float)HPmax * length;
	if (0.0 < (float)HP / (float)HPmax * length && (float)HP / (float)HPmax * length < 1.0)
	{
		Hbar++;
	}
	for (int a = 1; a <= length; a++)
	{
		if (length / 2 <= Hbar)
		{
			SetConsoleTextAttribute(color, 2);
		}
		else if (length / 4 <= Hbar)
		{
			SetConsoleTextAttribute(color, 12);
		}
		else if (length / 16 <= Hbar)
		{
			SetConsoleTextAttribute(color, 4);
		}
		if (a <= Hbar)
		{
			printf("%c", 220);
		}
		else
		{
			printf(" ");
		}
	}
	SetConsoleTextAttribute(color, 15);
}

void printHealthWindow(party& player, eparty& enemy)//player, enemy, number of enemies in eparty
{
	printf("%c", 218);
	for (int a = 1; a <= 64; a++)
	{
		printf("%c", 196);
	}
	printf("%c\n", 191);
	printf("%c", 179);
	if (1 == enemy.a)
	{
		printf("                      ");//print 22 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.e1.name.size())
			{
				cout << enemy.e1.name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("                      ");//print 22 spaces
		printf("%c\n", 179);
		printf("%c", 179);
		printf("                      ");//print 22 spaces
		printHbar(enemy.e1.HP, enemy.e1.HPmax, 20);
		printf("                      ");//print 22 spaces
		printf("%c\n", 179);
	}
	else if (2 == enemy.a)
	{
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.e1.name.size())
			{
				cout << enemy.e1.name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.e2.name.size())
			{
				cout << enemy.e2.name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//print 8 spaces
		printf("%c\n", 179);
		printf("%c", 179);
		printf("        ");//print 8 spaces
		printHbar(enemy.e1.HP, enemy.e1.HPmax, 20);
		printf("        ");//8 spaces
		printHbar(enemy.e2.HP, enemy.e1.HPmax, 20);
		printf("        ");//print 8 spaces
		printf("%c\n", 179);
	}
	else if (3 == enemy.a)
	{
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.e1.name.size())
			{
				cout << enemy.e1.name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.e2.name.size())
			{
				cout << enemy.e2.name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.e3.name.size())
			{
				cout << enemy.e3.name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.e1.HP, enemy.e1.HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.e2.HP, enemy.e1.HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.e3.HP, enemy.e3.HPmax, 20);
		printf("%c\n", 179);
	}
	printf("%c", 179);
	for (int a = 0; a < 64; a++)
	{
		if (a < enemy.ship.name.size())
		{
			cout << enemy.ship.name[a];
		}
		else
		{
			cout << " ";
		}
	}
	printf("%c\n", 179);
	printf("%c", 179);
	printHbar(enemy.ship.HP, enemy.ship.HPmax, 64);
	printf("%c\n", 179);
	printf("%c", 192);
	for (int a = 1; a <= 64; a++) { printf("%c", 196); }
	printf("%c\n", 217);
	//cout << enemy.graphic;//old ascii graphic
	pixelArtRelay(enemy.graphicID);//new pixel art system
	cout << "\n";
	//print player HP bars
	printf("%c", 218);
	for (int a = 1; a <= 64; a++)
	{
		printf("%c", 196);
	}
	printf("%c\n", 191);
	printf("%c", 179);
	for (int a = 0; a < 20; a++)
	{
		if (a < player.eng.name.size())
		{
			cout << player.eng.name[a];
		}
		else
		{
			cout << " ";
		}
	}
	cout << "  ";
	for (int a = 0; a < 20; a++)
	{
		if (a < player.sci.name.size())
		{
			cout << player.sci.name[a];
		}
		else
		{
			cout << " ";
		}
	}
	cout << "  ";
	for (int a = 0; a < 20; a++)
	{
		if (a < player.pil.name.size())
		{
			cout << player.pil.name[a];
		}
		else
		{
			cout << " ";
		}
	}
	printf("%c\n", 179);
	printf("%c", 179);
	printHbar(player.eng.HP, player.eng.HPmax, 20);
	cout << "  ";
	printHbar(player.sci.HP, player.sci.HPmax, 20);
	cout << "  ";
	printHbar(player.pil.HP, player.pil.HPmax, 20);
	printf("%c\n", 179);
	printf("%c", 179);
	for (int a = 0; a < 64; a++)
	{
		if (a < player.ship.name.size())
		{
			cout << player.ship.name[a];
		}
		else
		{
			cout << " ";
		}
	}
	printf("%c\n", 179);
	printf("%c", 179);
	printHbar(player.ship.HP, player.ship.HPmax, 64);
	printf("%c\n", 179);
	printf("%c", 192);
	for (int a = 1; a <= 64; a++)
	{
		printf("%c", 196);
	}
	printf("%c\n", 217);
}

void killAndTop(party& player, eparty& enemyParty)//use between ai and action loops
{
	if (!enemyParty.e1.dead)
	{
		if (enemyParty.e1.HP <= 0)
		{
			enemyParty.e1.dead = true;
		}
		else if (enemyParty.e1.HPmax < enemyParty.e1.HP)
		{
			enemyParty.e1.HP = enemyParty.e1.HPmax;
		}
	}
	if (!enemyParty.e2.dead)
	{
		if (enemyParty.e2.HP <= 0)
		{
			enemyParty.e2.dead = true;
		}
		else if (enemyParty.e2.HPmax < enemyParty.e2.HP)
		{
			enemyParty.e2.HP = enemyParty.e2.HPmax;
		}
	}
	if (!enemyParty.e3.dead)
	{
		if (enemyParty.e3.HP <= 0)
		{
			enemyParty.e3.dead = true;
		}
		else if (enemyParty.e3.HPmax < enemyParty.e3.HP)
		{
			enemyParty.e3.HP = enemyParty.e3.HPmax;
		}
	}

	if (!player.eng.dead)
	{
		if (player.eng.HP <= 0)
		{
			player.eng.dead = true;
		}
		else if (player.eng.HPmax < player.eng.HP)
		{
			player.eng.HP = player.eng.HPmax;
		}
	}
	if (!player.sci.dead)
	{
		if (player.sci.HP <= 0)
		{
			player.sci.dead = true;
		}
		else if (player.sci.HPmax < player.sci.HP)
		{
			player.sci.HP = player.sci.HPmax;
		}
	}
	if (!player.pil.dead)
	{
		if (player.pil.HP <= 0)
		{
			player.pil.dead = true;
		}
		else if (player.pil.HPmax < player.pil.HP)
		{
			player.pil.HP = player.pil.HPmax;
		}
	}
}


//pixel art related functions
void color(int a)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(standard, a);
	return;
}

void blocks(int a)
{
	string bb = "  ";
	bb[0] = 219;
	bb[1] = 219;
	for (int b = 0; b < a; b++)
	{
		cout << bb;
	}
	return;
}

void pixelArtRelay(int id)
{
	switch (id)
	{
	case 1:
		printSpaceGoblin();
		break;
	default:
		cout << "ERROR\n";
	}
	return;
}

void printSpaceGoblin()
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	string bb = "  ";
	bb[0] = 219;
	bb[1] = 219;
	char line = 186;
	int a;
	color(15);
	cout << line;
	for (int b = 0; b < 4; b++)
	{
		color(8);
		for (a = 0; a < 6; a++)
		{
			cout << bb;
		}
		color(7);
		cout << bb;
	}
	color(8);
	for (a = 0; a < 4; a++)
	{
		cout << bb;
	}
	color(15);
	cout << line << "\n" << line;//end of line 0
	color(8);
	for (a = 0; a < 6; a++)
	{
		cout << bb;
	}
	color(7);
	cout << bb;
	color(8);
	for (a = 0; a < 6; a++)
	{
		cout << bb;
	}
	color(7);
	cout << bb;
	color(10);
	for (a = 0; a < 4; a++)
	{
		cout << bb;
	}
	color(8);
	cout << bb << bb;
	color(7);
	cout << bb;
	color(8);
	for (a = 0; a < 6; a++)
	{
		cout << bb;
	}
	color(7);
	cout << bb;
	color(8);
	cout << bb << bb << bb << bb;
	color(15);
	cout << line << "\n" << line;//end of line 1
	color(8);
	for (a = 0; a < 6; a++)
	{
		cout << bb;
	}
	color(7);
	cout << bb;
	color(8);
	cout << bb << bb << bb;
	color(2);
	cout << bb << bb;
	color(8);
	cout << bb;
	color(10);
	for (a = 0; a < 6; a++)
	{
		cout << bb;
	}
	color(8);
	cout << bb;
	color(2);
	cout << bb << bb;
	color(8);
	blocks(5);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(15);
	cout << line << "\n" << line;//end of line 2
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(2);
	blocks(2);
	color(10);
	cout << bb;
	color(4);
	cout << bb;
	color(10);
	blocks(2);
	color(4);
	cout << bb;
	color(10);
	cout << bb;
	color(2);
	blocks(2);
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(15);
	cout << line << "\n" << line;//end of line 3
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(2);
	blocks(2);
	color(10);
	blocks(2);
	color(2);
	blocks(2);
	color(10);
	blocks(2);
	color(2);
	blocks(2);
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(15);
	cout << line << "\n" << line;//end of line 4
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(5);
	color(2);
	cout << bb;
	color(10);
	blocks(2);
	color(2);
	blocks(2);
	color(10);
	blocks(2);
	color(2);
	cout << bb;
	color(7);
	cout << bb;
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(15);
	cout << line << "\n" << line;//end of line 5
	color(8);
	blocks(7);
	color(7);
	cout << bb;
	color(8);
	blocks(5);
	color(10);
	blocks(6);
	color(8);
	blocks(2);
	color(7);
	cout << bb;
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(3);
	color(15);
	cout << line << "\n" << line;//end of line 6
	color(8);
	blocks(8);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(10);
	blocks(2);
	color(2);
	blocks(2);
	color(10);
	blocks(2);
	color(8);
	blocks(3);
	color(7);
	cout << bb;
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	blocks(2);
	color(15);
	cout << line << "\n" << line;//end of line 7
	color(8);
	blocks(9);
	color(7);
	cout << bb;
	color(8);
	blocks(4);
	color(10);
	blocks(4);
	color(8);
	blocks(5);
	color(7);
	cout << bb;
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	cout << bb;
	color(15);
	cout << line << "\n" << line;//end of line 8
	color(8);
	blocks(9);
	color(7);
	cout << bb;
	color(8);
	cout << bb;
	color(2);
	blocks(4);
	color(10);
	blocks(2);
	color(2);
	blocks(4);
	color(8);
	blocks(2);
	color(7);
	cout << bb;
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	cout << bb;
	color(15);
	cout << line << "\n" << line;//end of line 9
	color(8);
	blocks(9);
	color(7);
	cout << bb;
	color(2);
	blocks(12);
	color(8);
	cout << bb;
	color(7);
	cout << bb;
	color(8);
	blocks(6);
	color(7);
	cout << bb;
	color(8);
	cout << bb;
	color(15);
	cout << line;//end of line 10
	return;
}


//enemy specific combat related functions
void spaceGoblinAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	action temp;
	int tempTarget;
	int random = _dice(100);
	if (turnNum % 3 == 0)//higher chance to attack every 3 turns
	{
		if (turnNum % 4 == 0)//chance to heal every 4 turns
		{
			if (1 <= random && random <= 10)//10 percent evade chance
			{
				temp = evade;
			}
			else if (11 <= random && random <= 60)//50 percent attack chance
			{
				temp = attack;
			}
			else if (61 <= random && random <= 100)//40 percent heal chance
			{
				temp = special;
			}
		}
		else//else no heal chance
		{
			if (1 <= random && random <= 20)//20 percent evade chance
			{
				temp = evade;
			}
			else if (21 <= random && random <= 100)//80 percent attack chance
			{
				temp = attack;
			}
		}
	}
	else//else higher chance to evade
	{
		if (turnNum % 4 == 0)//chance to heal every 4 turns
		{
			if (1 <= random && random <= 10)//10 percent attack chance
			{
				temp = attack;
			}
			else if (11 <= random && random <= 60)//50 percent evade chance
			{
				temp = evade;
			}
			else if (61 <= random && random <= 100)//40 percent heal chance
			{
				temp = special;
			}
		}
		else//else no heal chance
		{
			if (1 <= random && random <= 20)//20 percent atack chance
			{
				temp = attack;
			}
			else if (21 <= random && random <= 100)//80 percent evade chance
			{
				temp = evade;
			}
		}
	}
	switch (a)
	{
	case 0:
		enemyParty.e1.act = temp;
		break;
	case 1:
		enemyParty.e2.act = temp;
		break;
	case 2:
		enemyParty.e3.act = temp;
	}
	if (attack == temp)
	{
		int randAttack = _dice(player.eng.HP + player.sci.HP + player.pil.HP);
		if (randAttack <= player.eng.HP)//attack engineer
		{
			tempTarget = 1;
		}
		else if (player.eng.HP < randAttack && randAttack <= player.eng.HP + player.sci.HP)//attack scientist
		{
			tempTarget = 2;
		}
		else if (player.eng.HP + player.sci.HP < randAttack && randAttack <= player.eng.HP + player.sci.HP + player.pil.HP)//attack pilot
		{
			tempTarget = 3;
		}
	}
	else if (special == temp)
	{
		if (enemyParty.e1.HP != enemyParty.e1.HPmax && !enemyParty.e1.dead)
		{
			if (enemyParty.e2.HP != enemyParty.e2.HPmax && !enemyParty.e2.dead)
			{
				if (enemyParty.e3.HP != enemyParty.e3.HPmax && !enemyParty.e3.dead)//123
				{
					tempTarget = _dice(3);
				}
				else//12
				{
					tempTarget = _dice(2);
				}
			}
			else if (enemyParty.e3.HP != enemyParty.e3.HPmax && !enemyParty.e3.dead)//13
			{
				tempTarget = _dice(2);
				if (2 == tempTarget)
				{
					tempTarget++;
				}
			}
			else//1
			{
				tempTarget = 1;
			}
		}
		else if (enemyParty.e2.HP != enemyParty.e2.HPmax && !enemyParty.e2.dead)
		{
			if (enemyParty.e3.HP != enemyParty.e3.HPmax && !enemyParty.e3.dead)//23
			{
				tempTarget = _dice(2) + 1;
			}
			else//2
			{
				tempTarget = 2;
			}
		}
		else if (enemyParty.e3.HP != enemyParty.e3.HPmax && !enemyParty.e3.dead)//3
		{
			tempTarget = 3;
		}
		else//choose randomly
		{
			tempTarget = _dice(3);
		}
	}
	else if (evade == temp)
	{
		tempTarget = _dice(3);
	}
	switch (a)
	{
	case 0:
		enemyParty.e1.target = tempTarget;
		break;
	case 1:
		enemyParty.e2.target = tempTarget;
		break;
	case 2:
		enemyParty.e3.target = tempTarget;
	}
	return;
}

void spaceGoblinActions(party& player, eparty& enemyParty, int e)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	SetConsoleTextAttribute(standard, 15);
	coord.X = 0;
	coord.Y = 29;
	SetConsoleCursorPosition(standard, coord);
	for (int a = 0; a < 80; a++)
	{
		cout << " ";
	}
	coord.X = 0;
	coord.Y = 29;
	SetConsoleCursorPosition(standard, coord);
	int e1TempAtk = (float)enemyParty.e1.atk * enemyParty.e1.atkMulti;
	int e2TempAtk = (float)enemyParty.e2.atk * enemyParty.e2.atkMulti;
	int e3TempAtk = (float)enemyParty.e3.atk * enemyParty.e3.atkMulti;
	if (1 == e && !enemyParty.e1.dead)//enemy 1
	{
		if (attack == enemyParty.e1.act)//attacks 
		{
			if (1 == enemyParty.e1.target)//eng
			{
				if (evade == player.eng.act && 1 == player.eng.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.eng.name << " fails to dodge " << enemyParty.e1.name << "'s attack for " << e1TempAtk << " damage";
						player.eng.HP -= e1TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.eng.name << " dodges " << enemyParty.e1.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e1.name << " attacks " << player.eng.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e1.name << " attacks " << player.eng.name << " for " << e1TempAtk << " damage";
						player.eng.HP -= e1TempAtk;
					}
				}
			}
			else if (2 == enemyParty.e1.target)//sci
			{
				if (evade == player.sci.act && 1 == player.sci.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.sci.name << " fails to dodge " << enemyParty.e1.name << "'s attack for " << e1TempAtk << " damage";
						player.sci.HP -= e1TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.sci.name << " dodges " << enemyParty.e1.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e1.name << " attacks " << player.sci.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e1.name << " attacks " << player.sci.name << " for " << e1TempAtk << " damage";
						player.sci.HP -= e1TempAtk;
					}
				}
			}
			else if (3 == enemyParty.e1.target)//pil
			{
				if (evade == player.pil.act && 1 == player.pil.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.pil.name << " fails to dodge " << enemyParty.e1.name << "'s attack for " << e1TempAtk << " damage";
						player.pil.HP -= e1TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.pil.name << " dodges " << enemyParty.e1.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e1.name << " attacks " << player.pil.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e1.name << " attacks " << player.pil.name << " for " << e1TempAtk << " damage";
						player.pil.HP -= e1TempAtk;
					}
				}
			}
			else
			{
				cout << "ERROR";
			}
		}
		else if (special == enemyParty.e1.act)//uses special on
		{
			if (1 == enemyParty.e1.target)//goblin targeting itself increases it's attack multiplier
			{
				cout << enemyParty.e1.name << " gives its ray gun a smack, causing it to surge with energy";
				enemyParty.e1.atkMulti *= 1.1;
			}
			else if (2 == enemyParty.e1.target)
			{
				cout << enemyParty.e1.name << " tends to " << enemyParty.e2.name << "'s wounds for 3 health";
				enemyParty.e2.HP += 3;
			}
			else if (3 == enemyParty.e1.target)
			{
				cout << enemyParty.e1.name << " tends to " << enemyParty.e3.name << "'s wounds for 3 health";
				enemyParty.e3.HP += 3;
			}
			else
			{
				cout << "ERROR";
			}
		}
	}
	else if (2 == e && !enemyParty.e2.dead)//enemy 2
	{
		if (attack == enemyParty.e2.act)//attacks 
		{
			if (1 == enemyParty.e2.target)//eng
			{
				if (evade == player.eng.act && 1 == player.eng.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.eng.name << " fails to dodge " << enemyParty.e2.name << "'s attack for " << e2TempAtk << " damage";
						player.eng.HP -= e2TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.eng.name << " dodges " << enemyParty.e2.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e2.name << " attacks " << player.eng.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e2.name << " attacks " << player.eng.name << " for " << e2TempAtk << " damage";
						player.eng.HP -= e2TempAtk;
					}
				}
			}
			else if (2 == enemyParty.e2.target)//sci
			{
				if (evade == player.sci.act && 1 == player.sci.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.sci.name << " fails to dodge " << enemyParty.e2.name << "'s attack for " << e2TempAtk << " damage";
						player.sci.HP -= e2TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.sci.name << " dodges " << enemyParty.e2.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e2.name << " attacks " << player.sci.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e2.name << " attacks " << player.sci.name << " for " << e2TempAtk << " damage";
						player.sci.HP -= e2TempAtk;
					}
				}
			}
			else if (3 == enemyParty.e2.target)//pil
			{
				if (evade == player.pil.act && 1 == player.pil.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.pil.name << " fails to dodge " << enemyParty.e2.name << "'s attack for " << e2TempAtk << " damage";
						player.pil.HP -= e2TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.pil.name << " dodges " << enemyParty.e2.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e2.name << " attacks " << player.pil.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e2.name << " attacks " << player.pil.name << " for " << e2TempAtk << " damage";
						player.pil.HP -= e2TempAtk;
					}
				}
			}
			else
			{
				cout << "ERROR";
			}
		}
		else if (special == enemyParty.e2.act)//uses special on
		{
			if (1 == enemyParty.e2.target)//goblin targeting itself increases it's attack multiplier
			{
				cout << enemyParty.e2.name << " tends to " << enemyParty.e1.name << "'s wounds for 3 health";
				enemyParty.e1.HP += 3;
			}
			else if (2 == enemyParty.e2.target)
			{
				cout << enemyParty.e2.name << " gives its ray gun a smack, causing it to surge with energy";
				enemyParty.e2.atkMulti *= 1.1;
			}
			else if (3 == enemyParty.e2.target)
			{
				cout << enemyParty.e2.name << " tends to " << enemyParty.e3.name << "'s wounds for 3 health";
				enemyParty.e3.HP += 3;
			}
			else
			{
				cout << "ERROR";
			}
		}
	}
	else if (3 == e && !enemyParty.e3.dead)//enemy 3
	{
		if (attack == enemyParty.e3.act)//attacks 
		{
			if (1 == enemyParty.e3.target)//eng
			{
				if (evade == player.eng.act && 1 == player.eng.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.eng.name << " fails to dodge " << enemyParty.e3.name << "'s attack for " << e3TempAtk << " damage";
						player.eng.HP -= e3TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.eng.name << " dodges " << enemyParty.e3.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e3.name << " attacks " << player.eng.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e3.name << " attacks " << player.eng.name << " for " << e3TempAtk << " damage";
						player.eng.HP -= e3TempAtk;
					}
				}
			}
			else if (2 == enemyParty.e3.target)//sci
			{
				if (evade == player.sci.act && 1 == player.sci.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.sci.name << " fails to dodge " << enemyParty.e3.name << "'s attack for " << e3TempAtk << " damage";
						player.sci.HP -= e3TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.sci.name << " dodges " << enemyParty.e3.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e3.name << " attacks " << player.sci.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e3.name << " attacks " << player.sci.name << " for " << e3TempAtk << " damage";
						player.sci.HP -= e3TempAtk;
					}
				}
			}
			else if (3 == enemyParty.e3.target)//pil
			{
				if (evade == player.pil.act && 1 == player.pil.target)//engineer evades
				{
					if (1 == _dice(4))//25% chance to not dodge
					{
						cout << player.pil.name << " fails to dodge " << enemyParty.e3.name << "'s attack for " << e3TempAtk << " damage";
						player.pil.HP -= e3TempAtk;
					}
					else//75% chance to dodge
					{
						cout << player.pil.name << " dodges " << enemyParty.e3.name << "'s attack";
					}
				}
				else//engineer does not evade
				{
					if (1 == _dice(4))//25% chance to miss
					{
						cout << enemyParty.e3.name << " attacks " << player.pil.name << " but misses";
					}
					else//75% chance to not miss
					{
						cout << enemyParty.e3.name << " attacks " << player.pil.name << " for " << e3TempAtk << " damage";
						player.pil.HP -= e3TempAtk;
					}
				}
			}
			else
			{
				cout << "ERROR";
			}
		}
		else if (special == enemyParty.e3.act)//uses special on
		{
			if (1 == enemyParty.e1.target)//goblin targeting itself increases it's attack multiplier
			{
				cout << enemyParty.e3.name << " tends to " << enemyParty.e1.name << "'s wounds for 3 health";
				enemyParty.e1.HP += 3;
			}
			else if (2 == enemyParty.e1.target)
			{
				cout << enemyParty.e3.name << " tends to " << enemyParty.e2.name << "'s wounds for 3 health";
				enemyParty.e2.HP += 3;
			}
			else if (3 == enemyParty.e1.target)
			{
				cout << enemyParty.e3.name << " gives its ray gun a smack, causing it to surge with energy";
				enemyParty.e3.atkMulti *= 1.1;
			}
			else
			{
				cout << "ERROR";
			}
		}
	}
	if (1 == e && evade != enemyParty.e1.act)
	{
		_getch();
	}
	else if (2 == e && evade != enemyParty.e2.act)
	{
		_getch();
	}
	else if (3 == e && evade != enemyParty.e3.act)
	{
		_getch();
	}
	return;
}
