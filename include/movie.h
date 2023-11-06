#ifndef MOVIE_H
#define MOVIE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "popup.h"


// Error codes
typedef enum 
{
    MOVIE_SUCCESS,
    MOVIE_ERROR_NULL_POINTER,
    MOVIE_ERROR_MEMORY_ALLOCATION,
} MovieError;

typedef struct 
{
    char *title;
    char *director;
    int year;
    float rating;  // Added this for the movie rating
} Movie;

// Function Prototypes
Movie* create_movie(const char *title, const char *director, int year);
MovieError update_movie(Movie *movie, const char *new_title, const char *new_director, int new_year);
void display_movie(const Movie *movie);
Movie* search_movie(Movie* movies[], int count, const char* title);
void sort_movies(Movie* movies[], int count); // Sort by criteria like title or year
void rate_movie(Movie *movie); // Correctly declared
void delete_movie(Movie ***movies, int *count, int index);
void handle_deletion(Movie ***movies, int *count, int selected_index); 


#endif //MOVIE_H
