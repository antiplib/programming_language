
#include "syntaxer_funcs.h"
#include "Lexer.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <fcntl.h>
#include <map>

class  Parser {
public:
    Parser(std::vector<Token> tokens) :  iter(0) {}

    void pars() {
        programma();
    }

private:
    int iter = 0;
    int count = 0;
    std::vector<Token> lexems;
    void programma() {
        while (count  < lexems.size()) {
            if (lexems[count].value== "int" || lexems[count].value == "string" || lexems[count].value == "char" || lexems[count].value == "bool" || lexems[count].value == "void") {
                ++count;
                //id();
                if (lexems[count].value == "(") {
                    //iter -= 2;
                    //function();
                } else {

                }

            }
        }
    }
    void id() {
        if (lexems[count].type == 2) {
            count++;
        } else {
            //error;
        }
    }


};


int main() {
    Parser parser(lexer());

}