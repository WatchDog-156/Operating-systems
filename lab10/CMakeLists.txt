cmake_minimum_required(VERSION 3.10)
project(shered_mem)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Ustawienie flag kompilatora
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# Dodanie pliku wykonywalnego
add_executable(zad1 zad1.c)
add_executable(zad2k zad2k.c)
add_executable(zad2p zad2p.c)

# informacja dla CMake, że wymagane jest linkowanie z biblioteką "rt"
target_link_libraries(zad1 rt)
target_link_libraries(zad2k rt)
target_link_libraries(zad2p rt)