#include "syntaxer_funcs.h"
#include <string>

#include <iostream>
#include <vector>


class  Parser {
public:
    Parser(std::vector<Token> tokens_) :  tokens((tokens_)), curr(0) {}

    void pars() {
        programma();
    }
private:
    int curr = 0;
    std::vector<Token> tokens;
    void check_semicolon() {
        if (tokens[curr].value == ";") {
            curr++;
        } else {
            throw("error - miss ';' it line  " + std::to_string(tokens[curr].line));
        }
    }

    void declaration_id() {
        //std::cout << "here4" << tokens[curr].value << std::endl;
        if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
            ++curr;
        }
        if (tokens[curr].type == IDENTIFIER) {
            ++curr;
            //std::cout <<
        } else {
            throw("error - miss IDENTIFIER it line  " + std::to_string(tokens[curr].line));
        }
        if (tokens[curr].value == "=") {
            ++curr;
            expression();
        }  else if (tokens[curr].value == "[") {
            ++curr;
            expression();
            if (tokens[curr].value == "]") {
                ++curr;
            } else {
                throw("error - miss ] it line  " + std::to_string(tokens[curr].line));
            };
            if (tokens[curr].value == "=") {
                ++curr;
                if (tokens[curr].value == "{") {
                    ++curr;
                    if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER) {
                        ++curr;
                    } else {
                        throw("error - not type 3 or not type 2 it line  " + std::to_string(tokens[curr].line));
                    }
                    while (tokens[curr].value == ",") {
                        ++curr;
                        if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER) {
                            ++curr;
                        } else {
                            throw("error - not type 3 or not type 2 it line  " + std::to_string(tokens[curr].line));
                        }
                    }
                    if (tokens[curr].value == "}") {
                        ++curr;
                    } else {
                        throw("error - miss } it line  " + std::to_string(tokens[curr].line));
                    };

                } else {
                    //throw
                };
            }
            //return;
        }

    }
    void declaration_many_id() {
        declaration_id();
        while(tokens[curr + 1].value == ",") {
            ++curr;
            declaration_id();
        }
    }
    void declaration_many_parametr() {
        declaration_parametr();
        while(tokens[curr].value == ",") {
            ++curr;
            declaration_parametr();
        }
    }
    void declaration_parametr() {
        //std::cout << "checkkkk" << tokens[curr].value << std::endl;
        if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
            curr++;
        } else {
            throw("error - miss int or double or string  it line  " + std::to_string(tokens[curr].line));
        }
        if (tokens[curr].type == IDENTIFIER) {
            curr++;
        } else {
            throw("error - miss IDENTIFIER it line  " + std::to_string(tokens[curr].line));
        }
    }
    void list_instructions() {
        instruction();
    }
    void function() {
        while (tokens[curr].value != ")") {
            //std::cout << "check" << tokens[curr].value << std::endl;
            declaration_many_parametr();


        }
        curr++;

        if (tokens[curr].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr].line));
        }
        curr++;
        //std::cout << "here2" << std::endl;
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw("error - miss } it line  " + std::to_string(tokens[curr].line));
        }
    }
    void declaration() {
        //std::cout << "SIZE= " << tokens.size() << std::endl;
        while (curr <tokens.size() - 1) {
            //std::cout << "curr" << curr;
            if (tokens[curr].value == "void" || tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value == "double") {
                curr++;
                if (tokens[curr].type == IDENTIFIER) {
                    curr++;
                } else {
                    throw("error - miss identifier it line  " + std::to_string(tokens[curr].line));
                }
                if (tokens[curr].value == "(") {
                    curr++;
                    //std::cout << "here" << std::endl;
                    function();
                } else  {
                    curr -= 1;
                    declaration_many_id();
                    check_semicolon();
                }
            }
        }
    }

    void function_return() {
        if (tokens[curr].value == ";") {
            ++curr;
        } else {
            expression();
            check_semicolon();
        }
    }
    void call_function() {
        if (tokens[curr].type == IDENTIFIER) {
            curr++;
        } else {
            throw("error - miss identifier it line  " + std::to_string(tokens[curr].line));
        }
        if (tokens[curr].value == "(") {
            ++curr;
        } else {
            throw("error - miss ( it line  " + std::to_string(tokens[curr].line));
        }
        expression();
        while (tokens[curr].value == ",") {
            ++curr;
            expression();
        }
        if (tokens[curr].value == ")") {
            ++curr;
        } else {
            throw("error - miss ) it line  " + std::to_string(tokens[curr].line));
        }

    }

    void function_else() {
        curr++;
        if (tokens[curr].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr].line));
        };
        list_instructions();
        if (tokens[curr].value != "}")  {
            throw("error - miss } it line  " + std::to_string(tokens[curr].line));
        }
    }
    void function_elif() {
        curr++;
        if (tokens[curr].value != "(") {
            throw("error - miss ( it line  " + std::to_string(tokens[curr].line));
        };
        expression();
        if (tokens[curr++].value != ")") {
            throw("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr++].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();

        std::cout << "IF2" << " " << tokens[curr].value << std::endl;
        if (tokens[curr++].value != "}") {
            throw("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        };
        if (tokens[curr++].value == "elif")  {
            function_elif();
        }

    }

    void function_if() {
        curr++;
        if (tokens[curr++].value != "(") {
            throw("error - miss ( it line  " + std::to_string(tokens[curr - 2].line));
        };
        expression();
        if (tokens[curr++].value != ")") {
            throw("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        };
        std::cout << "IF" << tokens[curr].value << std::endl;
        if (tokens[curr++].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value  != "}") {
            throw("error - miss }* it line  " + std::to_string(tokens[curr - 2].line));
        }
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
            throw("error - miss ( it line  " + std::to_string(tokens[curr].line));
        }
        expression();
        if (tokens[curr++].value != ")") {
            throw("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr++].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value != "}")  {
            throw("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
    }
    void function_case() {
        curr++;
        expression();
        if (tokens[curr++].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value != "}")  {
            throw("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
    }
    void function_default() {
        if (tokens[curr++].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
    }
    void function_switch() {
        curr++;
        if (tokens[curr++].value != "(") {
            throw("error - miss ( it line  " + std::to_string(tokens[curr - 2].line));
        }
        expression();
        if (tokens[curr++].value!= ")")  {
            throw("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr++].value != "{")  {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        while (tokens[curr].value == "case") {
            function_case();
        }

        if (tokens[curr].value == "default") {
            curr++;
            function_default();
        }

        if (tokens[curr++].value != "}") {
            throw("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
    }
    void function_for() {
        curr++;
        if (tokens[curr++].value != "(")  {
            throw("error - miss ( it line  " + std::to_string(tokens[curr - 2].line));
        }
        if (tokens[curr].value != ";") {
            declaration_id();
            check_semicolon();
        } else {
            curr++;
        }
        if (tokens[curr].value != ";") {
            expression();
            check_semicolon();
        } else {
            curr++;
        }
        if (tokens[curr].value != ")") {
            expression();
            if (tokens[curr++].value!= ")")  {
                throw("error - miss ) it line  " + std::to_string(tokens[curr - 2].line));
            }
        } else {
            curr++;
        }
        if (tokens[curr++].value != "{") {
            throw("error - miss { it line  " + std::to_string(tokens[curr - 2].line));
        }
        list_instructions();
        if (tokens[curr++].value != "}") {
            throw("error - miss } it line  " + std::to_string(tokens[curr - 2].line));
        }
    }

    void instruction() {
        //std::cout << "istruction " << " " << tokens[curr].value << std::endl;
        if (tokens[curr].value == "int" || tokens[curr].value == "string" || tokens[curr].value== "bool") {
            declaration_many_id();
            //std::cout << "here3" << std::endl;
            check_semicolon();
        } else if (tokens[curr].value == "if") {
            function_if();
        } else if (tokens[curr].value == "for") {
            //std::cout << "for" << std::endl;
            function_for();
        } else if (tokens[curr].value== "while") {
            function_while();
        } else if (tokens[curr].value == "switch") {
            function_switch();
        }
        else if (tokens[curr].value == "break") {
            curr++;
            check_semicolon();
        }
        else if (tokens[curr].value == "continue") {
            curr++;
            check_semicolon();
        }
        else if (tokens[curr].value == "return") {
            function_return();
        } else {
            if (tokens[curr].value == "}") {
                //std::cout << "end" << std::endl;

                return;
                //std::cout << "break";
            } else {
                //std::cout << "ree";
                expression();
                check_semicolon();


            }
        }
        instruction();
    }
    void expression() {
        level_11();
    }
    void level_11() {
        level_10();
        while (curr < tokens.size()) {
            if (tokens[curr].value == ",") {
                ++curr;
                level_10();
            } else {
                break;
            }
        }
    }
    void level_10(){
        level_9();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "=" || tokens[curr].value== "+=" || tokens[curr].value == "-=") {
                ++curr;
                level_9();
            } else {
                break;
            }
        }
    };
    void level_9() {
        level_8();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "||") {
                ++curr;
                level_8();
            } else {
                break;
            }
        }
    }
    void level_8() {
        level_7();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "&&") {
                ++curr;
                level_7();
            } else {
                break;
            }
        }
    }
    void level_7() {
        level_6();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "|") {
                ++curr;
                level_6();
            } else {
                break;
            }
        }
    }
    void level_6() {
        level_5();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "&") {
                ++curr;
                level_5();
            } else {
                break;
            }
        }
    }
    void level_5() {
        level_4();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "<=" || tokens[curr].value == ">=" || tokens[curr].value == "==" || tokens[curr].value == ">" || tokens[curr].value == "<" || tokens[curr].value== "!=") {
                ++curr;
                level_4();
            } else {
                break;
            }
        }
    }
    void level_4() {
        level_3();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "+" || tokens[curr].value == "-") {
                ++curr;
                level_3();
            } else {
                break;
            }
        }
    }
    void level_3() {
        level_2();
        while (curr < tokens.size()) {
            if (tokens[curr].value == "*" || tokens[curr].value == "/") {
                ++curr;
                level_2();
            } else {
                break;
            }
        }
    }
    void level_2() {
        //level_1();
        //std::cout << "here5" << " " << tokens[curr].value << std::endl;
        if (tokens[curr].value == "++" || tokens[curr].value == "--") {
            ++curr;
        }
       // std::cout << "here7" << " " << tokens[curr].value << std::endl;
        if (tokens[curr].type == LITERAL || tokens[curr].type == IDENTIFIER) {
            //std::cout << "here6" << " " << tokens[curr].value << std::endl;
            level_1();
        } else if (curr < tokens.size() && tokens[curr].value == "(") {
            ++curr;
            expression();
            if (curr < tokens.size() && tokens[curr].value == ")") {
                ++curr;
            } else {
                throw("error - miss ) it line  " + std::to_string(tokens[curr].line));
            }
        } else {
            throw("error - not type 2 or not type 3 it line  " + std::to_string(tokens[curr].line));
        }

    }

    void level_1() {
        //std::cout << "here6" << " " << tokens[curr].value << std::endl;
        if (tokens[curr].type == LITERAL) {
            ++curr;
        } else if (tokens[curr].type == IDENTIFIER) {
            if (tokens[curr + 1].value == "(") {
                call_function();
            } else {
                ++curr;
            }
        } else {
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
        std::cout << "------------------------" << std::endl;
        std::cout << "beautiful code, bro";
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
//

int main(int argc, char* argv[]) {
    get_filename(argc, argv);
    Parser parser(lexer());
    parser.pars();

}