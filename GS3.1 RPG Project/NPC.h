#pragma once
#include <string>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Inventory.h"
/*
pulls info from a file
	image
	blurbs
	items
	item descriptions
each NPC will give the player a choice between two items
*/
class NPC
{
public:
	NPC();
	NPC(std::string sourceFile);
	NPC(const NPC& o);
	int encounter();
private:
	std::string image;
	std::vector <std::string> blurbs;
	int ID1, ID2;
	void color(int a);
	void blocks(int a);
	void printImage(std::string image, int Y);
	char _getdir();
};

NPC::NPC()
{

}
NPC::NPC(std::string sourceFile)
{
	std::ifstream in;
	in.open(sourceFile);
	std::string fileTemp;
	for (int a = 0; a < 24; a++)
	{
		std::getline(in, fileTemp);
		image += fileTemp + 'n';
	}
	std::getline(in, fileTemp);
	image += fileTemp;
	std::getline(in, fileTemp, ',');
	ID1 = std::stoi(fileTemp);
	std::getline(in, fileTemp);
	ID2 = std::stoi(fileTemp);
	while (std::getline(in, fileTemp))
	{
		blurbs.push_back(fileTemp);
	}
}
NPC::NPC(const NPC& o)
{

}
int NPC::encounter()
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	printImage(image, 0);
	char ul = 218;
	char ur = 191;
	char ll = 192;
	char lr = 217;
	char h = 196;
	char v = 179;
	COORD coord;
	coord.X = 0;
	coord.Y = 25;
	SetConsoleCursorPosition(standard, coord);
	std::cout << ul;
	for (int a = 0; a < 118; a++)
	{
		std::cout << h;
	}
	std::cout << ur;
	for (coord.Y = 26; coord.Y < 29; coord.Y++)
	{
		coord.X = 0;
		SetConsoleCursorPosition(standard, coord);
		std::cout << v;
		coord.X = 119;
		SetConsoleCursorPosition(standard, coord);
		std::cout << v;
	}
	coord.Y = 29;
	coord.X = 0;
	SetConsoleCursorPosition(standard, coord);
	std::cout << ll;
	for (int a = 0; a < 118; a++)
	{
		std::cout << h;
	}
	std::cout << lr;
	coord.X = 1;
	coord.Y = 27;
	for (int a = 0; a < blurbs.size() - 1; a++)
	{
		SetConsoleCursorPosition(standard, coord);
		std::cout << blurbs[a].substr(0, 118);//------------------------------------posible source of errors
		_getch();
		SetConsoleCursorPosition(standard, coord);
		for (int b = 0; b < 118; b++)
		{
			std::cout << ' ';
		}
	}
	coord.Y = 26;
	SetConsoleCursorPosition(standard, coord);
	std::cout << blurbs[blurbs.size() - 1].substr(0, 118);
	Inventory tempI;
	tempI.addItem(ID1);//this is not how this should be used but it works
	tempI.addItem(ID2);
	std::string item1 = tempI.getName(0);
	std::string item2 = tempI.getName(1);
	coord.Y = 27;
	coord.X = 89;
	SetConsoleCursorPosition(standard, coord);
	std::cout << item2;
	coord.X = 29;
	SetConsoleCursorPosition(standard, coord);
	std::cout << item1;
	bool done = false;
	coord.Y = 28;
	COORD past = coord;
	char line = 205;
	while (!done)
	{
		SetConsoleCursorPosition(standard, past);
		if (29 == past.X)
		{
			for (int a = 0; a < item1.size(); a++)
			{
				std::cout << ' ';
			}
		}
		else if (89 == past.X)
		{
			for (int a = 0; a < item2.size(); a++)
			{
				std::cout << ' ';
			}
		}
		SetConsoleCursorPosition(standard, coord);
		if (29 == coord.X)
		{
			for (int a = 0; a < item1.size(); a++)
			{
				std::cout << line;
			}
		}
		else if (89 == coord.X)
		{
			for (int a = 0; a < item2.size(); a++)
			{
				std::cout << line;
			}
		}
		past = coord;
		char inputc = _getdir();
		if ('r' == inputc)
		{
			coord.X = 89;
		}
		else if ('l' == inputc)
		{
			coord.X = 29;
		}
		else if (13 == inputc)
		{
			if (29 == coord.X)
			{
				return ID1;
			}
			else if (89 == coord.X)
			{
				return ID2;
			}
			done = true;
		}
	}
}
void NPC::color(int a)
{
	HANDLE standard = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(standard, a);
	return;
}
void NPC::blocks(int a)
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
void NPC::printImage(std::string image, int Y)
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
char NPC::_getdir()
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
