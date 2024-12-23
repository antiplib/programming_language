//
// Created by Владислав Отвагин on 22.12.2024.
//

#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H
#include "parser.h"
#include <stack>

std::stack <Tree_Tid> functions;
extern std::vector <node>POLIZ;
bool is_ch(std::string &s) {
    bool frack = 0;
    bool minus = 0;
    for(int i =0; i < s.size; ++i) {
        if(s[i] -'0' >= 0 && s[i] -'0' <= 9) {
            continue;
        }
        if(s[i] == '-' && i == 0) {
            minus = 1;
            continue;
        }else
            return 0;
        if(s[i] == '.') {
            if(frack) {
                return 0;
            }
            frack = 1;
            continue;
        }
    }
    return 1;
}

bool is_frack(std::string &s) {
    bool frack = 0;
    bool minus = 0;
    for(int i =0; i < s.size; ++i) {
        if(s[i] -'0' >= 0 && s[i] -'0' <= 9) {
            continue;
        }
        if(s[i] == '-' && i == 0) {
            minus = 1;
            continue;
        }else
            return 0;
        if(s[i] == '.') {
            if(frack) {
                return 0;
            }
            frack = 1;
            continue;
        }
    }
    return frack;
}

double get_double(std::string & s) {
    double ans = 0.0;
    int one = 0;
    int sec = 0;
    bool frack = 0;
    bool minus = 0;
    int f = s.size();
    for(int i = 0; i < s.size(); ++i) {
        if(s[i] == '.') {
            f = i+1;;
            frack = 1;
            break;
        }
        if(s[i] == '-') {
            minus = 1;
            continue;
        }

            one*=10;
            one+=s[i]-'0';
    }
    for(int i = s.size()-1;i >= f; --i) {
        sec/=10;
        sec+=s[i]-'0';
    }
    ans = sec + one;
    return ans;
}

class Interpretator {
    public:
    Interpretator() {
        int curr = 0;
        while(true){
            if(POLIZ[curr].is_operand) {
                oper.push(POLIZ[curr].token);
                ++curr;
                continue;
            }
            else if(POlIZ[curr].token->type == TokenType::OPERATOR) {
                if(POLIZ[curr].token->value == "--" ) {
                } else if(POLIZ[curr].token->value == "++") {
                } else if(POLIZ[curr] == '*') {
                } else if(POLIZ[curr].token->value == "/") {
                } else if(POLIZ[curr].token->value == "%") {
                }else if(POLIZ[curr].token->value == "=") {
                } else if(POLIZ[curr].token->value == "-") {
                } else if(POLIZ[curr].token->value == "+") {
                } else if(POLIZ[curr].token->value == "+=") {
                } else if(POLIZ[curr].token->value == "-=") {
                } else if(POLIZ[curr].token->value == "*=") {
                } else if(POLIZ[curr].token->value == "/=") {
                } else if(POLIZ[curr].token->value == "%=") {
                } else if(POLIZ[curr].token->value == ">=") {
                } else if(POLIZ[curr].token->value == "<=") {
                } else if(POLIZ[curr].token->value == "!=") {
                } else if(POLIZ[curr].token->value == "==") {
                } else if(POLIZ[curr].token->value == ">") {
                } else if(POLIZ[curr].token->value == "<") {
                }


            }
            else if(POLIZ[curr].token->value == "!") {
            } else if(POLIZ[curr].token->value == "!F") {
            } else if (POLIZ[curr] == "int") {
            } else if (POLIZ[curr] == "double") {
            } else if (POLIZ[curr] == "string") {
            } else if (POLIZ[curr] == "return") {
            } else if (POLIZ[curr] == "cin") {
            } else if (POLIZ[curr] == "cout") {
            } else {
            }
        }
    }

private:
    std::stack<Token*> oper;

};


#endif //INTERPRETATOR_H
