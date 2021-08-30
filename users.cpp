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

void Users::setID(int num)
{
	if (num)
	{
		id = num;
	}
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

void Users::updateDatabase(Connection* conn)
{
	Statement* stmt = conn->createStatement();
	stmt->setSQL("UPDATE tictactoe SET username = (:1), pass = (:2), win  = (:3), loss = (:4), draw = (:5) WHERE userid = (:6)");
	stmt->setString(1, username);
	stmt->setString(2, pass);
	stmt->setInt(3, win);
	stmt->setInt(4, loss);
	stmt->setInt(5, draws);
	stmt->setInt(6, score('i'));
	stmt->executeQuery();
	conn->commit();
}

void Users::displayScore()const
{
	cout.setf(ios::left);
	cout << setw(10) << "Username" << setw(6) << "Wins" << setw(6) << "Loses" << setw(6) << "Draws" << endl;
	cout.unsetf(ios::left);
	cout.setf(ios::right);
	cout << setw(10) << setfill('=') << "= " << setw(6) << setfill('=') << "= " << setw(6) << setfill('=') << "= " << setw(6) << setfill('=') << "= " << endl;
	cout.unsetf(ios::right);
	cout.setf(ios::left);

	cout << setfill(' ') << setw(10) << name() << setw(6) << score('w') << setw(6) << score('l') << setw(6) << score('d') << endl;
	cout.unsetf(ios::left);
	cout << endl << endl;
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
		id = usr->id;
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
Users& Users::operator=(const Users& usr)
{
	if (this != &usr)
	{
		id = usr.id;
		delete[] username;
		setName(usr.username);
		delete[] pass;
		setPassword(usr.pass);
		if (usr.win)
		{
			win = usr.win;
			loss = usr.loss;
			draws = usr.draws;
		}
	}
	return *this;
}
Game::Game(Connection* conn)
{
	int i = 0;
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT COUNT(*) FROM tictactoe");

	while (rs->next())
	{
		users[i++] = new Users();
		totalUser = rs->getInt(1);
	}
}
Game::~Game()
{
	for (int i = 0; i < totalUser; i++)
	{
		delete users[i];
	}
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
		stmt->setSQL("INSERT INTO tictactoe (username, pass,win,loss,draw) VALUES ((:1), (:2),0,0,0)");
		stmt->setString(1, username);
		stmt->setString(2, pass);
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
	for (i = 0; i < totalUser && strcmp(users[i]->name(), name); i++);

	if (i != totalUser)
	{
		index = i;
	}
	return index;
}

int Game::matchingPasswords(const char* password)
{
	int i, index = -1;
	for (i = 0; i < totalUser && strcmp(users[i]->password(), password); i++);

	return i == totalUser ? -1 : i;
}



Game& Game::operator=(const Game* src)
{
	if (this != src)
	{
		for (int i = 0; i < totalUser; i++)
		{
			delete users[i];
		}
		for (int i = 0; i < src->totalUser; i++)
		{
			users[i] = src->users[i];
		}
		totalUser = src->totalUser;
	}
	return *this;
}
int Game::login(Connection* conn)
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

void Game::scoreboard(Connection* conn)
{
	Statement* stmt = conn->createStatement();
	ResultSet* rs = stmt->executeQuery("SELECT username, win, draw FROM tictactoe ORDER BY win DESC, username ASC FETCH NEXT 10 ROWS ONLY");

	cout.setf(ios::left);
	cout << setw(6) << "Rank" << setw(15) << "Username" << setw(6) << "Wins" << setw(7) << "Draws" << endl;
	cout.unsetf(ios::left);
	cout.setf(ios::right);
	cout << setw(6) << setfill('=') << "= " << setw(15) << setfill('=') << "= " << setw(6) << setfill('=') << "= " << setw(7) << setfill('=') << "= " << endl;
	cout.unsetf(ios::right);

	cout << setfill(' ');
	cout.setf(ios::left);
	for (int i = 0; rs->next(); i++)
	{
		cout << setfill(' ') << setw(6) << i + 1 << setw(15) << rs->getString(1) << setw(6) << rs->getInt(2) << setw(7) << rs->getInt(3) << endl;
	}
	cout.unsetf(ios::left);
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
			userChar = '\0';
			users[index]->setPassword(&userChar);
			selection = 0;
		}
		break;
		default:
			break;
		}
	} while (selection);
	return users[index]->password();
}

void Game::display(char userChar)
{
	userChar == 'X' ? cout << "User: X" : cout << "User: O";
	cout << "\t\t";
	userChar == 'X' ? cout << "Computer: O" : cout << "Computer: X";
	cout << endl << endl;
	for (int i = 1, j = 0; i <= 3; i++, j += 3)
	{
		//cout << "_" << j << "_|_" << j + 1 << "_|_" << j + 2 << "_|" << endl;

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
			turn(1);
		}
		else
		{
			//call computer turn
			turn(2);
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
	//win == 1 ? cout << "You\t" : cout << "Computer\t";
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
		//win == 1 ? cout << "Computer" : cout << "You";
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
	//updatescore
}

int Game::turn(int playerNumber)
{
	int userTurn = 0, flag = 0;
	do
	{
		if (playerNumber == 1)
		{
			userTurn = getInt("Select a number: ", 1, 9, "Choose between 1 and 9");

			if (tictactoe[userTurn - 1] == 0)
			{
				tictactoe[userTurn - 1] = 1;
				flag = 1;
			}
			else
			{
				cerr << "ERROR: Select empty boxes. Try again." << endl;
			}
		}
		else
		{
			srand(time(NULL));
			userTurn = rand() % 9 + 1;

			if (tictactoe[userTurn - 1] == 0)
			{
				tictactoe[userTurn - 1] = 2;
				flag = 1;
			}
		}
	} while (!flag);
	return 0;
}

int Game::winner()
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

void Game::updateDatabase(Connection* conn, int index)
{
	users[index]->updateDatabase(conn);
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

		selection = getInt("Enter Menu option: ", 0, 2, "Enter within the range of menu options");
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
					cout << "ERROR: A user with the same username exists. Enter a different username." << endl;
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

void Game::deleteUser(Connection* conn, int index)
{
	Statement* stmt = conn->createStatement();
	stmt->setSQL("DELETE FROM tictactoe WHERE userid = (:1)");
	stmt->setInt(1, users[index]->score('i'));
	stmt->executeQuery();
	conn->commit();

	removeDynamicElement(users, index, totalUser);

}
