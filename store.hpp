#ifndef _STORE_H
#define _STORE_H
#include <iostream>
#include <pqxx/pqxx>
#include "functions.hpp"
using namespace std;
using namespace pqxx;

class Main
{
    public:
        void createAccount();
};

class User
{   private:
        int cid;
        string database;
    public:
        void loginPage(string); 
        void userMenu(connection&);
        void checkOut();
        void viewOrders(connection&);
        void viewAccount(connection&);
        void updateAccountInfo(connection&);
        void changeEmail(connection&, connection&);
        bool changePassword(connection&, connection&);
        void changeAddress(connection&, connection&);
        void changeName(connection&, connection&);
        void changeBirthday(connection&, connection&);
};

class Admin
{
    public:
        void loginAdmin();
        void updateMovies();
};

#endif
