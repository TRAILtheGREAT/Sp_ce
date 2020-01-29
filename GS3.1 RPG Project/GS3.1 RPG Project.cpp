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
//12-13-19, starting work on pokemon-esque move system
	//each move has an id number, make a function that takes the id and returns a string containing the name of the move (for displaying moves in the combat menue)
	//each move also has a void function that carries out the action. this function is accessed through a switch (moveID) containing all of the moves
	//need to update enemies attacking characters to be based on player's defensive move
	//update health window to remove the ship hp and allow for up to 6 enemies
		//will also need to update combat menu to include more enemies
		//and update enemy AI and actions to work with larger parties
	//need to put player characters in a vector - Done? - Done!
	//move evade to lower right of special menu (instead of ship), escape will become Info
		//Info will display: Ship HP, Credits, Items
		//display in window on the right side (54 x 25)
	//create NPCs
		//for now, NPCs should introduce themselves and allow the player to purchase a move for one of their characters

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

enum turn { eng, sci, pil };
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
	int HP, HPmax, atk;
	bool dead;
	action act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;//damage multiplier
	int attackID, specialID, defenceID;
	int defenceValue;
};
struct vessel
{
	string name;
	int HP, HPmax;
	action act;
};
struct party
{
	vector <crew> cVect;
	vessel ship;
	int projectsPrinted;//used for print project attack
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
	int dodgeValue;//add to dodge chance when evading/blocking
};
struct eparty
{
	vector <enemy> eVect;
	int a;//number of enemies in the party that need to be displayed
	int credits;
	int graphicBackID, graphicForID;
};

void displayMap(bool(&VdoorArray)[4][5], bool(&HdoorArray)[5][4], int C, int R);
int _dice(int max);
bool encounter(party& player, int encounterID, int& credits, vector <eparty> enemyVector);
int roundUp(int number, int devisor);
void printHbar(int HP, int HPmax, int length);
void printHealthWindow(party& player, eparty& enemy);//player, enemy, number of enemies in eparty
char _getdir();
void killAndTop(party& player, eparty& enemyParty);
string getMoveName(action type, int ID);
bool combat(party& player, eparty enemyParty, int& credits);

void color(int a);
void blocks(int a);
void pixelArtRelay(int backID, int forID);
void printCombatGraphic(string image, int Y);
void printImage(string image, int Y);

void spaceGoblinAI(party& player, eparty& enemyParty, int a, int turnNum);
void spaceGoblinActions(party& player, eparty& enemyParty, int e);
void squidAI(party& player, eparty& enemyParty, int a, int turnNum);
void squidActions(party& player, eparty& enemyParty, int e);
void droneAI(party& player, eparty& enemyParty, int a, int turnNum);
void droneActions(party& player, eparty& enemyParty, int e);
void droneBrainAI(party& player, eparty& enemyParty, int a, int turnNum);
void droneBrainActions(party& player, eparty& enemyParty, int e);

void largeStapelerAttack(party& player, eparty& enemyParty, int p);
void basicAttack(party& player, eparty& enemyParty, int p);
void basicHeal(party& player, eparty& enemyParty, int p);
void basicEvade(party& player, eparty& enemyParty, int p);

int main()
{
	srand((unsigned)time(NULL));
	cout << "LOADING...";
	//realy wish i could use file IO
	//easy enemies: space Golbin, Ooze, spider drone, squid
	//medium enemies: zombie, rock man
	//hard enemy: astroid worm
	eparty tempParty;
	enemy tempEnemy;
	vector <eparty> enemyVector;
	//space Goblin Party of 6
	tempEnemy.name = "Tam";
	tempEnemy.atk = 3;
	tempEnemy.atkMulti = 1.0;
	tempEnemy.HP = 9;
	tempEnemy.HPmax = 9;
	tempEnemy.dodgeValue = 10;
	tempEnemy.dead = false;
	tempEnemy.ID = 0;
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Tem";
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Tim";
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Tom";
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Tum";
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Tym";
	tempParty.eVect.push_back(tempEnemy);
	tempParty.graphicBackID = 0;
	tempParty.graphicForID = 0;
	tempParty.a = 6;
	enemyVector.push_back(tempParty);
	//squid party of 3
	tempParty.eVect.clear();
	tempEnemy.name = "Squid";
	tempEnemy.atk = 4;
	tempEnemy.atkMulti = 1.0;
	tempEnemy.HP = 8;
	tempEnemy.HPmax = 8;
	tempEnemy.dodgeValue = 30;
	tempEnemy.dead = false;
	tempEnemy.ID = 1;
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Squidy";
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "Squiddidily";
	tempParty.eVect.push_back(tempEnemy);
	tempParty.graphicBackID = 2;
	tempParty.graphicForID = 2;
	tempParty.a = 3;
	enemyVector.push_back(tempParty);
	//drone swarm (4 drones and one stronger brain drone)
	tempParty.eVect.clear();
	tempEnemy.name = "alpha drone";
	tempEnemy.atk = 2;
	tempEnemy.atkMulti = 1.0;
	tempEnemy.HP = 4;
	tempEnemy.HPmax = 4;
	tempEnemy.dodgeValue = 30;
	tempEnemy.dead = false;
	tempEnemy.ID = 2;
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "omega brain";
	tempEnemy.atk = 8;
	tempEnemy.atkMulti = 1.0;
	tempEnemy.HP = 20;
	tempEnemy.HPmax = 4;
	tempEnemy.dodgeValue = 30;
	tempEnemy.dead = false;
	tempEnemy.ID = 3;
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "beta drone";
	tempEnemy.atk = 2;
	tempEnemy.atkMulti = 1.0;
	tempEnemy.HP = 4;
	tempEnemy.HPmax = 4;
	tempEnemy.dodgeValue = 30;
	tempEnemy.dead = false;
	tempEnemy.ID = 2;
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "gamma drone";
	tempParty.eVect.push_back(tempEnemy);
	tempEnemy.name = "delta drone";
	tempParty.eVect.push_back(tempEnemy);
	tempParty.graphicBackID = 2;
	tempParty.graphicForID = 3;
	tempParty.a = 5;
	enemyVector.push_back(tempParty);
	system("cls");
	printf("Hello!\nMy name is Trail Sammarco, the creator of this game.\nMiyoshi says that introductions should be written last so that's all I'll say about that.\n\n");
	printf("You are in control of a group of 3 cosmonauts.\n");
	printf("After a terrible accident they are stranded in a dangerous region of the galaxy thousands of lightyears from home.\n");
	printf("They will need to fight through the galaxy's most dangerous foes to get them home and find out the truth about what\nhappened in...\n\n");
	_getch();
	system("cls");
	pixelArtRelay(1, 1);
	_getch();
	system("cls");
	party player;
	crew tempCrew;
	player.cVect.push_back(tempCrew);
	player.cVect.push_back(tempCrew);
	player.cVect.push_back(tempCrew);
	printf("your first crew member is the engineer\nWhat is their name?\n");
	getline(cin, player.cVect[0].name);
	printf("your second crew member is the scientist\nWhat is their name?\n");
	getline(cin, player.cVect[1].name);
	printf("the last crew member is the pilot\nWhat is their name?\n");
	getline(cin, player.cVect[2].name);
	printf("generating random health pools...\n");
	int crewMin = 9; //using variables in case I want to add difficulty modes later
	int crewMax = 15;
	player.cVect[0].HP = crewMin + rand() % (crewMax - crewMin + 1) + 3; //engi has more HP, sci has less
	player.cVect[1].HP = crewMin + rand() % (crewMax - crewMin + 1) - 2;
	player.cVect[2].HP = crewMin + rand() % (crewMax - crewMin + 1);
	player.cVect[0].HPmax = player.cVect[0].HP;
	player.cVect[0].dead = false;
	player.cVect[1].HPmax = player.cVect[1].HP;
	player.cVect[1].dead = false;
	player.cVect[2].HPmax = player.cVect[2].HP;
	player.cVect[2].dead = false;
	int shipMin = 15;
	int shipMax = 25;
	player.ship.HP = 15 + rand() % (25 - 15 + 1);
	player.ship.HPmax = player.ship.HP;
	printf("%s the engineer: %i HP\n", player.cVect[0].name.c_str(), player.cVect[0].HP);
	printf("%s the scientist: %i HP\n", player.cVect[1].name.c_str(), player.cVect[1].HP);
	printf("%s the pilot: %i HP\n", player.cVect[2].name.c_str(), player.cVect[2].HP);

	printf("%s, %s, and %s have found themselves stranded on an abandoned asteroid mining station\n", player.cVect[0].name.c_str(), player.cVect[1].name.c_str(), player.cVect[2].name.c_str());
	printf("they find an old mining vesslewith a bit of fuel still in the tank\n");
	printf("what do they name the ship?\n");
	getline(cin, player.ship.name);
	printf("after %s does some repairs, %s is ready to fly\n", player.cVect[0].name.c_str(), player.ship.name.c_str());
	printf("%s has %i HP\n[press any key to continue]\n", player.ship.name.c_str(), player.ship.HP);
	_getch();
	system("cls");
	printf("there are some items left in the lockers that the miners left behind\n");
	printf("the crew scavenges what they can\n");
	player.cVect[0].attackID = 0;
	player.cVect[0].defenceID = 0;
	player.cVect[0].specialID = 0;
	player.cVect[1].attackID = 1;
	player.cVect[1].defenceID = 0;
	player.cVect[1].specialID = 0;
	player.cVect[2].attackID = 0;
	player.cVect[2].defenceID = 0;
	player.cVect[2].specialID = 0;
	char inputc;
	bool exit = false;
	// JoeJoeJoeBob
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
	//navigation/main game loop
	while (!exit)
	{
		system("cls");
		
		printf("you arive in sector (%i, %i), %s\n", playerC + 1, playerR + 1, roomArray[playerC][playerR].name.c_str());
		bool victory = encounter(player, roomArray[playerC][playerR].encounterID, credits, enemyVector);
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

bool encounter(party &player, int encounterID, int &credits, vector <eparty> enemyVector)//this is why we use structs
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
		case 1://spaceGoblins
			cout << "a haphazardly constructed vessel suddenly careens into the ship's starbord side" << endl;
			cout << "rushing to assess the damage, your crew stumbles upon a band of three raggedy goblins ravaging the crew quarters" << endl;
			_getch();
			combat(player, enemyVector[0], credits);
			break;
		case 2:
			cout << "your crew exits warp straight into an astroid field" << endl;
			cout << "they begin to get an uneasy feeling when a breach is detected and tentacled creatures can be seen creeping into the ship" << endl;
			_getch();
			combat(player, enemyVector[1], credits);
			break;
		case 3:
			cout << "lazers flash across the sky in this new sector" << endl;
			cout << "a squad of armed drones spot your ship and move in to attack" << endl;
			_getch();
			combat(player, enemyVector[2], credits);
			break;
		}
		return 1;
	}
	else if (3 == encounterID)//medium difficulty enemies
	{
		switch (_dice(2))
		{
		case 1:
			cout << "your crew exits warp straight into an astroid field" << endl;
			cout << "they begin to get an uneasy feeling when a breach is detected and tentacled creatures can be seen creeping into the ship" << endl;
			_getch();
			combat(player, enemyVector[1], credits);
			break;
		case 2:
			cout << "lazers flash across the sky in this new sector" << endl;
			cout << "a squad of armed drones spot your ship and move in to attack" << endl;
			_getch();
			combat(player, enemyVector[2], credits);
			break;
		}
		return 1;
	}
	else if (4 == encounterID)//hard difficulty enemies
	{
		cout << "hard difficulty enemy";
		_getch();
		return 1;
	}
	else if (5 == encounterID)//shop
	{
		cout << "shop";
		_getch();
		return 1;
	}
	else if (6 == encounterID)//npc 1
	{
		cout << "first NPC";
		_getch();
		return 1;
	}
	else if (7 == encounterID)//npc 2
	{
		cout << "second NPC\n";
		_getch();
		return 1;
	}
	else if (8 == encounterID)//npc 3
	{
		cout << "third NPC\n";
		_getch();
		return 1;
	}
	else if (9 == encounterID)//treasure trove
	{
		cout << "treasure\n";
		_getch();
		return 1;
	}
}


