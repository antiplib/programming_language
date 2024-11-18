// 1 - KEYWORD, 2 - IDENTIFIER, 3 - LITERAL, 4 - OPERATOR, 5 - PUNCTUATION,
// 6 - POINT, 7 - COMMA, 8 - BRACKETS, 9 - OTHER, 10 -COMMENT
#include "syntaxer_funcs.h"
 std::vector<Token> tokens;
 std::string project;
 Bor keywords;

int curr = 0;

bool is_in_func = 0;
bool is_type(Token &t) {
    if(t.value == "int" || t.value == "float" || t.value == "double"
         || t.value == "string")
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
                if( tokens[curr].value == "(") {
//                    if(is_in_func) {
//                        throw("declaration_func_in_func_error in line " + std::to_string(tokens[curr].line));
//                    }
                    is_in_func = true;
                    up_curr_declaration();
                    if(tokens[curr].value == ")") {
                        if(tokens[curr-1].line != tokens[curr].line) {
                            throw("func_declaration_error_missed - ) - in line " + std::to_string(tokens[curr-1].line));
                        }
                    }
                    else {
                        while(tokens[curr].value != ")") {
                            //declaration();
                            if(tokens[curr].value != ")") {
                                if(tokens[curr].type == COMMA) {
                                    up_curr_declaration();
                                    continue;
                                } else {
                                    throw("unknown symbol spotted: '" + tokens[curr].value  +"'"+ " in line - " + std::to_string(tokens[curr].line));
                                }
                            }
                        }
                        is_in_func = false;
                    }
                    if(tokens[curr].value != ")")
                        throw("missing ')' in declaration of func in line " + std::to_string(tokens[curr].line));
                    ++curr;

                } else if(is_in_func == 0) {
                    throw("declaration_error - missed ';' in line " + std::to_string(tokens[--curr].line));
                }
                if(tokens[curr].value == "{") {
                    is_in_func = true;
                    up_curr_instruction();
                    while(tokens[curr].value != "}") {
                        declaration();
                        up_curr_instruction();
                    }
                    is_in_func = false;
                } else {
                    if(tokens[curr].value == ";") {
                        if(is_in_func) {
                            throw("error_declaration_of_func - excess ';' it line  " + std::to_string(tokens[curr].line));
                        }
                        return;
                    }
                    else {
                        if(!is_in_func) {
                            throw("error_declaration_of_func - missed ';' in the end of line " + std::to_string(tokens[curr].line));
                        }
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


//void expression() {
//    level_11();
//}
//void level_11() {
//    level_10();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == ",") {
//            ++curr;
//            level_10();
//        } else {
//            break;
//        }
//    }
//}
//void level_10(){
//    level_9();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "=" || tokens[curr].value== "+=" || tokens[curr].value == "-=") {
//            ++curr;
//            level_9();
//        } else {
//            break;
//        }
//    }
//};
//void level_9() {
//    level_8();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "||") {
//            ++curr;
//            level_8();
//        } else {
//            break;
//        }
//    }
//}
//void level_8() {
//    level_7();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "&&") {
//            ++curr;
//            level_7();
//        } else {
//            break;
//        }
//    }
//}
//void level_7() {
//    level_6();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "|") {
//            ++curr;
//            level_6();
//        } else {
//            break;
//        }
//    }
//}
//void level_6() {
//    level_5();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "&") {
//            ++curr;
//            level_5();
//        } else {
//            break;
//        }
//    }
//}
//void level_5() {
//    level_4();
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "<=" || tokens[curr].value == ">=" || tokens[curr].value == "==" || tokens[curr].value == ">" || tokens[curr].value == "<" || tokens[curr].value== "!=") {
//            ++curr;
//            level_4();
//        } else {
//            break;
//        }
//    }
//}
//void level_4() {
//    level_3()
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "+" || tokens[curr].value == "-") {
//            ++curr;
//            level_3();
//        } else {
//            break;
//        }
//    }
//}
//void level_3() {
//    level_2()
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "*" || tokens[curr].value == "/") {
//            ++curr;
//            level_2();
//        } else {
//            break;
//        }
//    }
//}
//void level_2() {
//    level_1()
//    while (curr < tokens.size()) {
//        if (tokens[curr].value == "++" || tokens[curr].value == "--") {
//            ++curr;
//            level_1();
//        } else {
//            break;
//        }
//    }
//}
//









void syntax_analyzer() {
    try {

        while(curr < tokens.size()) {
            declaration();
            ++curr;
        }


    } catch(std::string c) {
        std::cerr << c << '\n';
        exit(1);
    }
    std::cout << "beautiful code, bro";
}