/**
 * @file main.c
 * @brief Main program module for the Movie and TV Series Management System.
 *
 * This file contains the main() function where the program starts execution. It initializes
 * and manages the application flow, handling the creation, loading, display, and saving of
 * movie and TV series entries. The UI is managed through curses library calls for a
 * terminal-based interface.
 *
 * It also includes utility functions such as `resize_entries` for dynamic array resizing,
 * `save_movies_to_file` for persisting movie data to a file, and `load_movies_from_file`
 * for initializing the program state from a file.
 *
 * The program utilizes a menu-driven interface to navigate through different functionalities:
 * adding new entries, displaying lists of entries, and exiting the program while ensuring
 * all changes are saved.
 *
 * @note All UI-related functionalities are assumed to be implemented in separate modules
 * referenced via "ui.h".
 *
 * @version 1.0
 * @date November 6, 2023
 *
 * @author Daskalos Angelos-Christos
 *
 * @par Revision History:
 * - v1.0 (November 6, 2023): Initial release.
 */

/*LIBRARY INCLUSIONS*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "movie.h"
#include "tv_series.h"
#include "ui.h"

/*FUNCTION PROTOTYPES*/
void save_movies_to_file(const char *filename, Movie **movies, int count);
void load_movies_from_file(const char *filename, Movie ***movies, int *count, int *capacity);
void* resize_entries(void** entries, int* capacity); 


/**
 * @brief The entry point of the program, responsible for managing movies and TV series.
 *
 * This function initializes the necessary data structures for storing movies and TV series,
 * handles user interaction through a menu system, and provides functionality for adding,
 * displaying, and saving movies to a file. It uses dynamic memory allocation to maintain
 * arrays of movies and TV series and ensures that the memory is correctly managed throughout
 * the program's execution. It persists movie data across sessions by saving to and loading from
 * a file. The user interface is command-line based, potentially using ncurses for windowing.
 *
 * @return int The exit status of the program. Returns 0 on successful completion, or 1 if an
 *             error occurred during initialization.
 */

int main() 
{
    int movie_capacity = 10, tv_series_capacity = 10;
    int movie_count = 0, tv_series_count = 0;
    Movie** movies = (Movie**) malloc(movie_capacity * sizeof(Movie*));

    ///NOTE: Un-comment the following line when implemented 
    //TV_Series** tv_series = (TV_Series**) malloc(tv_series_capacity * sizeof(TV_Series*));

    if (!movies) // ||!tvseries
    {
        ui_print_error("Failed to allocate memory.");
        return 1;
    }

   load_movies_from_file("movies.txt", &movies, &movie_count, &movie_capacity);
   bool data_changed = false; // flag to track if we have unsaved changes

   MenuOption choice;
   do 
   {
    choice = main_menu();
    switch (choice) 
    {
        case MENU_MOVIE_ADD:
            if (movie_count == movie_capacity) 
            {
                Movie** temp = (Movie**) resize_entries((void**)movies, &movie_capacity);
                if (!temp)
                {
                    show_popup("ERROR", "Failed to resize movie array.\n");
                    break; // Break out of the switch case if resize fails
                }
                movies = temp;
            }
            
            //Define dimensions and starting position for the new window
            int height = 10, width = 50, starty = 5, startx = 5;
            WINDOW* input_win = newwin(height, width, starty, startx);
            box(input_win, 0, 0);
            wrefresh(input_win);

            char title[100], director[100], year_str[10];
            int year;
            echo();

            bool valid_title = false;
            bool valid_director = false;
            bool valid_year = false;
            
            //Calculate the current year
            time_t now = time(NULL);
            struct tm *tm_struct = localtime(&now);
            int current_year = tm_struct->tm_year + 1900;

            while (!valid_title || !valid_director || !valid_year) 
            {
                if (!valid_title) 
                {
                    mvwprintw(input_win, 1, 2, "Enter movie title: ");
                    wgetstr(input_win, title);
                    if (strlen(title) == 0) 
                    {
                        mvwprintw(input_win, 5, 2, "Error: Title cannot be blank.");
                        wrefresh(input_win);
                        continue;
                    }
                    valid_title = true;
                }

                if (!valid_director) 
                {
                    mvwprintw(input_win, 2, 2, "Enter movie director: ");
                    wgetstr(input_win, director);
                    if (strlen(director) == 0) 
                    {
                        mvwprintw(input_win, 5, 2, "Error: Director cannot be blank.");
                        wrefresh(input_win);
                        continue;
                    }
                    valid_director = true;
                }
                
                if (!valid_year) 
                {
                    mvwprintw(input_win, 3, 2, "Enter movie year: ");
                    wgetstr(input_win, year_str);
                    year = atoi(year_str); // Attempt conversion
                    if (strlen(year_str) == 0 || year <= 1800 || year > current_year) 
                    {
                        mvwprintw(input_win, 5, 2, "Error: Please enter a valid year (after 1800).");
                        wrefresh(input_win);
                        continue;
                    }
                    valid_year = true;
                }
            }

            noecho();
            delwin(input_win);

            // Look for the first available NULL position
            int insert_index = -1;
            for (int i = 0; i < movie_count; i++) 
            {
                if (movies[i] == NULL) 
                {
                    insert_index = i;
                    break;
                }
            }

            // Create the movie and add it to the list
            // 1.Reuse the first available spot
            // 2.If there's no gap, add to the end of the array
            Movie* new_movie = create_movie(title, director, year);
            if (new_movie) 
            {
                if (insert_index != -1) 
                {
                    movies[insert_index] = new_movie;
                } 
                else 
                {
                    movies[movie_count++] = new_movie;
                }
            } 
            else 
            {
                show_popup("ERROR", "Failed to create a new movie entry.\n");
                ///NOTE:Add Additional Error Handling (if needed!)
            }
            data_changed = true;
        break;
        case MENU_MOVIE_DISPLAY:
            if(movie_count == 0) 
            {
                show_popup("WARNING","No movies to display!\n");
                clear();
                refresh(); 
            } 
            else 
            {
                display_movie_list_ui(movies, movie_count); 
            }
            data_changed = true;
        break;


        case MENU_TV_SERIES_ADD:
            ///TODO:
            break;

        case MENU_TV_SERIES_DISPLAY:
            ///TODO:
            break;

        case MENU_EXIT:
            if (data_changed) 
            {
                save_movies_to_file("movies.txt", movies, movie_count);
            }
        break;

        default:
            ui_print_error("Invalid choice, please try again.");
        }
    } while (choice != MENU_EXIT);
    save_movies_to_file("movies.txt", movies, movie_count);

    ///NOTE: FREE ALLOCATED MEMORY AT THE END OF SESSION
    for (int i = 0; i < movie_count; ++i) 
    {
        free(movies[i]);
    }
    // for (int i = 0; i < tv_series_count; ++i) 
    // {
    //     free(&tv_series[i]);
    // }
    free(movies);
    //free(tv_series);
    show_popup("WARNING","Exiting Program...\n");

    return 0;
}


