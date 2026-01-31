cmake_minimum_required(VERSION 3.10)
project(program)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Ustawienie flag kompilatora
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# Dodanie pliku wykonywalnego
add_executable(program zadanie8.c)