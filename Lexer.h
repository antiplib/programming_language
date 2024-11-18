
#pragma once
#ifndef LEXER_H
#define LEXER_H
#include "keywords.h"
//

extern std::vector<Token> tokens;
extern std::string project;
extern Bor keywords;

std::vector <Token> lexer();
#endif //LEXER_H