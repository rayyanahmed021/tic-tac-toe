

#include <iostream>
#include <occi.h>
#include "users.h"
#include "menus.h"
#include "helperFunctions.h"

using namespace std;
int main()
{
	//OCCI Variables
	Environment* env = nullptr;
	Connection* conn = nullptr;
	

	//string objects
	string usr = "dbs211_212d04";
	string pass = "26473162";
	string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";

	try
	{
		//create an environment
		env = Environment::createEnvironment(Environment::DEFAULT);
		//create a connection with username, password and server
		conn = env->createConnection(usr, pass, srv);
		//create a statement
		Statement* stmt = conn->createStatement();
		Game game(conn);
		//load data
		game.loadData(conn);
		//display the menu
		startMenu(conn,game);
		//terminate statement
		conn->terminateStatement(stmt);
		//terminate connection
		env->terminateConnection(conn);
		//terminate environment
		Environment::terminateEnvironment(env);
	}
	//in case of any errors, display the exact error message
	catch (SQLException& sqlExcp)
	{
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}
