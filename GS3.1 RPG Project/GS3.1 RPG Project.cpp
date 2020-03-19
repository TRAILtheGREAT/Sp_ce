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
#include <fstream>
#include <cmath>
#include "Enemy.h"
#include "Inventory.h"

using namespace std;

enum turn { eng, sci, pil };
enum action { attack, evade, special, escape };

struct roomInfo //room struct will store location and encounter. door state will be stored in the door arrays
{
	string name;
	int encounterID = 0;//npc, enemy, or encounter in this room//is this legal?
	bool visited;
};
struct point
{
	int X, Y;
};
struct crew
{
	string name;
	int HP, HPmax;
	bool dead;
	action act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;//damage multiplier
	int attackID, specialID, defenceID;
	int defenceValue, defenceBonus;
};
struct vessel
{
	string name;
	int HP, HPmax;
};
struct party
{
	vector <crew> cVect;
	vessel ship;
	int projectsPrinted;//used for print project attack
	Inventory inventory;
	int credits;
};
struct eparty
{
	vector <Enemy> eVect;
	int a;//number of enemies in the party that need to be displayed
	int credits;
	int graphicBackID, graphicForID;
};

void displayMap(bool(&VdoorArray)[4][5], bool(&HdoorArray)[5][4], int C, int R);
int _dice(int max);
bool encounter(party& player, int encounterID, vector <eparty> enemyVector);
int roundUp(int number, int devisor);
void printHbar(int HP, int HPmax, int length);
void printHealthWindow(party& player, eparty& enemy);//player, enemy, number of enemies in eparty
char _getdir();
void killAndTop(party& player, eparty& enemyParty);
string getMoveName(action type, int ID);
bool combat(party& player, eparty enemyParty);
void treasureCache(party& player);

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
void wormAI(party& player, eparty& enemyParty, int a, int turnNum);
void wormActions(party& player, eparty& enemyParty, int e);
void retinazerAI(party& player, eparty& enemyParty, int a, int turnNum);
void spazmatismAI(party& player, eparty& enemyParty, int a, int turnNum);
void retinazerActions(party& player, eparty& enemyParty, int e);
void spazmatismActions(party& player, eparty& enemyParty, int e);
void golemAI(party& player, eparty& enemyParty, int a, int turnNum);
void golemActions(party& player, eparty& enemyParty, int e);
void guardianAI(party& player, eparty& enemyParty, int a, int turnNum);
void guardianActions(party& player, eparty& enemyParty, int e, int turnNum);

void largeStapelerAttack(party& player, eparty& enemyParty, int p);
void basicAttack(party& player, eparty& enemyParty, int p);
void basicHeal(party& player, eparty& enemyParty, int p);
void basicEvade(party& player, int p);
void printInventory(party& player);
void equipItem(party& player, int i, int a);//player, item, character

void updateCrewSave(party player, int playerC, int playerR);
void updateMapSave(bool VdoorArray[4][5], bool HdoorArray[5][4], roomInfo roomArray[5][5]);

