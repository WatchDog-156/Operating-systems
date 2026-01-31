cmake_minimum_required(VERSION 3.10)
project(shered_mem)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Ustawienie flag kompilatora
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

# Dodanie pliku wykonywalnego
add_executable(producent producent.c)
add_executable(konsument konsument.c)
add_executable(producent_v2 producent_v2.c)
add_executable(konsument_v2 konsument_v2.c)

# informacja dla CMake, że wymagane jest linkowanie z biblioteką "rt"
target_link_libraries(producent rt)
target_link_libraries(konsument rt)
target_link_libraries(producent_v2 rt)
target_link_libraries(konsument_v2 rt)