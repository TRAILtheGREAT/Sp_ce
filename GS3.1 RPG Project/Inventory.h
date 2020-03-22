#pragma once
#include <vector>
#include <string>
//enum place {eng, sci, pil, notEquiped};
struct item
{
	int ID;
	int location;
};
class Inventory
{
public:
	Inventory();
	void addItem(int ID);
	void setEquip(int i, int character);
	int getItemID(int i);
	int getItemEquip(int i);
	int getSize();
	std::string getName(int i);
	int getItemType(int i);//0 = attack, 1 = special, 2 = defence
	int searchForItem(int ID, int equipedBy);
private:
	std::vector <item> iVect;
};
Inventory::Inventory()
{

}
void Inventory::addItem(int ID)
{
	item temp;
	temp.ID = ID;
	temp.location = 3;
	iVect.push_back(temp);
}
void Inventory::setEquip(int i, int character)
{
	iVect[i].location = character;
}
int Inventory::getItemID(int i)
{
	return iVect[i].ID;
}
int Inventory::getItemEquip(int i)
{
	return iVect[i].location;
}
int Inventory::getSize()
{
	return iVect.size();
}
std::string Inventory::getName(int i)
{
	std::string name;
	switch (iVect[i].ID)
	{
	case 0:
		name = "Blunt Object";
		break;
	case 1:
		name = "Large Stapler";
		break;
	case 2:
		name = "Room 206 Printer";
		break;
	case 3:
		name = "First Aid Kit";
		break;
	case 4:
		name = "Lizard Staff";
		break;
	case 5:
		name = "Wizard Tail";
		break;
	case 6:
		name = "Knee Pads";
		break;
	case 7:
		name = "Book of Insults";
		break;
	case 8:
		name = "Body Armor";
		break;
	default:
		name = "[no name for Item ID]";
	}
	return name;
}
int Inventory::getItemType(int i)
{
	int type;
	switch (iVect[i].ID)
	{
	case 0://attack
		type = 0;
		break;
	case 1:
		type = 0;
		break;
	case 2:
		type = 0;
		break;
	case 3://special
		type = 1;
		break;
	case 4:
		type = 1;
		break;
	case 5:
		type = 1;
		break;
	case 6://defence
		type = 2;
		break;
	case 7:
		type = 2;
		break;
	case 8:
		type = 2;
		break;
	}
	return type;
}
int Inventory::searchForItem(int ID, int equipedBy)
{
	int match;
	for (int b = 0; b < iVect.size(); b++)
	{
		if (equipedBy == iVect[b].location && ID == iVect[b].ID)
		{
			match = b;
			break;
		}
	}
	return match;
}