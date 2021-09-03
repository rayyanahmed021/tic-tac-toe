/*
===============================
Author: Rayyan Ahmed
Date: 28/8/2021
Module: Game
Project: tic-tac-toe
===============================
*/

#ifndef MENUS_H
#define MENUS_H

#include <occi.h>
#include "users.h"
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;


class Game
{
	Users** users = nullptr;
	int totalUser = 0;
	int tictactoe[9] = { 0 }; // 1 for user and 2 for computer and 0 for none
public:
	//constructors and destructor
	Game(Connection* conn);
	~Game();


	//queries
	int matchingNames(const char* name)const;
	int matchingPasswords(const char* password)const;
	int login()const;
	void scoreboard()const;
	void display(char userChar)const;
	int winner()const;
	void updateDatabase(Connection* conn)const;
	
	
	//modifier
	void loadData(Connection* conn);
	void registerUser(Connection* conn);
	void addUser(Users* user);
	char* tttMenu(int index);
	void wholeGame(char userChar, int index);
	int turn(int playerNumber);
	void updateMenu(int index);
};

void startMenu(Connection* conn, Game& game);

#endif // !MENUS_H