int main()
{
	srand((unsigned)time(NULL));
	ifstream in;
	ofstream out;
	in.open("Enemies.txt");
	cout << "LOADING...";
	//realy wish i could use file IO
	//easy enemies: space Golbin, Ooze, spider drone, squid
	//medium enemies: zombie, rock man
	//hard enemy: astroid worm
	eparty tempParty;
	Enemy tempEnemy;
	vector <eparty> enemyVector;
	while (true)
	{
		tempParty.eVect.clear();
		string temp;
		getline(in, temp);
		if ("END" == temp)
		{
			break;
		}
		tempParty.graphicBackID = stoi(temp);
		getline(in, temp);
		tempParty.graphicForID = stoi(temp);
		getline(in, temp);
		tempParty.a = stoi(temp);
		while (true)
		{
			getline(in, temp, ',');
			if ("ENDPARTY" == temp)
			{
				getline(in, temp);
				break;
			}
			tempEnemy.setID(stoi(temp));
			getline(in, temp, ',');
			tempEnemy.setName(temp);
			getline(in, temp, ',');
			tempEnemy.setAtk(stoi(temp));
			getline(in, temp, ',');
			tempEnemy.setHP(stoi(temp));
			tempEnemy.setHPmax(stoi(temp));
			getline(in, temp);
			tempEnemy.setDodgeValue(stoi(temp));
			tempEnemy.setAtkMulti(1.0);
			tempEnemy.setDead(false);
			tempEnemy.setDodgeBonus(0);
			tempParty.eVect.push_back(tempEnemy);
		}
		enemyVector.push_back(tempParty);
	}
	in.close();
	in.open("CrewSave.txt");
	string fileTemp;
	getline(in, fileTemp, ',');
	party player;
	int playerC;
	int playerR;
	if ("0" == fileTemp)//no savestate
	{
		out.open("CrewSave.txt");
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
		player.inventory.addItem(0);
		player.inventory.setEquip(0, 0);
		player.cVect[0].defenceID = 0;
		player.inventory.addItem(6);
		player.inventory.setEquip(1, 0);
		player.cVect[0].specialID = 0;
		player.inventory.addItem(3);
		player.inventory.setEquip(2, 0);
		player.cVect[1].attackID = 0;
		player.inventory.addItem(0);
		player.inventory.setEquip(3, 1);
		player.cVect[1].defenceID = 0;
		player.inventory.addItem(6);
		player.inventory.setEquip(4, 1);
		player.cVect[1].specialID = 0;
		player.inventory.addItem(3);
		player.inventory.setEquip(5, 1);
		player.cVect[2].attackID = 0;
		player.inventory.addItem(0);
		player.inventory.setEquip(6, 2);
		player.cVect[2].defenceID = 0;
		player.inventory.addItem(6);
		player.inventory.setEquip(7, 2);
		player.cVect[2].specialID = 0;
		player.inventory.addItem(3);
		player.inventory.setEquip(8, 2);
		player.projectsPrinted = 0;
		playerC = 4;
		playerR = 4;
		//JoeJoeJoeBob
		updateCrewSave(player, playerC, playerR);
		printf("With the ship fully operational and your party fully equiped, the crew sets off into the great unknown...\n[press any key to continue]");
		_getch();
	}
	else//inport existing savestate
	{
		player.ship.name = fileTemp;
		getline(in, fileTemp, ',');
		player.ship.HP = stoi(fileTemp);
		getline(in, fileTemp);
		player.ship.HPmax = stoi(fileTemp);
		getline(in, fileTemp);
		player.projectsPrinted = stoi(fileTemp);
		getline(in, fileTemp, ',');
		playerR = stoi(fileTemp);
		getline(in, fileTemp);
		playerC = stoi(fileTemp);
		crew temp;
		for (int a = 0; a < 3; a++)
		{
			getline(in, fileTemp, ',');
			temp.name = fileTemp;
			getline(in, fileTemp, ',');
			temp.HP = stoi(fileTemp);
			getline(in, fileTemp, ',');
			temp.HPmax = stoi(fileTemp);
			getline(in, fileTemp, ',');
			temp.dead = stoi(fileTemp);
			getline(in, fileTemp, ',');
			temp.attackID = stoi(fileTemp);
			getline(in, fileTemp, ',');
			temp.specialID = stoi(fileTemp);
			getline(in, fileTemp);
			temp.defenceID = stoi(fileTemp);
			temp.defenceValue = 1;
			temp.defenceBonus = 0;
			player.cVect.push_back(temp);
		}
		in.close();
		in.open("Inventory.txt");
		int i = 0;
		while (getline(in, fileTemp, ','))
		{
			player.inventory.addItem(stoi(fileTemp));
			getline(in, fileTemp);
			player.inventory.setEquip(i, stoi(fileTemp));
			i++;
		}
	}
	system("cls");
	in.close();
	out.close();
	in.open("MapSave.txt");
	getline(in, fileTemp);
	roomInfo roomArray[5][5];
	bool VdoorArray[4][5];
	bool HdoorArray[5][4];
	if ("0" == fileTemp)//no map generated
	{
		in.close();
		out.open("MapSave.txt");
		printf("input seed number:");
		int intIn;
		cin >> intIn;
		printf("generating map...\n");
		srand(intIn);
		bool exit = false;
		
		//assigning all doors closed
		for (int c = 0; c < 4; c++)
		{
			for (int r = 0; r < 5; r++)
			{
				VdoorArray[c][r] = 1;
			}
		}
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
				roomArray[c][r].visited = false;
				if ((0 == c && 0 == r) || (0 == c && 4 == r) || (4 == c && 0 == r))
				{
					roomArray[c][r].name = "Ancient Warp Gate";
					roomArray[c][r].encounterID = 1;
					continue;
				}
				if ((4 == c && 4 == r))
				{
					roomArray[c][r].name = "Ancient Warp Gate";
					roomArray[c][r].encounterID = 0;
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
		//print map to file
		out << 1 << '\n';
		for (int c = 0; c < 4; c++)//print VdoorArray
		{
			for (int r = 0; r < 5; r++)
			{
				out << VdoorArray[c][r] << ',';
			}
		}
		out << '\n';
		for (int c = 0; c < 5; c++)//print HdoorArray
		{
			for (int r = 0; r < 4; r++)
			{
				out << HdoorArray[c][r] << ',';
			}
		}
		out << '\n';
		for (int r = 0; r < 5; r++)//print name and encounterID and visited
		{
			for (int c = 0; c < 5; c++)
			{
				out << roomArray[r][c].name << ',' << roomArray[r][c].encounterID << ',' << (int)roomArray[r][c].visited << ',';
			}
		}
		out.close();
	}
	else
	{
		for (int c = 0; c < 4; c++)//get VdoorArray
		{
			for (int r = 0; r < 5; r++)
			{
				getline(in, fileTemp, ',');
				VdoorArray[c][r] = stoi(fileTemp);
			}
		}
		getline(in, fileTemp);
		for (int c = 0; c < 5; c++)//get HdoorArray
		{
			for (int r = 0; r < 4; r++)
			{
				getline(in, fileTemp, ',');
				HdoorArray[c][r] = stoi(fileTemp);
			}
		}
		getline(in, fileTemp);
		for (int r = 0; r < 5; r++)//get name and encounterID
		{
			for (int c = 0; c < 5; c++)
			{
				getline(in, fileTemp, ',');
				roomArray[r][c].name = fileTemp;
				getline(in, fileTemp, ',');
				roomArray[r][c].encounterID = stoi(fileTemp);
				getline(in, fileTemp, ',');
				roomArray[r][c].visited = stoi(fileTemp);
			}
		}
	}
	char inputc;
	bool exit = false;
	player.credits = 0;
	//navigation/main game loop
	while (!exit)
	{
		system("cls");
		printf("you arive in sector (%i, %i), %s\n", playerC + 1, playerR + 1, roomArray[playerC][playerR].name.c_str());
		bool victory;
		if (false == roomArray[playerC][playerR].visited)
		{
			victory = encounter(player, roomArray[playerC][playerR].encounterID, enemyVector);
			roomArray[playerC][playerR].visited = true;
		}
		else
		{
			cout << "there is nothing else of interest here\n";
			_getch();
			victory = true;
		}
		updateCrewSave(player, playerC, playerR);
		updateMapSave(VdoorArray, HdoorArray, roomArray);
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
			cout << "i. view inventory\n";
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('m' == inputc)
			{
				displayMap(VdoorArray, HdoorArray, playerC, playerR);
				_getch();
			}
			else if ('i' == inputc)
			{
				printInventory(player);
			}
			else if (0 == HdoorArray[playerC][playerR - 1] && 0 != playerR && 'n' == inputc)
			{
				playerR--;
				break;
			}
			else if (0 == VdoorArray[playerC][playerR] && 4 != playerC && 'e' == inputc)
			{
				playerC++;
				break;
			}
			else if (0 == HdoorArray[playerC][playerR] && 4 != playerR && 's' == inputc)
			{
				playerR++;
				break;
			}
			else if (0 == VdoorArray[playerC - 1][playerR] && 0 != playerC && 'w' == inputc)
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

bool encounter(party &player, int encounterID, vector <eparty> enemyVector)//this is why we use structs
{
	if (0 == encounterID)//starting sector
	{
		printf("as your party leaves the mining station you see an ancient warp gate, long lost to time, shrinking in the distance\n");
		printf("while this region of the galaxy isn't necessarily the most civilized of places, there are still enough warp nodes\nuntouched to get from sector to sector\n");
		_getch();
		return 1;
	}
	else if (1 == encounterID)
	{
		cout << "your party approaches an ancient warp gate which could be their ticket out of these wastes" << endl;
		cout << "unfortunately, it won't be that easy, as a Warpgate Guardian steps through the gate to block their path" << endl;
		_getch();
		return combat(player, enemyVector[6]);
	}
	else if (2 == encounterID)//low difficulty enemies
	{
		switch (_dice(3))
		{
		case 1://spaceGoblins
			cout << "a haphazardly constructed vessel suddenly careens into the ship's starbord side" << endl;
			cout << "rushing to assess the damage, your crew stumbles upon a band of three raggedy goblins ravaging the crew quarters" << endl;
			_getch();
			return combat(player, enemyVector[0]);
		case 2:
			cout << "your crew exits warp straight into an astroid field" << endl;
			cout << "they begin to get an uneasy feeling when a breach is detected and tentacled creatures can be seen creeping into the ship" << endl;
			_getch();
			return combat(player, enemyVector[1]);
		case 3:
			cout << "lazers flash across the sky in this new sector" << endl;
			cout << "a squad of armed drones spot your ship and move in to attack" << endl;
			_getch();
			return combat(player, enemyVector[2]);
		}
	}
	else if (3 == encounterID)//medium difficulty enemies
	{
		string wormName;
		switch (1)
		{
		case 1://eyes
			cout << "as the ship exits warp, the air in the cabin seems to get colder" << endl;
			cout << player.cVect[1].name << " utters ominously:" << endl;
			cout << endl << "...This is going to be a terrible night..." << endl;
			_getch();
			return combat(player, enemyVector[3]);
		case 2://worm
			cout << "your crew finds a small moon with an abandoned mining station and decide to search for food an suplies" << endl;
			cout << "as they touch down, a blood curdling scream shatters the silence, and a large worm-like creature erupts from the ground" << endl;
			_getch();
			switch (_dice(5))//pick a name
			{
			case 1:
				wormName = "Akka";
				break;
			case 2:
				wormName = "Eir";
				break;
			case 3:
				wormName = "Ur";
				break;
			case 4:
				wormName = "Xol";
				break;
			case 5:
				wormName = "Yul";
				break;
			}
			eparty worm = enemyVector[4];
			worm.eVect[0].setName(wormName);
			return combat(player, worm);
		}
	}
	else if (4 == encounterID)//hard difficulty enemies
	{
		//golem
		cout << "your ship exits warp a little to close to a large volcanic planet" << endl;
		cout << "as the ship is pulled in by the planet's superior gravity, an enormous volcano erupts, revaling an ancient magma titan" << endl;
		_getch();
		return combat(player, enemyVector[5]);
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
		cout << "while carfully navigating an astroid field, a glint catches " << player.cVect[2].name << "'s eye\n";
		cout << "beaming the object into the ship reveals it to be a lost credit cache\n";
		_getch();
		treasureCache(player);
		return 1;
	}
}


//combat related functions
bool combat(party& player, eparty enemyParty)
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
		//print health window
		if (6 == enemyParty.graphicForID)
		{
			eparty temp = enemyParty;
			temp.graphicForID = (turnNum % 3) + 9;
			printHealthWindow(player, temp);
		}
		else
		{
			printHealthWindow(player, enemyParty);
		}
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
								if (a <= enemyParty.eVect[0].getName().length())
								{
									cout << enemyParty.eVect[0].getName()[a];
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
								if (a <= enemyParty.eVect[1].getName().length())
								{
									cout << enemyParty.eVect[1].getName()[a];
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
								if (a <= enemyParty.eVect[2].getName().length())
								{
									cout << enemyParty.eVect[2].getName()[a];
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
								if (a <= enemyParty.eVect[3].getName().length())
								{
									cout << enemyParty.eVect[3].getName()[a];
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
								if (a <= enemyParty.eVect[4].getName().length())
								{
									cout << enemyParty.eVect[4].getName()[a];
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
								if (a <= enemyParty.eVect[5].getName().length())
								{
									cout << enemyParty.eVect[5].getName()[a];
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
								cout << enemyParty.eVect[0].getName();
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[2].getName();
							}
							else if (1 == menu.X && 3 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[4].getName();
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 24;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[1].getName();
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[3].getName();
							}
							else if (2 == menu.X && 3 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[5].getName();
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
									cout << enemyParty.eVect[0].getName();
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[2].getName();
								}
								else if (1 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[4].getName();
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 24;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[1].getName();
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[3].getName();
								}
								else if (2 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[5].getName();
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
								if (a <= enemyParty.eVect[0].getName().length())
								{
									cout << enemyParty.eVect[0].getName()[a];
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
								if (a <= enemyParty.eVect[1].getName().length())
								{
									cout << enemyParty.eVect[1].getName()[a];
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
								if (a <= enemyParty.eVect[2].getName().length())
								{
									cout << enemyParty.eVect[2].getName()[a];
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
								if (a <= enemyParty.eVect[3].getName().length())
								{
									cout << enemyParty.eVect[3].getName()[a];
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
								if (a <= enemyParty.eVect[4].getName().length())
								{
									cout << enemyParty.eVect[4].getName()[a];
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
								if (a <= enemyParty.eVect[5].getName().length())
								{
									cout << enemyParty.eVect[5].getName()[a];
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
								cout << enemyParty.eVect[0].getName();
							}
							else if (1 == menu.X && 2 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[2].getName();
							}
							else if (1 == menu.X && 3 == menu.Y)
							{
								coord.X = 1;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[4].getName();
							}
							else if (2 == menu.X && 1 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 24;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[1].getName();
							}
							else if (2 == menu.X && 2 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 25;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[3].getName();
							}
							else if (2 == menu.X && 3 == menu.Y)
							{
								coord.X = 33;
								coord.Y = 26;
								SetConsoleCursorPosition(standard, coord);
								SetConsoleTextAttribute(color, 4);
								cout << enemyParty.eVect[5].getName();
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
									cout << enemyParty.eVect[0].getName();
								}
								else if (1 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[2].getName();
								}
								else if (1 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 1;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[4].getName();
								}
								else if (2 == menuPast.X && 1 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 24;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[1].getName();
								}
								else if (2 == menuPast.X && 2 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 25;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[3].getName();
								}
								else if (2 == menuPast.X && 3 == menuPast.Y)
								{
									coord.X = 33;
									coord.Y = 26;
									SetConsoleCursorPosition(standard, coord);
									SetConsoleTextAttribute(color, 15);
									cout << enemyParty.eVect[5].getName();
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
						menuReprint = true;
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
		for (int a = 0; a < enemyParty.eVect.size(); a++)
		{
			switch (enemyParty.eVect[a].getID())
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
			case 4:
				retinazerAI(player, enemyParty, a, turnNum);
				break;
			case 5:
				spazmatismAI(player, enemyParty, a, turnNum);
				break;
			case 6:
				wormAI(player, enemyParty, a, turnNum);
				break;
			case 7:
				golemAI(player, enemyParty, a, turnNum);
				break;
			case 8:
				guardianAI(player, enemyParty, a, turnNum);
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
			cout << "e1: action " << enemyParty.eVect[0].getAction() << " target " << enemyParty.eVect[0].getTarget() << "\n";
			cout << "e2: action " << enemyParty.eVect[1].getAction() << " target " << enemyParty.eVect[1].getTarget() << "\n";
			cout << "e3: action " << enemyParty.eVect[2].getAction() << " target " << enemyParty.eVect[2].getTarget() << "\n";
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
			player.cVect[a].defenceBonus = 0;
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
					basicEvade(player, a);
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
			switch (enemyParty.eVect[a].getID())
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
			case 4:
				retinazerActions(player, enemyParty, a);
				break;
			case 5:
				spazmatismActions(player, enemyParty, a);
				break;
			case 6:
				wormActions(player, enemyParty, a);
				break;
			case 7:
				golemActions(player, enemyParty, a);
				break;
			case 8:
				guardianActions(player, enemyParty, a, turnNum);
				break;
			}
		}
		killAndTop(player, enemyParty);
		bool allDead = true;
		for (int a = 0; a < enemyParty.a; a++)
		{
			if (!enemyParty.eVect[a].getDead())
			{
				allDead = false;
			}
		}
		if (allDead)
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
			if (a < enemy.eVect[0].getName().size())
			{
				cout << enemy.eVect[0].getName()[a];
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
		printHbar(enemy.eVect[0].getHP(), enemy.eVect[0].getHPmax(), 20);
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
			if (a < enemy.eVect[0].getName().size())
			{
				cout << enemy.eVect[0].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].getName().size())
			{
				cout << enemy.eVect[1].getName()[a];
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
		printHbar(enemy.eVect[0].getHP(), enemy.eVect[0].getHPmax(), 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[1].getHP(), enemy.eVect[1].getHPmax(), 20);
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
			if (a < enemy.eVect[0].getName().size())
			{
				cout << enemy.eVect[0].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].getName().size())
			{
				cout << enemy.eVect[1].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].getName().size())
			{
				cout << enemy.eVect[2].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[0].getHP(), enemy.eVect[0].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[1].getHP(), enemy.eVect[1].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[2].getHP(), enemy.eVect[2].getHPmax(), 20);
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
			if (a < enemy.eVect[0].getName().size())
			{
				cout << enemy.eVect[0].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].getName().size())
			{
				cout << enemy.eVect[1].getName()[a];
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
		printHbar(enemy.eVect[0].getHP(), enemy.eVect[0].getHPmax(), 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[1].getHP(), enemy.eVect[1].getHPmax(), 20);
		printf("        ");//print 8 spaces
		printf("%c\n", 179);
		printf("%c", 179);
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].getName().size())
			{
				cout << enemy.eVect[2].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[3].getName().size())
			{
				cout << enemy.eVect[3].getName()[a];
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
		printHbar(enemy.eVect[2].getHP(), enemy.eVect[2].getHPmax(), 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[3].getHP(), enemy.eVect[3].getHPmax(), 20);
		printf("        ");//print 8 spaces
	}
	else if (5 == enemy.a)
	{
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].getName().size())
			{
				cout << enemy.eVect[0].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].getName().size())
			{
				cout << enemy.eVect[1].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].getName().size())
			{
				cout << enemy.eVect[2].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[0].getHP(), enemy.eVect[0].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[1].getHP(), enemy.eVect[1].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[2].getHP(), enemy.eVect[2].getHPmax(), 20);
		printf("%c\n", 179);
		printf("%c", 179);
		printf("        ");//print 8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[3].getName().size())
			{
				cout << enemy.eVect[3].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("        ");//8 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[4].getName().size())
			{
				cout << enemy.eVect[4].getName()[a];
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
		printHbar(enemy.eVect[3].getHP(), enemy.eVect[3].getHPmax(), 20);
		printf("        ");//8 spaces
		printHbar(enemy.eVect[4].getHP(), enemy.eVect[4].getHPmax(), 20);
		printf("        ");//print 8 spaces
	}
	else if (6 == enemy.a)
	{
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[0].getName().size())
			{
				cout << enemy.eVect[0].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[1].getName().size())
			{
				cout << enemy.eVect[1].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[2].getName().size())
			{
				cout << enemy.eVect[2].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[0].getHP(), enemy.eVect[0].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[1].getHP(), enemy.eVect[1].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[2].getHP(), enemy.eVect[2].getHPmax(), 20);
		printf("%c\n", 179);
		printf("%c", 179);
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[3].getName().size())
			{
				cout << enemy.eVect[3].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[4].getName().size())
			{
				cout << enemy.eVect[4].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("  ");//2 spaces
		for (int a = 0; a < 20; a++)
		{
			if (a < enemy.eVect[5].getName().size())
			{
				cout << enemy.eVect[5].getName()[a];
			}
			else
			{
				cout << " ";
			}
		}
		printf("%c\n", 179);
		printf("%c", 179);
		printHbar(enemy.eVect[3].getHP(), enemy.eVect[3].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[4].getHP(), enemy.eVect[4].getHPmax(), 20);
		printf("  ");//2 spaces
		printHbar(enemy.eVect[5].getHP(), enemy.eVect[5].getHPmax(), 20);
	}
	printf("%c\n", 179);
	printf("%c", 192);
	for (int a = 1; a <= 64; a++) { printf("%c", 196); }
	printf("%c\n", 217);
	//print graphic
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
	for (int e = 0; e < enemyParty.a; e++)
	{
		if (!enemyParty.eVect[e].getDead())
		{
			if (enemyParty.eVect[e].getHP() <= 0)
			{
				enemyParty.eVect[e].setDead(true);
			}
			else if (enemyParty.eVect[e].getHPmax() < enemyParty.eVect[e].getHP())
			{
				enemyParty.eVect[e].setHP(enemyParty.eVect[e].getHPmax());
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

void treasureCache(party& player)
{
	system("cls");
	COORD coord;
	coord.X = 0;
	coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "[press any key to open]";
	pixelArtRelay(7, 7);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "the cache contained:                             \n";
	int random = _dice(6);
	if (1 == random)
	{
		cout << "Large Stapler (attack)\n";
		player.inventory.addItem(1);
	}
	else if (2 == random)
	{
		cout << "Room 206 Printer (attack)\n";
		player.inventory.addItem(2);
	}
	else if (3 == random)
	{
		cout << "Lizard Staff (special)\n";
		player.inventory.addItem(4);
	}
	else if (4 == random)
	{
		cout << "Wizard Tail (special)\n";
		player.inventory.addItem(5);
	}
	else if (5 == random)
	{
		cout << "Book of Insults (defensive)\n";
		player.inventory.addItem(7);
	}
	else if (6 == random)
	{
		cout << "Body Armor (defensive)\n";
		player.inventory.addItem(8);
	}
	int creditsFound = 199 + _dice(400);
	cout << creditsFound << " credits\n";
	player.credits += creditsFound;
	_getch();
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
	ifstream in;
	string image = "";
	vector <string> animation;
	string temp, temp2;
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
	case 3://drone swarm
		break;
	case 4://golem
		image += "00666000000000000000000000066600n";
		image += "00666000000000000000000000066600n";
		image += "00777000000000000000000000077700n";
		image += "00666000000000000000000000066600n";
		image += "00666000000000000000000000066600n";
		image += "00666000000000000000000000066600n";
		image += "00666000000000000000000000066600n";
		image += "00777008848888848888888840077700n";
		image += "00666888448888844888884488866600n";
		image += "08666444884888888488884448866680n";
		image += "86666688884888888488888884666668";
		printCombatGraphic(image, 6);
		break;
	case 5://worm
		break;
	case 6://guardian
		image += "f000000f00007777777700000000f000n";
		image += "0000f0000077dddddd557700f0000000n";
		image += "000000000755555555dd557000000000n";
		image += "000000500755dddd5555d570050000f0n";
		image += "00000000075d55555555d570000f0000n";
		image += "0f0000700755dddddddd557007000000n";
		image += "0000507f007755555555770007050000n";
		image += "000000700088777777778800070000f0n";
		image += "00007078888888888888888887070000n";
		image += "f0007088888888888888888888070000n";
		image += "00007888888888888888888888870000";
		printCombatGraphic(image, 6);
		break;
	case 7://chest opening
		break;
	case 8://eyes
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000n";
		image += "00000000000000000000000000000000";
		printCombatGraphic(image, 6);
		break;
	default:
		cout << "ERROR\n";
	}
	image = "";
	switch (forID)
	{
	case 0:
		image += "                                n";
		image += "              aaaa              n";
		image += "          22 aaaaaa 22          n";
		image += "           22a4aa4a22           n";
		image += "           22aa22aa22           n";
		image += "            2aa22aa2            n";
		image += "             aaaaaa             n";
		image += "             aa22aa             n";
		image += "              aaaa              n";
		image += "           2222aa2222           n";
		image += "          222222222222          ";
		printCombatGraphic(image, 6);
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
		image += "00000004000000044000000040000000n";
		image += "0000004c4000004444000004c4000000n";
		image += "0048004c4000044cc4400004c4880400n";
		image += "04c4804c400044cffc440004c4884c40n";
		image += "04c4804c40044cf55fc44004c4884c40n";
		image += "04c4804440044cf55fc4400444884c40n";
		image += "04c48884000044cffc44000048884c40n";
		image += "00408884000044cccc44000040000400n";
		image += "004400044000044cc448800440004400n";
		image += "00044000440004444448804400044000n";
		image += "00004444044004444440044044440000";
		printCombatGraphic(image, 6);
		break;
	case 3:
		image += "00000000000000000000000000000000n";
		image += "000000fff0000f0000f0000000000000n";
		image += "00000ff4ff000f0000f00000000fff00n";
		image += "000000fff00000f00f00000000ff4ff0n";
		image += "00000000000000f00f000000000fff00n";
		image += "0000000000000ffffff0000000000000n";
		image += "000fff000000ffffffff000000000000n";
		image += "00ff4ff0000ffff4cffff000fff00000n";
		image += "000fff00000ffff44ffff00ff4ff0000n";
		image += "000000000000ffffffff0000fff00000n";
		image += "0000000000000ffffff0000000000000";
		printCombatGraphic(image, 6);
		break;
	case 4://golem forground
		image += "                                n";
		image += "              6666              n";
		image += "             666666             n";
		image += "             846648             n";
		image += "             886688             n";
		image += "             888888             n";
		image += "            66888866            n";
		image += "          668666666866          n";
		image += "         66668888886666         n";
		image += "         66666666666666         n";
		image += "         66666666666666         ";
		printCombatGraphic(image, 6);
		break;
	case 5://worm forground
		image += "00000000000000000000000000000000n";
		image += "08000000000006666660000000000000n";
		image += "08800088000060f0f0f6000000800000n";
		image += "000000000066f0000000600000000800n";
		image += "0000000007660000000f608800000800n";
		image += "000000006766f0000000600000000000n";
		image += "0080000067660000000f600000000000n";
		image += "0000000667666f0f0f06000000000000n";
		image += "00000006677666666666000008000000n";
		image += "00008007667766666666080000088000n";
		image += "00000007766677776660000000088000";
		printCombatGraphic(image, 6);
		break;
	case 6://guardian forground
		//see case 9, 10, and 11
		break;
	case 7://chest animation
		in.open("chest_opening_animation.txt");
		temp2 = "";
		for (int f = 0; f < 9; f++)
		{
			for (int a = 0; a < 25; a++)
			{
				getline(in, temp);
				temp2 += temp + 'n';
			}
			getline(in, temp);
			animation.push_back(temp2);
			temp2 = "";
		}
		in.close();
		printImage(animation[0], 0);
		_getch();
		for (int a = 1; a < 6; a++)
		{
			printImage(animation[a], 0);
			Sleep(30);
		}
		break;
	case 8://eyes
		image += "                                n";
		image += "                                n";
		image += "        ff2          444        n";
		image += "       4f202       44  44       n";
		image += "       4ff2f      4    444      n";
		image += "      444fff    44   44444      n";
		image += "      44444   44    fff444      n";
		image += "      444    4      f4ff4       n";
		image += "       44  44       404f4       n";
		image += "        444          4ff        n";
		image += "                                ";
		printCombatGraphic(image, 6);
		break;
	case 9:
		image += "             f7777f             n";
		image += "             ff77ff             n";
		image += "             ffffff             n";
		image += "             ff44ff             n";
		image += "             ff44ff             n";
		image += "          fff7ffff7fff          n";
		image += "         fffff7777fffff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         ";
		printCombatGraphic(image, 6);
		break;
	case 10:
		image += "             f7777f             n";
		image += "             ff77ff             n";
		image += "             ffffff             n";
		image += "             ff22ff             n";
		image += "             ff22ff             n";
		image += "          fff7ffff7fff          n";
		image += "         fffff7777fffff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         ";
		printCombatGraphic(image, 6);
		break;
	case 11:
		image += "             f7777f             n";
		image += "             ff77ff             n";
		image += "             ffffff             n";
		image += "             ff55ff             n";
		image += "             ff55ff             n";
		image += "          fff7ffff7fff          n";
		image += "         fffff7777fffff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         n";
		image += "         fff7ffffff7fff         ";
		printCombatGraphic(image, 6);
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
	enemyParty.eVect[a].setAction((int)temp);
	enemyParty.eVect[a].setDodgeBonus(0);
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
				if (!enemyParty.eVect[0].getDead())
				{
					tempTarget = 0;
					pickedOne = true;
				}
				break;
			case 2:
				if (!enemyParty.eVect[1].getDead())
				{
					tempTarget = 1;
					pickedOne = true;
				}
				break;
			case 3:
				if (!enemyParty.eVect[2].getDead())
				{
					tempTarget = 2;
					pickedOne = true;
				}
				break;
			case 4:
				if (!enemyParty.eVect[3].getDead())
				{
					tempTarget = 3;
					pickedOne = true;
				}
				break;
			case 5:
				if (!enemyParty.eVect[4].getDead())
				{
					tempTarget = 4;
					pickedOne = true;
				}
				break;
			case 6:
				if (!enemyParty.eVect[5].getDead())
				{
					tempTarget = 5;
					pickedOne = true;
				}
			}
		}

	}
	else if (evade == temp)
	{
		tempTarget = 0;
		enemyParty.eVect[a].setDodgeBonus(3);
	}
	enemyParty.eVect[a].setTarget(tempTarget);
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " attacks " << player.cVect[enemyParty.eVect[e].getTarget()].name << " for " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//uses special on
		{
			if (e == enemyParty.eVect[e].getTarget())//goblin targeting itself increases it's attack multiplier
			{
				cout << enemyParty.eVect[e].getName() << " gives its ray gun a smack, causing it to surge with energy";
				enemyParty.eVect[e].multAtkMulti(1.1);
			}
			else if (0 == enemyParty.eVect[e].getTarget())
			{
				cout << enemyParty.eVect[e].getName() << " tends to " << enemyParty.eVect[0].getName() << "'s wounds for 3 health";
				enemyParty.eVect[0].addHP(3);
			}
			else if (1 == enemyParty.eVect[e].getTarget())
			{
				cout << enemyParty.eVect[e].getName() << " tends to " << enemyParty.eVect[1].getName() << "'s wounds for 3 health";
				enemyParty.eVect[1].addHP(3);
			}
			else if (2 == enemyParty.eVect[e].getTarget())
			{
				cout << enemyParty.eVect[e].getName() << " tends to " << enemyParty.eVect[2].getName() << "'s wounds for 3 health";
				enemyParty.eVect[2].addHP(3);
			}
			else
			{
				cout << "ERROR: NO TARGET";
			}
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " has defended from the attack";
		}
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
	enemyParty.eVect[a].setAction((int)temp);
	enemyParty.eVect[a].setDodgeBonus(0);
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
				if (!enemyParty.eVect[0].getDead())
				{
					tempTarget = 0;
					pickedOne = true;
				}
				break;
			case 2:
				if (!enemyParty.eVect[1].getDead())
				{
					tempTarget = 1;
					pickedOne = true;
				}
				break;
			case 3:
				if (!enemyParty.eVect[2].getDead())
				{
					tempTarget = 2;
					pickedOne = true;
				}
				break;
			case 4:
				if (!enemyParty.eVect[3].getDead())
				{
					tempTarget = 3;
					pickedOne = true;
				}
				break;
			case 5:
				if (!enemyParty.eVect[4].getDead())
				{
					tempTarget = 4;
					pickedOne = true;
				}
				break;
			case 6:
				if (!enemyParty.eVect[5].getDead())
				{
					tempTarget = 5;
					pickedOne = true;
				}
			}
		}

	}
	else if (evade == temp)
	{
		tempTarget = 0;
		enemyParty.eVect[a].setDodgeBonus(2);
	}
	enemyParty.eVect[a].setTarget(tempTarget);
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " whips " << player.cVect[enemyParty.eVect[e].getTarget()].name << " with it's tentacles, doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//uses special on
		{
			if (e == enemyParty.eVect[e].getTarget())//squid targeting itself increases it's attack multiplier
			{
				cout << enemyParty.eVect[e].getName() << " begins to spin, building momentum";
				enemyParty.eVect[e].multAtkMulti(1.4);
			}
			else
			{
				cout << enemyParty.eVect[e].getName() << " engulfs " << enemyParty.eVect[enemyParty.eVect[e].getTarget()].getName() << " in a cloud of healing ink, healing 2 damage";
				enemyParty.eVect[enemyParty.eVect[e].getTarget()].addHP(2);
			}
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " has defended from the attack";
		}
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
	enemyParty.eVect[a].setAction((int)temp);
	enemyParty.eVect[a].setDodgeBonus(0);
	if (special == temp)
	{
		enemyParty.eVect[a].setTarget(1);//drones always target the brain
	}
	else if (attack == temp)
	{
		enemyParty.eVect[a].setTarget(_dice(3) - 1);
	}
	else if (evade == temp)
	{
		enemyParty.eVect[a].setTarget(0);
		enemyParty.eVect[a].setDodgeBonus(3);
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " fires it's lazer at " << player.cVect[enemyParty.eVect[e].getTarget()].name << " doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//uses special on
		{
			cout << enemyParty.eVect[e].getName() << " siphons energy into " << enemyParty.eVect[enemyParty.eVect[e].getTarget()].getName();
			enemyParty.eVect[enemyParty.eVect[e].getTarget()].multAtkMulti(1.2);
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " has defended from the attack";
		}
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
	enemyParty.eVect[a].setAction((int)temp);
	enemyParty.eVect[a].setDodgeBonus(0);
	if (attack == temp)
	{
		enemyParty.eVect[a].setTarget(_dice(3) - 1);
	}
	else if (evade == temp)
	{
		enemyParty.eVect[a].setTarget(0);
		enemyParty.eVect[a].setDodgeBonus(4);
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " fires a beam at " << player.cVect[enemyParty.eVect[e].getTarget()].name << " for " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " has defended from the attack";
		}
		_getch();
	}
	return;
}

void wormAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	enemyParty.eVect[a].setDodgeBonus(0);
	int random = _dice(6);
	if (1 <= random && random <= 3)//attack
	{
		enemyParty.eVect[a].setAction(0);
		enemyParty.eVect[a].setTarget(turnNum % 3);
	}
	else if (4 == random)//evade, builds up armor
	{
		enemyParty.eVect[a].setAction(1);
		enemyParty.eVect[a].setTarget(turnNum % 3);
		enemyParty.eVect[a].setDodgeBonus(2);
		enemyParty.eVect[a].setDodgeValue(enemyParty.eVect[a].getDodgeValue() + 1);//add 1 to armor
	}
	else if (5 <= random && random <= 6)//special, sheds its armor to damage every party member
	{
		enemyParty.eVect[a].setAction(2);
		enemyParty.eVect[a].setTarget(0);
		if (1 <= enemyParty.eVect[a].getDodgeValue())//subtract 1 from dodgeValue
		{
			enemyParty.eVect[a].setDodgeValue(enemyParty.eVect[a].getDodgeValue() - 1);
		}
	}
	return;
}

void wormActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " charges at " << player.cVect[enemyParty.eVect[e].getTarget()].name << " doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//damages all party members
		{
			cout << enemyParty.eVect[e].getName() << " flings fragments of its shell in every direction, doing " << TempAtk / 3 << " damage";
			for (int a = 0; a < 3; a++)
			{
				player.cVect[a].HP -= (TempAtk / 3);
			}
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " built up its armored shell and hunkered down";
		}
		_getch();
	}
	if (0 == enemyParty.eVect[e].getDodgeValue() && special == (action)enemyParty.eVect[e].getAction())//set HP to 1 if dodgeValue = 0
	{
		SetConsoleCursorPosition(standard, coord);
		for (int a = 0; a < 80; a++)
		{
			cout << " ";
		}
		SetConsoleCursorPosition(standard, coord);
		cout << enemyParty.eVect[e].getName() << " has shed it's entire armored shell. It is vulnerable!";
		enemyParty.eVect[e].setHP(1);
	}
	return;
}

void retinazerAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	if (enemyParty.eVect[a].getHP() <= 13)
	{
		enemyParty.eVect[a].setDodgeBonus(4);
		enemyParty.eVect[a].setAtkMulti(1.2);
	}
	else
	{
		enemyParty.eVect[a].setDodgeBonus(0);
		enemyParty.eVect[a].setAtkMulti(1.0);
	}
	int tempTarget = 0;
	//determine target (character with most health)
	if (player.cVect[1].HP <= player.cVect[0].HP && player.cVect[2].HP <= player.cVect[0].HP)
	{
		tempTarget = 0;
	}
	else if (player.cVect[0].HP <= player.cVect[1].HP && player.cVect[2].HP <= player.cVect[1].HP)
	{
		tempTarget = 1;
	}
	else if (player.cVect[0].HP <= player.cVect[2].HP && player.cVect[1].HP <= player.cVect[2].HP)
	{
		tempTarget = 2;
	}
	//determine action
	int random = _dice(4);//1 in 4 chance of using special attack
	if (1 == random)//special attack
	{
		enemyParty.eVect[a].setAction(2);
	}
	else//normal attack
	{
		enemyParty.eVect[a].setAction(0);

	}
	enemyParty.eVect[a].setTarget(tempTarget);
	return;
}

void spazmatismAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	float turnBonus = 1.0;
	for (int b = 0; b < turnNum; b++)
	{
		turnBonus *= 1.05;
	}
	if (enemyParty.eVect[a].getHP() <= 13)
	{
		enemyParty.eVect[a].setDodgeBonus(5);
		enemyParty.eVect[a].setAtkMulti(1.3 * turnBonus);
	}
	else
	{
		enemyParty.eVect[a].setDodgeBonus(0);
		enemyParty.eVect[a].setAtkMulti(1.0 * turnBonus);
	}
	int tempTarget = 2;
	//determine target (character with least health)
	if (player.cVect[0].HP < player.cVect[1].HP && player.cVect[0].HP < player.cVect[2].HP)
	{
		tempTarget = 0;
	}
	else if (player.cVect[1].HP < player.cVect[0].HP && player.cVect[1].HP < player.cVect[2].HP)
	{
		tempTarget = 1;
	}
	else if (player.cVect[2].HP < player.cVect[0].HP && player.cVect[2].HP < player.cVect[1].HP)
	{
		tempTarget = 2;
	}
	//determine action
	int random = _dice(4);//1 in 4 chance of using special attack
	if (1 == random)//special attack
	{
		enemyParty.eVect[a].setAction(2);
	}
	else//normal attack
	{
		enemyParty.eVect[a].setAction(0);

	}
	enemyParty.eVect[a].setTarget(tempTarget);
	return;
}

void retinazerActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " charges at " << player.cVect[enemyParty.eVect[e].getTarget()].name << " doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//damages all party members
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0))) + 2;
			cout << enemyParty.eVect[e].getName() << " fires its lazer at " << player.cVect[enemyParty.eVect[e].getTarget()].name << ", doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		_getch();
	}
	return;
}

void spazmatismActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " charges at " << player.cVect[enemyParty.eVect[e].getTarget()].name << " doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//damages all party members
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0))) + 2;
			cout << enemyParty.eVect[e].getName() << " sinks its teeth into " << player.cVect[enemyParty.eVect[e].getTarget()].name << ", doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		_getch();
	}
	return;
}

void golemAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	enemyParty.eVect[a].setDodgeBonus(0);
	int random = turnNum % 3;
	enemyParty.eVect[a].setAction(player.cVect[random].act);
	enemyParty.eVect[a].setTarget(2 - random);
	if (evade == enemyParty.eVect[a].getAction())
	{
		enemyParty.eVect[a].setDodgeBonus(6);
	}
	return;
}

void golemActions(party& player, eparty& enemyParty, int e)
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " pounds " << player.cVect[enemyParty.eVect[e].getTarget()].name << " into the ground, doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//damages all party members
		{
			cout << enemyParty.eVect[e].getName() << " bellows an ancient curse";
			player.cVect[0].atkMulti *= 0.95;
			player.cVect[1].atkMulti *= 0.95;
			player.cVect[2].atkMulti *= 0.95;
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " built up it's rocky crust";
		}
		_getch();
	}
	return;
}

