/*
===============================
Author: Rayyan Ahmed
Date: 28/8/2021
Module: Game
Project: tic-tac-toe
===============================
*/
#include <iostream>
#include <string>
#include <iomanip>
#include <occi.h>
#include "users.h"
#include "game.h"
#include "helperFunctions.h"
using namespace std;



void startMenu(Connection* conn, Game& game)
{
	int selection, index = 0;
	char* tempStr = nullptr;

	do
	{
		cout << "Login Menu" << endl;
		cout << "================" << endl;
		cout << "1) Register" << endl;
		cout << "2) Login" << endl;
		cout << "3) Scoreboard" << endl;
		cout << "0) Exit" << endl;
		selection = getInt("Enter Menu option: ", 0, 3,
			"Enter within the range of menu options");
		cout << endl;

		switch (selection)
		{
		case 1:
		{
			game.registerUser(conn);
		}
		break;
		case 2:
		{
			index = game.login();
			if (index != -1)
			{
				game.tttMenu(index);
			}
		}
		break;
		case 3:
		{
			game.scoreboard();
		}
		break;
		case 0:
		{
			game.updateDatabase(conn);
		}
		break;
		default:
			break;
		}
	} while (selection);
}


Game::Game(Connection* conn)
{
	int i = 0;

	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT COUNT(*) FROM tictactoe");

	while (rs->next())
	{
		totalUser = rs->getInt(1);
	}
}
Game::~Game()
{
	for (int i = 0; i < totalUser; i++)
	{
		delete users[i];
	}
}
void Game::loadData(Connection* conn)
{
	int i = 0;
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT * FROM tictactoe");
	users = new Users * [totalUser];

	while (rs->next())
	{
		users[i] = new Users(rs->getString(2).c_str(),
			rs->getString(3).c_str(),
			rs->getInt(4),
			rs->getInt(5),
			rs->getInt(6),
			i + 1);
		i++;
	}
}
void Game::addUser(Users* newUser)
{
	Users** tempArray = new Users * [totalUser + 1]{};
	totalUser++;
	for (int i = 0; i < totalUser - 1; i++)
	{
		tempArray[i] = users[i];
	}
	tempArray[totalUser - 1] = newUser;

	delete[] users;

	users = tempArray;
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
			cout << "ERROR: A user with the same username exists."
				<< "Enter a different username." << endl;
		}
	} while (flag != -1);
	pass = getString("Password", 4, 10, cin);

	val = getChar("Are you sure you want to register? (Y/N): ", "YN", cin);
	if (val == 'Y')
	{
		addUser(new Users(totalUser + 1, username, pass));
	}
}

int Game::matchingNames(const char* name)const
{
	int i, index = -1;
	for (i = 0; i < totalUser && strcmp(users[i]->name(), name); i++);

	if (i != totalUser)
	{
		index = i;
	}
	return index;
}

int Game::matchingPasswords(const char* password)const
{
	int i, index = -1;
	for (i = 0; i < totalUser && strcmp(users[i]->password(), password); i++);

	return i == totalUser ? -1 : i;
}

int Game::login()const
{
	string name, pass;
	int flag = 0;
	char valid = '\0';
	int index = -1;
	do
	{
		cout << "Username: ";
		getline(cin, name, '\n');
		cout << "Password: ";
		getline(cin, pass, '\n');
		cout << endl;
		index = matchingNames(name.c_str());

		if (index != -1 && strcmp(users[index]->password(), pass.c_str()) == 0)
		{
			flag = 1;
		}
		else
		{
			cerr << "ERROR: Username and passwords is incorrect." << endl;
			valid = getChar("Do you wish to try again ? (Y/N): ", "YN", cin);
			if (valid == 'N')
			{
				flag = 1;
				index = -1;
			}

		}
	} while (!flag);
	return index;
}

