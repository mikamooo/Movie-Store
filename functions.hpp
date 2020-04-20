#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H
#include <pqxx/pqxx>
#include <iostream>
using namespace std;
using namespace pqxx;

class functions{

public:
result *res; // this holds the result of the querry to be accessed by users
functions();
~functions();
void browseMovies(connection &C);
void browseMoviesUser(connection &C);
int selectMovie(connection &C, int user =0);
void displayAllMovies(connection &C);
void displayByPrice(connection &C);
void displayByTitle(connection &C);
void displayAllMoviesByGenre(connection &C, string genre);
void genrePrompt(connection &C);
void addToCart(connection &C, int cid, int num);
void viewCart(connection &C, int cid);
};

#endif