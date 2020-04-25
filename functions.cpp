
#include "functions.hpp"

functions::functions()
{
res = nullptr;
}
functions::~functions()
{
delete res;
}

int functions::browseMovies(connection &C)
{
int option, opt;
cout            << "***********************************************************************" << endl
                << "*                         Browse Movies                               *" << endl
                << "*                                                                     *" << endl
                << "*                    Please select an option.                         *" << endl
                << "***********************************************************************" << endl << endl 
                << "1) View all" << endl
                << "2) Browse by Genre" << endl
                << "3) Browse by Price(low-high)" << endl
                << "4) Browse by Title" << endl;
 cin >> option;
    switch(option)
    {
        case 1:
        {
            displayAllMovies(C);
            break;
        }
        case 2:
        {
            genrePrompt(C);
            break;
        }
        case 3:
        {
            displayByPrice(C);
            break;
        }
        case 4:
        {   
            do
            {
                opt = displayByTitle(C);
                if (opt == -1)
                    return -1;
                
            } while(opt);
            break;
        }
        default:
        option =-1;
             break;
           
    }

return 0;
}

void functions::displayAllMovies(connection &C)
{
string sql = "SELECT * FROM movies;";   
nontransaction N1(C); // Create a non-transactional object

result *R = new result(N1.exec(sql)); // Get the result of the query
printMovies(R);
if(res!=nullptr)//if result is not null, delete current contents.
    {
    delete res;
    }
res = R;
return;
}

void functions::displayAllMoviesByGenre(connection &C, string genre)
{
string sql = "SELECT * FROM movies WHERE Genre ='" +genre +"';";   
nontransaction N1(C); // Create a non-transactional object
result *R = new result(N1.exec(sql)); // Get the result of the query
printMovies(R);
if(res!=nullptr)//if result is not null, delete current contents.
    {
    delete res;
    }
res = R;
return;
}

void functions::genrePrompt(connection &C)
{
int option;
cout            << "***********************************************************************" << endl
                << "*                         Select Genre                                *" << endl
                << "*                                                                     *" << endl
                << "*                    Please select an option.                         *" << endl
                << "***********************************************************************" << endl << endl 
                << "1) Comedy" << endl
                << "2) Drama" << endl
                << "3) Horror" << endl
                << "4) Sci-Fi" << endl
                << "5) Animation" << endl
                << "6) Fantasy" << endl
                << "7) Crime" << endl
                << "8) Mystery" << endl;
 cin >> option;
    switch(option)
    {
        case 1:
        {
            displayAllMoviesByGenre(C, "Comedy");
            break;
        }
        case 2:
        {
            displayAllMoviesByGenre(C, "Drama");
            break;
        }
        case 3:
        {
            displayAllMoviesByGenre(C, "Horror");
            break;
        }
        case 4:
        {
            displayAllMoviesByGenre(C, "Sci-Fi");
            break;
        }
        case 5:
        {
            displayAllMoviesByGenre(C, "Animation");
            break;
        }
        case 6:
        {
            displayAllMoviesByGenre(C, "Fantasy");
            break;
        }
        default:
        case 7:
        {
            displayAllMoviesByGenre(C, "Crime");
            break;
        }
        case 8:
        {
            displayAllMoviesByGenre(C, "Mystery");
            break;
        }
        option =-1;
             break;      
    }
}

