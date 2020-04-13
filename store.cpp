#include "store.hpp"

void User::loginPage(string db)
{
    string connect = "dbname = " + db + " user = movie_customer password = password hostaddr = 127.0.0.1 port = 5432";
    connection C(connect);

    string email, pass, sql;
    int tries = 2, cid; // Only allow 2 more log in attempts after initial attempt
    bool attempts = false;

    cout << "***********************************************************************" << endl
         << "*                                                                     *" << endl
         << "*                            Customer Login                           *" << endl
         << "*                                                                     *" << endl
         << "***********************************************************************" << endl << endl
         << "Email: "; 
    cin >> email;
    cout << "Password: ";
    cin >> pass;

    do // Check if the user is in the database
    {
        sql = "SELECT * FROM Customers WHERE Email = '" + email + "' AND Password = '" + pass + "';";
        
        nontransaction N1(C); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        if (R.size() == 0) // If the result doesn't return a tuple, the email or password was incorrect
        {
            if (tries == 0) // Once the user has run out of attempts, stop giving them chances to login
            {
                attempts = true;
                break;
            }    
                
            int option; // Prompt users to retry, create an account, or exit
            cout << "Incorrect email or password." << endl
                 << "1) Try again" << endl
                 << "2) Create an account" << endl
                 << "3) Exit" << endl;
            cin >> option;

            if (option == 1)
            {
                cout << "Email: ";
                cin >> email;
                cout << "Password: ";
                cin >> pass;
                tries--;
            }
            else if (option == 2)
            {
                // Call a Main function
                return; 
            }
            else if (option == 3)
            {
                C.disconnect();
                return;
            }
        }
        else
        {
            result::const_iterator c = R.begin();
            cid = c[0].as<int>();
            break;
        }

    } while(!attempts);

    if (attempts) // Let the user know they ran out of attempts and exit
    {
        cout << "Too many invalid login attempts. You will be disconnected." << endl;
        C.disconnect();
        return;
    }
    else // Or let the user know that they have successfully logged in.
    {
        cout << "You have successfully logged in!" << endl;
        CID = cid;
        userMenu(C);
    }

}

void User::userMenu(connection& C)
{
    int option;
    cout << "***********************************************************************" << endl
         << "*                    Welcome back movie lover!                        *" << endl
         << "*                                                                     *" << endl
         << "*                     Please select an option.                        *" << endl
         << "***********************************************************************" << endl << endl
         << "1) Browse for movies" << endl
         << "2) View orders" << endl
         << "3) View cart" << endl
         << "4) View account info" << endl
         << "5) Log Out" << endl;
    cin >> option;

    switch(option)
    {
        case 1:
            break;
        case 2:
            viewOrders(C);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            option =-1;
                    break;
    }

}

void User::viewOrders(connection& C)
{
    int option;
    cout << "***********************************************************************" << endl
         << "*                                                                     *" << endl
         << "*                             Your Orders                             *" << endl
         << "*                                                                     *" << endl
         << "***********************************************************************" << endl << endl;

    string sql = "SELECT OID, Received, Shipped, Address, Status FROM Orders WHERE CID = " 
                 + to_string(CID) + " ORDER BY Received;";
        
    nontransaction N1(C); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query

    for (result::const_iterator c = R.begin(); c != R.end(); c++)
    {
        cout << "Order Number: " << c[0].as<string>() << endl;
        cout << "Date Received: " << c[1].as<string>() << endl;
        cout << "Date Shipped: " << c[2].as<string>() << endl;
        cout << "Shipping Address: " << c[3].as<string>() << endl;
        cout << "Order Status: " << c[4].as<string>() << endl << endl;
    } 

    do
    {
        cout << "1) Back to user menu" << endl;
        cin >> option;

        switch(option)
        {
            case 1:
                userMenu(C);
                break;
            default:
                cout << "Please select a valid option." << endl;
                break;
        }

    } while (option != 1);
}
