cmake_minimum_required(VERSION 3.26)
project(programming_language)

set(CMAKE_CXX_STANDARD 17)

add_executable(programming_language main.cpp
        keywords.h
        Lexer.cpp
        syntaxer_funcs.h
        Lexer.h
        sem_analyzer.h
        sem_analyzer.cpp
        head.h
        parser.h
        parser.cpp
        TID.h
        TID.cpp
)
