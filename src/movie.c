/**
 * @file movie.c
 * @brief Implementation of Movie management functions.
 *
 * This source file contains the implementation of functions declared in movie.h.
 * It provides detailed logic for creating, updating, displaying, rating, and
 * deleting movie records. It includes robust error checking and user interaction
 * via console and pop-up dialogs using the ncurses library. The functions
 * manage dynamic memory allocation and handle input/output operations for
 * movie data, ensuring a seamless user experience in movie management tasks.
 * Error handling is consistently implemented, with informative pop-up messages
 * for the user in cases of incorrect or invalid operations.
 */
/*LIBRARY INCLUSIONS*/
#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include "movie.h"
#include "popup.h"


/**
 * @function create_movie
 * @brief Creates a new movie record.
 *
 * This function dynamically allocates memory for a new movie structure and
 * initializes it with the provided title, director, and year. It performs
 * necessary validations to ensure that none of the input parameters are NULL
 * and that the year is a reasonable value (greater than 1800). In case of
 * successful creation, a pointer to the new movie structure is returned.
 *
 * Memory allocation is performed for the movie structure itself and the strings
 * for the title and director. If any memory allocation fails, the function
 * cleans up any allocated memory before returning NULL to prevent leaks.
 *
 * @param title Pointer to a string representing the movie's title.
 * @param director Pointer to a string representing the movie's director.
 * @param year Integer representing the year the movie was released.
 * @return Movie* A pointer to the newly created movie structure, or NULL if
 *         an error occurred during creation.
 */

Movie* create_movie(const char* title, const char* director, int year) 
{
    if (!title || !director || year <= 1800) 
    {
        return NULL; // Updated check to be consistent with main function
    }

    Movie* new_movie = (Movie*)malloc(sizeof(Movie));
    if (!new_movie) 
    {
        show_popup("Warning", "New Movie Allocation Failed.\n");
        return NULL;
    }

    new_movie->title = strdup(title);
    if (!new_movie->title) 
    { // Check strdup success for title
        free(new_movie); // Ensure no memory leak
        show_popup("Warning", "Memory Allocation for Title Failed.\n");
        return NULL;
    }

    new_movie->director = strdup(director);
    if (!new_movie->director) { // Check strdup success for director
        free(new_movie->title); // Ensure no memory leak
        free(new_movie); // Ensure no memory leak
        show_popup("Warning", "Memory Allocation for Director Failed.\n");
        return NULL;
    }

    new_movie->year = year;

    return new_movie;
}


///FIXME:IMPLEMENT THIS!
MovieError update_movie(Movie* movie, const char* new_title, const char* new_director, int new_year) 
{
    if (!movie || !new_title || !new_director || new_year <= 0) 
    {
        return MOVIE_ERROR_NULL_POINTER; // Check for invalid input
    }

    free(movie->title);
    free(movie->director);

    movie->title = strdup(new_title);
    movie->director = strdup(new_director);
    movie->year = new_year;

    return MOVIE_SUCCESS; // Successfully updated
}

///FIXME:IMPLEMENT THIS!
void display_movie(const Movie* movie) 
{
        if (movie) 
        {
            printf("Title: %s,Director: %s,Year: %d\n", movie->title,movie->director,movie->year);
        }
        else
            printf("Attempted to Display a NULL movie.\n");
}

/**
 * @function rate_movie
 * @brief Rates a movie with user input.
 *
 * This function allows the user to input a rating for a given movie. The
 * function checks if the provided movie pointer is valid and if the movie
 * has a title. If the movie is valid, it prompts the user to enter a rating
 * from 1 to 5.
 *
 * The function uses ncurses functions for input and output to the terminal.
 * It enables echoing of characters so that the user's input is visible on
 * the terminal. It then enters a loop, prompting the user for a rating until
 * a valid digit between 1 and 5 is entered. If the input is not valid, a
 * popup warning is shown, and the input is cleared until the end of the line.
 *
 * Once a valid rating is entered, it is stored in the movie's rating field,
 * and echoing of characters is disabled before the function returns.
 *
 * @param movie Pointer to a Movie structure to be rated.
 */


