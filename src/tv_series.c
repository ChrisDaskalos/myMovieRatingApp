///FIXME: Make the Implementation
#include "tv_series.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function to create a new TV series
TV_Series* create_tv_series(const char* title, const char* creator, int seasons, int episodes) {
    if (!title || !creator || seasons < 1 || episodes < 1) {
        return NULL; // Check for invalid input as per error codes
    }

    TV_Series* new_series = (TV_Series*)malloc(sizeof(TV_Series));
    if (!new_series) {
        return NULL; // Memory allocation failed
    }

    new_series->title = strdup(title);
    new_series->creator = strdup(creator);
    new_series->seasons = seasons;
    new_series->episodes = episodes;

    return new_series;
}

// Function to update the details of a TV series
TV_SeriesError update_tv_series(TV_Series* series, const char* new_title, const char* new_creator, int new_seasons, int new_episodes) {
    if (!series || !new_title || !new_creator || new_seasons < 1 || new_episodes < 1) {
        return TV_SERIES_ERROR_NULL_POINTER; // Check for invalid input
    }

    free(series->title);
    free(series->creator);

    series->title = strdup(new_title);
    series->creator = strdup(new_creator);
    series->seasons = new_seasons;
    series->episodes = new_episodes;

    return TV_SERIES_SUCCESS; // Successfully updated
}

// Function to display the details of a TV series
void display_tv_series(const TV_Series* series) {
    if (series) {
        printf("Title: %s\n", series->title);
        printf("Creator: %s\n", series->creator);
        printf("Seasons: %d\n", series->seasons);
        printf("Episodes: %d\n", series->episodes);
    }
}

// Function to delete a TV series and free the memory
void delete_tv_series(TV_Series* series) {
    if (series) {
        free(series->title);
        free(series->creator);
        free(series);
    }
}

// Function to search for a TV series by title
TV_Series* search_tv_series(TV_Series* series[], int count, const char* title) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(series[i]->title, title) == 0) {
            return series[i];
        }
    }
    return NULL;
}

// Function to sort an array of TV series alphabetically by title
void sort_tv_series(TV_Series* series[], int count) {
    bool swapped;
    do {
        swapped = false;
        for (int i = 0; i < count - 1; ++i) {
            if (strcmp(series[i]->title, series[i+1]->title) > 0) {
                TV_Series* temp = series[i];
                series[i] = series[i+1];
                series[i+1] = temp;
                swapped = true;
            }
        }
    } while (swapped);
}
