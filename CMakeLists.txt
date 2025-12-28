cmake_minimum_required(VERSION 3.20)
project(forge)

set(CMAKE_CXX_STANDARD 17)

add_executable(forge
        main.cpp
        directory_parser.cpp
        planner.cpp
        executor.cpp
        types.h
        ForgeFunct.h
        colours.h
        Constants.h
        GeneratorTools.cpp
        GeneratorTools.h
)