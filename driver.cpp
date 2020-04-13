#include <iostream>
#include <pqxx/pqxx>
#include "store.hpp"

using namespace std;
using namespace pqxx;
void displayAllMovies(connection &C);
int main()
{
    string db, connect;

    // Get the info for the database to connect to
    cout << "Enter the name of the database: "; 
    cin >> db;

    connect = "dbname = " + db + " user = guest password = password hostaddr = 127.0.0.1 port = 5432";

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
                customer.loginPage(db);
                break;
            }
            case 3:
            {
                displayAllMovies(C);
                break;
            }
            default:
                break;
           
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

void displayAllMovies(connection &C)
{
string sql = "SELECT * FROM movies;";   
nontransaction N1(C); // Create a non-transactional object
result R(N1.exec(sql)); // Get the result of the query
int i = 1;
for(auto row : R)
{
    cout << i<< ". " << row["title"] << endl;
    i++;
}
return;
}