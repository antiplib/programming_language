//
// Created by Владислав Отвагин on 13.12.2024.
//

#ifndef PARSER_H
#define PARSER_H
#include "poliz.h"
#include <iostream>
#include <string>
#include <stack>
#include <iostream>
#include <vector>
extern std::vector<Token> tokens;
extern std::string project;
extern Bor keywords;
extern std::vector<node> POLIZ;

int getPrecedence(const std::string& op) {
    if (op == "++" || op == "--") return 6; //Унарный
    if (op == "*" || op == "/") return 5;
    if (op == "+" || op == "-") return 4;
    if (op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=") return 3;
    if(op == "&") return 2;
    if(op == "|") return 1;
    if (op == "&&") return 0;
    if (op == "||") return 0;
    if (op == "=" || op == "+=" || op == "-=") return 0;
    if (op == "(") return -1;
    return -2; // Для прочих случаев (можно сделать ошибку)
}

class  Parser {
public:
    Parser(std::vector<Token> tokens_) : tokens((tokens_)), curr(0) {}

    void pars() {
        programma();
    }

private:
    Tree_Tid tree;
    struct func {
        func(std::string type_, std::string name_) : type_answer(type_), name(name_) {}
        bool operator==(const func &function) const {
            if (name == function.name && parameters.size() == function.parameters.size()) {
                for (size_t i = 0; i < parameters.size(); ++i) {
                    if (*parameters[i] != *function.parameters[i]) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        bool matches(const func &function) const {
            if (parameters.size() != function.parameters.size()) {
                //std::cout << "size not correct " << parameters.size() << " " << function.parameters.size() << std::endl;
                return false;
            }
            for (size_t i = 0; i < parameters.size(); ++i) {
                if (*parameters[i] != *function.parameters[i]) {
                    //std::cout << "parametr != parametr" << std::endl;
                    return false;
                }
            }
            return true;
        }

        std::string name;
        std::string type_answer;
        std::vector<parametr *> parameters;
    };
    std::vector<func*> functions;
    void push_func(func *function) {
        for (func *f: functions) {
            if (*function == *f) {
                throw std::string (" function has been declared");
            }
        }

        functions.push_back(function);
    }
    std::string check_func(func *function) {
        for (auto f: functions) {
            if (function->matches(*f)) {
                return f->type_answer;
            }
        }
        throw std::string("function call is incorrect");
    }

    int curr = 0;
    bool flag_for_comma = 1;
    std::vector<Token> tokens;

    void check_semicolon() {
        if (tokens[curr].value == ";") {
            curr++;
        } else {
            throw std::string ("error - miss ';' it line  " + std::to_string(tokens[curr].line));
        }
    }

    void declaration_id() {
        //std::cout << "here4" << tokens[curr].value << std::endl;
        std::string type = "";
        if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
            type = tokens[curr].value;
            ++curr;
        }

        if (tokens[curr].type == IDENTIFIER) {
            ++curr;
        } else {
            throw std::string ("error - miss IDENTIFIER it line  " + std::to_string(tokens[curr].line));
        }
        parametr pr(type, tokens[curr - 1].value);
        //std::cout << "NET" << tokens[curr].value << std::endl;
        if (tokens[curr].value == "=") {
            ++curr;
            flag_for_comma = 0;
            std::cout << "NEEEEEE";
            expression();

            flag_for_comma = 1;
        } else if (tokens[curr].value == "[") {
            ++curr;
            expression();
            if (tokens[curr].value == "]") {
                ++curr;
            } else {
                throw std::string ("error - miss ] it line  " + std::to_string(tokens[curr].line));
            };
            if (tokens[curr].value == "=") {
                ++curr;
                if (tokens[curr].value == "{") {
                    ++curr;
                    if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER) {
                        //tree.push_id(pr);
                        //parametr pr(type, tokens[curr].value);
                        ++curr;
                    } else {
                        throw std::string ("error - not type 3 or not type 2 it line  " + std::to_string(tokens[curr].line));
                    }
                    while (tokens[curr].value == ",") {
                        ++curr;
//                        std::string type = "";
//                        if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
//                            type = tokens[curr].value;
//                            ++curr;
//                        }
                        if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER) {
                            //parametr pr(type, tokens[curr].value);
                            ++curr;
                        } else {
                            throw std::string ("error - not type 3 or not type 2 it line  " + std::to_string(tokens[curr].line));
                        }
                    }
                    if (tokens[curr].value == "}") {
                        ++curr;
                    } else {
                        throw std::string ("error - miss } it line  " + std::to_string(tokens[curr].line));
                    };

                } else {
                    //throw
                };
            }
            //return;
        }
        //std::cout << pr.type << " " << pr.id << std::endl;
        tree.push_id(pr);

    }

    void declaration_many_id() {
        declaration_id();
        std::cout << "сheck" << tokens[curr].value << std::endl;
        while (tokens[curr].value == ",") {
            ++curr;
            declaration_id();
            //std::cout << "сheck" << tokens[curr].value << std::endl;
        }
    }

    void list_instructions() {
        instruction();
    }

    void function() {
        std::string tp = tokens[curr].value;
        curr++;
        std::string name = tokens[curr].value;
        curr++;
        func *cur_func = new func(tp, name);
        push_func(cur_func);
        if (tokens[curr].value != "(") {
            throw std::string ("( miss");
        } else {
            curr++;
        }
        tree.create_scope();
        if (tokens[curr].value != ")") {
            if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
                tp = tokens[curr].value;
                curr++;
            } else {
                throw std::string ("error - miss type it line  " + std::to_string(tokens[curr].line));
            }
            if (tokens[curr].type == IDENTIFIER) {
                name = tokens[curr].value;
                curr++;
            } else {
                throw std::string ("error - miss IDENTIFIER it line  " + std::to_string(tokens[curr].line));
            }
            parametr * x = new parametr(tp, name);
            cur_func->parameters.push_back(x);
            tree.push_id(*x);
            while (tokens[curr].value == ",") {
                ++curr;
                if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
                    tp = tokens[curr].value;
                    curr++;
                } else {
                    throw std::string ("error - miss type it line  " + std::to_string(tokens[curr].line));
                }
                if (tokens[curr].type == IDENTIFIER) {
                    name = tokens[curr].value;
                    curr++;
                } else {
                    throw std::string ("error - miss IDENTIFIER it line  " + std::to_string(tokens[curr].line));
                }
                parametr * x = new parametr(tp, name);
                cur_func->parameters.push_back(x);
                tree.push_id(*x);
            };
        }


        curr++;

        if (tokens[curr].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr].line));
        }
        curr++;
        //std::cout << "RERE" << std::endl;
        list_instructions();
        if (tokens[curr].value !=  "return") {
            throw std::string("miss return");

        } else {
            curr++;
           // std::cout << "W HE" << std::endl;
            function_return(cur_func) ;
        }
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr].line));
        }

        tree.exit_scope();

    }

    void declaration() {
        //std::cout << "SIZE= " << tokens.size() << std::endl;
        while (curr < tokens.size() - 1) {
            //std::cout << "curr" << curr;
            if (tokens[curr].value == "void" || tokens[curr].value == "int" || tokens[curr].value == "string" ||
                tokens[curr].value == "double") {
                curr++;
                bool f = 0;
                if (tokens[curr].value == "[") {
                    f = 1;
                    curr++;

                }
                //std::cout << "hjdjhdj " << tokens[curr].value;
                if (tokens[curr].value == "]") {
                    curr++;
                } else if (tokens[curr].value != "]" && f == 1) {
                    throw std::string ("error - miss ] it line  " + std::to_string(tokens[curr].line));
                }
                if (tokens[curr].type == IDENTIFIER) {
                    curr++;
                } else {
                    throw std::string ("error - miss identifier it line  " + std::to_string(tokens[curr].line));
                }
                //std::cout << "aaaa" << tokens[curr - 2].value << std::endl;
                if (tokens[curr].value == "(") {
                    curr -= 2;
                    //std::cout << "here" << std::endl;
                    function();
                } else {
                    if (tokens[curr - 2].value == "void") {
                        throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr - 1].line));
                    }
                    curr -= 1;
                    declaration_many_id();
                    check_semicolon();
                }
            }
        }
    }

    void function_return(func * function) {
        //std::cout << "exp=" <<  tokens[curr].value << std::endl;
         if (function->type_answer == "void") {
            check_semicolon();
            return;
        }
         flag_for_comma = 1;
        expression();
        flag_for_comma = 0;
        std::string tp = st.types.top();
        st.types.pop();
        if (function->type_answer == tp) {
           check_semicolon();

        } else {
            throw std::string ("wrong type of return");

        }

    }

    void call_function() {
        push_poliz(tokens[curr], true);
        if (tokens[curr].type == IDENTIFIER) {
            curr++;
        } else {
            throw std::string ("error - miss identifier it line  " + std::to_string(tokens[curr].line));
        }
        if (tokens[curr].value == "(") {
            ++curr;
        } else {
            throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr].line));
        }
        func *cur_func = new func(" ", " ");
        flag_for_comma = 0;
        expression();
        flag_for_comma = 1;
        std::string tp = st.types.top();
        parametr * x = new parametr(tp, "");
        cur_func->parameters.push_back(x);
        while (tokens[curr].value == ",") {
            ++curr;
            flag_for_comma = 0;
            expression();
            tp = st.types.top();
            //std::cout << "type= " << tp << std::endl;
            parametr * y = new parametr(tp, "");
            cur_func->parameters.push_back(y);
            st.types.pop();
            flag_for_comma = 1;
        }
        //std::cout << cur_func->parameters.size();
        if (tokens[curr].value == ")") {
            ++curr;
        } else {
            throw std::string ("error - miss ) it line  " + std::to_string(tokens[curr].line));
        }
        tp = check_func(cur_func);
        //std::cout <<"tp = " <<  tp << std::endl;
        st.push_sem_stack_type(tp);

    }

    void function_else() {
        curr++;
        if (tokens[curr].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr].line));
        };
        list_instructions();
        if (tokens[curr].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr].line));
        }
    }

    void function_elif() {
        curr++;
        if (tokens[curr].value != "(") {
            throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr].line));
        };
        expression();
        check_bool();
        if (tokens[curr++].value != ")") {
            throw std::string ("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.create_scope();
        list_instructions();

        //std::cout << "IF2" << " " << tokens[curr].value << std::endl;
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        };
        tree.exit_scope();
        if (tokens[curr++].value == "elif") {
            function_elif();
        }

    }

    void function_if() {
        curr++;
        if (tokens[curr++].value != "(") {
            throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr - 2].line));
        };
        expression();
        check_bool();
        if (tokens[curr++].value != ")") {
            throw std::string ("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        };
        std::cout << "IF" << tokens[curr].value << std::endl;
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.create_scope();
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss }* it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.exit_scope();
        if (tokens[curr].value == "elif") {
            function_elif();
        }
        if (tokens[curr].value == "else") {
            function_else();
        }
    }

    void function_while() {
        curr++;
        if (tokens[curr++].value != "(") {
            throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr].line));
        }
        expression();
        check_bool();
        if (tokens[curr++].value != ")") {
            throw std::string ("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.create_scope();
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.exit_scope();
    }

    void function_case() {
        curr++;
        expression();
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
    }

    void function_default() {
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
    }

    void function_switch() {
        curr++;
        if (tokens[curr++].value != "(") {
            throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr - 2].line));
        }
        expression();
        if (tokens[curr++].value != ")") {
            throw std::string ("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        while (tokens[curr].value == "case") {
            function_case();
        }

        if (tokens[curr].value == "default") {
            curr++;
            function_default();
        }

        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
    }

    void function_for() {
        curr++;
        if (tokens[curr++].value != "(") {
            throw std::string ("error - miss ( it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr].value != ";") {
            declaration_id();
            check_semicolon();
        } else {
            curr++;
        }
        if (tokens[curr].value != ";") {
            expression();
            check_bool();
            check_semicolon();
        } else {
            curr++;
        }
        if (tokens[curr].value != ")") {
            expression();
            if (tokens[curr++].value != ")") {
                throw std::string ("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
            }
        } else {
            curr++;
        }
        if (tokens[curr++].value != "{") {
            throw std::string ("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.create_scope();
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw std::string ("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
        tree.exit_scope();
    }

    void instruction() {
        //std::cout << "istruction " << " " << tokens[curr].value << std::endl;
        if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "bool" ||
            tokens[curr].value == "double") {
            declaration_many_id();
            //std::cout << "here3" << std::endl;
            check_semicolon();
        } else if (tokens[curr].value == "if") {
            function_if();
        } else if (tokens[curr].value == "for") {
            //std::cout << "for" << std::endl;
            function_for();
        } else if (tokens[curr].value == "while") {
            function_while();
        } else if (tokens[curr].value == "switch") {
            function_switch();
        } else if (tokens[curr].value == "break") {
            curr++;
            check_semicolon();
        } else if (tokens[curr].value == "continue") {
            curr++;
            check_semicolon();
        }  else {
            if (tokens[curr].value == "}" || tokens[curr].value == "return") {
                //std::cout << "end" << std::endl;

                return;
                //std::cout << "break";
            } else {
                //std::cout << "ree" << std::endl;
                expression();
                //std::cout << "CHECH2 " << tokens[curr].value;
                check_semicolon();
                //std::cout << "CHECH2 " << tokens[curr].value;
            }
        }
        instruction();
    }

    void expression() {
        level_11();
    }

    void level_11() {
        level_10();
        if (flag_for_comma) {
            while (curr < tokens.size()) {
                if (tokens[curr].value == ",") {
                    st.push_sem_stack_lex(tokens[curr].value);
                    ++curr;
                    level_10();
                } else {
                    break;
                }
            }
        }
    }

    void level_10() {
        level_9();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "=" ||tokens[curr].value == "+=" || tokens[curr].value == "-=") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }

                st.push_sem_stack_lex(tokens[curr].value);
                ++curr;
                level_9();

                //std::cout << "ty";
                check_bin();
                //std::cout << "ty";
            } else {
                break;
            }
        }
    };

    void level_9() {
        level_8();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "||") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                st.push_sem_stack_lex(tokens[curr].value);
                ++curr;
                level_8();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_8() {
        level_7();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "&&") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                push_poliz(tokens[curr], false);
                st.push_sem_stack_lex(tokens[curr].value);
                ++curr;
                level_7();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_7() {
        level_6();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "|") {

                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                st.push_sem_stack_lex(tokens[curr].value);
                push_poliz(tokens[curr], false);
                ++curr;
                level_6();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_6() {
        level_5();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "&") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                push_poliz(tokens[curr], false);

                st.push_sem_stack_lex(tokens[curr].value);
                ++curr;
                level_5();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_5() {
        level_4();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "<=" || tokens[curr].value == ">=" || tokens[curr].value == "==" ||
                tokens[curr].value == ">" || tokens[curr].value == "<" || tokens[curr].value == "!=") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                st.push_sem_stack_lex(tokens[curr].value);
                push_poliz(tokens[curr], false);
                ++curr;
                level_4();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_4() {
        level_3();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "+" || tokens[curr].value == "-") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                st.push_sem_stack_lex(tokens[curr].value);
                push_poliz(tokens[curr], false);

                ++curr;
                level_3();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_3() {
        level_2();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "*" || tokens[curr].value == "/") {
                while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                    push_poliz(Token(OPERATOR, st.operations.top(), -1));
                    st.operations.pop();
                }
                st.push_sem_stack_lex(tokens[curr].value);
                push_poliz(tokens[curr], false);

                ++curr;
                level_2();
                check_bin();
            } else {
                break;
            }
        }
    }

    void level_2() {
        //level_1();
        //std::cout << "here5" << " " << tokens[curr].value << std::endl;
        if (tokens[curr].value == "++" || tokens[curr].value == "--") {
            while(!st.operations.empty() && getPrecedence(st.operations.top()) >= getPrecedence(tokens[curr].value)){
                push_poliz(Token(OPERATOR, st.operations.top(), -1));
                st.operations.pop();
            }
            //std::cout << "CHECK " << tokens[curr].value << std::endl;
            st.push_sem_stack_lex(tokens[curr].value);
            push_poliz(tokens[curr], false);
            if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER)
            {
                st.push_sem_stack_value(tokens[curr-1].value);
            }
            ++curr;
            level_1();
           // std::cout << "wwww";
            check_uno();
            //std::cout << "wwww";
        } else {
            //curr++;
            level_1();
        }
    }

    void level_1() {
        // std::cout << "here7" << " " << tokens[curr].value << std::endl;
        if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER) {
            //std::cout << "here6" << " " << tokens[curr].value << std::endl;
            //level_0();
            if (tokens[curr + 1].value == "++" || tokens[curr + 1].value == "--") {
                curr++;
                st.push_sem_stack_lex(tokens[curr].value);
                curr++;
                std::cout << "CHECK " << tokens[curr].value << std::endl;
                if (tokens[curr].value != ")") {
                    level_0();
                }
                //level_0();
                check_uno();
            } else {
                //std::cout << "apel" << std::endl;
                level_0();

            }

            // check_uno();
        } else if (curr < tokens.size() && tokens[curr].value == "(") {
            ++curr;
            //std::cout << "HERE5" << std::endl;
            expression();
            std::cout << "HERE5*" <<  tokens[curr].value << std::endl;
            if (curr < tokens.size() && tokens[curr].value == ")") {
                ++curr;
            } else {
                throw std::string("error - miss ) it line  " + std::to_string(tokens[curr].line));
            }


        } else {
            if (tokens[curr].value == ";") {
                return;
            }
            std::cout << "HERE " << tokens[curr].value;
            throw std::string("error - not type 2 or not type 3 it line  " + std::to_string(tokens[curr].line));
        }

    }

    void level_0() {
        //std::cout << "here6" << " " << tokens[curr].value << std::endl;
        if (tokens[curr].type == LITERAL) {
            bool fl_chislo = 1;
            bool fl_float = 0;
            for (int i = 0; i < tokens[curr].value.size(); ++i ) {
                if (!(tokens[curr].value[i] >= '0' && tokens[curr].value[i] <= '9')  && tokens[curr].value[i] != '.') {
                    fl_chislo = 0;
                }
                if (tokens[curr].value[i] == '.') {
                    fl_float = 1;
                }
            }
            if (!fl_chislo) {
                st.push_sem_stack_type("string");
            } else {
                if (fl_float) {
                    st.push_sem_stack_type("float");
                } else {
                    st.push_sem_stack_type("int");
                }
            }
            push_poliz(tokens[curr], false);

            st.push_sem_stack_value(tokens[curr].value); // Добавляем значение литерала

            ++curr;
            //std::cout << "WER" << tokens[curr].value << std::endl;
        } else if (tokens[curr].type == IDENTIFIER) {
            if (tokens[curr + 1].value == "(") {
                std::cout << "call_function" << std::endl;
                call_function();
            } else {
                //std::cout << "here7" << tp << std::endl;
                std::string tp;
                //std::cout << "here7" << tokens[curr].value << std::endl;
                if (tree.check_id(tokens[curr].value).second == 0) {
                    //std::cout << "here7" << std::endl;
                    throw std::string("id is not found");
                } else {
                    //std::cout << "here7" << std::endl;
                    tp = tree.check_id(tokens[curr].value).first;
                }
                st.push_sem_stack_type(tp);
                push_poliz(tokens[curr], false);

                ++curr;
            }
        } else if (tokens[curr].value == ";") {

        } else {
            //std::cout << "HERE3 " << tokens[curr].value;
            throw("error - not type 2 or not type 3 it line  " + std::to_string(tokens[curr].line));
        }
    }

    void programma() {
        try {
            declaration();
        } catch(std::string c) {
            std::cerr << c << '\n';
            exit(1);
        }
        std::cout <<  "------------------------" << std::endl;
        std::cout << "beautiful code\n";
        std::cout <<  "------------------------" << std::endl;
    }
    struct sem_stack {
        std::stack <std::string> types;
        std::stack <std::string> operations;
        std::stack <std::string> values;
        void stack_clear() {
            while (types.empty()) {
                types.pop();
            }
            while (operations.empty()) {
                operations.pop();
            }
        }
        void push_sem_stack_value(std::string s) {
            values.push(s);
        }

        void push_sem_stack_lex(std::string lex) {
            operations.push(lex);
        }
        void push_sem_stack_type(std::string type) {
            types.push(type);
        }
    };

    void check_bin() {
        std::string type_l = st.types.top();
        std::cout << type_l;
        st.types.pop();
        std::string type_r;
        if (st.types.size() == 0) {
            type_r = "";
        } else {
            type_r = st.types.top();
            st.types.pop();
        }
        std::string operation = st.operations.top();

        if (operation == ",") {
            st.types.push(type_l);
        }
        // std::cout << "Check_bin" << type_l << type_r << std::endl;
        if (type_l == "int" && type_r == "int") {
                st.types.push("int");
        } else if ((type_l == "float" || type_l == "int") && (type_r == "float" || type_r == "int") ){
                st.types.push("float");
        } else if (type_l == "bool"  && type_r == "bool") {
                st.types.push("bool");
        } else if((type_l == "bool" || type_l == "int")  && (type_r == "bool" || type_r == "int")) {
                st.types.push("bool");
        } else if (type_l == "string" && type_r == "string") {
            if (operation == "==" || operation == "!=" || operation == "=") {
                st.types.push("string");
            } else {
                throw std::string("error - operation not correct ");
            }
        } else {
            throw std::string("error - types are not correct ");
        }
        Token t(OPERATOR, st.operations.top(), -1);
        push_poliz(t, false);
        st.operations.pop();
    }
    void check_uno() {
        std::string type = st.types.top();
        st.types.pop();
        std::string operation = st.operations.top();
        st.operations.pop();
        // std::cout << "TYPE=" << type << std::endl;
        // std::cout << "operation=" << operation << std::endl;
        if (type != "int" &&  type  != "float" && type != "bool") {
            if (operation == "*" || operation == "&") {
                st.types.push(type);
            } else {
                throw std::string(" error - type is not correct with operation");
            }
        } else {
            if ((type == "bool" || type == "float")  && (operation == "++" || operation == "--")) {
                throw std::string(" error - operation is not correct with bool or  float");
            }
            st.types.push(type);
        }
    }
    bool check_bool() {
        std::string type = st.types.top();
        if (type != "bool" && type != "int") {
            throw std::string(" error - type is not bool or int ");
        }
        return true;
    }
    sem_stack st;

};




#endif //PARSER_H