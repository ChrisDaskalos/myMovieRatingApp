/**
 * @file ui.c
 * @brief User Interface (UI) implementation for managing movies and TV series.
 *
 * This file contains the implementation of the user interface for a multimedia
 * application that allows users to manage their movie and TV series collections.
 * It uses the ncurses library to create a terminal-based graphical user interface.
 * The implementation includes functions for displaying a main menu, handling user
 * interactions, displaying lists of movies and TV series, and editing movie details.
 * This UI module works closely with the movie data structures and provides a
 * user-friendly environment for data input and navigation.
 *
 * The menu options are handled via a switch statement that responds to user
 * keyboard input, providing functionalities like adding new movies, displaying
 * movie lists, and exiting the application. The user can navigate through the
 * list of movies, rate them, delete them, or quit the movie display. Error
 * messages and input prompts are displayed in a separate window.
 *
 * Functions in this file include:
 * - `main_menu()`: Presents the main menu and captures user selection.
 * - `print_menu()`: Prints the menu options with navigation highlights.
 * - `print_to_left()`: Outputs strings to a window, aligned to the left.
 * - `display_movie_list_ui()`: Displays the list of movies and handles user interaction.
 * - `ui_print_error()`: Displays error messages to the user.
 * - `edit_movie_ui()`: Interface to edit the details of a movie entry.
 *
 * @note This file is part of the UI module and should be included with `ui.h`.
 *       The functions assume that the ncurses library has been initialized
 *       correctly before being called.
 */

#include "ui.h"
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include "movie.h"

/*FUNCTION PROTOTYPES*/
void print_menu(WINDOW *menu_win, int highlight);
void print_to_left(WINDOW *win, int starty, const char *string, chtype color);

//MENU CHOICES
const char *choices[] = 
{
    "1) ADD MOVIE",
    "2) DISPLAY MOVIES",
    "3) ADD TV SERIES",
    "4) DISPLAY TV SERIES",
    "5) EXIT",
    (char *)NULL
};

const int width = 30;  
const int height = 12;


/**
 * @enum MenuOption
 * @brief Enumerates the menu options.
 * 
 * This enum should be declared in the corresponding header file and should list
 * all the options available in the main menu.
 * Example:
 * enum MenuOption { OPTION_ONE, OPTION_TWO, OPTION_THREE, OPTION_FOUR, OPTION_FIVE };
 */

/**
 * @fn MenuOption main_menu()
 * @brief Creates a menu window and handles the keyboard interactions to navigate through the menu options.
 *
 * Initializes an ncurses window to display the menu, captures the user's keystrokes to navigate
 * through the options, and returns the selected option as a `MenuOption` enum value. It uses
 * the arrow keys for navigation and the Enter key for selection.
 *
 * @return The selected menu option as a `MenuOption` enum value.
 *
 * @pre The ncurses library should be initialized with `initscr()` and the color pairs should
 *      be defined with `init_pair()`.
 * @post The function returns the user's selection from the main menu and the ncurses window
 *       is closed with `endwin()`.
 * 
 * @note The function assumes that `width` and `height` are previously defined with the dimensions
 *       of the menu window and that `print_menu()` is a function that handles the actual printing of the
 *       menu options based on the highlighted option.
 * 
 * @warning If the ncurses library is not properly initialized, or if the color pairs are not
 *          set up correctly, this function may result in undefined behavior or crashes.
 */


MenuOption main_menu() 
{
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    initscr();
    clear();
    noecho();
    cbreak();
    start_color();

    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    int startx = (60 - width) / 2;
    int starty = (20 - height) / 2 - 2;  // Further adjusted starting position

    menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);

    mvprintw(0,4, "ARROW KEYS TO NAVIGATE, ENTER TO SELECT A CHOICE");
    refresh();
    print_menu(menu_win, highlight);

    while (1) 
    {
        
        c = wgetch(menu_win);
        switch (c) 
        {
            case KEY_UP:
                if (highlight == 1)
                    highlight = 5;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 5)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10:  // Enter key
                choice = highlight;
                break;
            default:
                mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
                refresh();
                break;
        }
        print_menu(menu_win, highlight);
        if (choice != 0)
            break;
    }
    clrtoeol();
    refresh();
    endwin();
    return (MenuOption)(choice - 1);
}

