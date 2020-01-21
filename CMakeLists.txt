cmake_minimum_required (VERSION 3.10)
project (Pirates)
add_executable(Pirates main.cpp)
target_sources(Pirates PUBLIC coordinates.cpp game.cpp map.cpp messenger.cpp options.cpp pirate.cpp ship.cpp)
