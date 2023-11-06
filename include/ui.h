#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include "movie.h"
#include "tv_series.h"

// Menu options enumeration
typedef enum 
{
    MENU_MOVIE_ADD,
    MENU_MOVIE_DISPLAY,
    MENU_TV_SERIES_ADD,
    MENU_TV_SERIES_DISPLAY,
    MENU_EXIT,
    // Add more menu options as necessary
} MenuOption;

// Function Prototypes
void init_ui(); // Initialize the UI environment
void end_ui();  // Clean up and close the UI

MenuOption main_menu(); // Display the main menu and get user choice

void input_movie(Movie *movie);  // Function to input movie details interactively
void input_tv_series(TV_Series *series); // Function to input TV series details interactively

void display_movie_list(const Movie *movies, int count); // Display a list of movies
void display_tv_series_list(const TV_Series *series, int count); // Display a list of TV series
void print_to_left(WINDOW *win, int starty, const char *string, chtype color);

// Add missing function prototypes
void display_movie_list_ui(Movie** movies, int count);
void ui_print_error(const char* format, ...);
void edit_movie_ui(Movie** movies, int count);




#endif //UI_H