int functions::selectMovie(connection &C, int user)
{
    int option; int select = -1;
    
    do
    {
    
    cout    <<"Would you like to select a movie?" <<endl
            <<"1) Yes"<<endl
            <<"2) No"<<endl;

    cin>>option;
        switch(option)
        {
            case 1:
            {
                cout << "Enter the number of the movie you would like to select!" <<endl;
                cin >> select;
                if(res ==nullptr || select <1 || select >res->size())//if there is no
                    {cout << "Sorry! We experienced some technical difficulties"<<endl;
                    select = -1;
                    break;
                    }
                string temp = to_string(res->at(select-1)["title"]);
                if(temp.length()>40)
                    temp= temp.substr(0,34) + "...";

                cout << endl << string(75, '=') << endl << setw(40) << left << temp << setw(15) 
                << "Rated: "+ to_string(res->at(select-1)["rating"]) 
                << "Released: " + to_string(res->at(select-1)["year"]) << endl << string(75, '=') << endl
                
                <<endl <<"Synopsis: "<<res->at(select-1)["des"]  <<endl << endl;
                if(user ==1){
                    cout << "Would you like to add this movie to your cart?"<<endl
                        <<"1) Yes"<<endl
                        <<"2) No" <<endl;
                    cin >> option;
                    switch (option)
                    {
                    case 1:
                        {option = -1;
                        break;
                        }
                    default:
                        {select = -1;
                        option = 0;
                        break;
                        }
                    }
                    }
                break;
            }
            case 2:
            {
                option = -1;
                break;
            }
            default:
            {
                break;
            }
        }
    } while (option !=-1);
    return select;
}

void functions::addToCart(connection &C, int cid, int num)
{
        if(res ==nullptr || num <1 || num >res->size())//if there is no
                    {cout << "Sorry! We experienced some technical difficulties"<<endl;
            return;
            }
            if(res->at(num-1)["qty"].as<int>()<=0)
                {cout << "Sorry! We dont have that item in stock."<<endl;
                return;
                }
           string sql = "SELECT qty FROM cart WHERE cid = " +to_string(cid)+" AND OID =-1 AND " 
                        "mid ="+res->at(num-1)["mid"].as<string>() +";";   
           nontransaction N1(C); // Create a non-transactional object
            

            result *R = new result(N1.exec(sql)); // Get the result of the query
            N1.commit();
            if(R->size()>0)
                if(R->at(0)["qty"].as<int>() >=res->at(num-1)["qty"].as<int>())
                    {cout << "It looks like you already have all these in your cart!"<<endl
                            << "Try checking out!"<<endl;
                            return;
                    }
            int addQTY;
            cout << "How many would you like to add to your cart?" <<endl;
            cin >>addQTY;

            if(addQTY>res->at(num-1)["qty"].as<int>())
                {
                    cout << "Sorry, we don't have enough in stock to place into your cart"<<endl;
                    return;
                }
            if(R->size()>0)
                if(R->at(0)["qty"].as<int>()+addQTY >=res->at(num-1)["qty"].as<int>())
                    {cout << "Sorry, we don't have enough in stock to place into your cart"<<endl;
                    return;}
            
                

            string insert = "INSERT INTO CART(CID, mid, qty)"
                        "VALUES ("+to_string(cid) +',' +res->at(num-1)["mid"].as<string>()+","+to_string(addQTY)+")"
                        "ON CONFLICT ON CONSTRAINT cart_pk DO UPDATE SET qty = CART.qty +" +to_string(addQTY)+";";
            work W(C); // Create a transactional object
            W.exec(insert);
            W.commit();
       
    return;
}

int functions::viewCart(connection &C, int CID){

    cout << "***********************************************************************" << endl
         << "*                                                                     *" << endl
         << "*                             Your Cart                               *" << endl
         << "*                                                                     *" << endl
         << "***********************************************************************" << endl << endl;

string sql = "SELECT title, qty2, (movies.price*cart2.qty2) as price FROM movies NATURAL JOIN "
"(SELECT cid, mid, qty as qty2 FROM cart WHERE cid = " +to_string(CID)+" AND OID =-1) as cart2;";   
nontransaction N1(C); // Create a non-transactional object

result *R = new result(N1.exec(sql)); // Get the result of the query
int i = 1;
for(auto row : *R)
{int qty = row["qty2"].as<int>();

    string temp = to_string(row["title"]);
    if(temp.length()>40)
        temp= temp.substr(0,34) + "...";

    cout <<setw(4)<<left << to_string(i)+ ". " << setw(40) << temp << setw(5) << qty <<setw(12)<< "$"
    + to_string(row["price"]) << endl;
    //cout << i<< ". " << row["title"] << "\t" << qty<< "\t$" <<row["price"]<< endl;
    i++;
}
cout <<endl;
N1.commit();

if(res!=nullptr)//if result is not null, delete current contents.
    {
    delete res;
    }
res = R;
if(i>1)
{
    sql = "SELECT SUM(price) FROM (SELECT (movies.price*cart2.qty2) as price FROM movies NATURAL JOIN "
    "(SELECT cid, mid, qty as qty2 FROM cart WHERE cid = " +to_string(CID)+" AND OID =-1) as cart2) as total;"; 

    nontransaction N2(C); // Create a non-transactional object
    result R2(N2.exec(sql)); // Get the result of the query
    result::const_iterator c = R2.begin();
    N2.commit();

    if (R2.size() != 0)
        cout << "Your total: $" << c[0].as<string>() << endl << endl;

    return 1;
}

return 0;
}

