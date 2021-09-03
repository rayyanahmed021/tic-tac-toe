/*
===============================
Author: Rayyan Ahmed
Date: 28/8/2021
Module: Users
Project: tic-tac-toe
===============================
*/

#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include <iomanip>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

#include "users.h"
#include "helperFunctions.h"
using namespace std;


Users::Users(int idNum, const char* name, const char* password)
{
	if (idNum)
	{
		id = idNum;
	}
	setName(name);
	setPassword(password);
	win = 0;
	loss = 0;
	draws = 0;
}

Users::Users(const char* name, const char* password, int w, int l, int d, int idNumber)
{
	setName(name);
	setPassword(password);
	win = w;
	loss = l;
	draws = d;
	id = idNumber;
}

Users::Users() :username(nullptr), pass(nullptr), win(0), loss(0), draws(0), id(0)
{
}

void Users::setName(const char* name)
{
	if (name)
	{
		delete[] username;
		username = new char[strlen(name) + 1];
		strcpy(username, name);
	}
}

char* Users::name() const
{
	return username;
}


char* Users::password() const
{
	return pass;
}

void Users::updateScore(char winloss)
{
	if (winloss == 'w')
	{
		win++;
	}
	else if (winloss == 'l')
	{
		loss++;
	}
	else
	{
		draws++;
	}
}

void Users::setID(int idNum)
{
	if (idNum)
	{
		id = idNum;
	}
}

int Users::score(char winloss)const
{
	int result;
	if (winloss == 'w')
	{
		result = win;
	}
	else if (winloss == 'l')
	{
		result = loss;
	}
	else if (winloss == 'd')
	{
		result = draws;
	}
	else
	{
		result = id;
	}
	return result;
}

void Users::updateDatabase(Connection* conn)const
{
	Statement* stmt = conn->createStatement();
	stmt->setSQL("INSERT INTO tictactoe VALUES ((:1), (:2), (:3), (:4), (:5), (:6))");
	stmt->setInt(1, id);
	stmt->setString(2, username);
	stmt->setString(3, pass);
	stmt->setInt(4, win);
	stmt->setInt(5, loss);
	stmt->setInt(6, draws);
	stmt->executeQuery();
	conn->commit();
}

void Users::displayScore()const
{
	cout.setf(ios::left);
	cout << setw(10) << "Username" <<
		setw(6) << "Wins" <<
		setw(6) << "Loses" <<
		setw(6) << "Draws" <<
		endl;
	cout.unsetf(ios::left);
	cout.setf(ios::right);
	cout << setw(10) << setfill('=') << "= "
		<< setw(6) << setfill('=') << "= "
		<< setw(6) << setfill('=') << "= "
		<< setw(6) << setfill('=') << "= "
		<< endl;
	cout.unsetf(ios::right);
	cout.setf(ios::left);

	cout << setfill(' ') << setw(10) << name()
		<< setw(6) << score('w')
		<< setw(6) << score('l')
		<< setw(6) << score('d')
		<< endl;
	cout.unsetf(ios::left);
	cout << endl << endl;
}


void Users::setPassword(const char* password)
{
	if (password)
	{
		delete[] pass;
		pass = new char[strlen(password) + 1];
		strcpy(pass, password);
	}
}

Users::~Users()
{
	delete[] username;
	delete[] pass;
}