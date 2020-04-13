#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H
#include <pqxx/pqxx>
#include <iostream>
using namespace std;
using namespace pqxx;


void browseMovies(connection &C);
void displayAllMovies(connection &C);
void displayAllMoviesByGenre(connection &C, string genre);
void genrePrompt(connection &C);


#endif