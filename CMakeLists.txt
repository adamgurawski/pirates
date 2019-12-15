cmake_minimum_required (VERSION 2.6)
project (Pirates)
add_executable(Pirates main.cpp)
target_sources(Pirates PUBLIC console.cpp game.cpp map.cpp ship.cpp)
