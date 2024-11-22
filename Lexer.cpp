#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <fcntl.h>

#include <map>

#include "head.h"

 std::vector<Token> tokens;
 std::string project;
 Bor keywords;


extern void create_bor(Bor bor) {
    std::ifstream fin(project + "/keyword.txt");
    std::string s;


    while (getline(fin, s)) {
        if(s[s.size() - 1]=='\r')
            s.erase(s.end() - 1);

        bor.add(s);
    }
    fin.close();
}
extern int  read_file() {
    std::ifstream file(project + "/code.txt", std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    buffer[size] = '\0';
    file.close();

}


 std::vector <Token> lexer() {
    // int size = read_file();
    const char*c;
    std::string q = project + "/code.txt";
    for(int i : q) {
        c+=i;
    }
    std::ifstream file(project + "/code.txt", std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    buffer[size] = '\0';
    file.close();

    create_bor(keywords);

    int position = 0;
    int line = 1;

    for(int i = 0; i < size; ) {
        if(buffer[i] == ' ' ) {
            ++i;
            continue;
        }

        if(buffer[i] == '\n'  || buffer[i] == '\r') {
            ++i;
            ++line;
            if(i < size && buffer[i] == '\n' || i == size && buffer[i] == '\r') {
                ++i;
            }
            continue;
        }
        std::string lexeme = "";
        while(i < size && isalpha(buffer[i])) {
            lexeme += buffer[i];
            ++i;
        }

        if (keywords.check(lexeme)) {
            tokens.push_back(Token(KEYWORD, lexeme, line));
            continue;
        }
        if (lexeme.size() != 0) {
            tokens.push_back(Token(IDENTIFIER, lexeme, line));
            continue;
        }
        lexeme+=buffer[i];
        ++i;
        if(lexeme.size() == 2) {
            lexeme.erase(lexeme.end() - 1);
        }
        if (lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "%" ||
            lexeme == "=" || lexeme == "<" || lexeme == ">" || lexeme == "!" || lexeme == "&" ) {
            if(i < size && buffer[i] == '=') {
                lexeme += buffer[i];
                tokens.push_back(Token(OPERATOR, lexeme, line));
                ++i;

            } else if((lexeme == "+" || lexeme == "-" || lexeme == ">" || lexeme == "<")&&(i < size && buffer[i] == lexeme[0])) {
                lexeme += buffer[i];
                tokens.push_back(Token(OPERATOR, lexeme, line));
                ++i;

            } else {
                tokens.push_back(Token(OPERATOR, lexeme, line));
            }
            continue;
        }
        if(lexeme[0] == '"') {
            lexeme = "";
            while(i < size && buffer[i] != '"') {
                lexeme += buffer[i];
                ++i;
            }
            tokens.push_back(Token(LITERAL, lexeme, line));
            ++i;
            continue;
        }
        if(lexeme == "(" || lexeme == ")") {
            tokens.push_back(Token(BRACKETS, lexeme, line));
            continue;
        }
        if(lexeme == "{" || lexeme == "}" || lexeme == ";") {
            tokens.push_back(Token(PUNCTUATION, lexeme, line));
            continue;
        }
        if(lexeme == ".") {
            tokens.push_back(Token(POINT, lexeme, line));
            continue;
        }
        if(lexeme == ",") {
            tokens.push_back(Token(COMMA, lexeme, line));
            continue;
        }
        if(lexeme == "#") {
            lexeme = "";
            ++i;
            while(i < size && (buffer[i] != '\n' && buffer[i] != '\r')) {
                lexeme += buffer[i];
                ++i;
            }
            continue;
        }
        if(lexeme == "[" || lexeme == "]") {
            tokens.push_back(Token(OPERATOR, lexeme, line));
            continue;
        }
        if(lexeme[0] - '0' >= 0 && lexeme[0] - '0' < 10) {
            bool p = 0;
            while(i < size && ((buffer[i] - '0' >= 0 && buffer[i] - '0' < 10) || (buffer[i] == '.' && !p)) ) {
                if(buffer[i] == '.') {
                    p = 1;
                }
                lexeme += buffer[i];
                ++i;
            }
        }
        tokens.push_back(Token(LITERAL, lexeme, line));
    }

    return tokens;
}