#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <fcntl.h>

#include <map>


class Bor {
public:
    void add(const std::string& s) {
        node *cur = root;
        for (auto u : s) {
            if (cur->next[u - 'a'] == nullptr) {
                cur->next[u - 'a'] = new node();
            }
            cur = cur->next[u - 'a'];
        }
        ++cur->strings;
    }
    bool check(const std::string& s) {
        node *cur = root;
        for (auto u: s) {
            if (cur->next[u - 'a'] != nullptr) {
                cur = cur->next[u - 'a'];
            } else return false;
        }
        if (cur->strings > 0) return 1;
        else return 0;
    }
private:
    struct node {
        node *next[26];
        int strings;
        node() {
            for (int i = 0; i < 26; i++) {
                next[i] = nullptr;
            }
            strings = 0;
        }
    };
    node *root = new node();
};

//enum TokenType {
//    KEYWORD, IDENTIFIER, LITERAL, OPERATOR, PUNCTUATION, POINT, COMMA, BRACKETS, OTHER, COMMENT
//};
//
//
//struct Token {
//    TokenType type;
//    std::string value;
//    int line;
//
//    Token(TokenType type, const std::string& value, int line) :
//            type(type), value(value), line(line) {}
//
//    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
//        os << "<" << token.type + 1 << ", " << token.value << ", " << token.line << ">";
//        return os;
//    }
//};

//std::string project;
//std::vector<Token> tokens;
//Bor keywords;