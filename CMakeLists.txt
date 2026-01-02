cmake_minimum_required(VERSION 3.20)
project(forge)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(forge
        src/main.cpp
        src/directory_parser.cpp
        src/planner.cpp
        src/executor.cpp
        src/utils/GeneratorTools.cpp
        src/utils/InputTools.cpp
        include/InputTools.h
)