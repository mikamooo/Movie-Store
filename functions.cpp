
#include "functions.hpp"



void browseMovies(connection &C)
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

void displayAllMoviesByGenre(connection &C, string genre)
{
string sql = "SELECT * FROM movies WHERE Genre ='" +genre +"';";   
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

void genrePrompt(connection &C)
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