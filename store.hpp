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
        void searchPage();
        void createAccount();
};

class User
{   private:
    int cid;
    public:
        void loginPage(string);
        void userMenu(connection&);
        void userSearch();
        void addToCart();
        void checkOut();
        void viewOrders();
        void updateAccountInfo();
};

class Admin
{
    public:
        void loginAdmin();
        void updateMovies();
};

#endif
