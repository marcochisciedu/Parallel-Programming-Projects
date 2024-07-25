cmake_minimum_required(VERSION 3.22)
project(ProgettoEsame)

set(CMAKE_CXX_STANDARD 17)

# in Linux just use -fopenmp, without -lomp and -Xpreprocessor
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp ")

set(SOURCE_FILES main.cpp)
add_executable(ProgettoEsame ${SOURCE_FILES})
