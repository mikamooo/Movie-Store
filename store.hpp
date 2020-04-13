#ifndef _STORE_H
#define _STORE_H
#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class Main
{
    public:
        void searchPage();
        void createAccount();
};

class User
{
    private:
        int CID;
    public:
        void loginPage(string);
        void userMenu(connection&);
        void userSearch();
        void addToCart();
        void checkOut();
        void viewCart();
        void viewOrders(connection&);
        void viewAccount(connection&);
        void updateAccountInfo(connection&);
};

class Admin
{
    public:
        void loginAdmin();
        void updateMovies();
};

#endif
