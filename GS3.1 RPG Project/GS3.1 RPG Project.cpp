// GS3.1 RPG Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//Author: Trail Sammarco
//Project: RPG Project
//File Name: GS3.1 RPG Project.cpp
//Purpose: continue make the rpg
//explanation: add randomly generated health, damage, and names.
//maintenance log: 
//9-12-19, wrote introduction,
//9-21-19, wrote naming system and health pool randomization
//9-24-19, wrote damage randomization

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <string>

using namespace std;
int main()
{
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
	int sciHP; //scientist HP
	int pilHP; //pilot HP
	int crewMin = 9; //using variables in cas i want to add difficulty modes later
	int crewMax = 15;
	srand((unsigned)time(NULL));
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
	printf("%s has %i HP\n", shipName.c_str(), shipHP);
	printf("With the ship fully operational, the crew sets off into the great unknown...\n");
	return 0;
}

