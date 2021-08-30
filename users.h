#ifndef USERS_H
#define USERS_H


#include <occi.h>
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;

template <typename type>
void removeDynamicElement(type* array, int index, int& size)
{
	delete array[index];
	for (int j = index; j < size; j++)
	{
		array[j] = array[j + 1];
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
	Users(const char* name, const char* password);
	Users(const char* name, const char* password, int w, int l, int d, int idNumber);
	Users();
	~Users();
	void setPassword(const char* password);
	void setName(const char* name);
	char* name()const;
	void setID(int num);
	char* password()const;
	void updateScore(char winloss);
	int score(char winloss)const;
	void updateDatabase(Connection* conn);
	void displayScore() const;
	Users& operator = (const Users*);
	Users& operator = (const Users&);
	Users(const Users&) = delete;
};

class Game
{
	Users* users[100]{};
	int totalUser = 0;
	int tictactoe[9] = {0}; // 1 for user and 2 for computer and 0 for none
public:
	Game(Connection* conn);
	~Game();
	void loadData(Connection* conn);
	void registerUser(Connection* conn);
	int matchingNames(const char* name);
	int matchingPasswords(const char* password);
	int login(Connection* conn);
	void scoreboard(Connection* conn);
	char* tttMenu(int index);
	void display(char userChar);
	void wholeGame(char userChar, int index);
	int turn(int playerNumber);
	int winner();
	void updateDatabase(Connection* conn, int index);
	void updateMenu(int index);
	void deleteUser(Connection* conn, int index);
	Game& operator = (const Game* src);
};


#endif // !USERS_H	