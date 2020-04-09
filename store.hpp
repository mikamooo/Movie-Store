#ifndef _STORE_H
#define _STORE_H
#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class Main
{
    public:
        void welcomePage();
        void searchPage();
};

class User
{
    public:
        void loginPage(string);
        void userMenu(string);
        void userSearch();
        void addToCart();
        void checkOut();
        void viewOrders();
};

class Admin
{
    public:
        void loginAdmin();
        void updateMovies();
};

#endif