void guardianAI(party& player, eparty& enemyParty, int a, int turnNum)
{
	enemyParty.eVect[a].setDodgeBonus(0);
	int random = _dice(5);
	if (1 == random)//attack
	{
		enemyParty.eVect[a].setAction(0);
	}
	else if (2 == random)//evade
	{
		enemyParty.eVect[a].setAction(1);
		enemyParty.eVect[a].setDodgeBonus(5);
	}
	else//special
	{
		enemyParty.eVect[a].setAction(2);
	}
	int tempTarget = 0;
	//determine target (character with most health)
	if (player.cVect[1].HP <= player.cVect[0].HP && player.cVect[2].HP <= player.cVect[0].HP)
	{
		tempTarget = 0;
	}
	else if (player.cVect[0].HP <= player.cVect[1].HP && player.cVect[2].HP <= player.cVect[1].HP)
	{
		tempTarget = 1;
	}
	else if (player.cVect[0].HP <= player.cVect[2].HP && player.cVect[1].HP <= player.cVect[2].HP)
	{
		tempTarget = 2;
	}
	enemyParty.eVect[a].setTarget(tempTarget);
	return;
}

void guardianActions(party& player, eparty& enemyParty, int e, int turnNum)
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
	int TempAtk = (float)enemyParty.eVect[e].getAtk() * enemyParty.eVect[e].getAtkMulti();
	if (!enemyParty.eVect[e].getDead())//enemy
	{
		if (attack == (action)enemyParty.eVect[e].getAction())//attacks 
		{
			int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
			cout << enemyParty.eVect[e].getName() << " pummles " << player.cVect[enemyParty.eVect[e].getTarget()].name << ", doing " << damage << " damage";
			player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
		}
		else if (special == (action)enemyParty.eVect[e].getAction())//damages all party members
		{
			if (0 == turnNum % 3)//special attack
			{
				int damage = (int)ceil(((float)TempAtk / (0.15 * (float)player.cVect[enemyParty.eVect[e].getTarget()].defenceValue + 1.0)));
				cout << enemyParty.eVect[e].getName() << " fires a beam of hot plasma at " << player.cVect[enemyParty.eVect[e].getTarget()].name << ", doing " << damage << " damage";
				player.cVect[enemyParty.eVect[e].getTarget()].HP -= damage;
			}
			else if (1 == turnNum % 3)//heal self
			{
				cout << enemyParty.eVect[e].getName() << " siphons energy from the portal to replenish its shields";
				enemyParty.eVect[e].addHP(9);
			}
			else//charge attack
			{
				cout << enemyParty.eVect[e].getName() << " charges its weapons for a powerful attack";
				enemyParty.eVect[e].multAtkMulti(1.15);
			}
		}
		else if (evade == (action)enemyParty.eVect[e].getAction())
		{
			cout << enemyParty.eVect[e].getName() << " protected itself with a force field";
		}
		_getch();
	}
	return;
}

