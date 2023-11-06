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
