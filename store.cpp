#include "store.hpp"
#include <string>

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

    do // Check if the customer is in the database
    {
        // Create SQL statement to get the tuple with the given email and password
        sql = "SELECT * FROM Customers WHERE Email = '" + email + "' AND Password = '" + pass + "';";
        
        nontransaction N1(C); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        if (R.size() == 0) // If the result doesn't return a tuple, the email or password was incorrect
        {
            if (tries == 0) // Once the customer has run out of attempts, stop giving them chances to login
            {
                attempts = true;
                break;
            }    
                
            int option; // Prompt users to retry, create an account, return to main menu, or exit
            cout << "Incorrect email or password." << endl
                 << "1) Try again" << endl
                 << "2) Create an account" << endl
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
                default:
                    cout << "Please select a valid option." << endl;
                    break; 
            }
        }
        else
        {
            cid = R.at(0)["cid"].as<int>();
            break;
        }

    } while(!attempts);

    if (attempts) // Let the customer know they ran out of attempts and exit
    {
        cout << "Too many invalid login attempts. You will be disconnected." << endl;
        C.disconnect();
        return;
    }
    else // Or let the customer know that they have successfully logged in.
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
                int select = function.selectMovie(C,1);
                if(select!=-1)
                    function.addToCart(C,cid, select);
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
        cout << "1) Return to user menu" << endl; // Prompt the customer to return to the user menu
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

        cout << "1) Return to user menu" << endl // Prompt the customer to return to the user menu or update their info
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
    connection C1(connect); // Create new connections so that another nontransaction can be used
    connection C2(connect); // And so that transactions can be used after a nontransaction

    int option, tries = 3;
    string sql, change, curr, confirm;
    bool valid;
    Utility u;

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
                u.changeEmail(C1, C2, "CustomerView");
                break;
            case 2:
            {
                if (!u.changePassword(C1, C2, "CustomerView")) // If the user enters an invalid current password too many times, disconnect
                {
                    C1.disconnect();
                    C2.disconnect();
                    C.disconnect();
                    return; 
                }
                
                break;
            }
            case 3:
                changeAddress(C1, C2);
                break;
            case 4:
                u.changeName(C1, C2, "CustomerView", "CName");
                break;
            case 5:
                changeBirthday(C1, C2);
                break;
            case 6:
                return;
            default:
                cout << "Please select a valid option." << endl;
                break;
        }

    } while(option != 6);

    C1.disconnect();
    C2.disconnect();
}

void User::changeAddress(connection& C1, connection& C2)
{
    // Create SQL statement to get the customer's address
    string sql = "SELECT Address FROM CustomerView;";
    string street, city, state, zip;
                    
    nontransaction N1(C1); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query
    result::const_iterator c = R.begin(); 
                
    cout << "Your address is " << c[0].as<string>() << endl
         << "Enter your new street address: ";
    getline(cin, street); // Get the newline character after selecting an option first
    getline(cin, street);
    cout << "Enter your new city: ";
    getline(cin, city);
    cout << "Enter your new state: ";
    getline(cin, state);
    cout << "Enter your new zipcode: ";
    getline(cin, zip);

    // Create SQL statement to update the customer's address
    sql = "UPDATE CustomerView SET Address = '" + street + " " + city + ", " + state + " " + zip + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your address has been succesfully changed." << endl << endl;
}

void User::changeBirthday(connection& C1, connection& C2)
{
    // Create SQL statement to get the customer's birthday
    string sql = "SELECT DOB FROM CustomerView;";
    string year, month, day;
                    
    nontransaction N1(C1); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query
    result::const_iterator c = R.begin(); 
                
    cout << "Your birthday is " << c[0].as<string>() << endl
         << "Enter your new birthday year (YYYY): ";
    cin >> year;
    cout << "Enter your new birthday month (MM): ";
    cin >> month;
    cout << "Enter your new birthday day (DD): ";
    cin >> day;

    // Create SQL statement to update the customer's birthday
    sql = "UPDATE CustomerView SET DOB = '" + year + "-" + month + "-" + day + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your birthday has been succesfully changed." << endl << endl;
}

