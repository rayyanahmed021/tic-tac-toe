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


Users::Users(const char* name, const char* password)
{
	setName(name);
	setPassword(password);
	win = 0;
	loss = 0;
	draws = 0;
}

Users::Users(const char* name, const char* password, int w, int l, int d, int id)
{
	setName(name);
	setPassword(password);
	win = w;
	loss = l;
	draws = d;
}

Users::Users() :username(nullptr), pass(nullptr), win(0), loss(0), draws(0)
{
}

void Users::setName(const char* name)
{
	if (name)
	{
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


void Users::setPassword(const char* password)
{
	if (password)
	{
		pass = new char[strlen(password) + 1];
		strcpy(pass, password);
	}
}

Users::~Users()
{
	delete[] username;
	delete[] pass;
}

Users& Users::operator=(const Users* usr)
{
	if (this != usr)
	{
		delete[] username;
		setName(usr->username);
		delete[] pass;
		setPassword(usr->pass);
		if (usr->win)
		{
			win = usr->win;
			loss = usr->loss;
			draws = usr->draws;
		}
	}
	return *this;
}
Game::Game(Connection* conn)
{
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT COUNT(*) FROM tictactoe");

	while (rs->next())
	{
		users = new Users[rs->getInt(1)];
		totalUser = rs->getInt(1);
	}
}
Game::~Game()
{
	delete[] users;
	//can be loop where each user is delted
}
void Game::loadData(Connection* conn)
{
	int i = 0;
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT * FROM tictactoe");

	while (rs->next())
	{
		users[i] = new Users(rs->getString(2).c_str(), rs->getString(3).c_str(), rs->getInt(4), rs->getInt(5), rs->getInt(6), rs->getInt(1));
		i++;
	}
}
void Game::registerUser(Connection* conn)
{
	char* username;
	char* pass;
	char val;
	int flag = 0;

	do
	{
		username = getString("Username", 1, 30, cin);
		flag = matchingNames(username);
		if (flag != -1)
		{
			cout << "ERROR: A user with the same username exists. Enter a different username." << endl;
		}
	} while (flag != -1);

	pass = getString("Password", 4, 10, cin);

	val = getChar("Are you sure you want to register? (Y/N): ", "YN", cin);
	if (val == 'Y')
	{
		Statement* stmt = conn->createStatement();
		stmt->setSQL("INSERT INTO tictactoe VALUES ((:1), (:2), (:3),0,0,0)");
		stmt->setInt(1, ++totalUser);
		stmt->setString(2, username);
		stmt->setString(3, pass);
		stmt->executeQuery();
		conn->commit();

		//create game()
		*this = new Game(conn);
		// call loaddata
		loadData(conn);
	}
}

int Game::matchingNames(const char* name)
{
	int i, index = -1;
	for (i = 0; i < totalUser && strcmp(users[i].name(), name); i++);

	if (i != totalUser)
	{
		index = i;
	}
	return index;
}

int Game::matchingPasswords(const char* password)
{
	int i, index = -1;
	for (i = 0; i < totalUser && strcmp(users[i].password(), password); i++);

	return i == totalUser ? -1 : i;
}



Game& Game::operator=(const Game* src)
{
	if (this != src)
	{
		delete[] users;
		for (int i = 0; i < src->totalUser; i++)
		{
			users[i] = src->users[i];
		}
		totalUser = src->totalUser;
	}
	return *this;
}
void Game::login()
{
	string name, pass;
	int flag = 0;
	char valid = '\0';
	int index = 0;
	do
	{
		cout << "Username: ";
		getline(cin, name, '\n');
		cout << "Password: ";
		getline(cin, pass, '\n');
		cout << endl;
		index = matchingNames(name.c_str());

		if (index == matchingPasswords(pass.c_str()))
		{
			cout << "SUCCESS!" << endl;
			flag = 1;
			//call the tic tac toe menu
			tttMenu(index);
		}
		else
		{
			cerr << "ERROR: Username and passwords is incorrect." << endl;
			valid = getChar("Do you wish to try again ? (Y/N): ", "YN", cin);
			cout << endl;
			if (valid == 'N')
			{
				flag = 1;
			}

		}
	} while (!flag);
}

void Game::scoreboard(Connection* conn)
{
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT username, win, draw FROM tictactoe ORDER BY win, username FETCH NEXT 10 ROWS ONLY");
	
	cout.setf(ios::left);
	cout << setw(6) << "Rank" << setw(15) << "Username" << setw(6) << "Wins" << setw(7) << "Draws" << endl;
	cout.unsetf(ios::left);
	cout.setf(ios::right);
	cout << setw(6) << setfill('=') << "= " << setw(15) << setfill('=') << "= " << setw(6) << setfill('=') << "= " << setw(7) << setfill('=') << "= " << endl;
	cout.unsetf(ios::right);

	cout.setf(ios::left);
	for (int i = 0; rs->next(); i++)
	{
		cout << setfill(' ') << setw(6) << i + 1 << setw(15) << rs->getString(1) << setw(6) << rs->getInt(2) << setw(7) << rs->getInt(3) << endl;
	}
	cout << endl;
}

void Game::tttMenu(int index)
{
	int selection;
	char userChar = '\0';
	do
	{
		cout << "Welcome " << users[index].name() << "!" << endl;
		cout << "================" << endl;
		cout << "1) Play" << endl;
		cout << "2) Stats" << endl;
		cout << "3) Delete Account" << endl;
		cout << "0) Log Out" << endl;
		selection = getInt("Enter Menu option: ", 0, 3, "Enter within the range of menu options");

		switch (selection)
		{
		case 1:
		{
			userChar = getChar("Select a character (X/O): ", "XO", cin);
			//wholeGame(userChar);
		}
		break;
		case 2:
		{

		}
		break;
		break;
		default:
			break;
		}
	} while (selection);
}

