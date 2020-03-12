#pragma once
#include "Goblin.h"
#include <string>

enum enemyID {goblin, squid, brainDrone, smallDrone};

class Enemy
{
public:
	Enemy();
	Enemy(enemyID ID);
	int getRawAttack(std::string targetName);
	action getAction(int turnNumber);
	void setE(int _e);
private:
	enemyID enemyType;
	Goblin spaceGoblin;
};

Enemy::Enemy()
{
	enemyType = goblin;
}

Enemy::Enemy(enemyID ID)
{
	enemyType = ID;
}

int Enemy::getRawAttack(std::string targetName)
{
	switch (enemyType)
	{
	case goblin:
		return spaceGoblin.getRawAttack(targetName);
	}
}

void Enemy::setE(int _e)
{
	switch (enemyType)
	{
	case goblin:
		spaceGoblin.setE(_e);
		break;
	}
}