//player attacks
void printProjectAttack(party& player, eparty& enemyParty, int p)
{
	int e = player.cVect[p].target;
	int damage = (0.06 * player.projectsPrinted * player.projectsPrinted) + (0.1 * player.projectsPrinted) * player.cVect[p].atkMulti;
	cout << player.cVect[p].name << " turns their project in to " << enemyParty.eVect[e].getName() << " it's " << player.projectsPrinted << " pages long!";
	enemyParty.eVect[e].addHP((int)ceil(((float)damage / (0.15 * (float)(enemyParty.eVect[e].getDodgeBonus() + enemyParty.eVect[e].getDodgeValue() + 1)))) * -1);
}

void largeStapelerAttack(party& player, eparty& enemyParty, int p)//p = which player is executing the action (0-2)
{
	int e = player.cVect[p].target;
	int damage = (2 + _dice(5)) * ((player.cVect[p].atkMulti - 1.0) * 3.0 + 1.0);
	cout << player.cVect[p].name << " pelts " << enemyParty.eVect[e].getName() << " with large staples for " << damage << " damage";
	enemyParty.eVect[e].addHP((int)ceil(((float)damage / (0.15 * (float)(enemyParty.eVect[e].getDodgeBonus() + enemyParty.eVect[e].getDodgeValue() + 1)))) * -1);
}