/**
 * @brief Resizes the array of entries to double its current capacity.
 *
 * This function takes a pointer to an array and its current capacity and reallocates
 * the array to double its size, which is a common operation in dynamic array management
 * to accommodate more entries as they are added.
 *
 * @param[in,out] entries A pointer to the array of pointers that needs resizing.
 * @param[in,out] capacity A pointer to the current capacity of the array. This value
 *                         will be modified to reflect the new capacity after resizing.
 * @return A pointer to the resized array, or NULL if the reallocation fails.
 */

void* resize_entries(void** entries, int* capacity) 
{
    *capacity *= 2;
    return realloc(entries, (*capacity) * sizeof(void*));
}


/**
 * @brief Saves the array of movies to a specified file.
 *
 * Writes the movie details to a file with each movie's attributes separated by a pipe ('|')
 * character and each movie entry on a new line. The function handles the file opening,
 * writing to the file, and closing the file once writing is complete.
 *
 * @param[in] filename The name of the file to which the movie details will be saved.
 * @param[in] movies A pointer to the array of Movie pointers that will be saved to the file.
 * @param[in] count The number of movies to be saved from the array.
 */

void save_movies_to_file(const char *filename, Movie **movies, int count) 
{
    FILE *file = fopen(filename, "w"); // Open the file for writing
    if (file == NULL) 
    {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < count; ++i) 
    {
        if (movies[i] != NULL) 
        {
            // Save the movie details in the format: title|director|year|rating\n
            fprintf(file, "%s|%s|%d|%.1f\n", movies[i]->title, movies[i]->director, movies[i]->year, movies[i]->rating);
        }
    }

    fclose(file); // Close the file
}


/**
 * @brief Loads movie details from a specified file into an array of Movie structures.
 *
 * Reads movie details from a file with each movie's attributes expected to be separated by
 * a pipe ('|') character. Each line corresponds to one movie. If the array is full, it is
 * resized by calling the `resize_entries` function to double its capacity. The function
 * handles opening the file, reading from it, and closing it after loading the entries.
 *
 * @param[in] filename The name of the file from which the movie details will be read.
 * @param[out] movies A double pointer to the array of Movie pointers where loaded movies
 *                    will be stored.
 * @param[out] count A pointer to an integer that will hold the number of movies loaded.
 * @param[in,out] capacity A pointer to the current capacity of the movies array. It may be
 *                         modified if the array needs to be resized.
 */

void load_movies_from_file(const char *filename, Movie ***movies, int *count, int *capacity) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        perror("Could not open file for reading");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) 
    {
        //Trim '\n' char
        line[strcspn(line, "\n")] = 0;

        //Resize array in needed!
        if (*count == *capacity) 
        {
            *movies = (Movie**)resize_entries((void**)*movies, capacity);
        }

        char *title = strtok(line, "|");
        char *director = strtok(NULL, "|");
        char *year_str = strtok(NULL, "|");
        if (title && director && year_str) 
        {
            int year = atoi(year_str);

            Movie *movie = create_movie(title, director, year);
            if (movie) 
            {
                (*movies)[(*count)++] = movie;
            }
        } 
        else 
        {
            fprintf(stderr, "Error parsing line: %s\n", line);
        }
    }

    fclose(file);
}
