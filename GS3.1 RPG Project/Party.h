#pragma once
//setters that set values for all crew members?
#include <string>
#include <vector>
#include <Windows.h>
#include <tgmath.h>
enum action { attack, evade, special, escape };
struct vessel
{
	std::string name;
	int HP, HPmax;
};
struct crew
{
	std::string name;
	int HP, HPmax;
	bool dead;
	action act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;//damage multiplier
	int attackID, specialID, defenceID;
	int defenceValue;
};

class Party
{
public:
	Party();//must initialize the crew vector
	Party(Party& o);
	//int a is the vector subscript of the crew member
	void setCrewAtkMulti(int a, )
	void setCrewHPmax(int a, int HPmax);
	void setCrewHP(int a, int HP);//a = which crew member
	void setCrewName(int a, std::string name);
	void setAttackID(int a, int attackID);
	void setSpecialID(int a, int specialID);
	void setDefenceID(int a, int defenceID);
	void setAction(int a, action _act);
	int getCrewHP(int a);
	std::string getCrewName(int a);
	int getRawAttack(int a, std::string targetName);
	void executeSpecial(int a, int target);//party member "a" uses their special ability on party member "target"
	void recieveDamage(int a, int rawDamage);//temporaraly modifies defenceValue based on defenceID
	bool killAndTop();//kills characters and lowers their health to HPmax
private:
	vessel ship;
	std::vector <crew> cVect;
	int dice(int max);
	int projectsPrinted;
};

int Party::dice(int max)//outputs a random number from 1 to max
{
	int rn;
	rn = 1 + rand() % (max);
	return rn;
}

Party::Party()
{
	cVect.resize(3);
	projectsPrinted = 0;
}

Party::Party(Party& o)//fill this in if it is needed
{
	
}

void Party::setCrewHPmax(int a, int HPmax)
{
	cVect[a].HPmax = HPmax;
}

void Party::setCrewHP(int a, int HP)
{
	cVect[a].HP = HP;
}

void Party::setCrewName(int a, std::string name)
{
	cVect[a].name = name;
}

void Party::setAttackID(int a, int attackID)
{
	cVect[a].attackID = attackID;
}

void Party::setSpecialID(int a, int specialID)
{
	cVect[a].specialID = specialID;
}

void Party::setDefenceID(int a, int defenceID)
{
	cVect[a].defenceID = defenceID;
}

void Party::setAction(int a, action _act)
{
	cVect[a].act = _act;
}

int Party::getCrewHP(int a)
{
	return cVect[a].HP;
}

std::string Party::getCrewName(int a)
{
	return cVect[a].name;
}

int Party::getRawAttack(int a, std::string targetName)
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
	
	int atk;
	switch (cVect[a].attackID)
	{
	case 0:
		atk = 8;
		std::cout << cVect[a].name << " takes a swing at " << targetName;
		break;
	case 1:
		atk = 1 + dice(12);
		std::cout << cVect[a].name << " fires a volley of staples at " << targetName;
		break;
	case 2:
		atk = (0.06 * projectsPrinted * projectsPrinted) + (0.1 * projectsPrinted);
		projectsPrinted++;
		std::cout << cVect[a].name << " prints their RPG project, to the dismay of " << targetName;
		break;
	default:
		std::cout << "something went wrong, " << cVect[a].name << " has no attackID!";
		atk = 100000;
	}
	return atk * cVect[a].atkMulti;
}

void Party::executeSpecial(int a, int target)
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
	switch (cVect[a].specialID)
	{
	case 0:
		cVect[target].HP += 4;
		std::cout << cVect[a].name << " applies first aid to " << cVect[target].name;
		break;
	case 1:
		for (int i = 0; i < 3; i++)
		{
			cVect[i].HP += 2;
			cVect[i].atkMulti *= 1.1;
		}
		std::cout << cVect[a].name << " uses the Lizard Staff, healing and buffing the party";
		break;
	case 2:
		cVect[target].atkMulti += 0.3;
		std::cout << cVect[a].name << " uses the Wizard Tail to boost " << cVect[target].name << "'s attack";
		break;
	default:
		std::cout << "something went wrong, " << cVect[a].name << "has no specialID";
	}
	std::cout << cVect[a].name << " fires a lazer at " << cVect[target].name;
}

void Party::recieveDamage(int a, int rawDamage)
{
	int totalDefence;
	switch (cVect[a].defenceID)
	{
	case 0:
		totalDefence = cVect[a].defenceValue + 2;
		break;
	case 1:
		totalDefence = cVect[a].defenceValue + 4;
		break;
	default:
		totalDefence = cVect[a].defenceValue + 5000;
	}
	int damage = ceil((float)rawDamage / ((float)totalDefence + 1.0));
	cVect[a].HP -= damage;
	std::cout << cVect[a].name << " takes " << damage << " damage";
}

bool Party::killAndTop()
{
	bool gameOver = false;
	for (int i = 0; i < 3; i++)
	{
		if (cVect[i].HPmax < cVect[i].HP)
		{
			cVect[i].HP = cVect[i].HPmax;
		}
		if (cVect[i].HP <= 0)
		{
			cVect[i].dead = true;
			gameOver = true;
		}
	}
	return gameOver;
}