//
// Created by Владислав Отвагин on 22.11.2024.
//

#ifndef SEM_ANALYZER_H
#define SEM_ANALYZER_H
#include "Lexer.h"
#include "head.h"
/*
 НУЖНО ЕЩЁ
 1) добавить выражения
 2) rval / lval
 3) сделать список переменных для каждой функции (перегрузка)

*/
extern std::vector<Token> tokens;
extern std::string project;
extern Bor keywords;

struct variable{
  variable(std::string a, std::string b){
        name = a;
        type = b;
  }
    std::string name;
    std::string type;
    bool rval = 0;
};

bool is_tp(std::string s) {
    if(s == "int" || s == "char" || s == "string" || s == "bool" ||
        s == "float" || s == "double" ) {
        return true;
    }
    return false;
}
bool is_func_tp(std::string s) {
    if(s == "int" || s == "char" || s == "string" ||
        s == "bool" || s == "float" || s == "double" ||
        s == "void" ) {
        return true;
    }
    return false;
}

class type_tree{
public:
    type_tree(){};

    void add_global(std::string name, std::string type, int line){
         for(auto e : mp["global"]){
             if(e.name == name){
               throw ("multiply definition in line " + std::to_string(line) + " is not allowed");
             }
         }
        mp["global"].push_back(variable(name, type));
    }
    void add_local(std::string name, std::string type, std::string func, int line){
        for(auto e : mp[func]){
            if(e.name == name){
                throw ("multiply definition in line " + std::to_string(line) + " is not allowed");
            }
        }
      mp[func].push_back(variable(name, type));
    }

    void add_func(std::string name, std::string type, int line){
          if(mp.find(name) != mp.end()) {
              if(is_defined[name] != 0) {
                  throw("redifinition of_function " + name + " in line " + std::to_string(line));
              }

              if(type_of_func[name] != type) {
                  throw("function " + name + " has also another type in line " + std::to_string(line));
              }
          }
        mp[name] = {};
        is_defined[name] = 1;
        type_of_func[name] = type;
    }

    bool is_defin(std::string name) { // определена ли функция
        return is_defined[name];
    }

    void ask(std::string func, std::string name, int line){
        bool ans = 0;
        for(auto e : mp[func]) {
            if(name == e.name) {
                ans = 1;
            }
        }

        if(!ans) {
            for(auto e : mp["global"]) {
                if(name == e.name) {
                    ans = 1;
                }
            }
            if(!ans)
                throw("unknown variable in line " + std::to_string(line));
        }
    }

private:
     std::map<std::string, std::vector <variable>> mp;
     std::map <std::string, std::string> type_of_func;
     std::map<std::string, bool> is_defined;
};

class sem_analyzer {
public:
    sem_analyzer(std::vector <Token> token) {
        this->tokens = token;
        try {
            analyze();
        }
        catch(std::string s) {
            std::cerr << s;
            return;
        }
        std::cout << "there is no error\n";
    }
    void analyze() {
        int curr = 0;
        std::string curr_func = "global";
        while(curr < tokens.size()) {
            if(tokens[curr].type == 1) {
                int helper = 1+curr;
                while(true) {
                    if(tokens[helper].type == BRACKETS) {
                        break;
                    }
                    if(tokens[helper].type == OPERATOR ||
                        tokens[helper].type == PUNCTUATION ||
                        tokens[helper].type == COMMA) {
                        break;
                    }
                    helper++;
                }
                if(tokens[helper].type == PUNCTUATION) {
                    type_tree.add_local(tokens[curr].value, tokens[curr-1].value, curr_func, tokens[helper].line);
                } else {
                    type_tree.add_func(tokens[curr].value, tokens[curr-1].value, tokens[helper].line);
                }
                curr = helper;
            } else if(tokens[curr].type == IDENTIFIER) {

            } else if(tokens[curr].value == "(") {
                curr_func = tokens[curr - 1].value;
                ++curr;
                while(tokens[curr].value != ")") {
                    if(tokens[curr].type == IDENTIFIER) {
                        ++curr;
                        type_tree.add_local(tokens[curr].value, tokens[curr-1].value, curr_func, tokens[curr].line);
                    }
                    ++curr;
                }curr_func = "global";
            } else if(tokens[curr].value == "{") {
                int helper = curr;
                while(tokens[helper].value != "(") {
                    --helper;
                }
                --helper;
                while(tokens[curr].value != "}") {
                    curr_func = tokens[helper].value;
                    if(tokens[curr].type == KEYWORD && is_tp(tokens[curr].value)) {
                        ++curr;
                        type_tree.add_local(tokens[curr].value, tokens[curr-1].value, curr_func, tokens[curr].line);
                        ++curr;
                        if(tokens[curr].value == "(") {
                            throw ("there is declaration of function in function in line " + std::to_string(tokens[curr].line));
                        }
                        --curr;
                    }
                    ++curr;
                }
            }
            ++curr;
        }
    }
private:
    std::vector <Token> tokens;
    type_tree type_tree;
};



#endif //SEM_ANALYZER_H
