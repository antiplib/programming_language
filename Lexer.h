////
//// Created by Владислав Отвагин on 12.11.2024.
////
#pragma once
#ifndef LEXER_H
#define LEXER_H
#include "keywords.h"
//

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
std::vector<Token> tokens;
std::string project;
void create_bor(Bor bor) {
    std::ifstream fin(project + "/keyword.txt");
    std::string s;


    while (getline(fin, s)) {
        bor.add(s);
    }
    fin.close();
}
int  read_file() {
    std::ifstream in("code.txt");
    in.seekg (0, std::ios::end);
    int size_file = in.tellg();
    in.close();
    return size_file;
}
std::vector <Token> lexer() {
    int size = read_file();
    int fd = open("code.txt", O_RDWR);
    char * buffer = (char*) malloc(size);
    read(fd, buffer, size);



    Bor keywords;
    create_bor(keywords);


    int position = 0;
    int line = 1;
    std::cout << size;

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
        tokens.push_back(Token(OTHER, lexeme, line));
        ++i;
    }
    for (auto i : tokens)  {
        std::cout << i << std::endl;
    }
    return tokens;
}
#endif //LEXER_H
