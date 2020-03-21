#pragma once
#include "Inventory.h"
#include <vector>
#include <string>

enum action { attack, evade, special, escape };

struct crew
{
	std::string name;
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
	std::string name;
	int HP, HPmax;
};
struct party
{
	std::vector <crew> cVect;
	vessel ship;
	int projectsPrinted;//used for print project attack
	Inventory inventory;
	int credits;
};