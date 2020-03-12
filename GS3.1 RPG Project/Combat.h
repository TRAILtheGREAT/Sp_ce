#pragma once
#include "Party.h"
#include "Enemy.h"
#include <vector>
class Combat
{
public:
	Combat();
	Combat(Combat& o);
	void addEnemy(Enemy e);
	bool battleCycle(Party& player);
private:
	std::vector <Enemy> enemyVect;
	int turnNumber;
};

Combat::Combat()
{
	turnNumber = 0;
}

Combat::Combat(Combat& o)
{
	for (int i = 0; i < o.enemyVect.size(); i++)
	{
		enemyVect.push_back(o.enemyVect[i]);
	}
	turnNumber = o.turnNumber;
}

void Combat::addEnemy(Enemy e)
{
	enemyVect.push_back(e);
	for (int i = 0; i < enemyVect.size(); i++)
	{
		enemyVect[i].setE(enemyVect.size() + 1);
	}
}

enum turn { eng, sci, pil };

bool Combat::battleCycle(Party& player)
{
	system("cls");
	bool DEBUG = false;
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	player.set;
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
		bool allDead = true;
		for (int a = 0; a < enemyParty.a; a++)
		{
			if (!enemyParty.eVect[a].dead)
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