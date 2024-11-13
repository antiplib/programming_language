

#ifndef SYNTAXER_FUNCS_H
#define SYNTAXER_FUNCS_H

#include "Lexer.h"

bool is_type(Token &t);
bool is_type_func(Token &t);

void up_curr_declaration();
void up_curr_instruction();

void declaration();
void instruction();

void syntax_analyzer();


#endif //SYNTAXER_FUNCS_H