//combat related functions
bool combat(party& player, eparty enemyParty, int &credits)
{
	system("cls");
	bool DEBUG = false;
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	player.cVect[0].atkMulti = 1.0;
	player.cVect[1].atkMulti = 1.0;
	player.cVect[2].atkMulti = 1.0;
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
				currentChar = player.cVect[0].name;
				break;
			case sci:
				currentChar = player.cVect[1].name;
				break;
			case pil:
				currentChar = player.cVect[2].name;
				break;
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
			string attackMove;//name of current character's attack
			switch (pTurn)
			{
			case eng:
				attackMove = getMoveName(attack, player.cVect[0].attackID);
				break;
			case sci:
				attackMove = getMoveName(attack, player.cVect[1].attackID);
				break;
			case pil:
				attackMove = getMoveName(attack, player.cVect[2].attackID);
			}
			string defenceMove;//name of current character's defence
			switch (pTurn)
			{
			case eng:
				defenceMove = getMoveName(evade, player.cVect[0].defenceID);
				break;
			case sci:
				defenceMove = getMoveName(evade, player.cVect[1].defenceID);
				break;
			case pil:
				defenceMove = getMoveName(evade, player.cVect[2].defenceID);
			}
			string specialMove;//name of current character's special
			switch (pTurn)
			{
			case eng:
				specialMove = getMoveName(special, player.cVect[0].specialID);
				break;
			case sci:
				specialMove = getMoveName(special, player.cVect[1].specialID);
				break;
			case pil:
				specialMove = getMoveName(special, player.cVect[2].specialID);
			}
			for (int a = 0; a < 32; a++)//print attack
			{
				if (a < attackMove.size())
				{
					cout << attackMove[a];
				}
				else
				{
					cout << " ";
				}
			}
			for (int a = 0; a < 32; a++)//print defence
			{
				if (a < defenceMove.size())
				{
					cout << defenceMove[a];
				}
				else
				{
					cout << " ";
				}
			}
			printf("%c\n", 179);
			printf("%c", 179);
			for (int a = 0; a < 64; a++) { cout << " "; }
			printf("%c\n", 179);
			printf("%c", 179);
			for (int a = 0; a < 32; a++)//prints special
			{
				if (a < specialMove.size())
				{
					cout << specialMove[a];
				}
				else
				{
					cout << " ";
				}
			}
			for (int a = 0; a < 32; a++)
			{
				string temp = "Info";
				if (a < temp.size())
				{
					cout << temp[a];
				}
				else
				{
					cout << " ";
				}
			}
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
				if (1 == menu.X && 1 == menu.Y)//attack
				{
					coord.X = 1;
					coord.Y = 25;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << attackMove;
				}
				else if (1 == menu.X && 2 == menu.Y)//special
				{
					coord.X = 1;
					coord.Y = 27;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << specialMove;
				}
				else if (2 == menu.X && 1 == menu.Y)//defence
				{
					coord.X = 33;
					coord.Y = 25;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << defenceMove;
				}
				else if (2 == menu.X && 2 == menu.Y)//escape
				{
					coord.X = 33;
					coord.Y = 27;
					SetConsoleCursorPosition(standard, coord);
					SetConsoleTextAttribute(color, 4);
					cout << "Info";
				}
				//color past position
				if (menu.X != menuPast.X || menu.Y != menuPast.Y)
				{
					if (1 == menuPast.X && 1 == menuPast.Y)//attack
					{
						coord.X = 1;
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << attackMove;
					}
					else if (1 == menuPast.X && 2 == menuPast.Y)//special
					{
						coord.X = 1;
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << specialMove;
					}
					else if (2 == menuPast.X && 1 == menuPast.Y)//defence
					{
						coord.X = 33;
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << defenceMove;
					}
					else if (2 == menuPast.X && 2 == menuPast.Y)//escape
					{
						coord.X = 33;
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						SetConsoleTextAttribute(color, 15);
						cout << "Info";
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
						/*
						enemy1    enemy2
						enemy3    enemy4
						enemy5    enemy6
						Back
						*/
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
							else if ((currentChar.length() + 1) < a && a < currentChar.length() + attackMove.size() + 2)// if character length plus one
							{
								cout << attackMove[a - (currentChar.length() + 2)];
							}
							else
							{
								printf("%c", 196);
							}
						}
						printf("%c\n", 191);//upper right corner
						printf("%c", 179);
						if (1 <= enemyParty.a)//will always run, but whatever
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[0].name.length())
								{
									cout << enemyParty.eVect[0].name[a];
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
								if (a <= enemyParty.eVect[1].name.length())
								{
									cout << enemyParty.eVect[1].name[a];
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
						if (3 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[2].name.length())
								{
									cout << enemyParty.eVect[2].name[a];
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
						if (4 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[3].name.length())
								{
									cout << enemyParty.eVect[3].name[a];
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
						if (5 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[4].name.length())
								{
									cout << enemyParty.eVect[4].name[a];
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
						if (6 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[5].name.length())
								{
									cout << enemyParty.eVect[5].name[a];
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
						cout << "BACK";
						for (int a = 0; a < 60; a++)
						{
							cout << " ";
						}
						//menu navigation
						menu.X = 1;
						menu.Y = 1;
						menuPast.X = 1;
						menuPast.Y = 1;
						bool attackReprint = false;
						while (!attackReprint)
						{
							/*
							(1, 1) == (1, 24)
							(1, 2) == (1, 25)
							(1, 3) == (1, 26)
							(1, 4) == (1, 27)/Back
							(2, 1) == (33, 24)
							(2, 2) == (33, 25)
							(2, 3) == (33, 26)
							*/
							//color menu position
							if (1 == menu.X && 1 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 24;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[0].name;
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[2].name;
							}
							else if (1 == menu.X && 3 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[4].name;
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 24;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[1].name;
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[3].name;
							}
							else if (2 == menu.X && 3 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[5].name;
							}
							else if (4 == menu.Y)
							{
								coord.X = 1;
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
									coord.Y = 24;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[0].name;
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[2].name;
								}
								else if (1 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[4].name;
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 24;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[1].name;
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[3].name;
								}
								else if (2 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[5].name;
								}
								else if (4 == menuPast.Y)
								{
									coord.X = 1;
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
								if (1 != menu.Y)
								{
									menu.Y--;
								}
							}
							else if ('r' == inputc)
							{
								if (2 != menu.X && 4 != menu.Y)
								{
									menu.X++;
								}
							}
							else if ('d' == inputc)
							{
								if (4 != menu.Y)
								{
									menu.Y++;
								}
							}
							else if ('l' == inputc)
							{
								if (1 != menu.X && 4 != menu.Y)
								{
									menu.X--;
								}
							}
							//fix position based on number of enemies
							if (1 == enemyParty.a)
							{
								if ('d' == inputc)
								{
									menu.Y = 4;
								}
								else if ('u' == inputc)
								{
									menu.Y = 1;
								}
								if (2 == menu.X)
								{
									menu.X = 1;
								}
							}
							else if (2 == enemyParty.a)
							{
								if ('d' == inputc)
								{
									menu.Y = 4;
								}
								else if ('u' == inputc)
								{
									menu.Y = 1;
									menu.X = 1;
								}
							}
							else if (3 == enemyParty.a)
							{
								if (2 == menu.X && 2 == menu.Y)
								{
									if ('d' == inputc)
									{
										menu.X = 1;
										menu.Y = 2;
									}
									else if ('r' == inputc)
									{
										menu.X = 2;
										menu.Y = 1;
									}
								}
								else if (3 == menu.Y)
								{
									if ('u' == inputc)
									{
										menu.X = 1;
										menu.Y = 2;
									}
									else if ('d' == inputc)
									{
										menu.Y = 4;
									}
								}
							}
							else if (4 == enemyParty.a)
							{
								if (3 == menu.Y)
								{
									if ('u' == inputc)
									{
										menu.X = 1;
										menu.Y = 2;
									}
									else if ('d' == inputc)
									{
										menu.Y = 4;
									}
								}
							}
							else if (5 == enemyParty.a)
							{
								if (3 == menu.Y)
								{
									menu.X = 1;
								}
							}
							//record inputs
							if (13 == inputc)
							{
								if (1 == menu.X && 1 == menu.Y)//e1
								{
									player.cVect[pTurn].act = attack;
									player.cVect[pTurn].target = 0;
									attackReprint = true;
								}
								else if (1 == menu.X && 2 == menu.Y)//e3
								{
									player.cVect[pTurn].act = attack;
									player.cVect[pTurn].target = 2;
									attackReprint = true;
								}
								else if (1 == menu.X && 3 == menu.Y)//e5
								{
									player.cVect[pTurn].act = attack;
									player.cVect[pTurn].target = 4;
									attackReprint = true;
								}
								else if (2 == menu.X && 1 == menu.Y)//e2
								{
									player.cVect[pTurn].act = attack;
									player.cVect[pTurn].target = 1;
									attackReprint = true;
								}
								else if (2 == menu.X && 2 == menu.Y)//e4
								{
									player.cVect[pTurn].act = attack;
									player.cVect[pTurn].target = 3;
									attackReprint = true;
								}
								else if (2 == menu.X && 3 == menu.Y)//e6
								{
									player.cVect[pTurn].act = attack;
									player.cVect[pTurn].target = 5;
									attackReprint = true;
								}
								else if (4 == menu.Y)//BACK
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
							else if ((currentChar.length() + 1) < a && a < currentChar.length() + specialMove.size() + 2)// if character length plus one
							{
								cout << specialMove[a - (currentChar.length() + 2)];
							}
							else
							{
								printf("%c", 196);
							}
						}
						printf("%c\n\n", 191);//upper right corner
						printf("%c", 179);
						/*
						+----engineer---------------+
						|                           |
						|scientist        pilot     |
						|                           |
						|Back             Escape    |
						+---------------------------+
						+----scientist--------------+
						|                           |
						|engineer         pilot     |
						|                           |
						|Back             Escape    |
						+---------------------------+
						+----pilot------------------+
						|                           |
						|engineer         scientist |
						|                           |
						|Back             Escape    |
						+---------------------------+
						*/
						string firstMember, secondMember;
						//temporary
						if (eng == pTurn)
						{
							firstMember = player.cVect[1].name;
							secondMember = player.cVect[2].name;
						}
						else if (sci == pTurn)
						{
							firstMember = player.cVect[0].name;
							secondMember = player.cVect[2].name;
						}
						else if (pil == pTurn)
						{
							firstMember = player.cVect[0].name;
							secondMember = player.cVect[1].name;
						}
						coord.X = 1;
						coord.Y = 24;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 26;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						SetConsoleTextAttribute(color, 15);
						coord.X = 1;
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						cout << firstMember;
						coord.X = 33;
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						cout << secondMember;
						coord.X = 33;
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						cout << "Escape";
						coord.X = 1;
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						cout << "BACK";
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
								cout << "BACK";
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
								cout << "Escape";
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
									cout << "BACK";
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
									cout << "Escape";
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
								if (1 != menu.Y)
								{
									menu.Y--;
								}
							}
							else if ('r' == inputc)
							{
								if (2 != menu.X)
								{
									menu.X++;
								}
							}
							else if ('d' == inputc)
							{
								if (4 != menu.Y)
								{
									menu.Y++;
								}
							}
							else if ('l' == inputc)
							{
								if (1 != menu.X)
								{
									menu.X--;
								}
							}
							//record inputs
							if (13 == inputc)
							{
								if (1 == menu.X && 1 == menu.Y)//upper left menu position
								{
									if (eng == pTurn)
									{
										player.cVect[0].target = 1;
									}
									else if (sci == pTurn)
									{
										player.cVect[1].target = 0;
									}
									else if (pil == pTurn)
									{
										player.cVect[2].target = 0;
									}
									player.cVect[pTurn].act = special;
									attackReprint = true;
								}
								else if (1 == menu.X && 2 == menu.Y)//escape
								{
									menuReprint = true;
								}
								else if (2 == menu.X && 1 == menu.Y)//upper right menu position
								{
									if (eng == pTurn)
									{
										player.cVect[0].target = 2;
									}
									else if (sci == pTurn)
									{
										player.cVect[1].target = 2;
									}
									else if (pil == pTurn)
									{
										player.cVect[2].target = 1;
									}
									player.cVect[pTurn].act = special;
									attackReprint = true;
								}
								else if (2 == menu.X && 2 == menu.Y)//companion 1
								{
									player.cVect[pTurn].act = escape;
									player.cVect[pTurn].target = 0;
									attackReprint = true;
								}
								attackReprint = true;
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
							else if ((currentChar.length() + 1) < a && a < currentChar.length() + defenceMove.size() + 2)// if character length plus one
							{
								cout << defenceMove[a - (currentChar.length() + 2)];
							}
							else
							{
								printf("%c", 196);
							}
						}
						printf("%c\n", 191);//upper right corner
						printf("%c", 179);
						if (1 <= enemyParty.a)//will always run, but whatever
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[0].name.length())
								{
									cout << enemyParty.eVect[0].name[a];
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
								if (a <= enemyParty.eVect[1].name.length())
								{
									cout << enemyParty.eVect[1].name[a];
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
						if (3 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[2].name.length())
								{
									cout << enemyParty.eVect[2].name[a];
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
						if (4 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[3].name.length())
								{
									cout << enemyParty.eVect[3].name[a];
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
						if (5 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[4].name.length())
								{
									cout << enemyParty.eVect[4].name[a];
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
						if (6 <= enemyParty.a)
						{
							for (int a = 0; a < 32; a++)
							{
								if (a <= enemyParty.eVect[5].name.length())
								{
									cout << enemyParty.eVect[5].name[a];
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
						cout << "BACK";
						for (int a = 0; a < 60; a++)
						{
							cout << " ";
						}
						//menu navigation
						bool goBack = false;
						menu.X = 1;
						menu.Y = 1;
						menuPast.X = 1;
						menuPast.Y = 1;
						while (!goBack)
						{
							//color menu position
							if (1 == menu.X && 1 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 24;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[0].name;
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[2].name;
							}
							else if (1 == menu.X && 3 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[4].name;
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 24;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[1].name;
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[3].name;
							}
							else if (2 == menu.X && 3 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[5].name;
							}
							else if (4 == menu.Y)
							{
								coord.X = 1;
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
									coord.Y = 24;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[0].name;
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[2].name;
								}
								else if (1 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[4].name;
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 24;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[1].name;
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[3].name;
								}
								else if (2 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[5].name;
								}
								else if (4 == menuPast.Y)
								{
									coord.X = 1;
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
								if (1 != menu.Y)
								{
									menu.Y--;
								}
							}
							else if ('r' == inputc)
							{
								if (2 != menu.X && 4 != menu.Y)
								{
									menu.X++;
								}
							}
							else if ('d' == inputc)
							{
								if (4 != menu.Y)
								{
									menu.Y++;
								}
							}
							else if ('l' == inputc)
							{
								if (1 != menu.X && 4 != menu.Y)
								{
									menu.X--;
								}
							}
							//fix position based on number of enemies
							if (1 == enemyParty.a)
							{
								if ('d' == inputc)
								{
									menu.Y = 4;
								}
								else if ('u' == inputc)
								{
									menu.Y = 1;
								}
								if (2 == menu.X)
								{
									menu.X = 1;
								}
							}
							else if (2 == enemyParty.a)
							{
								if ('d' == inputc)
								{
									menu.Y = 4;
								}
								else if ('u' == inputc)
								{
									menu.Y = 1;
									menu.X = 1;
								}
							}
							else if (3 == enemyParty.a)
							{
								if (2 == menu.X && 2 == menu.Y)
								{
									if ('d' == inputc)
									{
										menu.X = 1;
										menu.Y = 2;
									}
									else if ('r' == inputc)
									{
										menu.X = 2;
										menu.Y = 1;
									}
								}
								else if (3 == menu.Y)
								{
									if ('u' == inputc)
									{
										menu.X = 1;
										menu.Y = 2;
									}
									else if ('d' == inputc)
									{
										menu.Y = 4;
									}
								}
							}
							else if (4 == enemyParty.a)
							{
								if (3 == menu.Y)
								{
									if ('u' == inputc)
									{
										menu.X = 1;
										menu.Y = 2;
									}
									else if ('d' == inputc)
									{
										menu.Y = 4;
									}
								}
							}
							else if (5 == enemyParty.a)
							{
								if (3 == menu.Y)
								{
									menu.X = 1;
								}
							}
							//record inputs
							if (13 == inputc)
							{
								if (1 == menu.X && 1 == menu.Y)//e1
								{
									player.cVect[pTurn].act = evade;
									player.cVect[pTurn].target = 0;
								}
								else if (1 == menu.X && 2 == menu.Y)//e3
								{
									player.cVect[pTurn].act = evade;
									player.cVect[pTurn].target = 2;
								}
								else if (1 == menu.X && 3 == menu.Y)//e5
								{
									player.cVect[pTurn].act = evade;
									player.cVect[pTurn].target = 4;
								}
								else if (2 == menu.X && 1 == menu.Y)//e2
								{
									player.cVect[pTurn].act = evade;
									player.cVect[pTurn].target = 1;
								}
								else if (2 == menu.X && 2 == menu.Y)//e4
								{
									player.cVect[pTurn].act = evade;
									player.cVect[pTurn].target = 3;
								}
								else if (2 == menu.X && 3 == menu.Y)//e6
								{
									player.cVect[pTurn].act = evade;
									player.cVect[pTurn].target = 5;
								}
								else if (4 == menu.Y)//BACK
								{
									menuReprint = true;
								}
								goBack = true;
							}
						}
					}
					else if (2 == menu.X && 2 == menu.Y)//Info
					{
						coord.X = 4;
						coord.Y = 23;
						SetConsoleCursorPosition(standard, coord);
						cout << "Info";
						for (int a = 0; a < 32; a++)
						{
							cout << char(196);
						}
						coord.X = 1;
						coord.Y = 24;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 25;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 26;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 27;
						SetConsoleCursorPosition(standard, coord);
						for (int a = 0; a < 64; a++)
						{
							cout << " ";
						}
						coord.Y = 24;
						SetConsoleCursorPosition(standard, coord);
						cout << "this is the info window";
						_getch();
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
		//randomize enemy input
		//enemies don't escape
		for (int a = 0; a < enemyParty.eVect.size(); a++)
		{
			switch (enemyParty.eVect[a].ID)
			{
			case 0:
				spaceGoblinAI(player, enemyParty, a, turnNum);
				break;
			case 1:
				squidAI(player, enemyParty, a, turnNum);
				break;
			case 2:
				droneAI(player, enemyParty, a, turnNum);
				break;
			case 3:
				droneBrainAI(player, enemyParty, a, turnNum);
				break;
			}
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
			cout << "e1: action " << enemyParty.eVect[0].act << " target " << enemyParty.eVect[0].target << "\n";
			cout << "e2: action " << enemyParty.eVect[1].act << " target " << enemyParty.eVect[1].target << "\n";
			cout << "e3: action " << enemyParty.eVect[2].act << " target " << enemyParty.eVect[2].target << "\n";
			_getch();
		}
		player.cVect[0].defenceValue = 10;//resetting/initializing defence Values
		player.cVect[1].defenceValue = 10;
		player.cVect[2].defenceValue = 10;
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
			if (attack == player.cVect[a].act)//attacks
			{
				switch (player.cVect[a].attackID)
				{
				case 0:
					basicAttack(player, enemyParty, a);
					break;
				case 1:
					largeStapelerAttack(player, enemyParty, a);
				}
			}
			else if (special == player.cVect[a].act)
			{
				switch (player.cVect[a].specialID)
				{
				case 0:
					basicHeal(player, enemyParty, a);
					break;
				}
			}
			else if (evade == player.cVect[a].act)
			{
				switch (player.cVect[a].defenceID)
				{
				case 0:
					basicEvade(player, enemyParty, a);
					break;
				}
			}
			if (player.cVect[a].act != evade)
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
			killAndTop(player, enemyParty);
		}
		//enemy actions
		for (int a = 0; a < enemyParty.a; a++)
		{
			switch (enemyParty.eVect[a].ID)
			{
			case 0:
				spaceGoblinActions(player, enemyParty, a);
				break;
			case 1:
				squidActions(player, enemyParty, a);
				break;
			case 2:
				droneActions(player, enemyParty, a);
				break;
			case 3:
				droneBrainActions(player, enemyParty, a);
				break;
			}
		}
		if (evade == player.cVect[0].act && evade == player.cVect[1].act && evade == player.cVect[2].act && evade == enemyParty.eVect[0].act && evade == enemyParty.eVect[1].act && evade == enemyParty.eVect[2].act)
		{
			coord.X = 0;
			coord.Y = 29;
			SetConsoleCursorPosition(standard, coord);
			for (int a = 0; a < 80; a++)
			{
				cout << " ";
			}
			SetConsoleCursorPosition(standard, coord);
			cout << "a staring contest insues";
			_getch();
			SetConsoleCursorPosition(standard, coord);
			for (int a = 0; a < 80; a++)
			{
				cout << " ";
			}
		}
		killAndTop(player, enemyParty);
		if (enemyParty.eVect[0].dead && enemyParty.eVect[1].dead && enemyParty.eVect[2].dead)
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
		else if (player.cVect[0].dead || player.cVect[1].dead || player.cVect[2].dead)
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
			if (a < enemy.eVect[0].name.size())
			{
				cout << enemy.eVect[0].name[a];
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
		printHbar(enemy.eVect[0].HP, enemy.eVect[0].HPmax, 20);
		printf("                      ");//print 22 spaces
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 64; a++)
		{
			cout << " ";
		}
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 64; a++)
		{
			cout << " ";
		}
	}
	else if (2 == enemy.a)
	{
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].name.size())
			{
				cout << enemy.eVect[0].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].name.size())
			{
				cout << enemy.eVect[1].name[a];
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
		printHbar(enemy.eVect[0].HP, enemy.eVect[0].HPmax, 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[1].HP, enemy.eVect[1].HPmax, 20);
		printf("        ");//print 8 spaces
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 64; a++)
		{
			cout << " ";
		}
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 64; a++)
		{
			cout << " ";
		}
	}
	else if (3 == enemy.a)
	{
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].name.size())
			{
				cout << enemy.eVect[0].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].name.size())
			{
				cout << enemy.eVect[1].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].name.size())
			{
				cout << enemy.eVect[2].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[0].HP, enemy.eVect[0].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[1].HP, enemy.eVect[1].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[2].HP, enemy.eVect[2].HPmax, 20);
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 64; a++)
		{
			cout << " ";
		}
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 64; a++)
		{
			cout << " ";
		}
	}
	else if (4 == enemy.a)
	{
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].name.size())
			{
				cout << enemy.eVect[0].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].name.size())
			{
				cout << enemy.eVect[1].name[a];
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
		printHbar(enemy.eVect[0].HP, enemy.eVect[0].HPmax, 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[1].HP, enemy.eVect[1].HPmax, 20);
		printf("        ");//print 8 spaces
		printf("%c\n", 179);
		printf("%c", 179);
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].name.size())
			{
				cout << enemy.eVect[2].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[3].name.size())
			{
				cout << enemy.eVect[3].name[a];
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
		printHbar(enemy.eVect[2].HP, enemy.eVect[2].HPmax, 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[3].HP, enemy.eVect[3].HPmax, 20);
		printf("        ");//print 8 spaces
	}
	else if (5 == enemy.a)
	{
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].name.size())
			{
				cout << enemy.eVect[0].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].name.size())
			{
				cout << enemy.eVect[1].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].name.size())
			{
				cout << enemy.eVect[2].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[0].HP, enemy.eVect[0].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[1].HP, enemy.eVect[1].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[2].HP, enemy.eVect[2].HPmax, 20);
		printf("%c\n", 179);
		printf("%c", 179);
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[3].name.size())
			{
				cout << enemy.eVect[3].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[4].name.size())
			{
				cout << enemy.eVect[4].name[a];
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
		printHbar(enemy.eVect[3].HP, enemy.eVect[3].HPmax, 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[4].HP, enemy.eVect[4].HPmax, 20);
		printf("        ");//print 8 spaces
	}
	else if (6 == enemy.a)
	{
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].name.size())
			{
				cout << enemy.eVect[0].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].name.size())
			{
				cout << enemy.eVect[1].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].name.size())
			{
				cout << enemy.eVect[2].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[0].HP, enemy.eVect[0].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[1].HP, enemy.eVect[1].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[2].HP, enemy.eVect[2].HPmax, 20);
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[3].name.size())
			{
				cout << enemy.eVect[3].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[4].name.size())
			{
				cout << enemy.eVect[4].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[5].name.size())
			{
				cout << enemy.eVect[5].name[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[3].HP, enemy.eVect[3].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[4].HP, enemy.eVect[4].HPmax, 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[5].HP, enemy.eVect[5].HPmax, 20);
	}
	printf("%c\n", 179);
	printf("%c", 192);
	for (int a = 1; a <= 64; a++) { printf("%c", 196); }
	printf("%c\n", 217);
	//cout << enemy.graphic;//old ascii graphic
	pixelArtRelay(enemy.graphicBackID, enemy.graphicForID);//new pixel art system
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
		if (a < player.cVect[0].name.size())
		{
			cout << player.cVect[0].name[a];
		}
		else
		{
			cout << " ";
		}
	}
	cout << "  ";
	for (int a = 0; a < 20; a++)
	{
		if (a < player.cVect[1].name.size())
		{
			cout << player.cVect[1].name[a];
		}
		else
		{
			cout << " ";
		}
	}
	cout << "  ";
	for (int a = 0; a < 20; a++)
	{
		if (a < player.cVect[2].name.size())
		{
			cout << player.cVect[2].name[a];
		}
		else
		{
			cout << " ";
		}
	}
	printf("%c\n", 179);
	printf("%c", 179);
	printHbar(player.cVect[0].HP, player.cVect[0].HPmax, 20);
	cout << "  ";
	printHbar(player.cVect[1].HP, player.cVect[1].HPmax, 20);
	cout << "  ";
	printHbar(player.cVect[2].HP, player.cVect[2].HPmax, 20);
	printf("%c\n", 179);
	printf("%c", 179);
	//add companion health bars here when they are added
	for (int a = 0; a < 64; a++)
	{
		cout << " ";
	}
	printf("%c\n", 179);
	printf("%c", 179);
	for (int a = 0; a < 64; a++)
	{
		cout << " ";
	}
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
	for (int e = 0; e < 3; e++)
	{
		if (!enemyParty.eVect[e].dead)
		{
			if (enemyParty.eVect[e].HP <= 0)
			{
				enemyParty.eVect[e].dead = true;
			}
			else if (enemyParty.eVect[e].HPmax < enemyParty.eVect[e].HP)
			{
				enemyParty.eVect[e].HP = enemyParty.eVect[e].HPmax;
			}
		}
	}
	for (int p = 0; p < 3; p++)
	{
		if (!player.cVect[p].dead)
		{
			if (player.cVect[p].HP <= 0)
			{
				player.cVect[p].dead = true;
			}
			else if (player.cVect[p].HPmax < player.cVect[p].HP)
			{
				player.cVect[p].HP = player.cVect[p].HPmax;
			}
		}
	}
}

