#ifndef MSD_CS6015_PROJECT_PARSE_HPP
#define MSD_CS6015_PROJECT_PARSE_HPP
#endif //MSD_CS6015_PROJECT_PARSE_HPP

#include <iostream>
#include "Env.h"
#include "val.h"
#include "Expr.hpp"
#include "pointer.h"

PTR(Expr) parse_str(const std::string& str);

PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_addend(std::istream & in);
PTR(Expr) parse_multicand(std::istream & in);
PTR(Expr) parse_inner(std::istream & in);

PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);


static void consume( std::istream &in, int expect);
static void skip_whitespace(std::istream &in);
static std::string parse_keyword(std::istream &in );
