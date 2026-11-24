cmake_minimum_required(VERSION 4.3)
project(Dviraciu_parduotuves_uzsakymas)

set(CMAKE_CXX_STANDARD 20)

configure_file(uzsakymas.txt uzsakymas.txt COPYONLY)

add_executable(Dviraciu_parduotuves_uzsakymas main.cpp)
