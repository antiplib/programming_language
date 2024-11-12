//
// Created by Владислав Отвагин on 25.10.2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <fcntl.h>

#include <map>
#include "keywords.h"

std::string project;
enum TokenType {
    KEYWORD, IDENTIFIER, LITERAL, OPERATOR, PUNCTUATION, POINT, COMMA, BRACKETS, OTHER, COMMENT
};

struct Token {
    TokenType type;
    std::string value;
    int line;

    Token(TokenType type, const std::string& value, int line) :
            type(type), value(value), line(line) {}

    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "<" << token.type + 1 << ", " << token.value << ", " << token.line << ">";
        return os;
    }
};

void create_bor(Bor bor) {
    std::ifstream fin(project + "/keyword.txt");
    std::string s;


    while (getline(fin, s)) {
        if(s[s.size() - 1]=='\r')
            s.erase(s.end() - 1);

        bor.add(s);
    }
    fin.close();
}
int  read_file() {
    std::ifstream in(project + "/code.txt");
    in.seekg (0, std::ios::end);
    int size_file = in.tellg();
    in.close();
    return size_file;
}



void lexer(){
    int size = read_file();
    std::string sss = ( project + "/code.txt");
    char* c = new char[sss.size()];
    for(int i = 0; i < sss.size(); ++i) {
        c[i] = sss[i];
    }
    int fd = open(c, O_RDWR);
    char * buffer = (char*) malloc(size);
    read(fd, buffer, size);

    std::vector<Token> tokens;
    Bor keywords;
    create_bor(keywords);


    int position = 0;
    int line = 1;
    for(int i = 0; i < size; ) {
        if(buffer[i] == ' ' || buffer[i] == '\r') {
            ++i;
            continue;
        }

        if(buffer[i] == '\n' ) {
            ++i;
            ++line;
            continue;
        }
        std::string lexeme = "";
        while(i < size && isalpha(buffer[i])) {
            lexeme += buffer[i];
            ++i;
        }

        if(keywords.check(lexeme)) {
            tokens.push_back(Token(KEYWORD, lexeme, line));
            continue;
        }
        if(lexeme.size() != 0) {
            tokens.push_back(Token(IDENTIFIER, lexeme, line));
            continue;
        }
        lexeme+=buffer[i];
        ++i;
        if(lexeme == "+" || lexeme == "-" || lexeme == "*" || lexeme == "/" || lexeme == "%" ||
            lexeme == "=" || lexeme == "<" || lexeme == ">" || lexeme == "!"  ) {
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
        tokens.push_back(Token(OTHER, lexeme, line));
        ++i;
    }

    for (const Token& token : tokens) {
        std::cout << token << std::endl;
    }
}