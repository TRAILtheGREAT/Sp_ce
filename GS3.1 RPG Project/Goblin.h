#pragma once
#include <string>
#include <Windows.h>
#include <iostream>
#include <tgmath.h>
enum action { attack, evade, special, escape };
class Goblin
{
public:
	Goblin();
	Goblin(int _HPmax, std::string _name, int _defenceStat);
	int getHP();
	std::string getName();
	void setAtk(int _atk);
	void setHP(int _HP);
	void setHPmax(int _HPmax);
	void setName(std::string _name);
	void setDefenceStat(int _defenceStat);
	int getRawAttack(std::string targetName);//outputs attack value, which can then be modified by entity recieving the damage
	void recieveDamage(int rawDamage);
	void useSpecial();//goblin increases it's own attack multiplier
	action determineAction(int turnNumber);//AI
	void setE(int _e);
private:
	std::string name;
	int HP, HPmax, atk;
	bool dead = true;
	action act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;
	int defenceStat;//effects damage taken
	int e;//number of enemies in the party
	int dice(int max);
};

int Goblin::dice(int max)//outputs a random number from 1 to max
{
	int rn;
	rn = 1 + rand() % (max);
	return rn;
}

Goblin::Goblin()
{
	HP = 1;
	HPmax = 1;
	name = "unnamedGoblin";
	defenceStat = 0;
}

Goblin::Goblin(int _HPmax, std::string _name, int _defenceStat)
{
	HP = _HPmax;
	HPmax = _HPmax;
	name = _name;
	defenceStat = _defenceStat;
}

int Goblin::getHP()
{
	return HP;
}

std::string Goblin::getName()
{
	return name;
}

void Goblin::setAtk(int _atk)
{
	atk = _atk;
}

void Goblin::setHP(int _HP)
{
	HP = _HP;
}

void Goblin::setHPmax(int _HPmax)
{
	HPmax = _HPmax;
}

void Goblin::setName(std::string _name)
{
	name = _name;
}

void Goblin::setDefenceStat(int _defenceStat)
{
	defenceStat = _defenceStat;
}

int Goblin::getRawAttack(std::string targetName)
{
	COORD coord;
	coord.X = 0;
	coord.Y = 29;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	for (int a = 0; a < 80; a++)
	{
		std::cout << " ";
	}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << name << " fires a lazer at " << targetName;
	return atk * atkMulti;
}

void Goblin::recieveDamage(int rawDamage)
{
	int damage = ceil((float)rawDamage / ((float)defenceStat + 1.0));
	HP -= damage;
	COORD coord;
	coord.X = 0;
	coord.Y = 29;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	for (int a = 0; a < 80; a++)
	{
		std::cout << " ";
	}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << name << " takes " << damage << " damage";
}

void Goblin::useSpecial()
{
	atkMulti *= 1.12;
	COORD coord;
	coord.X = 0;
	coord.Y = 29;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	for (int a = 0; a < 80; a++)
	{
		std::cout << " ";
	}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	std::cout << name << " gives it's ray gun a smack";
}

action Goblin::determineAction(int turnNumber)
{
	if (0 == turnNumber % 3)
	{

	}
}

void Goblin::setE(int _e)
{
	e = _e;
}