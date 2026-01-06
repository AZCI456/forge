cmake_minimum_required(VERSION 3.20)
project(forge)

set(CMAKE_CXX_STANDARD 17)

include_directories(include include/UtilityHeaders)

add_executable(forge
        src/main.cpp
        src/directory_parser.cpp
        src/planner.cpp
        src/executor.cpp
        src/utils/GeneratorTools.cpp
        src/utils/InputTools.cpp
        include/UtilityHeaders/InputTools.h
        src/utils/ClipboardTools.cpp
        include/UtilityHeaders/ClipboardTools.h
        src/error_messages.cpp
        src/error_messages.h
)