#include <iostream>
#include <string>
#include <occi.h>
#include "users.h"
#include "menus.h"
#include "helperFunctions.h"
using namespace std;


//void loadData(Connection* conn, Game& game)
//{
//	int i = 0;
//	Statement* stmt = conn->createStatement();
//	ResultSet* rs = stmt->executeQuery("SELECT * FROM tictactoe");
//
//	while (rs->next())
//	{
//		game.user[i] = new Users(rs->getString(2).c_str(), rs->getString(3).c_str(), rs->getInt(4), rs->getInt(5), rs->getInt(6), rs->getInt(1));
//		i++;
//	}
//}

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
		selection = getInt("Enter Menu option: ", 0, 3, "Enter within the range of menu options");
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
			index = game.login(conn);
			if (index != -1)
			{
				tempStr = game.tttMenu(index);
				if (tempStr[0] == '\0')
				{
					game.deleteUser(conn, index);
				}
				else
				{
					game.updateDatabase(conn, index);
				}
			}
		}
		break;
		case 3:
		{
			game.scoreboard(conn);
		}
		break;
		default:
			break;
		}
	} while (selection);
}