void Admin::loginAdmin(string db)
{
    string connect = "dbname = " + db + " user = movie_admin password = admin123 hostaddr = 127.0.0.1 port = 5432";
    connection C(connect);

    string email, pass, sql;
    int tries = 2; // Only allow 2 more log in attempts after initial attempt
    bool attempts = false;
    database = db;

    cout << "***********************************************************************" << endl
         << "*                                                                     *" << endl
         << "*                             Admin Login                             *" << endl
         << "*                                                                     *" << endl
         << "***********************************************************************" << endl << endl
         << "Email: "; 
    cin >> email;
    cout << "Password: ";
    cin >> pass;

    do // Check if the admin is in the database
    {
        // Create SQL statement to get the tuple with the given email and password
        sql = "SELECT * FROM Admins WHERE Email = '" + email + "' AND Password = '" + pass + "';";
        
        nontransaction N1(C); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        if (R.size() == 0) // If the result doesn't return a tuple, the email or password was incorrect
        {
            if (tries == 0) // Once the admin has run out of attempts, stop giving them chances to login
            {
                attempts = true;
                break;
            }    
                
            int option; // Prompt users to retry, create an account, return to main menu, or exit
            cout << "Incorrect email or password." << endl
                 << "1) Try again" << endl
                 << "2) Return to main menu" << endl;
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
                    return; 
                default:
                    cout << "Please select a valid option." << endl;
                    break;
            }
        }
        else
        {
            aid = R.at(0)["aid"].as<int>();
            break;
        }

    } while(!attempts);

    if (attempts) // Let the admin know they ran out of attempts and exit
    {
        cout << "Too many invalid login attempts. You will be disconnected." << endl;
        C.disconnect();
        return;
    }
    else // Or let the admin know that they have successfully logged in.
    {
        cout << "You have successfully logged in!" << endl;
        adminMenu(C);
    }
}

void Admin::adminMenu(connection& C)
{
    int option;
    do
    {
        cout << "***********************************************************************" << endl
             << "*                   Welcome back administrator!                       *" << endl
             << "*                                                                     *" << endl
             << "*                     Please select an option.                        *" << endl
             << "***********************************************************************" << endl << endl
             << "1) Browse for movies" << endl
             << "2) View your information" << endl
             << "3) Add a new administrator" << endl
             << "4) Log Out" << endl;

        cin >> option;
    
        switch(option)
        {
            case 1:{
                functions function;
                function.browseMovies(C);
                //Update movies function
                break;}
            case 2:
                viewAccount(C);
                break;
            case 3:{
                addNewAdmin(C);
                break;}
            case 4:
                break;
            default:
                cout << "Please select a valid option." << endl;
                break;
        }

    } while(option != 4);
}

