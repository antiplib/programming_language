#include <string>
#include <stack>
#include <iostream>
#include <vector>
#include "parser.h"

extern std::vector<Token> tokens;
extern std::string project;
extern Bor keywords;


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
//

int main(int argc, char* argv[]) {
    get_filename(argc, argv);
    Parser parser(lexer());
    for (auto i : tokens)  {
        std::cout << i << std::endl;
    }
    parser.pars();
    tokens.clear();
    //sem_analyzer a(lexer());
    std::cout <<  "------------------------" << std::endl;
}