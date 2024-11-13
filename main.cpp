
#include "syntaxer_funcs.h"
#include <string>

#include <iostream>
#include <fstream>
#include <vector>

#include <fcntl.h>
#include <map>
#include <filesystem>

class  Parser {
public:
    Parser(std::vector<Token> tokens) :  iter(0) {}

    void pars() {
        programma();
    }
//hhfjhf
private:
    int iter = 0;
    int count = 0;
    std::vector<Token> lexems;
    void programma() {
        while (count  < lexems.size()) {
            if (lexems[count].value== "int" || lexems[count].value == "string" || lexems[count].value == "char" || lexems[count].value == "bool" || lexems[count].value == "void") {
                ++count;
                id();
                if (lexems[count].value == "(") {
                    iter -= 2;
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
    //void
    void error() {
        std::cout << "error";
    }



};

void get_filename(int argc, char* argv[]) {
    std::string s = *argv;
    for(int i = 0; i < 18+21; ++i) {
        s.erase(s.end() -1);
    }
    if(s[s.size() - 1] == 'a') {
        for(int i = 0; i < 4;++i) {
            s.erase(s.end() -1);
        }
    }
    project = std::string(s);
}

int main(int argc, char* argv[]) {
    get_filename(argc, argv);
    lexer();
    syntax_analyzer();

}