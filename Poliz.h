//
// Created by Владислав Отвагин on 14.12.2024.
//

#ifndef POLIZ_H
#define POLIZ_H
#include "TID.h"
#include <stack>
extern std::vector<Token> tokens;
extern std::string project;
extern Bor keywords;

struct node {
    node(Token tok, bool operand,  bool is_function = 0)  {
        token = new Token(tok);
        operand = operand;
        is_func = is_function;
    }
    bool is_operand = false;
    Token *token;
    int ind = 0;
    bool is_func;

};

inline bool need(Token token) {
    return token.type == TokenType::LITERAL ||
        token.type == TokenType::OPERATOR ||
            token.type == TokenType::IDENTIFIER ||
                token.type == TokenType::BRACKETS ||
                    token.type == TokenType::COMMA;

}

inline bool is_operand(Token token) {
    bool ans = 0;
    ans = token.type == TokenType::LITERAL || token.type == TokenType::IDENTIFIER;
    return ans;
}
inline bool is_operator(Token token) {
    bool ans = 0;
    ans = token.type == TokenType::OPERATOR;
    return ans;
}

int get_prior(const std::string& op) {
    static const std::map<std::string, int> precedenceMap = {
        { "+",  3 },
        { "-",  3 },
        { "*",  2 },
        { "/",  2 },
        { "%",  2 },
        { "^", 1},   // Возведение в степень
        { "++", 1}, // Инкремент
        { "--", 1}, // Декремент
        { "(", 0 }, // Скобки, по сути, не являются операциями, но это нужно для ПОЛИЗ
        { ")", 0 }, // Скобки
        {"=", 5}    // Присваивание. Можно добавить логические, побитовые операторы.
    };
    auto it = precedenceMap.find(op);

    if (it != precedenceMap.end()) {
        return it->second;
    } else {
        return -1;
    }
}

 std::vector<node> POLIZ;
 void push_poliz(Token token, bool is_function = 0);
void push_poliz(std::string s) {
    Token t(OPERATOR, s, -1);
    push_poliz(t);
}
 void push_poliz(Token token, bool is_function ) {
    if(!need(token)) {
        return;
    }
    bool frst = is_operand(token);
    bool scnd = is_operator(token);
        if(frst)
            POLIZ.push_back(node(token,1, is_function));
        else {
            POLIZ.push_back(node(token,0, is_function));
        }
}


#endif //POLIZ_H