void Admin::viewAccount(connection& C)
{
    int option;

    do
    {
        cout << "***********************************************************************" << endl
            << "*                                                                     *" << endl
            << "*                        Your Account Info                            *" << endl
            << "*                                                                     *" << endl
            << "***********************************************************************" << endl << endl;

        // Create SQL statement to create a view for the admin's account info
        string sql = "DROP VIEW IF EXISTS AdminView CASCADE;"
                    "CREATE VIEW AdminView AS SELECT Email, Password, AName FROM Admins WHERE AID = " 
                    + to_string(aid) + " ;";

        work W1(C); // Create a transactional object
        W1.exec(sql);
        W1.commit();

        // Create SQL statement to get the admin's account info
        sql = "SELECT * FROM AdminView;";
            
        nontransaction N1(C); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        for (result::const_iterator c = R.begin(); c != R.end(); c++) // Print the results
        {
            cout << "Name: " << c[2].as<string>() << endl;
            cout << "E-mail: " << c[0].as<string>() << endl << endl;
        }

        cout << "1) Return to user menu" << endl // Prompt the admin to return to the admin menu or update their info
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

void Admin::updateAccountInfo(connection& C)
{
    string connect = "dbname = " + database + " user = movie_admin password = admin123 hostaddr = 127.0.0.1 port = 5432";
    connection C1(connect); // Create new connections so that another nontransaction can be used
    connection C2(connect); // And so that transactions can be used after a nontransaction

    int option, tries = 3;
    string sql, change, curr, confirm;
    bool valid;
    Utility u;

    do
    {
        cout << "***********************************************************************" << endl
            << "*                        Update Account Info                          *" << endl
            << "*                                                                     *" << endl
            << "*                      Please select an option.                       *" << endl
            << "***********************************************************************" << endl << endl
            << "1) Change e-mail" << endl
            << "2) Change password" << endl
            << "3) Change name" << endl
            << "4) Return to account info" << endl;
            cin >> option;

        switch(option)
        {
            case 1:
                u.changeEmail(C1, C2, "AdminView");
                break;
            case 2:
            {
                valid = u.changePassword(C1, C2, "AdminView");

                if (!valid) // If the admin enters an invalid current password too many times, disconnect
                {
                    C1.disconnect();
                    C2.disconnect();
                    C.disconnect();
                    return; 
                }
                
                break;
            }
            case 3:
                u.changeName(C1, C2, "AdminView", "AName");
                break;
            case 4:
                return;
            default:
                cout << "Please select a valid option." << endl;
                break;
        }

    } while(option != 4);

    C1.disconnect();
    C2.disconnect();
}

void Admin::addNewAdmin(connection& C)
{
    string connect = "dbname = " + database + " user = movie_admin password = admin123 hostaddr = 127.0.0.1 port = 5432";
    connection C1(connect); // Create new connections so that another nontransaction can be used
    connection C2(connect);

    string email, password, first, last, sql, confirm;
    int tries = 3;
    bool valid;

    cout << "Enter the new administrator's e-mail: ";
    cin >> email;
    cout << "Enter the new administrators's password: ";
    cin >> password;
    cout << "Enter the new administrator's first name: ";
    cin >> first;
    cout << "Enter the new administrator's last name: ";
    cin >> last;
    
    do
    {
        if (tries == 0) // Once the user has run out of attempts, stop giving them chances
            break;

        cout << "Enter your password to confirm: "; // Prompt customer/admin to enter their current password 
        cin >> confirm;

        // Create SQL statement to get the customer/admin's name (can retrieve any attibute, just not the password)
        sql = "SELECT * FROM Admins WHERE AID = " + to_string(aid) + " AND Password = '" + confirm + "';";

        nontransaction N1(C1); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        if (R.size() == 0) // Check that a tuple was returned
        {
            cout << "Invalid password." << endl; // If not, let the customer/admin try again
            valid = false;
            tries--;
        }
        else
            valid = true;

    } while(!valid); 
                
    if (tries == 0 && valid == false) // If the customer/admin used up all their attempts, disconnect
    {
        cout << "Too many invalid login attempts. You will be disconnected." << endl;
        C1.disconnect();
        C2.disconnect();
        C.disconnect();       
        return;
    }

    sql = "SELECT AID FROM Admins ORDER BY AID DESC LIMIT 1;";

    nontransaction N2(C2); // Create a non-transactional object
    result R(N2.exec(sql)); // Get the result of the query

    int new_aid = R.at(0)["aid"].as<int>() + 1;

    sql = "INSERT INTO ADMINS (AID, Email, Password, AName)"
            "VALUES (" + to_string(new_aid) +  ", '" + email + "', '" + password + "', '" + first + last + "');";
    
    work W1(C); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    C1.disconnect();
    C2.disconnect();
}

void Utility::changeEmail(connection& C1, connection& C2, string view)
{
    // Create SQL statement to get the current email
    string sql = "SELECT Email FROM " + view + ";";
    string change;
                    
    nontransaction N1(C1); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query
    result::const_iterator c = R.begin(); 
                
    cout << "Your current e-mail is " << c[0].as<string>() << endl 
         << "Enter your new e-mail: ";
    cin >> change;

    // Create SQL statement to update the customer/admin's e-mail
    sql = "UPDATE " + view + " SET Email = '" + change + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your e-mail has been succesfully changed." << endl << endl;
}

bool Utility::changePassword(connection& C1, connection& C2, string view)
{
    int tries = 3;
    string sql, change, curr, confirm;
    bool valid;

    do
    {
        if (tries == 0) // Once the user has run out of attempts, stop giving them chances
            break;

        cout << "Enter your current password: "; // Prompt customer/admin to enter their current password 
        cin >> curr;

        // Create SQL statement to get the customer/admin's name (can retrieve any attibute, just not the password)
        sql = "SELECT * FROM " + view + " WHERE Password = '" + curr + "';";

        nontransaction N1(C1); // Create a non-transactional object
        result R(N1.exec(sql)); // Get the result of the query

        if (R.size() == 0) // Check that a tuple was returned
        {
            cout << "Invalid password." << endl; // If not, let the customer/admin try again
            valid = false;
            tries--;
        }
        else
            valid = true;

    } while(!valid); 
                
    if (tries == 0 && valid == false) // If the customer/admin used up all their attempts, disconnect
    {
        cout << "Too many invalid login attempts. You will be disconnected." << endl;       
        return false;
    }

    cout << "Enter your new password: ";
    cin >> change;
    cout << "Confirm your new password: ";
    cin >> confirm; 

    while (confirm != change) // Make sure that the confirmation matches
    {
        cout << "Password does not match. Please confirm your new password." << endl
             << "Confirm your new password: ";
        cin >> confirm;
    }

    // Create SQL statement to update the customer/admin's password
    sql = "UPDATE " + view + " SET Password = '" + change + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your password has been succesfully changed." << endl << endl;
    return true;
}

void Utility::changeName(connection& C1, connection& C2, string view, string name)
{
    // Create SQL statement to get the customer/admin's name
    string sql = "SELECT " + name + " FROM " + view + ";";
    string first, last;
                    
    nontransaction N1(C1); // Create a non-transactional object
    result R(N1.exec(sql)); // Get the result of the query
    result::const_iterator c = R.begin(); 
                
    cout << "Your name is " << c[0].as<string>() << endl
         << "Enter your new first name: ";
    cin >> first;
    cout << "Enter your new last name: ";
    cin >> last;

    // Create SQL statement to update the customer/admin's name
    sql = "UPDATE " + view + " SET " + name + " = '" + first + " " + last + "';";

    work W1(C2); // Create a transactional object
    W1.exec(sql);
    W1.commit();

    cout << "Your name has been succesfully changed." << endl << endl;
}