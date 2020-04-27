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
        void createAccount(string);
};

class User
{   private:
        int cid;
        string database;
    public:
        void loginPage(string); 
        void userMenu(connection&);
        void placeOrder(connection&);
        void viewOrders(connection&);
        void viewAllOrders(connection&);
        int viewAccount(connection&);
        int updateAccountInfo(connection&);
        void changeAddress(connection&);
};

class Admin
{
    private:
        int aid;
        string database;
    public:
        void loginAdmin(string);
        void adminMenu(connection&);
        int viewAccount(connection&);
        int updateAccountInfo(connection&);
        int addNewAdmin(connection&);
        void updateMovieMenu(connection&);
        void updateMovies(connection&);
        int addMovies(connection&);
        
};

class Utility
{
    public:
        void changeEmail(connection&, string);
        bool changePassword(connection&, string);
        void changeName(connection&, string, string);
        void changeBirthday(connection&, string);
        void updateAttr(connection&, string);
};

#endif