void Game::scoreboard()const
{
	selectionSort(users, totalUser);
	cout.setf(ios::left);
	cout << setw(6) << "Rank" << setw(15) << "Username"
		<< setw(6) << "Wins" << setw(11) << "Total Games" << endl;
	cout.unsetf(ios::left);
	cout.setf(ios::right);
	cout << setw(6) << setfill('=') << "= "
		<< setw(15) << setfill('=') << "= "
		<< setw(6) << setfill('=') << "= "
		<< setw(11) << setfill('=') << "= "
		<< endl;
	cout.unsetf(ios::right);

	cout << setfill(' ');
	cout.setf(ios::left);
	for (int i = 0; i < totalUser; i++)
	{
		cout << setfill(' ') << setw(6) << i + 1
			<< setw(15) << users[i]->name()
			<< setw(6) << users[i]->score('w')
			<< setw(7) << users[i]->score('l') + users[i]->score('w') + users[i]->score('d')
			<< endl;
	}
	cout << endl;
}

char* Game::tttMenu(int index)
{
	int selection;
	char userChar = '\0';
	do
	{
		cout << "Welcome " << users[index]->name() << "!" << endl;
		cout << "================" << endl;
		cout << "1) Play" << endl;
		cout << "2) Stats" << endl;
		cout << "3) Update Account" << endl;
		cout << "4) Delete Account" << endl;
		cout << "0) Log Out" << endl;
		selection = getInt("Enter Menu option: ", 0, 4, "Enter within the range of menu options");
		cout << endl;
		switch (selection)
		{
		case 1:
		{
			userChar = getChar("Select a character (X/O): ", "XO", cin);
			wholeGame(userChar, index);
		}
		break;
		case 2:
		{
			users[index]->displayScore();
		}
		break;
		case 3:
		{
			updateMenu(index);
		}
		break;
		case 4:
		{
			removeDynamicElement(users, index, totalUser);
			selection = 0;
		}
		break;
		default:
			break;
		}
	} while (selection);
	return users[index]->password();
}

void Game::display(char userChar)const
{
	userChar == 'X' ? cout << "User: X" : cout << "User: O";
	cout << "\t\t";
	userChar == 'X' ? cout << "Computer: O" : cout << "Computer: X";
	cout << endl << endl;
	for (int i = 1, j = 0; i <= 3; i++, j += 3)
	{
		cout << "|_";
		if (tictactoe[j] == 1)
		{
			cout << userChar;
		}
		else if (tictactoe[j] == 2)
		{
			userChar == 'X' ? cout << 'O' : cout << 'X';
		}
		else
		{
			cout << j + 1;
		}
		cout << "_|_";


		if (tictactoe[j + 1] == 1)
		{
			cout << userChar;
		}
		else if (tictactoe[j + 1] == 2)
		{
			userChar == 'X' ? cout << 'O' : cout << 'X';
		}
		else
		{
			cout << j + 2;
		}
		cout << "_|_";

		if (tictactoe[j + 2] == 1)
		{
			cout << userChar;
		}
		else if (tictactoe[j + 2] == 2)
		{
			userChar == 'X' ? cout << 'O' : cout << 'X';
		}
		else
		{
			cout << j + 3;
		}
		cout << "_|" << endl;

	}
	cout << endl << endl;
}

void Game::wholeGame(char userChar, int index)
{
	int turns = 0, win;
	do
	{
		display(userChar);

		if (turns % 2 == 0)
		{
			//take user input
			userTurn();
		}
		else
		{
			//call computer turn
			compTurn();
		}
		turns++;
		win = winner();

	} while (!win && turns < 9);

	display(userChar);
	for (int i = 0; i < 9; i++)
	{
		tictactoe[i] = 0;
	}
	cout << "WINNER: ";

	if (win)
	{
		if (win == 1)
		{
			cout << "You\t";
			users[index]->updateScore('w');
		}
		else if (win == 2)
		{
			cout << "Computer\t";
		}
		cout << "LOSER: ";
		if (win == 1)
		{
			cout << "Computer";
		}
		else if (win == 2)
		{
			cout << "You";
			users[index]->updateScore('l');
		}
	}
	else
	{
		cout << "This game is a draw";
		users[index]->updateScore('d');
	}
	cout << endl << endl;
}

