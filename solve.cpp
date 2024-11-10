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

void solve(){
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
            if(lexeme[lexeme.size() - 1] == '\r') {
                lexeme.erase(lexeme.size() - 1);
            }
            if (keywords.check(lexeme)) {
                tokens.push_back(Token(KEYWORD, lexeme, line));
            } else {
                if(lexeme[lexeme.size() - 1] == '\r') {
                    lexeme.erase(lexeme.size() - 1);
                }
                    if(lexeme.size() != 0) {
                        tokens.push_back(Token(IDENTIFIER, lexeme, line));
                        continue;
                    }
                   while (position < size && *(buffer + position) == ' ') {
                       ++position;
                   }
                    lexeme = "";
                    if(position >= size) {
                        break;
                    }
                    lexeme += *(buffer + position);
                    ++position;
                    if(lexeme == "-" || lexeme == "+"|| lexeme == "*" || lexeme == "/") {
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
                    }else if(lexeme[0] == '"') {
                        tokens.push_back(Token(PUNCTUATION, lexeme, line));
                        lexeme = "";
                        while(position < size && *(buffer + position) != '"') {
                            lexeme += *(buffer + position);
                            ++position;
                        }
                        tokens.push_back(Token(LITERAL, lexeme, line));
                        if(position < size && *(buffer + position) == '"') {
                            lexeme = '"';
                        }
                        tokens.push_back(Token(PUNCTUATION, lexeme, line));

                    }else if(lexeme == "#") {
                        tokens.push_back(Token(PUNCTUATION, lexeme, line));
                        lexeme = "";
                        while(position < size && *(buffer + position) != '\n') {
                            lexeme += *(buffer + position);
                            ++position;
                        }
                        if(lexeme[lexeme.size() - 1] == '\r') {
                            lexeme.erase(lexeme.size() - 1);
                        }
                        tokens.push_back(Token(COMMENT, lexeme, line));
                    }
                else if(lexeme == "(" || lexeme ==")") {
                        tokens.push_back(Token(BRACKETS, lexeme, line));
                        --position;
                    } else if(lexeme == ">" || lexeme == "<" || lexeme == "=" ||
                        lexeme == "!"|| lexeme == "%" ) {
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
                        if(lexeme[lexeme.size() - 1] == '\r') {
                            lexeme.erase(lexeme.size() - 1);
                        }
                        tokens.push_back(Token(POINT, lexeme, line));
                    } else if(lexeme == ",") {
                        tokens.push_back(Token(COMMA, lexeme, line));
                        if(lexeme[lexeme.size() - 1] == '\r') {
                            lexeme.erase(lexeme.size() - 1);
                        }
                        --position;
                    } else if(*(buffer + position) != '\n'){
                        while(position < size && *(buffer + position) != '\n'  && isalpha(*(buffer + position))) {
                            lexeme += *(buffer + position);
                            ++position;
                        }
                        if(lexeme.size() != 0 && ( lexeme[0] -'0' >= 0 && lexeme[0]- '0' <= 9)) {
                            bool p = 0;
                            while( position < size && ((*(buffer + position) -'0' >= 0 && *(buffer + position)- '0' <= 9) ||( *(buffer + position) == '.') && !p)) {
                                lexeme += *(buffer + position);
                                if(*(buffer + position) == '.') {
                                    p = 1;
                                }
                                ++position;
                            }
                            --position;
                            if(lexeme[lexeme.size() - 1] == '\r') {
                                lexeme.erase(lexeme.size() - 1);
                            }
                        }
                        // if(position < size && *(buffer + position) == '')
                        if(lexeme[lexeme.size() - 1] == '\r') {
                            lexeme.erase(lexeme.size() - 1);
                        }
                        if(lexeme.size() == 0) {
                            continue;
                        }
                        tokens.push_back(Token(LITERAL, lexeme, line));
                    } else {
                        line++;
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

            lexeme = "";
            position++;
        }
    }

    for (const Token& token : tokens) {
        std::cout << token << std::endl;
    }
}