string getMoveName(action type, int ID)//0 = attack, 1 = defence, 2 = special
{
	string output;
	if (attack == type)//attack
	{
		vector <string> attackNames = { "Basic Attack", "Large Stapler", "Print Project" };
		output = attackNames[ID];
	}
	else if (evade == type)//defence
	{
		vector <string> defenceNames = { "Basic Evade", "Insult" };
		output = defenceNames[ID];
	}
	else if (special == type)//special
	{
		vector <string> specialNames = { "Basic Heal", "Lizard Staff", "Wizard Tail" };
		output = specialNames[ID];
	}
	return output;
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

void pixelArtRelay(int backID, int forID)
{
	string image = "";
	string image2 = "";
	switch (backID)
	{
	case 0:
		image += "88888878888887888888788888878888n";
		image += "88888878888887888888788888878888n";
		image += "88888878888887888888788888878888n";
		image += "88888878888887888888788888878888n";
		image += "88888878888887888888788888878888n";
		image += "88888878888887888888788888878888n";
		image += "88888887888888788888878888887888n";
		image += "88888888788888878888887888888788n";
		image += "88888888878888887888888788888878n";
		image += "88888888878888887888888788888878n";
		image += "88888888878888887888888788888878";
		printCombatGraphic(image, 6);
		break;
	case 1://Sp_ce
		break;
	case 2:
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00080000000000000000000000880000n";
		image += "00088000000000888000000000880000n";
		image += "00088000000008888000000000880000n";
		image += "00088000000008800000000000880000n";
		image += "00008880000000000000000008880000n";
		image += "00008880000000000088000000000000n";
		image += "00000000000000000088800000000000n";
		image += "00000000000000000008800000000000n";
		image += "00000000000000000000000000000000";
		printCombatGraphic(image, 6);
		break;
	default:
		cout << "ERROR\n";
	}
	switch (forID)
	{
	case 0:
		image2 += "                                n";
		image2 += "              aaaa              n";
		image2 += "          22 aaaaaa 22          n";
		image2 += "           22a4aa4a22           n";
		image2 += "           22aa22aa22           n";
		image2 += "            2aa22aa2            n";
		image2 += "             aaaaaa             n";
		image2 += "             aa22aa             n";
		image2 += "              aaaa              n";
		image2 += "           2222aa2222           n";
		image2 += "          222222222222          ";
		printCombatGraphic(image2, 6);
		break;
	case 1://Sp_ce
		image += "000000000000000000000000000000000000000000000000000000000000n";
		image += "000000000000000000000000000000000000000000000000000000000000n";
		image += "000000000000000000000000000000000000000000000000000000000000n";
		image += "000000000000000000000000000000000000000000000000000000000000n";
		image += "0000fffffffffff000000000000000000000000000000000000000000000n";
		image += "000ffffffffffff000000000000000000000000000000000000000000000n";
		image += "00fff0000000000000000000000000000000000000000000000000000000n";
		image += "0fff00000000000000000000000000000000000000000000000000000000n";
		image += "0ff000000000000000000000000000000000000000000000000000000000n";
		image += "0ff000000000000000000000000000000000000000000000000000000000n";
		image += "0ff000000000000000000000000000000000000000000000000000000000n";
		image += "0ff000000000000000000000000000000000000000000000000000000000n";
		image += "0fff00000000000000000000000000000000000000000000000000000000n";
		image += "00fff0000000000000000000000000000000000000000000000000000000n";
		image += "000fffffffff0000ff0fffff00000000000000000fffffff000ffffff000n";
		image += "0000fffffffff000fffffffff000000000000000ffffffff00ffffffff00n";
		image += "00000000000fff00ffff000fff0000000000000fff00000000ff0000ff00n";
		image += "000000000000fff0ff000000fff00000000000fff00000000ff000000ff0n";
		image += "0000000000000ff0ff0000000ff00000000000ff000000000ff000000ff0n";
		image += "0000000000000ff0ff0000000ff00000000000ff000000000ffffffffff0n";
		image += "0000000000000ff0ff0000000ff00000000000ff000000000ffffffffff0n";
		image += "0000000000000ff0ff0000000ff00000000000ff000000000ff000000000n";
		image += "000000000000fff0ff000000fff00000000000fff00000000ff000000000n";
		image += "00000000000fff00ffff000fff0000000000000fff00000000ff0000ff00n";
		image += "0ffffffffffff000fffffffff000fffffffff000ffffffff00ffffffff00n";
		image += "0fffffffffff0000ff0fffff0000fffffffff0000fffffff000ffffff000n";
		image += "0000000000000000ff000000000000000000000000000000000000000000n";
		image += "0000000000000000ff000000000000000000000000000000000000000000n";
		image += "0000000000000000ff000000000000000000000000000000000000000000n";
		image += "0000000000000000ff000000000000000000000000000000000000000000";
		printImage(image, 0);
		break;
	case 2:
		image2 = "";
		image2 += "00000004000000044000000040000000n";
		image2 += "0000004c4000004444000004c4000000n";
		image2 += "0048004c4000044cc4400004c4880400n";
		image2 += "04c4804c400044cffc440004c4884c40n";
		image2 += "04c4804c40044cf55fc44004c4884c40n";
		image2 += "04c4804440044cf55fc4400444884c40n";
		image2 += "04c48884000044cffc44000048884c40n";
		image2 += "00408884000044cccc44000040000400n";
		image2 += "004400044000044cc448800440004400n";
		image2 += "00044000440004444448804400044000n";
		image2 += "00004444044004444440044044440000";
		printCombatGraphic(image2, 6);
		break;
	case 3:
		image2 = "";
		image2 += "00000000000000000000000000000000n";
		image2 += "000000fff0000f0000f0000000000000n";
		image2 += "00000ff4ff000f0000f00000000fff00n";
		image2 += "000000fff00000f00f00000000ff4ff0n";
		image2 += "00000000000000f00f000000000fff00n";
		image2 += "0000000000000ffffff0000000000000n";
		image2 += "000fff000000ffffffff000000000000n";
		image2 += "00ff4ff0000ffff4cffff000fff00000n";
		image2 += "000fff00000ffff44ffff00ff4ff0000n";
		image2 += "000000000000ffffffff0000fff00000n";
		image2 += "0000000000000ffffff0000000000000";
		printCombatGraphic(image2, 6);
		break;
	default:
		cout << "ERROR" << endl;
	}
	return;
}

void printCombatGraphic(string image, int Y)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 0;
	coord.Y = Y;
	SetConsoleCursorPosition(standard, coord);
	coord.X = 3;
	char line = 186;
	color(15);
	cout << line;
	for (int c = 0; c < image.length(); c++)
	{
		if ('0' == image[c])
		{
			color(0);
			blocks(1);
		}
		else if ('1' == image[c])
		{
			color(1);
			blocks(1);
		}
		else if ('2' == image[c])
		{
			color(2);
			blocks(1);
		}
		else if ('3' == image[c])
		{
			color(3);
			blocks(1);
		}
		else if ('4' == image[c])
		{
			color(4);
			blocks(1);
		}
		else if ('5' == image[c])
		{
			color(5);
			blocks(1);
		}
		else if ('6' == image[c])
		{
			color(6);
			blocks(1);
		}
		else if ('7' == image[c])
		{
			color(7);
			blocks(1);
		}
		else if ('8' == image[c])
		{
			color(8);
			blocks(1);
		}
		else if ('9' == image[c])
		{
			color(9);
			blocks(1);
		}
		else if ('a' == image[c])
		{
			color(10);
			blocks(1);
		}
		else if ('b' == image[c])
		{
			color(11);
			blocks(1);
		}
		else if ('c' == image[c])
		{
			color(12);
			blocks(1);
		}
		else if ('d' == image[c])
		{
			color(13);
			blocks(1);
		}
		else if ('e' == image[c])
		{
			color(14);
			blocks(1);
		}
		else if ('f' == image[c])
		{
			color(15);
			blocks(1);
		}
		else if (' ' == image[c])
		{
			SetConsoleCursorPosition(standard, coord);
		}
		else if ('n' == image[c])
		{
			color(15);
			cout << line << "\n" << line;
			coord.Y++;
			coord.X = 1;
		}
		coord.X += 2;
	}
	coord.X -= 2;
	color(15);
	SetConsoleCursorPosition(standard, coord);
	cout << line;
}