/**
 * @fn void print_menu(WINDOW *menu_win, int highlight)
 * @brief Prints a menu with selectable options in an ncurses window, highlighting the selected option.
 * 
 * This function displays a menu of choices within a given ncurses `WINDOW`. Each menu
 * item is printed starting from the second line of the window. If an item's index
 * (1-based) matches the `highlight` parameter, that item is highlighted using an 
 * inverted color scheme to indicate selection.
 * 
 * @param menu_win Pointer to an ncurses `WINDOW` where the menu is to be printed.
 * @param highlight The 1-based index of the menu item to be highlighted.
 * 
 * @pre The ncurses library should be initialized, `menu_win` should be a valid window, and
 *      `choices` should be an array of strings containing the menu options.
 * @post The menu items from the `choices` array are printed in `menu_win`, with the
 *       item corresponding to `highlight` displayed with a reverse color effect.
 * 
 * @note This function assumes that `choices` is a global or otherwise accessible array
 *       of strings that contains the menu items to be displayed. It also assumes that
 *       the color pairs are initialized beforehand using the ncurses `init_pair` function.
 * 
 * @warning If `menu_win` is not a valid pointer to an ncurses window, if the `choices` array is
 *          not properly initialized, or if the ncurses library is not properly initialized,
 *          this function may cause undefined behavior or program crashes.
 */

void print_menu(WINDOW *menu_win, int highlight) 
{
    int x = 2, y = 2;

    box(menu_win, 0, 0);
    for (int i = 0; i < 5; ++i) 
    {
        if (highlight == i + 1) 
        {
            wattron(menu_win, A_REVERSE | COLOR_PAIR(2));
            print_to_left(menu_win, y, choices[i], COLOR_PAIR(2));
            wattroff(menu_win, A_REVERSE | COLOR_PAIR(2));
        } 
        else
            print_to_left(menu_win, y, choices[i], COLOR_PAIR(1));
        y += 2;
    }
    wrefresh(menu_win);
}


/**
 * @fn void print_to_left(WINDOW *win, int starty, const char *string, chtype color)
 * @brief Prints a string to the left inside an ncurses window with specified color.
 * 
 * This function places a given string at a specified vertical position (`starty`)
 * and a fixed horizontal margin (`x`) within the provided ncurses `WINDOW`.
 * The string is printed with the color pair attribute specified by `color`.
 * 
 * @param win Pointer to an ncurses `WINDOW` where the string is to be printed.
 * @param starty The y (vertical) position in the window where the string will start.
 * @param string The constant character string to be printed inside the window.
 * @param color The color pair attribute to be used when printing the string.
 * 
 * @pre The ncurses library should be initialized and the `win` should be a valid window.
 * @post The specified string is printed on the `win` at the `starty` row and 2 columns from the left edge,
 *       with the provided `color` attribute.
 * 
 * @note The horizontal position `x` is fixed at 2 columns from the left edge of the window.
 *       The `color` attribute should be initialized using ncurses functions before calling this function.
 *       The window is refreshed to update the display after printing the string.
 * 
 * @warning If `win` is not a valid pointer to an ncurses window, or if the ncurses library is not properly
 *          initialized, this function may cause undefined behavior or program crashes.
 */

void print_to_left(WINDOW *win, int starty, const char *string, chtype color) 
{ 
    int x = 2;  // Starting position on the left side of the box
    wattron(win, color);
    mvwprintw(win, starty, x, "%s", string);
    wattroff(win, color);
    refresh();
}