void rate_movie(Movie *movie) 
{
    if (!movie || !movie->title) 
    {
        printw("Invalid movie data.\n");
        return;
    }

    int ch;
    int rating = 0;
    echo(); // Enable echoing of input characters

    while (1) 
    {
        printw("Enter a rating for the movie (%s) from 1 to 5: ", movie->title);
        refresh(); // Refresh the screen to show the output

        ch = getch(); // Get one character from the user
        if (ch >= '1' && ch <= '5') 
        {
            rating = ch - '0';
            movie->rating = rating; 
            break; 
        } 
        else 
        {
            show_popup("Warning", "Invalid rating. Please try again.\n", 42);
            // Clear the rest of the line to avoid invalid character inputs to stay in buffer
            while ((ch = getch()) != '\n' && ch != EOF); 
        }
    }

    // Disable echoing of input characters as we're done with input
    noecho(); 
}


/**
 * @function delete_movie
 * @brief Deletes a movie from a dynamic array of movies.
 *
 * This function handles the deletion of a movie from a dynamic array based on
 * the index provided. It performs a series of checks to ensure the index is within
 * bounds and that the movie at the index has not already been deleted. If these
 * checks pass, it then prompts the user to confirm the deletion.
 *
 * The confirmation is acquired through a popup that asks the user to confirm with
 * 'y' or 'n'. If the user confirms, the function proceeds to free the memory
 * allocated for the movie's title and director, sets the movie pointer at that index
 * to NULL, and shifts all subsequent movie pointers in the array up by one position
 * to fill the gap, effectively reducing the array size by one.
 *
 * The function also handles user input directly via ncurses library functions
 * to fetch the user's choice and provides feedback popups based on the action taken,
 * whether the movie is deleted successfully or if the deletion is canceled.
 *
 * @param movies Triple pointer to an array of pointers to Movie structures.
 * @param count Pointer to the number of movies currently in the array.
 * @param index The index of the movie to be deleted in the array.
 */

void delete_movie(Movie ***movies, int *count, int index) 
{
    if (index < 0 || index >= *count || !(*movies)[index]) 
    {
        show_popup("WARNING", "Invalid index or movie already deleted.\n");
        return;
    }

    // Confirm deletion without needing to enter index, as index is already known
    char *message = "Delete selected movie? (y/n): ";
    show_popup("INFO", message);

    // Get the user's choice directly after the popup
    flushinp();
    int ch = getch();
    if (ch == 'y' || ch == 'Y') 
    {
        // Proceed with deletion
        free((*movies)[index]->title);
        free((*movies)[index]->director);
        free((*movies)[index]); 
        (*movies)[index] = NULL;

        // Shift the remaining elements up
        for (int i = index; i < *count - 1; i++) 
        {
            (*movies)[i] = (*movies)[i + 1];
        }
        (*movies)[*count - 1] = NULL;
        (*count)--;

        show_popup("INFO", "Movie deleted successfully!");
    } 
    else if (ch == 'n' || ch == 'N') 
    {
        show_popup("INFO", "Deletion canceled.");
    } 
    else 
    {
        // If the user input is not 'y/Y' or 'n/N', it's considered invalid
        show_popup("WARNING", "Invalid input. Deletion canceled.");
    }
}

/**
 * @function handle_deletion
 * @brief Handles the deletion process of a selected movie.
 *
 * This function is a higher-level wrapper for the `delete_movie` function. It is
 * responsible for validating the user's selection and initiating the deletion process.
 * It ensures the selected index for deletion is within the valid range of the movies
 * array. If the selection is valid, it calls the `delete_movie` function to perform
 * the actual deletion.
 *
 * Upon completion of the deletion process, the function clears the screen (which may be
 * displaying the movie list or other information) and refreshes it to update the display.
 * This is particularly useful in graphical or text-based user interfaces where the display
 * needs to reflect changes in real-time after an item is deleted.
 *
 * @param movies Triple pointer to an array of pointers to Movie structures.
 * @param count Pointer to the number of movies currently in the array.
 * @param selected_index The index of the movie selected for deletion.
 */

void handle_deletion(Movie ***movies, int *count, int selected_index) 
{
    // Ensure that the selection is valid before attempting to delete
    if (selected_index < 0 || selected_index >= *count) 
    {
        show_popup("WARNING", "No movie is selected or the selected movie is invalid.");
        return;
    }

    delete_movie(movies, count, selected_index); 
    clear();
    refresh(); 
}
