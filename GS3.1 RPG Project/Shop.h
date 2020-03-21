#pragma once
#include "PlayerStruct.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <conio.h>

class Shop
{
	struct item
	{
		std::string name;
		int ID;
		int price;
	};
public:
	Shop(std::string sourceFileName);
	Shop(const Shop& o);
	party enterShop(party player);
private:
	std::string banner;
	std::vector <item> stock;
	void color(int a);
	void blocks(int a);
	void printImage(std::string image, int Y);
	char _getdir();
};

Shop::Shop(std::string sourceFileName)
{
	std::ifstream in;
	in.open(sourceFileName);
	std::string temp;
	for (int a = 0; a < 5; a++)
	{
		std::getline(in, temp);
		banner += temp + 'n';
	}
	std::getline(in, temp);
	banner += temp;
	std::getline(in, temp, ',');
	while ("END" != temp)
	{
		item tempItem;
		tempItem.name = temp;
		std::getline(in, temp, ',');
		tempItem.ID = std::stoi(temp);
		std::getline(in, temp);
		tempItem.price = std::stoi(temp);
		stock.push_back(tempItem);
		std::getline(in, temp, ',');
	}
	in.close();
}
Shop::Shop(const Shop& o)
{

}
party Shop::enterShop(party player)
{
	bool exitShop = false;
	while (!exitShop)
	{
		system("cls");
		printImage(banner, 0);
		HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord;
		coord.X = 80;
		coord.Y = 7;
		SetConsoleCursorPosition(standard, coord);
		std::cout << "Credits: " << player.credits;
		coord.Y = 9;
		SetConsoleCursorPosition(standard, coord);
		std::cout << "Press enter to purchase";
		coord.X = 0;
		coord.Y = 6;
		SetConsoleCursorPosition(standard, coord);
		std::cout << "   Item             Price\n";
		for (int a = 0; a < stock.size(); a++)
		{
			if (player.credits < stock[a].price)
			{
				color(4);
			}
			else
			{
				color(15);
			}
			coord.X = 3;
			coord.Y = 7 + a;
			SetConsoleCursorPosition(standard, coord);
			std::cout << stock[a].name;
			coord.X = 20;
			SetConsoleCursorPosition(standard, coord);
			std::cout << stock[a].price;
		}
		color(15);
		coord.Y = 7 + stock.size();
		coord.X = 3;
		SetConsoleCursorPosition(standard, coord);
		std::cout << "DONE";
		COORD position;
		position.X = 1;
		position.Y = 7;
		COORD pastPosition;
		pastPosition = position;
		bool reprint = false;
		while (!reprint)
		{
			SetConsoleCursorPosition(standard, pastPosition);
			std::cout << " ";
			SetConsoleCursorPosition(standard, position);
			std::cout << ">";
			pastPosition = position;
			char inputc = _getdir();
			if ('u' == inputc)
			{
				if (7 != position.Y)
				{
					position.Y--;
				}
			}
			else if ('d' == inputc)
			{
				if (stock.size() + 7 != position.Y)
				{
					position.Y++;
				}
			}
			else if (13 == inputc)
			{
				int purchase = position.Y - 7;
				if (stock.size() == purchase)
				{
					exitShop = true;
				}
				else if (stock[purchase].ID <= 8)//ability
				{
					if (stock[purchase].price < player.credits)
					{
						player.inventory.addItem(stock[purchase].ID);
						player.credits -= stock[purchase].price;
					}
					else
					{
						coord.X = 50;
						coord.Y = 15;
						SetConsoleCursorPosition(standard, coord);
						color(4);
						std::cout << "[NOT ENOUGH CREDITS]";
						color(15);
						_getch();
					}
				}
				else if (9 == stock[purchase].ID || 10 == stock[purchase].ID)//add one HP
				{
					if ((9 == stock[purchase].ID && player.credits < 50) || (10 == stock[purchase].ID && player.credits < 30))
					{
						coord.X = 50;
						coord.Y = 15;
						SetConsoleCursorPosition(standard, coord);
						color(4);
						std::cout << "[NOT ENOUGH CREDITS]";
						color(15);
						_getch();
						break;
					}
					coord.X = 37;
					coord.Y = 7;
					for (; coord.Y <= 9; coord.Y++)
					{
						SetConsoleCursorPosition(standard, coord);
						std::cout << player.cVect[coord.Y - 7].name;
					}
					SetConsoleCursorPosition(standard, coord);
					std::cout << "CANCEL";
					position.X = 35;
					position.Y = 7;
					pastPosition = position;
					while (true)
					{
						SetConsoleCursorPosition(standard, pastPosition);
						std::cout << " ";
						SetConsoleCursorPosition(standard, position);
						std::cout << ">";
						pastPosition = position;
						char inputc = _getdir();
						if ('u' == inputc)
						{
							if (7 != position.Y)
							{
								position.Y--;
							}
						}
						else if ('d' == inputc)
						{
							if (10 != position.Y)
							{
								position.Y++;
							}
						}
						else if (13 == inputc)
						{
							if (10 != position.Y)
							{
								if (9 == stock[purchase].ID)
								{
									player.cVect[position.Y - 7].HPmax += 1;
									player.cVect[position.Y - 7].HP += 1;
									player.credits -= stock[purchase].price;
								}
								else if (10 == stock[purchase].ID)
								{
									player.cVect[position.Y - 7].defenceValue += 1;
									player.credits -= stock[purchase].price;
								}
							}
							break;
						}
					}
				}
				reprint = true;
			}
		}
	}
	color(15);
	return player;
}
void Shop::color(int a)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(standard, a);
	return;
}
void Shop::blocks(int a)
{
	std::string bb = "  ";
	bb[0] = 219;
	bb[1] = 219;
	for (int b = 0; b < a; b++)
	{
		std::cout << bb;
	}
	return;
}
void Shop::printImage(std::string image, int Y)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 0;
	coord.Y = Y;
	SetConsoleCursorPosition(standard, coord);
	coord.X = 2;
	for (int c = 0; c < image.length(); c++)
	{
		if ('0' == image[c])
		{
			color(0);
			blocks(1);
		}
		else if ('1' == image[c])
		{
			color(1);
			blocks(1);
		}
		else if ('2' == image[c])
		{
			color(2);
			blocks(1);
		}
		else if ('3' == image[c])
		{
			color(3);
			blocks(1);
		}
		else if ('4' == image[c])
		{
			color(4);
			blocks(1);
		}
		else if ('5' == image[c])
		{
			color(5);
			blocks(1);
		}
		else if ('6' == image[c])
		{
			color(6);
			blocks(1);
		}
		else if ('7' == image[c])
		{
			color(7);
			blocks(1);
		}
		else if ('8' == image[c])
		{
			color(8);
			blocks(1);
		}
		else if ('9' == image[c])
		{
			color(9);
			blocks(1);
		}
		else if ('a' == image[c])
		{
			color(10);
			blocks(1);
		}
		else if ('b' == image[c])
		{
			color(11);
			blocks(1);
		}
		else if ('c' == image[c])
		{
			color(12);
			blocks(1);
		}
		else if ('d' == image[c])
		{
			color(13);
			blocks(1);
		}
		else if ('e' == image[c])
		{
			color(14);
			blocks(1);
		}
		else if ('f' == image[c])
		{
			color(15);
			blocks(1);
		}
		else if (' ' == image[c])
		{
			SetConsoleCursorPosition(standard, coord);
		}
		else if ('n' == image[c])
		{
			std::cout << "\n";
			coord.Y++;
			coord.X = 0;
		}
		coord.X += 2;
	}
	color(15);
}
char Shop::_getdir()
{
	//returns u d l r
	//e = error
	int c = 0;

	c = _getch();
	//cout << c;
	if (224 == c)//arrow keys
	{
		switch (_getch())
		{
		case 72:
			return 'u';
		case 75:
			return 'l';
		case 77:
			return 'r';
		case 80:
			return 'd';
		}
	}
	else//regular keys
	{
		if ('w' == c)
		{
			return 'u';
		}
		else if ('s' == c)
		{
			return 'd';
		}
		else if ('a' == c)
		{
			return 'l';
		}
		else if ('d' == c)
		{
			return 'r';
		}
		else if (13 == c)//enter key
		{
			return 13;
		}
		else
		{
			return 'e';
		}
	}
}