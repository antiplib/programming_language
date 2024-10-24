//
// Created by Владислав Отвагин on 24.10.2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <fcntl.h>
#include "keywords.h"

enum TokenType {
    KEYWORD, IDENTIFIER, LITERAL, OPERATOR, PUNCTUATION, POINT,COMMA, BRACKETS, OTHER
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
    std::ifstream fin("keyword.txt");
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
int main() {

    int size = read_file();
    int fd = open("code.txt", O_RDWR);
    char * buffer = (char*) malloc(size);
    read(fd, buffer, size);

    std::vector<Token> tokens;
    Bor keywords;
    create_bor(keywords);


    int position = 0;
    int line = 1;
    while (position < size) {

        if (*(buffer + position) == '\n') {
            line++;

        }
        if (*(buffer + position) == ' ') {
            position++;

        } else {


            std::string lexeme = "";

            while (*(buffer + position) != ' ' && isalpha(*(buffer + position))) {
                lexeme += *(buffer + position);
                position++;
            }





            if (keywords.check(lexeme)) {
                tokens.push_back(Token(KEYWORD, lexeme, line));
            } else {
                if (lexeme != " " &&  lexeme != "") {
                    tokens.push_back(Token(OTHER, lexeme, line));
                }

            }
//            std::string s2 = "";
//            s2 += *(buffer + position);
//            if (*(buffer + position) != ' ' && *(buffer + position) != '\n') {
//                tokens.push_back(Token(OTHER, s2, line));
//            }
//            if (*(buffer + position) == ';' || *(buffer + position) == '{' || *(buffer + position) == '}') {
//                //std::cout << "push" << " " << s2 << " " << line << std::endl;
//                tokens.push_back(Token(PUNCTUATION, s2, line));
//            }


            position++;
        }
    }

    for (const Token& token : tokens) {
        std::cout << token << std::endl;
    }



    return 0;
}
