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
        void changeAddress(connection&, connection&);
        void changeBirthday(connection&, connection&);
};

class Admin
{
    private:
        int aid;
        string database;
    public:
        void loginAdmin(string);
        void adminMenu(connection&);
        void viewAccount(connection&);
        void updateAccountInfo(connection&);
        void addNewAdmin(connection&);
        void updateMovies();
};

class Utility
{
    public:
        void changeEmail(connection&, connection&, string);
        bool changePassword(connection&, connection&, string);
        void changeName(connection&, connection&, string, string);
};

#endif
