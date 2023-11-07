# myMovieRating

## Overview
`myMovieRating` is a C application designed to manage and rate your personal movie collection. It provides a text-based user interface to add, view, rate, and delete movies from your personal database.

## Features
- Add new movies with details such as title, director, year of release, and personal rating.
- View a list of all movies in your collection with an easy-to-navigate interface.
- Rate movies on a 5-point scale.
- Update the ratings for movies as you rewatch them and form new opinions.
- Navigate through the movie collection via command-line interface.
- Delete movies from your collection.
- Data persistence between sessions.

## Installation

### Prerequisites
Before installing `myMovieRating`, ensure you have the following installed:
- A C compiler (e.g., GCC or Clang)
- `make`
- `cmake` (version 3.10 or higher)
- `ncurses` library for the user interface

On Ubuntu, you can install these with the following command:

`bash`
sudo apt-get install gcc make cmake libncurses5-dev libncursesw5-dev

## Development
Check file CMakeLists.txt

### CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(myMovieRating)

# Find necessary packages
find_package(Curses REQUIRED)

# Include directories for header files
include_directories(${CURSES_INCLUDE_DIR})
include_directories(include)

# Add executable and its source files
add_executable(myMovieRating src/main.c src/movie.c src/tv_series.c src/ui.c src/popup.c)

# Link necessary libraries
target_link_libraries(myMovieRating ${CURSES_LIBRARIES})


### Compiling 
To compile myMovieRating, navigate to the root directory of the project and execute the following commands:

``mkdir build && cd build``
``cmake ..``
``make``
This will create a myMovieRating executable within the build directory.

## Usage
`./myMovieRatingApp`

## Contributions
myMovieRating is an open-source project and welcomes contributions. If you have suggestions or improvements, please fork the repository and submit a pull request with your changes.