void basicAttack(party& player, eparty& enemyParty, int p)
{
	int e = player.cVect[p].target;
	int damage = 8 * player.cVect[p].atkMulti;
	damage = (int)ceil(((float)damage / (0.15 * (float)(enemyParty.eVect[e].getDodgeBonus() + enemyParty.eVect[e].getDodgeValue()) + 1)));
	cout << player.cVect[p].name << " attacks " << enemyParty.eVect[e].getName() << " for " << damage << " damage";
	enemyParty.eVect[e].addHP(damage * -1);
}

//player specials
void basicHeal(party& player, eparty& enemyParty, int p)
{
	int healRate = 4;
	cout << player.cVect[p].name << " tends to " << player.cVect[player.cVect[p].target].name << "'s wounds, healing " << healRate << " HP";
	player.cVect[player.cVect[p].target].HP += healRate;
}

//player defenses
void basicEvade(party& player, int p)
{
	player.cVect[p].defenceBonus = 2;
}

//inventory stuff
void printInventory(party& player)
{
	bool exitInventory = false;
	while (!exitInventory)
	{
		system("cls");
		COORD position;
		cout << "Inventory:\n";
		for (int i = 0; i < player.inventory.getSize(); i++)
		{
			cout << "   " << player.inventory.getName(i);
			if (3 != player.inventory.getItemEquip(i))
			{
				cout << " (" << player.cVect[player.inventory.getItemEquip(i)].name << ")";
			}
			cout << "\n";
		}
		cout << "   Done";
		position.X = 93;
		position.Y = 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
		cout << "Credits: " << player.credits;
		position.X = 1;
		position.Y = 1;
		COORD pastPosition;
		pastPosition.X = 1;
		pastPosition.Y = 1;
		bool reprint = false;
		while (!reprint)
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pastPosition);
			cout << " ";
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
			cout << ">";
			pastPosition = position;
			char inputc = _getdir();
			if ('u' == inputc)
			{
				if (1 != position.Y)
				{
					position.Y--;
				}
			}
			else if ('d' == inputc)
			{
				if (player.inventory.getSize() + 1 != position.Y)
				{
					position.Y++;
				}
			}
			else if (13 == inputc)
			{
				int itemChosen = (int)position.Y - 1;
				if (player.inventory.getSize() == itemChosen)
				{
					exitInventory = true;
					break;
				}
				position.X = 30;
				position.Y = 0;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
				cout << "Equip to:";
				position.X = 33;
				for (int a = 1; a < 4; a++)//print crew names
				{
					position.Y = a;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
					cout << player.cVect[a - 1].name;
				}
				position.X = 31;
				position.Y = 1;
				while (true)//pick crew to equip item
				{

					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pastPosition);
					cout << " ";
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
					cout << ">";
					pastPosition = position;
					inputc = _getdir();
					if ('u' == inputc)
					{
						if (1 != position.Y)
						{
							position.Y--;
						}
					}
					else if ('d' == inputc)
					{
						if (3 != position.Y)
						{
							position.Y++;
						}
					}
					else if (13 == inputc)
					{
						int characterChosen = (int)position.Y - 1;
						equipItem(player, itemChosen, characterChosen);
						reprint = true;
						break;
					}
				}
			}
		}
	}
}

