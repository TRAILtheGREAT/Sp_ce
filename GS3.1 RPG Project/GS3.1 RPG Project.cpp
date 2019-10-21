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

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <string>

using namespace std;

void displayMap(bool(&VdoorArray)[4][5], bool(&HdoorArray)[5][4]);
int _dice(int max);
bool encounter(string engineer, string scientist, string pilot, int engHP, int engWEP, int sciHP, int sciWEP, int pilHP, int pilWEP, int encounterID);

struct roomInfo //room struct will store location and encounter. door state will be stored in the door arrays
{
	string name;
	int encounterID;//npc, enemy, or encounter in this room
};

int main()
{
	srand((unsigned)time(NULL));
	printf("Hello!\nMy name is Trail Sammarco, the creator of this game.\nMiyoshi says that introductions should be written last so that's all I'll say about that.\n\n");
	printf("You are in control of a group of 3 cosmonauts.\n");
	printf("After a terrible accident they are stranded in a dangerous region of the galaxy thousands of lightyears from home.\n");
	printf("They will need to fight through the galaxy's most dangerous foes to get them home and find out the truth about what happened...\n\n");
	printf("in Sp_ce\n\n");
	string engineer;
	string scientist;
	string pilot;
	printf("your first crew member is the engineer\nWhat is their name?\n");
	getline(cin, engineer);
	printf("your second crew member is the scientist\nWhat is their name?\n");
	getline(cin, scientist);
	printf("the last crew member is the pilot\nWhat is their name?\n");
	getline(cin, pilot);
	
	printf("generating random health pools...\n");
	int engHP; //engineer HP
	int engWep = 0;
	int sciHP; //scientist HP
	int sciWep = 0;
	int pilHP; //pilot HP
	int pilWep = 0;
	int crewMin = 9; //using variables in case I want to add difficulty modes later
	int crewMax = 15;
	engHP = crewMin + rand() % (crewMax - crewMin + 1) + 3; //engi has more HP, pilot has less
	sciHP = crewMin + rand() % (crewMax - crewMin + 1) - 2;
	pilHP = crewMin + rand() % (crewMax - crewMin + 1);
	int shipHP;
	int shipATK;
	int shipMin = 15;
	int shipMax = 25;
	shipHP = 15 + rand() % (25 - 15 + 1);
	shipATK = 4 + rand() % (7 - 4 + 1);
	printf("%s the engineer: %i HP\n", engineer.c_str(), engHP);
	printf("%s the scientist: %i HP\n", scientist.c_str(), sciHP);
	printf("%s the pilot: %i HP\n", pilot.c_str(), pilHP);

	printf("%s, %s, and %s have found themselves stranded on an abandoned asteroid mining station\n", engineer.c_str(), scientist.c_str(), pilot.c_str());
	printf("they find an old mining vessle outfited with a pair of weak mining lazers that do %i damge\n", shipATK);
	printf("what do they name the ship?\n");
	string shipName;
	getline(cin, shipName);
	printf("after %s does some repairs, %s is ready to fly\n", engineer.c_str(), shipName.c_str());
	printf("%s has %i HP\n[press any key to continue]\n", shipName.c_str(), shipHP);
	_getch();
	system("cls");
	printf("\nthere are some items left in the lockers that the miners left behind\n");
	printf("you crew scavenges what they can\n");
	char inputc;
	bool exit = false;
	bool b = 0;
	while (!exit) //equip menue
	{
		if (b == 1)
		{
			system("cls");
		}
		b = 1;
		printf("which character would you like to equip?\n");
		printf("a. %s\n", engineer.c_str());
		printf("b. %s\n", scientist.c_str());
		printf("c. %s\n", pilot.c_str());
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
				printf("%s finds three items:\n", engineer.c_str());
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
							printf("%s equips the monkey wrench\n", engineer.c_str());
							engWep = 1;
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
							printf("you equip the pipe wrench\n");
							engWep = 2;
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
							printf("%s equips the grenade belt\n", engineer.c_str());
							engWep = 3;
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
			data pad - minimal damage, great ship stat multipliers
			head set - no ship stat multipliers, increase crew stat multipliers
			*/
			system("cls");
			while (true)
			{
				system("cls");
				printf("%s finds three items:\n", scientist.c_str());
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
							printf("%s equips the ray gun\n", scientist.c_str());
							sciWep = 1;
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
							printf("%s equips the data pad\n", scientist.c_str());
							sciWep = 1;
							printf("[continue]\n");
							_getch();
						}
						else if ('b' == inputc)
						{
							printf("A flat screen that can be held in the hand, continuously displaying information and diagnostics.\n");
							printf("Optimizes your ship more than your fighting style.\n");
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
							printf("%s equips the head set\n", scientist.c_str());
							sciWep = 1;
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
			goggles - ship buffs
			trench coat - party buffs
			Hyperdrive turbo - double jump, recharges after 2 jumps
			*/
			system("cls");
			while (true)
			{
				system("cls");
				printf("%s finds three items:\n", engineer.c_str());
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
							printf("%s equips the flight goggles\n", pilot.c_str());
							pilWep = 1;
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
							printf("%s equip the trench coat\n", pilot.c_str());
							pilWep = 2;
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
							printf("%s equips the Hyperdrive Turbo\n", pilot.c_str());
							pilWep = 3;
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
	//random name generator
	for (int r = 0; r < 5; r++)//not neccessary for current goal sheet
	{
		break;
		for (int c = 0; c < 5; c++)
		{
			int adjective = _dice(25);
			int noun = _dice(25);
			switch (adjective)
			{
			case 1:
				roomArray[c][r].name = "soiled";//change to enum later
				break;
			case 2:
				roomArray[c][r].name = "spatered";
				break;
			case 3:
				roomArray[c][r].name = "arcane";
				break;
			case 4:
				roomArray[c][r].name = "soiled";
				break;
			case 5:
				roomArray[c][r].name = "great";
				break;
			}
			switch (noun)
			{
			case 1:
				roomArray[c][r].name = roomArray[c][r].name + " wastes";
				break;
			}
		}
	}
	exit = false;
	int playerC = 4;
	int playerR = 4;
	while (!exit)
	{
		printf("you arive in sector (%i, %i), %s\n", playerC + 1, playerR + 1, roomArray[playerC][playerR].name.c_str());

	}
	return 0;
}

void displayMap(bool(&VdoorArray)[4][5], bool(&HdoorArray)[5][4])
{
	int a = 1;
	printf("%c%c%c%c%c%c%c%c%c%c%c\n", 201, 205, 203, 205, 203, 205, 203, 205, 203, 205, 187);
	for (int r = 0; r < 5; r++)
	{
		a++;
		printf("%c", 186);
		for (int c = 0; c < 4; c++)
		{
			if (VdoorArray[c][r] == 1)
			{
				printf(" %c", 186);
			}
			else
			{
				printf("  ");
			}
		}
		printf(" %c\n", 186);
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

bool encounter(string engineer, string scientist, string pilot, int engHP, int engWEP, int sciHP, int sciWEP, int pilHP, int pilWEP, int encounterID)
{
	if (1 == encounterID)//starting sector
	{

	}
	if (2 == encounterID || 3 == encounterID || 4 == encounterID || 5 == encounterID || 6 == encounterID)//low difficulty enemies ID: 2-6
	{

	}
}