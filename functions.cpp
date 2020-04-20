
#include "functions.hpp"

functions::functions()
{
res = nullptr;
}
functions::~functions()
{
delete res;
}

void functions::browseMovies(connection &C)
{
int option;
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
            displayByTitle(C);
            break;
        }
        default:
        option =-1;
             break;
           
    }

return;
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
                cout <<res->at(select-1)["title"] <<endl <<"Synopsis: "<<res->at(select-1)["des"]  <<endl;
                if(user ==1){
                    cout << "Would you like to add this movie to your cart?"<<endl
                        <<"1)Yes"<<endl
                        <<"2)No" <<endl;
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
            string insert = "INSERT INTO CART(CID, mid, qty)"
                        "VALUES ("+to_string(cid) +',' +res->at(num-1)["mid"].as<string>()+",1)"
                        "ON CONFLICT ON CONSTRAINT cart_pk DO UPDATE SET qty = CART.qty + 1;";
            work W(C); // Create a transactional object
            W.exec(insert);
            W.commit();
       
    return;
}

void functions::viewCart(connection &C, int CID){
string sql = "SELECT title, qty2, price FROM movies NATURAL JOIN (SELECT cid, mid, qty as qty2 FROM cart) as cart2 WHERE cid = " +to_string(CID)+";";   
nontransaction N1(C); // Create a non-transactional object

result *R = new result(N1.exec(sql)); // Get the result of the query
int i = 1;
for(auto row : *R)
{
    cout << i<< ". " << row["title"] << "\t" << row["qty2"]<< "\t$" << row["price"]<< endl;
    i++;
}
cout <<endl;
if(res!=nullptr)//if result is not null, delete current contents.
    {
    delete res;
    }
res = R;
return;

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

void functions::displayByTitle(connection &C)
{
string option;
cout << "Enter the title of the movie: " <<endl;
cin.ignore();
getline(cin, option);
//cin >> option; cin.clear();
string sql = "SELECT * FROM movies WHERE title like'%"+option+"%';";   
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

void functions::printMovies(result *R)
{int max_text_size= 40;
int i = 1;
cout    <<setw(max_text_size+10+5+6)<<setfill('_') << ""<<endl
         <<"|#|"<<setw((max_text_size+10+5+6)/2 -2)<<setfill(' ') <<"Movie Title"
         <<setw((max_text_size+10+5+6)/2) << "| Genre | Price|"<<endl
         <<setw(max_text_size+10+5+6)<<setfill('-') << ""<<endl;
        // <<
cout <<setfill(' ');
for(auto row : *R)
{
    
    string temp = to_string(row["title"]);
    if(temp.length()>max_text_size)
        temp= temp.substr(0,max_text_size-3) + "...";
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
    

    cout <<setw(4)<<left <<to_string(i)+ ". "  << setw(40)<<temp<<setw(10)<<to_string(row["genre"]) <<setw(6)<< "$"
    + to_string(row["price"]) << " " <<st<<endl;
    i++;
}
cout <<endl;
return;
}