void equipItem(party& player, int i, int a)//player, inventory slot, character
{
	//find item id of players current item
	int currentItemID;
	if (0 == player.inventory.getItemType(i))//attack
	{
		switch (player.cVect[a].attackID)
		{
		case 0:
			currentItemID = 0;
			break;
		case 1:
			currentItemID = 1;
			break;
		case 2:
			currentItemID = 2;
			break;
		}
	}
	else if (1 == player.inventory.getItemType(i))//special
	{
		switch (player.cVect[a].specialID)
		{
		case 0:
			currentItemID = 3;
			break;
		case 1:
			currentItemID = 4;
			break;
		case 2:
			currentItemID = 5;
			break;
		}
	}
	else if (2 == player.inventory.getItemType(i))
	{
		switch (player.cVect[a].defenceID)
		{
		case 0:
			currentItemID = 6;
			break;
		case 1:
			currentItemID = 7;
			break;
		case 2:
			currentItemID = 8;
			break;
		}
	}
	//search for item equiped to character a and has and ID matching character a's old item and unequip it
	player.inventory.setEquip(player.inventory.searchForItem(currentItemID, a), 3);
	//check if item i is equiped by another character, if it is, use recursion to equip the offending character with a basic version of the attack
	if (a != player.inventory.getItemEquip(i) && 3 != player.inventory.getItemEquip(i))
	{
		int unusedItem;
		switch (player.inventory.getItemType(i))
		{
		case 0:
			unusedItem = player.inventory.searchForItem(0, 3);
			break;
		case 1:
			unusedItem = player.inventory.searchForItem(3, 3);
			break;
		case 2:
			unusedItem = player.inventory.searchForItem(6, 3);
			break;
		}
		equipItem(player, unusedItem, player.inventory.getItemEquip(i));//YEAH BABY, I USED RECURSION
	}
	//set item i to being equiped by character a
	player.inventory.setEquip(i, a);
	//set characters item ID to item i
	int tempID = player.inventory.getItemID(i);
	if (0 <= tempID && tempID <= 2)//attack item
	{
		int adjustedID;
		switch (player.inventory.getItemID(i))
		{
		case 0:
			adjustedID = 0;
			break;
		case 1:
			adjustedID = 1;
			break;
		case 2:
			adjustedID = 2;
		}
		player.cVect[a].attackID = adjustedID;
	}
	else if (3 <= tempID && tempID <= 5)//special
	{
		int adjustedID;
		switch (player.inventory.getItemID(i))
		{
		case 3:
			adjustedID = 0;
			break;
		case 4:
			adjustedID = 1;
			break;
		case 5:
			adjustedID = 2;
		}
		player.cVect[a].specialID = adjustedID;
	}
	else if (6 <= tempID && tempID <= 5)//defence
	{
		int adjustedID;
		switch (player.inventory.getItemID(i))
		{
		case 6:
			adjustedID = 0;
			break;
		case 7:
			adjustedID = 1;
			break;
		case 8:
			adjustedID = 2;
		}
		player.cVect[a].defenceID = adjustedID;
	}
}