/**
 * @fn void display_movie_list_ui(Movie **movies, int count)
 * @brief Displays the movie list in a paginated window using ncurses.
 * 
 * Creates an ncurses window to display a list of movies with pagination support.
 * It shows a limited number of movies at a time and allows the user to scroll through
 * the list with up and down arrow keys. Additional functionalities such as rating
 * a movie or deleting a movie can be invoked with key presses. The UI loop continues
 * until 'q' is pressed to quit.
 * 
 * @param movies A pointer to an array of pointers to Movie structures that hold movie details.
 * @param count The total number of movies in the movies array.
 * 
 * @pre The ncurses library should not be active as the function initializes it.
 * @post Upon exit (when 'q' is pressed), the ncurses library is terminated and the window is cleaned up.
 * 
 * @note The function is designed to handle KEY_UP and KEY_DOWN for navigation,
 *       'r' for rating a movie, 'd' for deleting a movie, and 'q' to quit the window.
 *       If 'r' or 'd' is pressed, the function calls `rate_movie()` or `handle_deletion()`,
 *       which are assumed to be implemented elsewhere.
 *       The list can be navigated only if there are movies to display.
 * 
 * @warning The function assumes that the movies array and count accurately represent the list
 *          of movies. Any discrepancy can lead to undefined behavior or crashes.
 *          The function also assumes that each Movie pointer in the array is valid and not NULL.
 *          If a NULL pointer is encountered, it is skipped.
 */

void display_movie_list_ui(Movie **movies, int count) 
{
    if (movies == NULL) return; // Check for NULL pointer

    WINDOW *movies_win;
    int ch, width = 70;
    int current_start = 0;
    int current_highlight = 0;
    const int display_count = 5;

    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    movies_win = newwin(display_count + 4, width, 1, (COLS - width) / 2);
    keypad(movies_win, TRUE);
    box(movies_win, 0, 0);
    mvwprintw(movies_win, 0, width / 2 - 7, " MOVIE LIST ");

    wattron(movies_win, A_BOLD);
    wattron(movies_win, COLOR_PAIR(3));
    mvwprintw(movies_win, 1, 1, " No  | Title           | Director       | Year | Rating ");
    wattroff(movies_win, COLOR_PAIR(3));
    wattroff(movies_win, A_BOLD);

    while (1) 
    {
        wclear(movies_win);
        box(movies_win, 0, 0);
        mvwprintw(movies_win, 0, width / 2 - 7, " MOVIE LIST ");
        wattron(movies_win, A_BOLD);
        wattron(movies_win, COLOR_PAIR(3));
        mvwprintw(movies_win, 1, 1, " No  | Title           | Director     | Year - Rating |");
        wattroff(movies_win, COLOR_PAIR(3));
        wattroff(movies_win, A_BOLD);

        for (int i = 0; i < display_count && (i + current_start) < count; i++) 
        {
            if (i == current_highlight) wattron(movies_win, A_REVERSE);

            Movie *current_movie = movies[i + current_start]; // Added to improve readability
            if (current_movie == NULL) continue; // Check for NULL pointer

            wattron(movies_win, COLOR_PAIR(1));
            mvwprintw(movies_win, i + 2, 1,
                      "%4d |%-15.15s |%-15.15s |%4d - %.1f/5",
                      i + 1 + current_start,
                      current_movie->title,
                      current_movie->director,
                      current_movie->year,
                      current_movie->rating);
            wattroff(movies_win, COLOR_PAIR(1));

            if (i == current_highlight) wattroff(movies_win, A_REVERSE);
        }

        mvwprintw(movies_win, display_count + 3, 6, "Arrow Keys: Navigation,'r':Rate,'d':Delete,'q':Quit.");
        wrefresh(movies_win);

        ch = wgetch(movies_win);

        switch (ch) 
        {
            case KEY_UP:
                if (current_highlight > 0) 
                {
                    current_highlight--;
                } 
                else if (current_start > 0) 
                {
                    current_start--;
                }
                break;
            case KEY_DOWN:
                if (current_highlight < display_count - 1 && (current_highlight + current_start) < count - 1) 
                {
                    current_highlight++;
                } 
                else if ((current_start + display_count) < count) 
                {
                    current_start++;
                }
                break;
            case 'r':
                if (count > 0 && (current_highlight + current_start) < count) 
                { // Added boundary check
                    rate_movie(movies[current_highlight + current_start]);
                }
                break;
            case 'd':
                if (count > 0 && current_highlight < count) 
                {
                    handle_deletion(&movies, &count, current_highlight + current_start);
                    if (current_highlight >= count) 
                    {
                        current_highlight = count - 1;
                    }
                    if (current_highlight < current_start) 
                    {
                        current_start = current_highlight;
                    }
                    if (count == 0) 
                    {
                        current_highlight = 0;
                        current_start = 0;
                    }
                } 
                else 
                {
                    show_popup("WARNING", "No movies to delete.");
                }
                break;
            case 'q':
                delwin(movies_win);
                endwin();
                return;
        }
    }
}


