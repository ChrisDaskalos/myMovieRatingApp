#ifndef TV_SERIES_H
#define TV_SERIES_H

#include <stdlib.h>

typedef struct 
{
    char *title;
    char *creator;
    int seasons; // Total number of seasons
    int episodes; // Total number of episodes
    // The rating and reviews are assumed to be handled externally
} TV_Series;

// Error codes similar to MovieError
typedef enum 
{
    TV_SERIES_SUCCESS,
    TV_SERIES_ERROR_NULL_POINTER,
    TV_SERIES_ERROR_MEMORY_ALLOCATION,
} TV_SeriesError;

// Function Prototypes
TV_Series* create_tv_series(const char *title, const char *creator, int seasons, int episodes);
TV_SeriesError update_tv_series(TV_Series *series, const char *new_title, const char *new_creator, int new_seasons, int new_episodes);
void display_tv_series(const TV_Series *series);
void delete_tv_series(TV_Series *series); // Just deletes the TV series, reviews are handled separately

#endif //TV_SERIES_H