void printImage(string image, int Y)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 0;
	coord.Y = Y;
	SetConsoleCursorPosition(standard, coord);
	coord.X = 2;
	for (int c = 0; c < image.length(); c++)
	{
		if ('0' == image[c])
		{
			color(0);
			blocks(1);
		}
		else if ('1' == image[c])
		{
			color(1);
			blocks(1);
		}
		else if ('2' == image[c])
		{
			color(2);
			blocks(1);
		}
		else if ('3' == image[c])
		{
			color(3);
			blocks(1);
		}
		else if ('4' == image[c])
		{
			color(4);
			blocks(1);
		}
		else if ('5' == image[c])
		{
			color(5);
			blocks(1);
		}
		else if ('6' == image[c])
		{
			color(6);
			blocks(1);
		}
		else if ('7' == image[c])
		{
			color(7);
			blocks(1);
		}
		else if ('8' == image[c])
		{
			color(8);
			blocks(1);
		}
		else if ('9' == image[c])
		{
			color(9);
			blocks(1);
		}
		else if ('a' == image[c])
		{
			color(10);
			blocks(1);
		}
		else if ('b' == image[c])
		{
			color(11);
			blocks(1);
		}
		else if ('c' == image[c])
		{
			color(12);
			blocks(1);
		}
		else if ('d' == image[c])
		{
			color(13);
			blocks(1);
		}
		else if ('e' == image[c])
		{
			color(14);
			blocks(1);
		}
		else if ('f' == image[c])
		{
			color(15);
			blocks(1);
		}
		else if (' ' == image[c])
		{
			SetConsoleCursorPosition(standard, coord);
		}
		else if ('n' == image[c])
		{
			cout << "\n";
			coord.Y++;
			coord.X = 0;
		}
		coord.X += 2;
	}
	color(15);
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
	enemyParty.eVect[a].act = temp;
	if (attack == temp)
	{
		int randAttack = _dice(player.cVect[0].HP + player.cVect[1].HP + player.cVect[2].HP);
		if (randAttack <= player.cVect[0].HP)//attack engineer
		{
			tempTarget = 0;
		}
		else if (player.cVect[0].HP < randAttack && randAttack <= player.cVect[0].HP + player.cVect[1].HP)//attack scientist
		{
			tempTarget = 1;
		}
		else if (player.cVect[0].HP + player.cVect[1].HP < randAttack && randAttack <= player.cVect[0].HP + player.cVect[1].HP + player.cVect[2].HP)//attack pilot
		{
			tempTarget = 2;
		}
	}
	else if (special == temp)
	{
		bool pickedOne = false;
		while (!pickedOne)
		{
			switch (_dice(enemyParty.a))
			{
			case 1:
				if (!enemyParty.eVect[0].dead)
				{
					tempTarget = 0;
					pickedOne = true;
				}
				break;
			case 2:
				if (!enemyParty.eVect[1].dead)
				{
					tempTarget = 1;
					pickedOne = true;
				}
				break;
			case 3:
				if (!enemyParty.eVect[2].dead)
				{
					tempTarget = 2;
					pickedOne = true;
				}
				break;
			case 4:
				if (!enemyParty.eVect[3].dead)
				{
					tempTarget = 3;
					pickedOne = true;
				}
				break;
			case 5:
				if (!enemyParty.eVect[4].dead)
				{
					tempTarget = 4;
					pickedOne = true;
				}
				break;
			case 6:
				if (!enemyParty.eVect[5].dead)
				{
					tempTarget = 5;
					pickedOne = true;
				}
			}
		}

	}
	else if (evade == temp)
	{
		tempTarget = _dice(3) - 1;
	}
	enemyParty.eVect[a].target = tempTarget;
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
	int TempAtk = (float)enemyParty.eVect[e].atk * enemyParty.eVect[e].atkMulti;
	if (!enemyParty.eVect[e].dead)//enemy
	{
		if (attack == enemyParty.eVect[e].act)//attacks 
		{
			if (evade == player.cVect[enemyParty.eVect[e].target].act && 1 == player.cVect[enemyParty.eVect[e].target].target)//player evades
			{
				if (player.cVect[enemyParty.eVect[e].target].defenceValue < 20 + _dice(10))//20 + _dice(10) is goblin's accuracy value. if the goblins accuracy is higher than the player's defence than the goblin hits
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " fails to dodge " << enemyParty.eVect[e].name << "'s attack for " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
				}
				else//75% chance to dodge
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " dodges " << enemyParty.eVect[e].name << "'s attack";
				}
			}
			else//engineer does not evade
			{
				if (1 == _dice(4))//25% chance to miss, 
				{
					cout << enemyParty.eVect[e].name << " attacks " << player.cVect[enemyParty.eVect[e].target].name << " but misses";
				}
				else//75% chance to not miss
				{
					cout << enemyParty.eVect[e].name << " attacks " << player.cVect[enemyParty.eVect[e].target].name << " for " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
				}
			}
		}
		else if (special == enemyParty.eVect[e].act)//uses special on
		{
			if (e == enemyParty.eVect[e].target)//goblin targeting itself increases it's attack multiplier
			{
				cout << enemyParty.eVect[e].name << " gives its ray gun a smack, causing it to surge with energy";
				enemyParty.eVect[e].atkMulti *= 1.1;
			}
			else if (0 == enemyParty.eVect[e].target)
			{
				cout << enemyParty.eVect[e].name << " tends to " << enemyParty.eVect[0].name << "'s wounds for 3 health";
				enemyParty.eVect[0].HP += 3;
			}
			else if (1 == enemyParty.eVect[e].target)
			{
				cout << enemyParty.eVect[e].name << " tends to " << enemyParty.eVect[1].name << "'s wounds for 3 health";
				enemyParty.eVect[1].HP += 3;
			}
			else if (2 == enemyParty.eVect[e].target)
			{
				cout << enemyParty.eVect[e].name << " tends to " << enemyParty.eVect[2].name << "'s wounds for 3 health";
				enemyParty.eVect[2].HP += 3;
			}
			else
			{
				cout << "ERROR: NO TARGET";
			}
		}
	}
	if (evade != enemyParty.eVect[e].act && !enemyParty.eVect[e].dead)
	{
		_getch();
	}
	return;
}

void squidAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	action temp;
	int tempTarget;
	int attackChance = 10;
	int evadeChance = 10;
	int specialChance = 10;
	if (0 == turnNum % 2)//every turn switch between high attack chance and high evade/special chance
	{
		attackChance += 40;
		evadeChance += 10;
	}
	else
	{
		evadeChance += 40;
		specialChance += 40;
		attackChance -= 5;
	}
	int random = _dice(attackChance + evadeChance + specialChance);
	if (random <= attackChance)
	{
		temp = attack;
	}
	else if (attackChance < random && random <= attackChance + evadeChance)
	{
		temp = evade;
	}
	else if (attackChance + evadeChance < random)
	{
		temp = special;
	}
	enemyParty.eVect[a].act = temp;
	if (attack == temp)//enemies should attack characters with more HP
	{
		int randAttack = _dice(player.cVect[0].HP + player.cVect[1].HP + player.cVect[2].HP);
		if (randAttack <= player.cVect[0].HP)//attack engineer
		{
			tempTarget = 0;
		}
		else if (player.cVect[0].HP < randAttack && randAttack <= player.cVect[0].HP + player.cVect[1].HP)//attack scientist
		{
			tempTarget = 1;
		}
		else if (player.cVect[0].HP + player.cVect[1].HP < randAttack)//attack pilot
		{
			tempTarget = 2;
		}
	}
	else if (special == temp)
	{
		bool pickedOne = false;
		while (!pickedOne)//loops until an enemy with more than 0 HP is randomly chosen
		{
			switch (_dice(enemyParty.a))
			{
			case 1:
				if (!enemyParty.eVect[0].dead)
				{
					tempTarget = 0;
					pickedOne = true;
				}
				break;
			case 2:
				if (!enemyParty.eVect[1].dead)
				{
					tempTarget = 1;
					pickedOne = true;
				}
				break;
			case 3:
				if (!enemyParty.eVect[2].dead)
				{
					tempTarget = 2;
					pickedOne = true;
				}
				break;
			case 4:
				if (!enemyParty.eVect[3].dead)
				{
					tempTarget = 3;
					pickedOne = true;
				}
				break;
			case 5:
				if (!enemyParty.eVect[4].dead)
				{
					tempTarget = 4;
					pickedOne = true;
				}
				break;
			case 6:
				if (!enemyParty.eVect[5].dead)
				{
					tempTarget = 5;
					pickedOne = true;
				}
			}
		}

	}
	else if (evade == temp)
	{
		tempTarget = _dice(3) - 1;
	}
	enemyParty.eVect[a].target = tempTarget;
	return;
}

void squidActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].atk * enemyParty.eVect[e].atkMulti;
	if (!enemyParty.eVect[e].dead)//enemy
	{
		if (attack == enemyParty.eVect[e].act)//attacks 
		{
			if (evade == player.cVect[enemyParty.eVect[e].target].act && 1 == player.cVect[enemyParty.eVect[e].target].target)//player evades
			{
				if (player.cVect[enemyParty.eVect[e].target].defenceValue < 25 + _dice(5))//
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " fails to dodge " << enemyParty.eVect[e].name << "'s attack doing " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
					enemyParty.eVect[e].atkMulti = 1.0;//reset charged attacks
				}
				else//75% chance to dodge
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " dodges " << enemyParty.eVect[e].name << "'s tentacle";
				}
			}
			else//player does not evade
			{
				if (1 == _dice(4))//25% chance to miss
				{
					cout << enemyParty.eVect[e].name << " charges at " << player.cVect[enemyParty.eVect[e].target].name << " but misses";
				}
				else//75% chance to not miss
				{
					cout << enemyParty.eVect[e].name << " slaps " << player.cVect[enemyParty.eVect[e].target].name << " doing " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
					enemyParty.eVect[e].atkMulti = 1.0;//reset charged attacks
				}
			}
		}
		else if (special == enemyParty.eVect[e].act)//uses special on
		{
			if (e == enemyParty.eVect[e].target)//squid targeting itself increases it's attack multiplier
			{
				cout << enemyParty.eVect[e].name << " begins to spin, building momentum";
				enemyParty.eVect[e].atkMulti *= 1.4;
			}
			else
			{
				cout << enemyParty.eVect[e].name << " engulfs " << enemyParty.eVect[enemyParty.eVect[e].target].name << " in a cloud of healing ink, healing 2 damage";
				enemyParty.eVect[enemyParty.eVect[e].target].HP += 2;
			}
		}
	}
	if (evade != enemyParty.eVect[e].act && !enemyParty.eVect[e].dead)
	{
		_getch();
	}
	return;
}

void droneAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	action temp;
	switch (_dice(3) - 1)
	{
	case 0:
		temp = attack;
		break;
	case 1:
		temp = special;
		break;
	case 2:
		temp = evade;
	}
	enemyParty.eVect[a].act = temp;
	if (special == temp)
	{
		enemyParty.eVect[a].target = 1;//drones always target the brain
	}
	else if (attack == temp)
	{
		enemyParty.eVect[a].target = _dice(3) - 1;
	}
	else if (evade == temp)
	{
		enemyParty.eVect[a].target = _dice(3) - 1;
	}
	return;
}

void droneActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].atk * enemyParty.eVect[e].atkMulti;
	if (!enemyParty.eVect[e].dead)//enemy
	{
		if (attack == enemyParty.eVect[e].act)//attacks 
		{
			if (evade == player.cVect[enemyParty.eVect[e].target].act && 1 == player.cVect[enemyParty.eVect[e].target].target)//player evades
			{
				if (player.cVect[enemyParty.eVect[e].target].defenceValue < 15 + _dice(15))//15 + _dice(15) is drone's accuracy value. if the drone's accuracy is higher than the player's defence than the drone hits
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " fails to dodge " << enemyParty.eVect[e].name << "'s lazer doing " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
				}
				else//75% chance to dodge
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " dodges " << enemyParty.eVect[e].name << "'s attack";
				}
			}
			else//player does not evade
			{
				if (1 == _dice(3))//33% chance to miss, 
				{
					cout << enemyParty.eVect[e].name << " attacks " << player.cVect[enemyParty.eVect[e].target].name << " but misses";
				}
				else//75% chance to not miss
				{
					cout << enemyParty.eVect[e].name << " hits " << player.cVect[enemyParty.eVect[e].target].name << " with a lazer for " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
				}
			}
		}
		else if (special == enemyParty.eVect[e].act)//uses special on
		{
			cout << enemyParty.eVect[e].name << " siphons energy into " << enemyParty.eVect[enemyParty.eVect[e].target].name;
			enemyParty.eVect[enemyParty.eVect[e].target].atkMulti *= 1.2;
		}
	}
	if (evade != enemyParty.eVect[e].act && !enemyParty.eVect[e].dead)
	{
		_getch();
	}
	return;
}

void droneBrainAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	action temp;
	if (0 == turnNum % 3)
	{
		temp = evade;
	}
	else
	{
		temp = attack;
	}
	enemyParty.eVect[a].act = temp;
	if (attack == temp)
	{
		enemyParty.eVect[a].target = _dice(3) - 1;
	}
	else if (evade == temp)
	{
		enemyParty.eVect[a].target = _dice(3) - 1;
	}
	return;
}

void droneBrainActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].atk * enemyParty.eVect[e].atkMulti;
	if (!enemyParty.eVect[e].dead)//enemy
	{
		if (attack == enemyParty.eVect[e].act)//attacks 
		{
			if (evade == player.cVect[enemyParty.eVect[e].target].act && 1 == player.cVect[enemyParty.eVect[e].target].target)//player evades
			{
				if (player.cVect[enemyParty.eVect[e].target].defenceValue < 15 + _dice(15))//15 + _dice(15) is drone's accuracy value. if the drone's accuracy is higher than the player's defence than the drone hits
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " fails to dodge " << enemyParty.eVect[e].name << "'s lazer doing " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
				}
				else//75% chance to dodge
				{
					cout << player.cVect[enemyParty.eVect[e].target].name << " dodges " << enemyParty.eVect[e].name << "'s attack";
				}
			}
			else//player does not evade
			{
				if (1 == _dice(3))//33% chance to miss, 
				{
					cout << enemyParty.eVect[e].name << " attacks " << player.cVect[enemyParty.eVect[e].target].name << " but misses";
				}
				else//75% chance to not miss
				{
					cout << enemyParty.eVect[e].name << " hits " << player.cVect[enemyParty.eVect[e].target].name << " with a lazer for " << TempAtk << " damage";
					player.cVect[enemyParty.eVect[e].target].HP -= TempAtk;
				}
			}
		}
	}
	if (evade != enemyParty.eVect[e].act && !enemyParty.eVect[e].dead)
	{
		_getch();
	}
	return;
}


//player attacks
void printProjectAttack(party& player, eparty& enemyParty, int p)
{
	int e = player.cVect[p].target;
	if (evade == enemyParty.eVect[e].act && 0 == enemyParty.eVect[e].target)//enemy evades
	{
		if (enemyParty.eVect[e].dodgeValue <= 15 + _dice(20))//hit
		{
			int damage = (0.06 * player.projectsPrinted * player.projectsPrinted) + (0.1 * player.projectsPrinted) * player.cVect[p].atkMulti;
			cout << player.cVect[p].name << " turns their project in to " << enemyParty.eVect[e].name << " it's " << player.projectsPrinted << " pages long!";
			enemyParty.eVect[e].HP -= damage;
		}
		else//miss
		{
			cout << enemyParty.eVect[e].name << " dodges " << player.cVect[p].name << "'s flurry of paper";
		}
	}
	else//enemy doesn't evade
	{
		if (1 != _dice(20))//hit
		{
			int damage = 8 * player.cVect[p].atkMulti;
			cout << player.cVect[p].name << " turns their project in to " << enemyParty.eVect[e].name << " it's " << player.projectsPrinted << " pages long!";
			enemyParty.eVect[e].HP -= damage;
		}
		else//miss
		{
			cout << player.cVect[p].name << " gets a paper jam and can't fire at " << enemyParty.eVect[e].name;
		}
	}
}

void largeStapelerAttack(party& player, eparty& enemyParty, int p)//p = which player is executing the action (0-2)
{
	//which player is attacking?
	//which enemy?
	//are they dodging?
	//flavor text
	int e = player.cVect[p].target;
	if (evade == enemyParty.eVect[e].act && 0 == enemyParty.eVect[e].target)//enemy evades
	{
		if (enemyParty.eVect[e].dodgeValue <= 20 + _dice(20))//hit
		{
			int damage = (1 + _dice(5)) * ((player.cVect[p].atkMulti - 1.0) * 3.0 + 1.0);
			cout << player.cVect[p].name << " pelts " << enemyParty.eVect[e].name << " with large staples for " << damage << " damage";
			enemyParty.eVect[e].HP -= damage;
		}
		else//miss
		{
			cout << enemyParty.eVect[e].name << " dodges " << player.cVect[p].name << "'s volley of staples";
		}
	}
	else//enemy doesn't evade
	{
		if (1 != _dice(20))//hit
		{
			int damage = (1 + _dice(5)) * ((player.cVect[p].atkMulti - 1.0) * 3.0 + 1.0);
			cout << player.cVect[p].name << " pelts " << enemyParty.eVect[e].name << " with large staples for " << damage << " damage";
			enemyParty.eVect[e].HP -= damage;
		}
		else//miss
		{
			cout << player.cVect[p].name << " mises " << enemyParty.eVect[e].name << " with a volley of staples";
		}
	}
}

void basicAttack(party& player, eparty& enemyParty, int p)
{
	int e = player.cVect[p].target;
	if (evade == enemyParty.eVect[e].act && 0 == enemyParty.eVect[e].target)//enemy evades
	{
		if (enemyParty.eVect[e].dodgeValue <= 10 + _dice(20))//hit
		{
			int damage = 8 * player.cVect[p].atkMulti;
			cout << player.cVect[p].name << " attacks " << enemyParty.eVect[e].name << " for " << damage << " damage";
			enemyParty.eVect[e].HP -= damage;
		}
		else//miss
		{
			cout << enemyParty.eVect[e].name << " dodges " << player.cVect[p].name << "'s attack";
		}
	}
	else//enemy doesn't evade
	{
		if (1 != _dice(20))//hit
		{
			int damage = 8 * player.cVect[p].atkMulti;
			cout << player.cVect[p].name << " attacks " << enemyParty.eVect[e].name << " for " << damage << " damage";
			enemyParty.eVect[e].HP -= damage;
		}
		else//miss
		{
			cout << player.cVect[p].name << " mises " << enemyParty.eVect[e].name;
		}
	}
}

//player specials
void basicHeal(party& player, eparty& enemyParty, int p)
{
	int healRate = 4;
	cout << player.cVect[p].name << " tends to " << player.cVect[player.cVect[p].target].name << "'s wounds, healing " << healRate << " HP";
	player.cVect[player.cVect[p].target].HP += healRate;
}

//player defenses
void basicEvade(party& player, eparty& enemyParty, int p)
{
	player.cVect[p].defenceValue += 20 + _dice(20);
}