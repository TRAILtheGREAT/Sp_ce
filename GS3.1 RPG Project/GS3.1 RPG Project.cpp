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
	int sectorX = 1;
	int sectorY = 1;
	exit = false;
	while (!exit)//movement loop
	{
		_getch();
		system("cls");
		printf("You are in sector (%i, %i)\n", sectorX, sectorY);
		if (1 == sectorX && 1 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (1, 2)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (1, 2)\n[press any key to continue]");
					sectorY++;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (2 == sectorX && 1 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (2, 2)\nb. (3, 1)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (2, 2)\n");
					sectorY++;
					break;
				}
				else if ('b' == inputc)
				{
					printf("you jump to sector (3, 1)\n");
					sectorX++;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (3 == sectorX && 1 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (2, 1)\nb. (4, 1)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (2, 1)\n");
					sectorX--;
					break;
				}
				else if ('b' == inputc)
				{
					printf("you jump to sector (4, 1)\n");
					sectorX++;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (4 == sectorX && 1 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (4, 2)\nb. (3, 1)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (4, 2)\n");
					sectorY++;
					break;
				}
				else if ('b' == inputc)
				{
					printf("you jump to sector (3, 1)\n");
					sectorX--;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (5 == sectorX && 1 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (5, 2)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (5, 2)\n");
					sectorY++;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (1 == sectorX && 2 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (1, 3)\nb. (1, 1)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (1, 3)\n[press any key to continue]");
					sectorY++;
					break;
				}
				else if ('b' == inputc)
				{
					printf("you jump to sector (1, 1)\n[press any key to continue]");
					sectorY--;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (2 == sectorX && 2 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (2, 3)\nb. (4, 1)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (2, 3)\n");
					sectorY++;
					break;
				}
				else if ('b' == inputc)
				{
					printf("you jump to sector (4, 1)\n");
					sectorY--;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (3 == sectorX && 2 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (4, 2)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (4, 2)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (4 == sectorX && 2 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (4, 3)\nb. (3, 2)\nc. (3, 1)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (4, 3)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (3, 2)\n");
				sectorX--;
				break;
			}
			else if ('c' == inputc)
			{
				printf("you jump to sector (3, 1)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (5 == sectorX && 2 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (5, 3)\nb. (5, 1)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (5, 3)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (5, 1)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (1 == sectorX && 3 == sectorY)
		{
			while (true)
			{
				printf("pick a jump location:\n");
				printf("a. (1, 4)\nb. (1, 2)\nc. (2, 3)\n");
				cin >> inputc;
				fseek(stdin, 0, SEEK_END);
				if ('a' == inputc)
				{
					printf("you jump to sector (1, 4)\n");
					sectorY++;
					break;
				}
				else if ('b' == inputc)
				{
					printf("you jump to sector (1, 2)\n");
					sectorY--;
					break;
				}
				else if ('c' == inputc)
				{
					printf("you jump to sector (2, 3)\n");
					sectorX++;
					break;
				}
				else
				{
					printf("[invalid input]\n");
					_getch();
					system("cls");
				}
			}
		}
		else if (2 == sectorX && 3 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (1, 3)\nb. (2, 2)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (1, 3)\n");
				sectorX--;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (2, 2)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (3 == sectorX && 3 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (3, 4)\nb. (4, 3)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (3, 4)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (4, 3)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (4 == sectorX && 3 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (3, 3)\nb. (4, 2)\nc. (5, 3)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (3, 3)\n");
				sectorX--;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (4, 2)\n");
				sectorY--;
				break;
			}
			else if ('c' == inputc)
			{
				printf("you jump to sector (5, 3)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (5 == sectorX && 3 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (5, 4)\nb. (4, 3)\nc. (5, 2)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (5, 4)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (4, 3)\n");
				sectorX--;
				break;
			}
			else if ('c' == inputc)
			{
				printf("you jump to sector (5, 2)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (1 == sectorX && 4 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (1, 3)\nb. (2, 4)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (1, 3)\n");
				sectorY--;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (2, 4)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (2 == sectorX && 4 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (2, 5)\nb. (1, 4)\nc. (3, 4)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (2, 5)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (1, 4)\n");
				sectorX--;
				break;
			}
			else if ('c' == inputc)
			{
				printf("you jump to sector (3, 4)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (3 == sectorX && 4 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (4, 2)\nb. (3, 3)\nc. (4, 4)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (4, 2)\n");
				sectorX--;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (3, 3)\n");
				sectorY--;
				break;
			}
			else if ('c' == inputc)
			{
				printf("you jump to sector (4, 4)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (4 == sectorX && 4 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (4, 5)\nb. (3, 4)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (4, 5)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (3, 4)\n");
				sectorX--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (5 == sectorX && 4 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (5, 5)\nb. (5, 3)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (5, 5)\n");
				sectorY++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (5, 3)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (1 == sectorX && 5 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (2, 5)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (2, 5)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (2 == sectorX && 5 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (1, 5)\nb. (2, 4)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (1, 5)\n");
				sectorX--;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (2, 4)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (3 == sectorX && 5 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (4, 5)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (4, 5)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (4 == sectorX && 5 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (3, 5)\nb. (4, 4)\nc. (5, 5)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (3, 5)\n");
				sectorX--;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (4, 4)\n");
				sectorY--;
				break;
			}
			else if ('c' == inputc)
			{
				printf("you jump to sector (5, 5)\n");
				sectorX++;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
		else if (5 == sectorX && 5 == sectorY)
		{
		while (true)
		{
			printf("pick a jump location:\n");
			printf("a. (4, 5)\nb. (5, 4)\n");
			cin >> inputc;
			fseek(stdin, 0, SEEK_END);
			if ('a' == inputc)
			{
				printf("you jump to sector (4, 5)\n");
				sectorX++;
				break;
			}
			else if ('b' == inputc)
			{
				printf("you jump to sector (5, 4)\n");
				sectorY--;
				break;
			}
			else
			{
				printf("[invalid input]\n");
				_getch();
				system("cls");
			}
		}
		}
	}
	return 0;
}

