#ifndef MENUS_H
#define MENUS_H

#include <occi.h>
using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
//void loadData(Connection* conn, Game& game);
void startMenu(Connection* conn, Game& game);

//void login();
#endif // !MENUS_H