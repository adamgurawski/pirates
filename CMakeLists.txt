cmake_minimum_required (VERSION 3.15)
project (Pirates)
add_executable(Pirates main.cpp)
target_sources(Pirates PUBLIC game.cpp map.cpp options.cpp pirate.cpp ship.cpp)
