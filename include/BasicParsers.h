#ifndef FL_2020_SPBU_SPR_BASICPARSERS_H
#define FL_2020_SPBU_SPR_BASICPARSERS_H

#include <functional>

#include "Parser.h"


extern Parser<std::string> pstring(const std::string &s);
extern Parser<char> pchar(char c);
extern Parser<char> pcheck(std::function<bool(char)> f);
extern Parser<char> psuc();
extern Parser<char> pfail();
extern Parser<std::string> puntil(const std::string &s);
extern Parser<std::string> pwhile(std::function<bool(char)> f);

#endif //FL_2020_SPBU_SPR_BASICPARSERS_H
