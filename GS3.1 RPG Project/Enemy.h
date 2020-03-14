#pragma once
#include <string>

class Enemy
{
public:
	Enemy();
	Enemy(std::string _name, int _HP, int _atk, int _ID, int _dodgeValue);
	Enemy(const Enemy& o);
	void setName(std::string _name);
	void setHPmax(int _HPmax);
	void setHP(int _HP);
	void addHP(int add);
	void setAtk(int _atk);
	void setDead(bool _dead);
	void setAction(int _act);
	void setTarget(int _target);
	void setAtkMulti(float _atkMulti);
	void multAtkMulti(float mult);
	void setID(int _ID);
	void setDodgeValue(int _dodgeValue);
	std::string getName();
	int getHPmax();
	int getHP();
	int getAtk();
	bool getDead();
	int getAction();
	int getTarget();
	float getAtkMulti();
	int getID();
	int getDodgeValue();
private:
	std::string name;
	int HP, HPmax, atk;
	bool dead;
	int act;//attacking, using special, evading...
	int target;//which crew/enemy is being targeted
	float atkMulti;
	int ID;
	int dodgeValue;//add to dodge chance when evading/blocking
};

Enemy::Enemy()
{
	name = "[NO NAME]";
	dead = true;
	atkMulti = 1.0;
	atk = 0;
}

Enemy::Enemy(std::string _name, int _HP, int _atk, int _ID, int _dodgeValue)
{
	name = _name;
	HP = _HP;
	HPmax = _HP;
	atk = _atk;
	ID = _ID;
	dodgeValue = _dodgeValue;
}

Enemy::Enemy(const Enemy& o)
{
	name = o.name;
	HP = o.HP;
	HPmax = o.HPmax;
	atk = o.atk;
	dead = o.dead;
	atkMulti = o.atkMulti;
	ID = o.ID;
	dodgeValue = o.dodgeValue;
	act = o.act;
	target = o.target;
}

void Enemy::setName(std::string _name)
{
	name = _name;
}
void Enemy::setHPmax(int _HPmax)
{
	HPmax = _HPmax;
}
void Enemy::setHP(int _HP)
{
	HP = _HP;
}
void Enemy::addHP(int add)
{
	HP += add;
}
void Enemy::setAtk(int _atk)
{
	atk = _atk;
}
void Enemy::setDead(bool _dead)
{
	dead = _dead;
}
void Enemy::setAction(int _act)
{
	act = _act;
}
void Enemy::setTarget(int _target)
{
	target = _target;
}
void Enemy::setAtkMulti(float _atkMulti)
{
	atkMulti = _atkMulti;
}
void Enemy::multAtkMulti(float mult)
{
	atkMulti *= mult;
}
void Enemy::setID(int _ID)
{
	ID = _ID;
}
void Enemy::setDodgeValue(int _dodgeValue)
{
	dodgeValue = _dodgeValue;
}
std::string Enemy::getName()
{
	return name;
}
int Enemy::getHPmax()
{
	return HPmax;
}
int Enemy::getHP()
{
	return HP;
}
int Enemy::getAtk()
{
	return atk;
}
bool Enemy::getDead()
{
	return dead;
}
int Enemy::getAction()
{
	return act;
}
int Enemy::getTarget()
{
	return target;
}
float Enemy::getAtkMulti()
{
	return atkMulti;
}
int Enemy::getID()
{
	return ID;
}
int Enemy::getDodgeValue()
{
	return dodgeValue;
}