int Game::userTurn()
{
	int userTurn = 0, flag = 0;
	do
	{
		//for user
		userTurn = getInt("Select a number: ", 1, 9,
			"Choose between 1 and 9");

		if (tictactoe[userTurn - 1] == 0)
		{
			tictactoe[userTurn - 1] = 1;
			flag = 1;
		}
		else
		{
			cerr << "ERROR: Select empty boxes. Try again." << endl;
		}
	} while (!flag);
	return 0;
}

int Game::compTurn()
{
	int turn = 0, flag = 1;
	bool strategic = false;
	do
	{
		if ((tictactoe[0] == 1 && tictactoe[1] == 1 && !tictactoe[2])
			|| (tictactoe[1] == 1 && tictactoe[2] == 1 && !tictactoe[0])
			|| (tictactoe[0] == 1 && tictactoe[2] == 1 && !tictactoe[1]))
		{
			if (!tictactoe[0])tictactoe[0] = 2;
			else if (!tictactoe[1])tictactoe[1] = 2;
			else if (!tictactoe[2])	tictactoe[2] = 2;
		}
		else if ((tictactoe[3] == 1 && tictactoe[4] == 1 && !tictactoe[5])
			|| (tictactoe[4] == 1 && tictactoe[5] == 1 && !tictactoe[3])
			|| (tictactoe[3] == 1 && tictactoe[5] == 1 && !tictactoe[4]))
		{
			if (!tictactoe[3]) tictactoe[3] = 2;
			else if (!tictactoe[4])	tictactoe[4] = 2;
			else if (!tictactoe[5])tictactoe[5] = 2;
		}
		else if ((tictactoe[6] == 1 && tictactoe[7] == 1 && !tictactoe[8])
			|| (tictactoe[7] == 1 && tictactoe[8] == 1 && !tictactoe[6])
			|| (tictactoe[6] == 1 && tictactoe[8] == 1 && !tictactoe[7]))
		{
			if (!tictactoe[6])tictactoe[6] = 2;
			else if (!tictactoe[7])tictactoe[7] = 2;
			else if (!tictactoe[8])	tictactoe[8] = 2;
		}
		//vertical columns
		else if ((tictactoe[0] == 1 && tictactoe[3] == 1 && !tictactoe[6])
			|| (tictactoe[3] == 1 && tictactoe[6] == 1 && !tictactoe[0])
			|| (tictactoe[0] == 1 && tictactoe[6] == 1 && !tictactoe[3]))
		{
			if (!tictactoe[0])tictactoe[0] = 2;
			else if (!tictactoe[3])tictactoe[3] = 2;
			else if (!tictactoe[6])	tictactoe[6] = 2;
		}
		else if ((tictactoe[1] == 1 && tictactoe[4] == 1 && !tictactoe[7])
			|| (tictactoe[4] == 1 && tictactoe[7] == 1 && !tictactoe[1])
			|| (tictactoe[1] == 1 && tictactoe[7] == 1 && !tictactoe[4]))
		{
			if (!tictactoe[1])tictactoe[1] = 2;
			else if (!tictactoe[4])tictactoe[4] = 2;
			else if (!tictactoe[7])	tictactoe[7] = 2;
		}
		else if ((tictactoe[2] == 1 && tictactoe[5] == 1 && !tictactoe[8])
			|| (tictactoe[5] == 1 && tictactoe[8] == 1 && !tictactoe[2])
			|| (tictactoe[2] == 1 && tictactoe[8] == 1 && !tictactoe[5]))
		{
			if (!tictactoe[2])tictactoe[2] = 2;
			else if (!tictactoe[5])tictactoe[5] = 2;
			else if (!tictactoe[8])	tictactoe[8] = 2;
		}
		//diagnol
		else if ((tictactoe[6] == 1 && tictactoe[4] == 1 && !tictactoe[2])
			|| (tictactoe[6] == 1 && tictactoe[2] == 1 && !tictactoe[4])
			|| (tictactoe[4] == 1 && tictactoe[2] == 1 && !tictactoe[6]))
		{
			if (!tictactoe[6])tictactoe[6] = 2;
			else if (!tictactoe[2])tictactoe[2] = 2;
			else if (!tictactoe[4])	tictactoe[4] = 2;
		}
		else if ((tictactoe[0] == 1 && tictactoe[4] == 1 && !tictactoe[8])
			|| (tictactoe[4] == 1 && tictactoe[8] == 1 && !tictactoe[0])
			|| (tictactoe[0] == 1 && tictactoe[8] == 1 && !tictactoe[4]))
		{
			if (!tictactoe[0])tictactoe[0] = 2;
			else if (!tictactoe[4])tictactoe[4] = 2;
			else if (!tictactoe[8])	tictactoe[8] = 2;
		}

		else
		{
			srand(time(NULL));
			turn = rand() % 9 + 1;

			if (tictactoe[turn - 1] == 0)
			{
				tictactoe[turn - 1] = 2;
				flag = 1;
			}
			else
			{
				flag = 0;
			}
		}
	} while (!flag);
	return 0;
}

