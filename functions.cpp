
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
                << "2) Browse by genre" << endl;
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
int i = 1;
for(auto row : *R)
{
    cout << i<< ". " << row["title"] << endl;
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

void functions::displayAllMoviesByGenre(connection &C, string genre)
{
string sql = "SELECT * FROM movies WHERE Genre ='" +genre +"';";   
nontransaction N1(C); // Create a non-transactional object
result *R = new result(N1.exec(sql)); // Get the result of the query
int i = 1;
for(auto row : *R)
{
    cout << i<< ". " << row["title"] << endl;
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

void functions::addToCart(connection &C, int cid)
{
int option;
cout <<"Would you like to add to cart?" <<endl
        <<"1) Yes"<<endl
        <<"2) No"<<endl;

 cin >> option;
    switch(option)
    {
        case 1:
        {
            cout << "Enter the number of the movie you would like to add to your cart!" <<endl;
            cin >> option;
            if(res ==nullptr || option <1 || option >res->size())//if there is no
                    {cout << "Sorry! We experienced some technical difficulties"<<endl;
                    return;
                    }
            string insert = "INSERT INTO CART(CID, mid, qty)"
                        "VALUES ("+to_string(cid) +',' +res->at(option-1)["mid"].as<string>()+",1)"
                        "ON CONFLICT ON CONSTRAINT cart_pk DO UPDATE SET qty = CART.qty + 1;";
            work W(C); // Create a transactional object
            W.exec(insert);
            W.commit();
            break;
        }
        case 2:
        break;
    }
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