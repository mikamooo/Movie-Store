#include "store.hpp"

void User::loginPage(string db)
{
    string connect = "dbname = " + db + " user = movie_customer password = password hostaddr = 127.0.0.1 port = 5432";
    connection C(connect);

    string email, pass, sql;
    int tries = 2; // Only allow 2 more log in attempts after initial attempt
    bool attempts = false;
    database = db;

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
        // Create SQL statement to get the tuple with the given email and password
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
                
            int option; // Prompt users to retry, create an account, return to main menu, or exit
            cout << "Incorrect email or password." << endl
                 << "1) Try again" << endl
                 << "2) Create an account" 
                 << "3) Return to main menu" << endl;
            cin >> option;

            switch(option) // Respond to the chosen option
            {
                case 1:
                {
                    cout << "Email: ";
                    cin >> email;
                    cout << "Password: ";
                    cin >> pass;
                    tries--;
                    break;
                }
                case 2:
                    // Call a Main function
                    return; 
                case 3:
                    return; 
            }
        }
        else
        {
            cid = R.at(0)["cid"].as<int>();
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
        userMenu(C);
    }

}

void User::userMenu(connection& C)
{
    int option;
    do{
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
            case 1:{
                functions function;
                function.browseMovies(C);
                function.addToCart(C,cid);
                break;}
            case 2:
                viewOrders(C);
                break;
            case 3:{
                functions function;
                function.viewCart(C, cid);
                break;}
            case 4:
                viewAccount(C);
                break;
            case 5:
                option = -1;
                break;
            default:
                option =-1;
                        break;
        }

    }while(option!=-1);

}

void User::viewOrders(connection& C)
{
    int option;
    cout << "***********************************************************************" << endl
         << "*                                                                     *" << endl
         << "*                             Your Orders                             *" << endl
         << "*                                                                     *" << endl
         << "***********************************************************************" << endl << endl;

    // Create SQL statement to get the customer's orders
    string sql = "SELECT OID, Received, Shipped, Address, Status FROM Orders WHERE CID = " 
                 + to_string(cid) + " ORDER BY Received;";
        
    nontransaction N1(C); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query

    for (result::const_iterator c = R.begin(); c != R.end(); c++) // Print the results
    {
        cout << "Order Number: " << c[0].as<string>() << endl;
        cout << "Date Received: " << c[1].as<string>() << endl;
        cout << "Date Shipped: " << c[2].as<string>() << endl;
        cout << "Shipping Address: " << c[3].as<string>() << endl;
        cout << "Order Status: " << c[4].as<string>() << endl << endl;
    } 

    do
    {
        cout << "1) Return to user menu" << endl; // Prompt the user to return to the user menu
        cin >> option;

        switch(option)
        {
            case 1:
                return;
                break;
            default:
                cout << "Please select a valid option." << endl;
                break;
        }

    } while (option != 1);
}

void User::viewAccount(connection& C)
{
    int option;

    do
    {
        cout << "***********************************************************************" << endl
            << "*                                                                     *" << endl
            << "*                        Your Account Info                            *" << endl
            << "*                                                                     *" << endl
            << "***********************************************************************" << endl << endl;

        // Create SQL statement to create a view for the customer's account info
        string sql = "DROP VIEW IF EXISTS CustomerView CASCADE;"
                    "CREATE VIEW CustomerView AS SELECT Email, Password, Cname, Created, Address, DOB FROM Customers WHERE CID = " 
                    + to_string(cid) + " ;";

        work W1(C); // Create a transactional object
        W1.exec(sql);
        W1.commit();

        // Create SQL statement to get the customer's account info
        sql = "SELECT * FROM CustomerView;";
            
        nontransaction N1(C); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        for (result::const_iterator c = R.begin(); c != R.end(); c++) // Print the results
        {
            cout << "Name: " << c[2].as<string>() << endl;
            cout << "E-mail: " << c[0].as<string>() << endl;
            cout << "Date Created: " << c[3].as<string>() << endl;
            cout << "Address: " << c[4].as<string>() << endl;
            cout << "Birthday: " << c[5].as<string>() << endl << endl;
        }

        cout << "1) Return to user menu" << endl // Prompt the user to return to the user menu or update their info
             << "2) Update account info" << endl; 
        cin >> option;

        switch(option)
        {
            case 1:
                return;
                break;
            case 2:
                updateAccountInfo(C);
                break;
            default:
                cout << "Please select a valid option." << endl;
                break;
        }

    } while (option != 1 || option != 2);

}

void User::updateAccountInfo(connection& C)
{
    string connect = "dbname = " + database + " user = movie_customer password = password hostaddr = 127.0.0.1 port = 5432";
    connection C1(connect);
    connection C2(connect);

    int option, tries = 3;
    string sql, change, curr, confirm;
    bool valid;

    do
    {
        cout << "***********************************************************************" << endl
            << "*                        Update Account Info                          *" << endl
            << "*                                                                     *" << endl
            << "*                      Please select an option.                       *" << endl
            << "***********************************************************************" << endl << endl
            << "1) Change e-mail" << endl
            << "2) Change password" << endl
            << "3) Change address" << endl
            << "4) Change name" << endl
            << "5) Change birthday" << endl
            << "6) Return to account info" << endl;
            cin >> option;

        switch(option)
        {
            case 1:
            {
                changeEmail(C1, C2);
                break;
            }
            case 2:
            {
                valid = changePassword(C1, C2);

                if (!valid)
                {
                    C1.disconnect();
                    C2.disconnect();
                    C.disconnect();
                    return; 
                }
                
                break;
            }
            case 3:{
                
                break;}
            case 4:
                
                break;
            case 5:
                option = -1;
                break;
            default:
                option =-1;
                break;
        }

    } while(option != -1);

    C1.disconnect();
    C2.disconnect();
}

void User::changeEmail(connection& C1, connection& C2)
{
    // Create SQL statement to get the customer's account info
    string sql = "SELECT Email FROM CustomerView;";
    string change;
                    
    nontransaction N1(C1); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query
    result::const_iterator c = R.begin(); 
                
    cout << "Your current e-mail is " << c[0].as<string>() << endl
         << "Enter your new e-mail: ";
    cin >> change;

    sql = "UPDATE CustomerView SET Email = '" + change + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your e-mail has been succesfully changed." << endl;
}

bool User::changePassword(connection& C1, connection& C2)
{
    int tries = 3;
    string sql, change, curr, confirm;
    bool valid;

    do
    {
        if (tries == 0) // Once the user has run out of attempts, stop giving them chances
            break;

        cout << "Enter your current password: "; // Prompt customer to enter current password 
        cin >> curr;

        // Create SQL statement to get the customer's name
        sql = "SELECT Cname FROM CustomerView WHERE Password = '" + curr + "';";

        nontransaction N1(C1); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query
        result::const_iterator c = R.begin(); 

        if (R.size() == 0) // Check that a tuple was returned
        {
            cout << "Invalid password." << endl; // If not, let the customer try again
            valid = false;
            tries--;
        }
        else
            valid = true;

    } while(!valid); 
                
    if (tries == 0 && valid == false) // If the customer used up all their attempts, disconnect
    {
        cout << "Too many invalid login attempts. You will be disconnected." << endl;       
        return false;
    }

    cout << "Enter your new password: ";
    cin >> change;
    cout << "Confirm your new password: ";
    cin >> confirm; 

    while (confirm != change)
    {
        cout << "Password does not match. Please confirm your new password." << endl
             << "Confirm your new password: ";
        cin >> confirm;
    }

    sql = "UPDATE CustomerView SET Password = '" + change + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your password has been succesfully changed." << endl;
    return true;
}