int Game::winner()const
{
	int winner = 0;

	if (tictactoe[0] == tictactoe[1] && tictactoe[2] == tictactoe[1])
	{
		winner = tictactoe[0];
	}
	else if (tictactoe[2] == tictactoe[5] && tictactoe[8] == tictactoe[5])
	{
		winner = tictactoe[2];
	}
	else if (tictactoe[6] == tictactoe[7] && tictactoe[8] == tictactoe[7])
	{
		winner = tictactoe[6];
	}
	else if (tictactoe[0] == tictactoe[3] && tictactoe[6] == tictactoe[3])
	{
		winner = tictactoe[0];
	}
	else if (tictactoe[1] == tictactoe[4] && tictactoe[7] == tictactoe[4])
	{
		winner = tictactoe[1];
	}
	else if (tictactoe[3] == tictactoe[4] && tictactoe[5] == tictactoe[4])
	{
		winner = tictactoe[3];
	}
	else if (tictactoe[0] == tictactoe[4] && tictactoe[8] == tictactoe[4])
	{
		winner = tictactoe[0];
	}
	else if (tictactoe[2] == tictactoe[4] && tictactoe[6] == tictactoe[4])
	{
		winner = tictactoe[6];
	}
	return winner;
}

void Game::updateDatabase(Connection* conn)const
{
	Statement* stmt = conn->createStatement();
	stmt->executeQuery("DELETE FROM tictactoe");
	for (int i = 0; i < totalUser; i++)
	{
		users[i]->updateDatabase(conn);
	}
}

void Game::updateMenu(int index)
{
	int selection;
	char* str, val;
	int flag = 0;
	do
	{
		cout << "Update Menu" << endl;
		cout << "============" << endl;
		cout << "1) Change Username" << endl;
		cout << "2) Change Password" << endl;
		cout << "0) Exit" << endl;

		selection = getInt("Enter Menu option: ", 0, 2,
			"Enter within the range of menu options");
		cout << endl;

		switch (selection)
		{
		case 1:
		{
			do
			{
				str = getString("Username", 1, 30, cin);
				flag = matchingNames(str);
				if (flag != -1)
				{
					cout << "ERROR: A user with the same username exists."
						<< "Enter a different username." << endl;
					val = getChar("Do you want to try again? (Y/N): ", "YN", cin);
					if (val == 'N')
					{
						flag = 0;
					}
				}
				else
				{
					val = getChar("Are you sure you want to update your username? (Y/N): ", "YN", cin);
					if (val == 'Y')
					{
						users[index]->setName(str);
					}
				}
			} while (flag != -1);

		}
		break;
		case 2:
		{
			str = getString("Password", 4, 10, cin);
			val = getChar("Are you sure you want to update your password? (Y/N): ", "YN", cin);
			if (val == 'Y')
			{
				users[index]->setPassword(str);
			}
		}
		break;
		default:
			break;
		}
	} while (selection);
}




