cmake_minimum_required(VERSION 3.20)
project(algorithms_2_2021)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -Wall -Werror -Wextra -std=c++20")

add_subdirectory(04_08)
add_subdirectory(04_22)
add_subdirectory(05_13)
