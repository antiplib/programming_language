cmake_minimum_required(VERSION 3.26)
project(programming_language)

set(CMAKE_CXX_STANDARD 17)

add_executable(programming_language main.cpp
        keywords.h
        Lexer.cpp
        Lexer.h
        sem_analyzer.h
        head.h
        parser.h
        TID.h
        Poliz.h
        Interpretator.h
)