/**
 * @fn void ui_print_error(const char* format, ...)
 * @brief Displays an error message on the screen using ncurses.
 * 
 * This function initializes the ncurses library if not already initialized, prints an error message
 * formatted similarly to printf, and then waits for any key press before closing the ncurses window.
 * 
 * @param format A format string as in printf that contains the error message to be displayed.
 * @param ... Variable arguments providing additional information (if necessary) to be included in the format string.
 * 
 * @pre The ncurses library must not be initialized before calling this function, as it calls initscr().
 * @post After the key press, ncurses is ended and the program will return to its normal console mode.
 * 
 * @note This function is blocking as it waits for a key press before it returns.
 * 
 * @warning The function assumes that the format and the arguments passed to it are valid and that the format contains no more than the expected number of placeholders.
 */

void ui_print_error(const char* format, ...) 
{
    va_list args;
    initscr();
    va_start(args, format);
    vw_printw(stdscr, format, args);
    va_end(args);
    getch();
    endwin();
}


/**
 * @file ui.c
 * @brief UI functions for the movie management system using ncurses library.
 */

/**
 * @fn void edit_movie_ui(Movie** movies, int count)
 * @brief Provides an interactive user interface to navigate through the list of movies and edit a selected one.
 * 
 * This function creates a new window where all the movies from the list are displayed. The user
 * can navigate through the list using the up and down arrow keys. Upon selecting a movie by pressing the
 * Enter key, the movie can be edited (functionality to be implemented).
 * 
 * @param movies A pointer to an array of pointers to Movie structures.
 * @param count The number of movies in the array.
 * 
 * @pre The ncurses library must be initialized using initscr() before calling this function.
 * @post If a movie is selected and the Enter key is pressed, the movie should be editable (not implemented in the provided code).
 * 
 * @note The function assumes the ncurses library is already initialized and will end the window session upon 'q' key press or after editing.
 * @note The function handles keyboard input within the ncurses window and uses the defined COLOR_PAIR(1) for display.
 * @note The current code does not handle the actual editing of the movie, it should be implemented where indicated.
 * @note Pressing 'q' will quit the edit menu and return to the previous screen.
 * 
 * @warning This function will not check for the validity of the pointers passed, it assumes they are valid and not NULL.
 */

void edit_movie_ui(Movie** movies, int count) 
{
    int ch;
    int current_highlight = 0;
    WINDOW *movie_win;

    // Initialization routines for ncurses
    initscr();
    clear();
    noecho();
    cbreak();  
    start_color();  
    keypad(stdscr, TRUE);  // We get F1, F2, etc...
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    // Create a new window for the movie list
    movie_win = newwin(10, 50, 0, 0);  
    keypad(movie_win, TRUE);  // Enable keyboard input for the window

    while (1) 
    {
        for (int i = 0; i < count; i++) 
        {
            if (i == current_highlight) wattron(movie_win, A_REVERSE);  // Highlight the selected movie
            mvwprintw(movie_win, i + 1, 1, "%s - %s (%d)", movies[i]->title, movies[i]->director, movies[i]->year);
            if (i == current_highlight) wattroff(movie_win, A_REVERSE);
        }

        // Update the window
        wrefresh(movie_win);  
        ch = wgetch(movie_win);  
        switch (ch) 
        {
            case KEY_UP:
                if (current_highlight > 0) 
                {
                    current_highlight--;
                }
                break;
            case KEY_DOWN:
                if (current_highlight < count - 1) 
                {
                    current_highlight++;
                }
                break;
            case 10:  // Enter key pressed
                werase(movie_win);  // Clear the window
                wrefresh(movie_win);  // Update the window
                // Insert code to call your update function here
                // update_movie(movies[current_highlight]);
                // Collect new details from user input and update the movie details
                break;
            case 'q':  // 'q' pressed, quit the loop
                endwin();
                return;
        }
    }
}