void functions::displayByPrice(connection &C)
{
string sql = "SELECT * FROM movies ORDER BY price;";   
nontransaction N1(C); // Create a non-transactional object
result *R = new result(N1.exec(sql)); // Get the result of the query
printMovies(R);
if(res!=nullptr)//if result is not null, delete current contents.
    {
    delete res;
    }
res = R;
return;
}

int functions::displayByTitle(connection &C)
{
    string option;
    cout << "Enter the title of the movie: " <<endl;
    cin.ignore();
    getline(cin, option);
    //cin >> option; cin.clear();
    string sql = "SELECT * FROM movies WHERE title ilike'%"+option+"%';";   
    nontransaction N1(C); // Create a non-transactional object
    result *R = new result(N1.exec(sql)); // Get the result of the query

    if (R->size() == 0)
    {
        cout << endl << "Uh Oh! It looks like we don't have that title yet." << endl
            << "Would you like to search for another title?" << endl
            << "1) Yes " << endl << "2) No" << endl;
        getline(cin, option);

        switch(stoi(option))
        {
            case 1:
                option = "Yes";
                break;
            default:
                option = "No";
                break;
        }
    }
    else
        printMovies(R);
    if(res!=nullptr)//if result is not null, delete current contents.
        {
        delete res;
        }
    res = R;
    if (option == "No")
        return -1;
    else if(option == "Yes")
        return 1;
    else
        return 0;
}

void functions::printMovies(result *R)
{int max_text_size= 40;
int i = 1;
string line = string(85, '=');

cout << line << endl
    << setw(17)<<left << "|#|" << setw(23) << "Movie Title" << setw(4) << "|" << setw(9) << "Genre"  
    << setw(4)<< "|" << setw(8) << "Price" << setw(4)<< "|" << setw(15) << "Availability" << "|"
    << endl << line << endl;

for(auto row : *R)
{
    string temp = to_string(row["title"]);
    if(temp.length()>max_text_size)
        temp= temp.substr(0,max_text_size-6) + "...";
    int stock = row["qty"].as<int>();
    string st;
    if(stock <=0)
    {st = "OUT OF STOCK";       }
    else if(stock<=3)
    {
        st = "LOW STOCK";
    }
    else
    {
        st = "IN STOCK";
    }
    

    cout <<setw(4)<<left << to_string(i)+ ". "  << setw(40)<<temp<<setw(13)<<to_string(row["genre"]) <<setw(12)<< "$"
    + to_string(row["price"]) << " " <<st<<endl;
    i++;
}
cout <<endl;
return;
}

int functions::selectAdmin(connection &C)
{
    int option; int select = -1;
    
    cout << "Enter the number of the movie you would like to update." <<endl;
    cin >> select;
    if(res ==nullptr || select <1 || select >res->size())//if there is no
    {   cout << "Sorry! We experienced some technical difficulties"<<endl;
        return -1;
    }

    // Create SQL statement to create a view for the customer's account info
    string sql = "DROP VIEW IF EXISTS UpdateView CASCADE;"
                "CREATE VIEW UpdateView AS SELECT * FROM Movies WHERE MID =" 
                + to_string(res->at(select-1)["mid"]) + ";";

    work W1(C); // Create a transactional object
    W1.exec(sql);
    W1.commit();
                
    return select;
}