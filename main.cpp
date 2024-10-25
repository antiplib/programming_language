//
// Created by Владислав Отвагин on 24.10.2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <fcntl.h>

#include <map>
#include "keywords.h"

enum TokenType {
    KEYWORD, IDENTIFIER, LITERAL, OPERATOR, PUNCTUATION, POINT, COMMA, BRACKETS, OTHER
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

int main() {
    system("chcp 65001");
    std::ifstream in("code.txt");
    in.seekg (0, std::ios::end);
    int size = 0;
    size = in.tellg();
    in.close();
    // setlocale(LC_ALL, "ru_RU.UTF-8");
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
                if (lexeme == " " &&  lexeme == "") {

                   while (position < size && *(buffer + position) == ' ') {
                       ++position;
                   }
                    lexeme = "";
                    if(position >= size) {
                        break;
                    }
                    lexeme += *(buffer + position);
                    ++position;
                    if(lexeme == "-" || lexeme == "+") {
                        if(position < size) {
                            if(*(buffer + position) == *(buffer + position - 1)) {
                                lexeme += *(buffer + position);
                                tokens.push_back(Token(OPERATOR, lexeme, line));
                            }
                            else {
                                tokens.push_back(Token(OPERATOR, lexeme, line));
                                --position;
                            }
                        }
                    } else if(lexeme == "(" ||lexeme ==")") {
                        tokens.push_back(Token(BRACKETS, lexeme, line));
                    } else if(lexeme == ">" || lexeme == "<" || lexeme == "=" ||
                        lexeme == "!" ) {
                        if(position < size) {
                            if(*(buffer + position) == '=') {
                                lexeme += *(buffer + position);
                                tokens.push_back(Token(OPERATOR, lexeme, line));
                            } else {
                                tokens.push_back(Token(OPERATOR, lexeme, line));
                                --position;
                            }
                        }
                    } else if(lexeme == ";" || lexeme == "{" || lexeme == "}") {
                        tokens.push_back(Token(PUNCTUATION, lexeme, line));
                        --position;
                    } else if(lexeme == ".") {
                        --position;
                        tokens.push_back(Token(POINT, lexeme, line));
                    } else if(lexeme == ",") {
                        tokens.push_back(Token(COMMA, lexeme, line));
                        --position;
                    } else {
                        while(position < size && (isalpha(*(buffer + position)) || *(buffer + position) -'0' >= 0 && *(buffer + position) - '0' <= 9)) {
                            lexeme += *(buffer + position);
                        }
                        tokens.push_back(Token(LITERAL, lexeme, line));
                    }
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