void updateCrewSave(party player, int playerC, int playerR)
 {
	ofstream out;
	out.open("CrewSave.txt");
	out << player.ship.name << ',' << player.ship.HP << ',' << player.ship.HPmax << '\n' << player.projectsPrinted << '\n';
	out << playerR << ',' << playerC << '\n';
	for (int a = 0; a < 3; a++)
	{
		out << player.cVect[a].name << ',' << player.cVect[a].HP << ',' << player.cVect[a].HPmax << ',';
		out << player.cVect[a].dead << ',' << player.cVect[a].attackID << ',';
		out << player.cVect[a].specialID << ',' << player.cVect[a].defenceID << '\n';
	}
	out.close();
	out.open("Inventory.txt");
	for (int i = 0; i < player.inventory.getSize(); i++)
	{
		out << player.inventory.getItemID(i) << ',' << player.inventory.getItemEquip(i) << '\n';
	}
	out.close();
}

void updateMapSave(bool VdoorArray[4][5], bool HdoorArray[5][4], roomInfo roomArray[5][5])
{
	ofstream out;
	out.open("MapSave.txt");
	out << 1 << '\n';
	for (int c = 0; c < 4; c++)//print VdoorArray
	{
		for (int r = 0; r < 5; r++)
		{
			out << VdoorArray[c][r] << ',';
		}
	}
	out << '\n';
	for (int c = 0; c < 5; c++)//print HdoorArray
	{
		for (int r = 0; r < 4; r++)
		{
			out << HdoorArray[c][r] << ',';
		}
	}
	out << '\n';
	for (int r = 0; r < 5; r++)//print name and encounterID and visited
	{
		for (int c = 0; c < 5; c++)
		{
			out << roomArray[r][c].name << ',' << roomArray[r][c].encounterID << ',' << (int)roomArray[r][c].visited << ',';
		}
	}
	out.close();
}