//
// Created by Владислав Отвагин on 24.10.2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

#include <fcntl.h>

#include <map>


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
    setlocale(LC_ALL, "ru_RU.UTF-8");
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

