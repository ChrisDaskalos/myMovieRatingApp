/**
 * @file popup_manager.c
 * @brief A utility module for displaying popup windows using the ncurses library.
 *
 * This file includes the implementation of a generic popup window function for command-line
 * interfaces that use the ncurses library. The popup window is designed to display a message
 * and a title within a bordered window. It provides visual feedback or interaction prompts
 * to the user and halts program execution until a key is pressed.
 *
 * Definitions:
 * - MAX_POPUP_WIDTH: Defines the maximum width of the popup window.
 * - POPUP_MARGIN: Specifies the margin width for text inside the popup.
 *
 * Functions:
 * - show_popup: Displays a popup window with a title and formatted message. This message can
 *   include variable arguments, similar to printf. It calculates the required size of the popup
 *   window, creates it, displays the message with word-wrapping, and waits for the user's input
 *   before cleaning up and returning control to the main program.
 *
 * Usage:
 * This module is intended to be used in text-based user interfaces where modal interaction is
 * necessary. It's suitable for displaying error messages, warnings, confirmations, or information
 * alerts.
 *
 * Example:
 * To display a simple alert with the message "Operation successful", one would call:
 * show_popup("Alert", "Operation successful");
 *
 * To display a formatted message, such as an error message with a variable, one could call:
 * show_popup("Error", "File '%s' not found.", filename);
 *
 * Dependencies:
 * This module depends on the ncurses library for window management and rendering. It assumes that
 * the ncurses library is initialized (initscr() has been called) prior to calling the show_popup
 * function. The ncurses library is a widely used library for text-based user interfaces in Unix,
 * Linux, and other compatible systems.
 *
 * Notes:
 * - The popup window is created at the center of the terminal window.
 * - The maximum width and margins can be adjusted via the MAX_POPUP_WIDTH and POPUP_MARGIN
 *   definitions to accommodate different terminal sizes and aesthetic preferences.
 * - The function handles text wrapping within the popup window if the message exceeds the
 *   calculated width.
 * - After displaying the message, the function waits for any key press before it returns.
 * - Once a key is pressed, the function clears the popup and refreshes the main window to
 *   ensure the interface returns to its original state.
 */

#include <stdarg.h>
#include <string.h>
#include <ncurses.h>

#define MAX_POPUP_WIDTH 60  // Maximum width of the popup window
#define POPUP_MARGIN 3      // Margin for text inside the popup


/**
 * @function show_popup
 * @brief Displays a popup window with a message and optional title.
 *
 * This function creates a modal popup window in the middle of the terminal screen using
 * ncurses functions. It is used to show messages to the user such as errors, warnings,
 * and information. The window will display the message and will wait for the user to
 * press a key before it returns control to the calling function.
 *
 * @param title A constant character pointer to the title of the popup window. If a title is
 * provided, it will be displayed at the top of the popup window. If this parameter is NULL,
 * no title will be shown.
 * @param format A constant character pointer to a format string that will be used to format
 * the message in the popup. This parameter follows the same specification as the format
 * parameter in the standard printf function.
 * @param ... A variable number of arguments that will be formatted into the message string
 * according to the format parameter.
 *
 * @note The function uses `printw`, `wrefresh`, `wgetch`, and other ncurses functions to
 * handle window drawing and input. The ncurses library must be initialized before this
 * function is called. Also, the function assumes that the maximum width of the popup and
 * the margins are defined by MAX_POPUP_WIDTH and POPUP_MARGIN constants.
 *
 * The popup will be bordered, and its size is dynamically calculated based on the length of
 * the message and the defined margins, but it will not exceed MAX_POPUP_WIDTH in width.
 * Word wrapping is implemented to handle messages that are longer than the popup's width.
 *
 * After displaying the message, the function will block and wait for any key press from the
 * user. Upon receiving a key press, it cleans up by deleting the popup window, clearing the
 * screen, and refreshing the main window to return it to its original state.
 *
 * Example Usage:
 * To show a popup with a message "File not found":
 * show_popup("Error", "File not found");
 *
 * To show a popup with a formatted error message for a missing file:
 * show_popup("Error", "The file '%s' could not be opened.", filename);
 */

void show_popup(const char* title, const char* format, ...) 
{
    int rows, cols, starty, startx;
    int length, width;
    WINDOW *popupwin;
    char message[256];
    va_list args;

    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    getmaxyx(stdscr, rows, cols);

    // Calculate width: the lesser of the screen width and MAX_POPUP_WIDTH
    width = (strlen(message) + 2 * POPUP_MARGIN < MAX_POPUP_WIDTH) ? strlen(message) + 2 * POPUP_MARGIN : MAX_POPUP_WIDTH;
    startx = (cols - width) / 2;

    // Determine the number of lines the message will take up & add padding
    int message_lines = (strlen(message) / (width - 2 * POPUP_MARGIN)) + 1;
    length = message_lines + 2 * POPUP_MARGIN;
    starty = (rows - length) / 2;

    popupwin = newwin(length, width, starty, startx);
    keypad(popupwin, TRUE); 
    box(popupwin, 0, 0);

    if (title != NULL) 
    {
        mvwprintw(popupwin, 1, (width - strlen(title)) / 2, "%s", title);
        mvwhline(popupwin, 2, 1, ACS_HLINE, width - 2); // Draw a horizontal line under the title
    }

    int line = (title != NULL) ? 3 : 1; 
    int column = POPUP_MARGIN; 
    const int message_width = width - 2 * POPUP_MARGIN;
    char *current_line = message;
    char *next_line;
    int len;

    while ((next_line = strchr(current_line, ' ')) != NULL || (len = strlen(current_line)) > 0) 
    {
        if (next_line != NULL) 
        {
            len = next_line - current_line;
        }
        if (column + len < message_width) 
        {
            mvwprintw(popupwin, line, column, "%.*s", len, current_line);
            column += len + 1; 
        } 
        else 
        {
            line++;
            column = POPUP_MARGIN;
            mvwprintw(popupwin, line, column, "%.*s ", len, current_line);
            column += len + 1; 
        }
        if (next_line == NULL) break; 
        current_line = next_line + 1; 
    }

    wrefresh(popupwin);
    wgetch(popupwin);
    delwin(popupwin);
    clear();
    refresh();
}
