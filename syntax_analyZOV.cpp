// 1 - KEYWORD, 2 - IDENTIFIER, 3 - LITERAL, 4 - OPERATOR, 5 - PUNCTUATION,
// 6 - POINT, 7 - COMMA, 8 - BRACKETS, 9 - OTHER, 10 -COMMENT
#include "syntaxer_funcs.h"
 std::vector<Token> tokens;
 std::string project;
 Bor keywords;
int curr = 0;
bool is_in_func = 0;
bool is_type(Token &t) {
    if(t.value == "int" || t.value == "float" || t.value == "double" ||
        t.value == "char" || t.value == "string" || t.value == "bool")
        return true;
    return false;
}
bool is_type_func(Token &t) {
    if(t.value == "int" || t.value == "float" || t.value == "double" ||
        t.value == "char" || t.value == "string" || t.value == "bool")
        return true;
    return false;
}


void up_curr_declaration() {
    ++curr;
    if(curr >= tokens.size())
        throw("declaration_error - in line " + std::to_string(tokens[--curr].line));
}

void up_curr_instruction() {
    ++curr;
    if(curr >= tokens.size())
        throw("instruction_error - in line " + std::to_string(tokens[--curr].line));
}


void declaration() {
    if(tokens[curr].type == KEYWORD) {
        if(is_type(tokens[curr])) {
            up_curr_declaration();
            if (tokens[curr].type == IDENTIFIER && tokens[curr-1].line == tokens[curr].line) {
                up_curr_declaration();
                if (tokens[curr].value == ";") {
                    return;
                }
                if(tokens[curr].type == BRACKETS && tokens[curr].value == "(") {
                    if(is_in_func) {
                        throw("declaration_func_in_func_error in line " + std::to_string(tokens[curr].line));
                    }
                    up_curr_declaration();
                    if(tokens[curr].value == ")") {
                        if(tokens[curr-1].line != tokens[curr].line) {
                            throw("func_declaration_error_missed - ) - in line " + std::to_string(tokens[curr-1].line));
                        }
                    }
                    else {
                        while(tokens[curr].value !=")") {
                            declaration();
                            up_curr_declaration();
                            if(tokens[curr].value != ")") {
                                if(tokens[curr].type == COMMA) {
                                    up_curr_declaration();
                                    continue;
                                } else {
                                    throw("missing ',' between declarations in func params in line" + std::to_string(tokens[curr].line));
                                }
                            }
                        }
                    }
                    if(tokens[curr].value != ")")
                        throw("missing ')' in declaration of func in line " + std::to_string(tokens[curr].line));

                } else {
                    throw("declaration_error - missed ';' in line " + std::to_string(tokens[--curr].line));
                }
                if(tokens[curr].value == "{") {
                    up_curr_instruction();
                    while(tokens[curr].value != "}") {
                        instruction();
                        up_curr_instruction();
                    }
                } else {
                    if(tokens[curr].value == ";") {
                        return;
                    }
                    else {
                        throw("error_declaration_of_func - missed ';' in the end of line " + std::to_string(tokens[curr].line));
                    }
                }
            }
        }
    }
}

void instruction() {
    if(is_type(tokens[curr])) {

    }
}

void syntax_analyzer() {
    try {

        while(curr < tokens.size()) {
            declaration();
        }



    } catch(const char * c) {
        std::cerr << c << '\n';
        exit(1);
    }
}