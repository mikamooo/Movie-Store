
#include "functions.hpp"

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