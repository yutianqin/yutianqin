#include <iostream>
#include <sstream>
#include "headers/parse.hpp"
#include "headers/pointer.h"
using namespace std;



/**
 * Convert a string to istringstream for the convenience of parsing
 * @param str
 * @return
 */
PTR(Expr) parse_str( const std::string& str )
{
    std::istringstream istringstream(str);
    PTR(Expr) expr = parse_expr(istringstream);
    skip_whitespace(istringstream);
    if (!istringstream.eof())
        throw std::runtime_error("invalid input");;
    return expr;
}




// Actual parsing:
//----------------------------------------------------------------------------------------------------------------------
PTR(Expr) parse_expr( std::istream &in )
{
    PTR(Expr) e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if ( c == '=' ) {
        consume( in, '=' );
        c = in.peek();
        if ( c != '=' ) {
            throw std::runtime_error("equal sign missing");
        }
        consume(in,'=');
        PTR(Expr) rhs = parse_expr(in);
        return NEW(EqExpr)(e, rhs);
    }
    else return e;
}




PTR(Expr) parse_comparg( std::istream &in )
{
    PTR(Expr) e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if ( c == '+' ) {
        consume( in, '+' );
        PTR(Expr) rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    }
    else return e;
}




PTR(Expr) parse_addend( std::istream &in )
{
    PTR(Expr) e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if ( c == '*' ) {
        consume(in,'*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    }
    else return e;
}




PTR(Expr) parse_multicand( std::istream &in )
{
    PTR(Expr) expr = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        expr = NEW(CallExpr)(expr, actual_arg);
    }
    return expr;
}




PTR(Expr) parse_inner( std::istream &in )
{
    skip_whitespace(in);
    int c = in.peek();

    if ( (c == '-') || isdigit(c) ) return parse_num(in);

    // parse VarExpr: (any ASCII value)
    else if ( isalpha(c) ) return parse_var(in);

    else if ( c == '_' ) {
        consume(in, c);
        string keyword = parse_keyword(in);
        // parse LetExpr:
        if ( keyword == "let" ) return parse_let(in);
        // parse IfExpr:
        else if ( keyword == "if" ) return parse_if(in);
        // parse BoolExpr:
        else if ( keyword == "true" ) return NEW(BoolExpr)(true);
        else if ( keyword == "false" ) return NEW(BoolExpr)(false);
        // parse FunExpr:
        else if ( keyword == "fun" ) return parse_fun(in);
        // error:
        else throw std::runtime_error("invalid input");
    }

    else if ( c == '(' ) {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    }

    else {
        consume(in, c );
        throw std::runtime_error("invalid input");
    }
}




// Inner
//----------------------------------------------------------------------------------------------------------------------
PTR(Expr) parse_var( std::istream &in )
{
    int skr = in.get();
    char temp = (char)skr;
    string str(1, temp);

    while ( true ) {
        int ascii = in.peek();
        if (isalpha(ascii)) {
            consume(in,ascii);
            char curr = char(ascii);
            str.push_back(curr);
        }
        else if ( ascii == '_' ) {
            throw std::runtime_error("invalid input");
        }
        else break;
    }
    return NEW(VarExpr)(str);
}




PTR(Expr) parse_fun(std::istream &in)
{
    string formal_arg;
    PTR(Expr) body;
    skip_whitespace(in);
    consume(in,'(');
    formal_arg = parse_keyword(in);
    consume(in,')');
    skip_whitespace(in);
    body = parse_expr(in);
    return NEW(FunExpr)(formal_arg, body);
}




PTR(Expr) parse_if( std::istream &in )
{
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;

    skip_whitespace(in);

    test_part = parse_expr(in);

    skip_whitespace(in);
    consume(in,'_');
    if ( parse_keyword(in) != "then" ) {
        throw std::runtime_error("invalid input");
    }

    then_part = parse_expr(in);

    skip_whitespace(in);
    consume(in,'_');
    if ( parse_keyword(in) != "else" ) {
        throw std::runtime_error("invalid input");
    }

    else_part = parse_expr(in);

    return NEW(IfExpr)(test_part,then_part,else_part);
}




PTR(Expr) parse_let( std::istream &in )
{
    string lhs;
    PTR(Expr)  rhs;
    PTR(Expr)  body;

    skip_whitespace(in);

    //assign value for lhs
    while(true) {
        if ( isalpha(in.peek()) )
            lhs.push_back((char)in.get());
        else break;
    }

    skip_whitespace(in);
    consume(in,'=');
    skip_whitespace(in);

    //assign value for rhs
    rhs = parse_expr(in);

    skip_whitespace(in);
    consume(in,'_');
    if ( parse_keyword(in) != "in" ) {
        throw std::runtime_error("invalid input");
    }
    skip_whitespace(in);

    body = parse_expr(in);

    return NEW(LetExpr)(lhs, rhs, body );
}




PTR(Expr) parse_num( std::istream &in )
{
    long n = 0;
    bool negative = false, hasDigit = false;
    if ( in.peek() == '-' ) {
        negative = true;
        consume(in, '-');
    }
    while (true) {
        int c = in.peek();
        if ( isdigit(c) ) {
            hasDigit = true;
            consume(in, c);
            n = n*10 + (c - '0');
        }
        else break;
    }
    if ( !hasDigit ) {
        throw std::runtime_error( "invalid input" );
    }
    if ( negative ) {
        n = -n;
    }
    if ( n < INT_MIN || n > INT_MAX ) {
        throw runtime_error("Integer overflow");
    }
    // parse EqExpr
    skip_whitespace(in);
    return NEW(NumExpr)((int)n);
}




// Helpers:
//----------------------------------------------------------------------------------------------------------------------
static void consume( std::istream &in, int expect )
{
    int c = in.get();
    if ( c != expect ) {
        throw std::runtime_error( "consume mismatch" );
    }
}


static void skip_whitespace( std::istream &in )
{
    while(true) {
        int c = in.peek();
        if ( !isspace(c) )
            break;
        consume(in, c);
    }
}


static std::string parse_keyword( std::istream &in ) {
    std::string keyword;
    while(true) {
        int c = in.peek();
        if ( !isalpha(c) )
            return keyword;
        keyword.push_back((char)c);
        consume(in, c);
    }
}


