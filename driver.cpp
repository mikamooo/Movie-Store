#include <iostream>
#include <pqxx/pqxx>
#include "store.hpp"
#include "functions.hpp"

using namespace std;
using namespace pqxx;
int main()
{
    string db, connect;

    // Get the info for the database to connect to
    cout << "Enter the name of the database: "; 
    cin >> db;

    connect = "dbname = " + db + " user = movie_guest password = password hostaddr = 127.0.0.1 port = 5432";

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
        do{
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
                    customer.loginPage(db);
                    option =-1;
                    break;
                }
                case 3:
                {functions function;
                    function.browseMovies(C);
                    function.selectMovie(C);
                    break;
                }
                case 4:
                {
                    Admin admin;
                    admin.loginAdmin(db);
                    option = -1;
                    break;
                }
                default:
                option =-1;
                    break;
           
            }
        
        }while(option!=-1);
    

        C.disconnect();
    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}

