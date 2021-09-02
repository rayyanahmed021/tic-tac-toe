/*
===============================
Author: Rayyan Ahmed
Date: 28/8/2021
Module: helperFunctions
Project: tic-tac-toe
===============================
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <string>
#include "helperFunctions.h"

using namespace std;

int getInt(const char* prompt, int min, int max, const char* errorMsg)
{
	int flag = 0, inputPrompt;

	//display the prompt if its valid
	if (prompt)
	{
		cout << prompt;
	}
	//take input until a valid value is entered
	do
	{
		cin >> inputPrompt;

		//input validation
		if (cin.fail())
		{
			cerr << "Bad integer value, try again: ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		else if (cin.get() != '\n')
		{
			cerr << "Enter only an integer, try again: ";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		else if (inputPrompt > max || inputPrompt < min)
		{
			cerr << "ERROR: " << errorMsg;
		}
		else
		{
			flag = 1;
		}
	} while (!flag);

	return inputPrompt;
}
char getChar(const char* prompt, const char* validChar, istream& is)
{
	string str;
	char chr;
	int flag = 0;

	if (prompt)
	{
		cout << prompt;
	}
	do
	{
		is.get(chr);
		cout << endl;

		if (is.get() != '\n')
		{
			is.ignore(1000, '\n');
		}
		else if (!strchr(validChar, chr))
		{
			cout << "ERROR: Select from the options [" << validChar << "]: ";
		}
		else
		{
			flag = 1;
		}

	} while (!flag);

	return chr;
}

char* getString(const char* prompt, int min, int max, std::istream& is)
{
	string str;
	int flag = 0;
	char* ptr = nullptr;

	if (prompt)
	{
		cout << prompt << ": ";
	}
	do
	{
		getline(is, str, '\n');
		if (str.length() > max || str.length() < min)
		{
			cout << "ERROR: " << prompt << " must be between " 
				<< min << " and " << max << " characters: ";
		}
		else
		{
			strcmp(prompt, "Username") == 0 ? flag = asciiValidation(1, str.c_str()) :
				flag = asciiValidation(2, str.c_str());
		}
	} while (!flag);

	ptr = new char[str.length() + 1];
	strcpy(ptr, str.c_str());
	return ptr;
}

int asciiValidation(int num, const char* str)
{

	int i, flag = 1, number, lower, upper, special;
	number = lower = special = upper = 0;

	for (i = 0; flag && i < strlen(str); i++)
	{
		//1 for username
		if (num == 1)
		{
			if (!((str[i] >= 48 && str[i] <= 57) ||
				(str[i] >= 65 && str[i] <= 90) ||
				(str[i] >= 97 && str[i] <= 122)))
			{
				flag = 0;
				cout << "ERROR: only numbers and alphabets are allowed: ";
			}
		}
		else if (num == 2)
		{
			if (str[i] >= 48 && str[i] <= 57)
			{
				number = 1;
			}
			else if (str[i] >= 65 && str[i] <= 90)
			{
				upper = 1;
			}
			else if (str[i] >= 97 && str[i] <= 122)
			{
				lower = 1;
			}
			else
			{
				special = 1;
			}
			(number + upper + lower + special) == 4 ? flag = 0 : flag = 1;
		}
	}
	if (num == 2 && flag)
	{
		cout << "ERROR: There should be atleast ONE";
		cout << "number, uppercase, lowercase and special character: ";
		flag = 0;
	}
	else if (num == 2 and !flag)
	{
		flag = 1;
	}
	return flag;
}
