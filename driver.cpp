#include <iostream>
#include <pqxx/pqxx>
#include "store.hpp"

using namespace std;
using namespace pqxx;

int main()
{
    string db, user, pass, connect;

    // Get the info for the database to connect to
    cout << "Enter the name of the database: "; 
    cin >> db;
    cout << "Enter the username: ";
    cin >> user;
    cout << "Enter the password: "; 
    cin >> pass;

    connect = "dbname = " + db + " user = " + user + " password = " + pass + " hostaddr = 127.0.0.1 port = 5432";

    try // Try connecting to the database
    {
        connection C(connect);

        if (C.is_open())
        {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else
        {
            cout << "Can't open database" << endl;
            return 1;
        }

        int option;
        cout << "***********************************************************************" << endl
             << "*                  Welcome to the Movie Store!                        *" << endl
             << "*                                                                     *" << endl
             << "*                    Please select an option.                         *" << endl
             << "***********************************************************************" << endl << endl
             << "1) Customer Login" << endl
             << "2) Create an Account" << endl
             << "3) Browse for movies" << endl
             << "4) Admin Login" << endl
             << "5) Exit" << endl;
        cin >> option;

        switch(option)
        {
            case 1:
            {
                User customer;
                customer.loginPage(connect);
                break;
            }
           
        }

        C.disconnect();
    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
