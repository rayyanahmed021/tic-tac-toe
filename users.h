#include <occi.h>
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;

class Users
{
	char* username{ nullptr };
	char* pass{ nullptr };
	int win = 0;
	int loss = 0;
	int draws = 0;
public:
	Users(const char* name, const char* password);
	Users(const char* name, const char* password, int w, int l, int d, int id);
	Users();
	~Users();
	void setPassword(const char* password);
	void setName(const char* name);
	char* name()const;
	char* password()const;
	

	Users& operator = (const Users*);
	Users(const Users&) = delete;
};

class Game
{
	Users* users;
	int totalUser = 0;
	int tictactoe[9]{ 0 };
public:
	Game(Connection* conn);
	~Game();
	void loadData(Connection* conn);
	void registerUser(Connection* conn);
	int matchingNames(const char* name);
	int matchingPasswords(const char* password);
	void login();
	void tttMenu(int index);
	void display();
	Game& operator = (const Game* src);
};
