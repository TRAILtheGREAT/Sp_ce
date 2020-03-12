#pragma once
#include <string>

class Enemy
{
	enum action { attack, evade, special, escape };
public:
	Enemy();
	Enemy(Enemy& o);
	void setName(std::string _name);
	void setHPmax(int _HPmax);
	void setHP(int _HP);
	void setAtk(int _atk);
	void setDead(bool _dead);
	void setAction(action _act);
	void setTarget(int _target);
	void setAtkMulti(float _atkMulti);
	void setID(int _ID);
	void setDodgeValue(int _dodgeValue);
	std::string getName();
	int getHPmax();
	int getHP();
	int getAtk();
	bool getDead();
	action getAction();
	int getTarget();
	float getAtkMulti();
	int getID();
	int getDodgeValue();
private:
	std::string name;
	int HP, HPmax, atk;
	bool dead;
	action act;//attacking, using special, evading...
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
}

Enemy::Enemy(Enemy& o)
{
	name = o.name;
	HP = o.HP;
	HPmax = o.HPmax;
	atk = o.atk;
	dead = o.dead;
	atkMulti = o.atkMulti;
	ID = o.ID;
	dodgeValue = o.dodgeValue;
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
void Enemy::setAtk(int _atk)
{
	atk = _atk;
}
void Enemy::setDead(bool _dead)
{
	dead = _dead;
}
void Enemy::setAction(action _act)
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
Enemy::action Enemy::getAction()
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