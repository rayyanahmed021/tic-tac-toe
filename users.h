/*
===============================
Author: Rayyan Ahmed
Date: 28/8/2021
Module: Users
Project: tic-tac-toe
===============================
*/


#ifndef USERS_H
#define USERS_H


#include <occi.h>
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;

template <typename type>
void removeDynamicElement(type* array[], int index, int& size)
{
	delete array[index];
	for (int j = index; j < size - 1; j++)
	{
		array[j] = array[j + 1];
		array[j]->setID(j + 1);
	}
	size--;
}

class Users
{
	int id;
	char* username{ nullptr };
	char* pass{ nullptr };
	int win = 0;
	int loss = 0;
	int draws = 0;
public:
	//constructors
	Users(int id, const char* name, const char* password);
	Users(const char* name, const char* password, int w, int l, int d, int idNumber);
	Users();
	~Users();
	
	//modifiers
	void setPassword(const char* password);
	void setName(const char* name);
	void updateScore(char winloss);
	void setID(int idNum);
	
	//queries
	char* name()const;
	char* password()const;
	int score(char winloss)const;
	void displayScore() const;
	void updateDatabase(Connection* conn)const;

	
	Users& operator = (const Users&) = delete;
	Users(const Users&) = delete;
};


#endif // !USERS_H	