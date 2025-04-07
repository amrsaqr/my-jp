cmake_minimum_required(VERSION 3.29)
project(my_jp)

set(CMAKE_CXX_STANDARD 17)

add_executable(my_jp src/main.cpp
        src/historical_reader.cpp
        src/historical_reader.h
        src/json_parsing_result.h
        src/json_number_parser.cpp
        src/json_number_parser.h
        src/json_string_parser.cpp
        src/json_string_parser.h
        src/json_parser.cpp
        src/json_parser.h
        src/json_constant_parser.cpp
        src/json_constant_parser.h
)
