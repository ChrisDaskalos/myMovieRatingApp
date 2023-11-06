#ifndef POPUP_H
#define POPUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

/**
 * @brief Displays a popup with the given title and formatted message.
 * 
 * This function is variadic, meaning it can take a varying number of
 * arguments similar to printf. The format and additional arguments
 * will be used to create the formatted message that the popup will display.
 *
 * @param title The title of the popup.
 * @param format The format string for the popup message (printf-style).
 * @param ... The arguments that fit the format string.
 */
void show_popup(const char* title, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif // POPUP_H
