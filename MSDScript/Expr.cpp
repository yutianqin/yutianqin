#include "headers/Env.h"
#include "headers/val.h"
#include "headers/Expr.hpp"

#include <iostream>
#include <utility>
#include <sstream>

using namespace std;




// to be inherited by subclasses
//----------------------------------------------------------------------------------------------------------------------
// to_string -> for derived class to inherit
std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}
// to_string -> for derived class to inherit
std::string Expr::to_string_pretty( ) {
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}
// driver for pretty_print_at
void Expr::pretty_print( ostream& os ) {
    streampos pos = 0;
    bool parenthesis = false;
    pretty_print_at( os, prec_none, pos, parenthesis );
}







// NumExpr
//----------------------------------------------------------------------------------------------------------------------
NumExpr::NumExpr(int rep) : rep(rep) {}

// equals( PTR(Expr)expr ) -> to only compare expression
/**
 * Check whether the parameter equals to the NumExpr object
 * @param expr
 * @return boolean
 */
bool NumExpr::equals(PTR(Expr) expr) {
    PTR(NumExpr) other_expr = CAST(NumExpr)(expr);
    if ( other_expr == nullptr )
        return false;
    else return this->rep == other_expr->rep;
}

// interp() -> get actual value
/**
 * Interpret this NumExpr object
 * -> get the actual value of this NumExpr
 * @return value of this NumExpr object
 */
PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(rep);
}

// print( ostream& os )
void NumExpr::print(ostream& os) {
    os << std::to_string(rep);
}

// pretty_print_at(s)
void NumExpr::pretty_print_at(std::ostream& os, precedence_t prec, streampos& pos, bool parenthesis ) {
    os << std::to_string(rep);
}







// AddExpr
//----------------------------------------------------------------------------------------------------------------------
AddExpr::AddExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
}

/**
 * Check whether the parameter equals to the AddExpr object
 * @param expr
 * @return boolean
 */
bool AddExpr::equals(PTR(Expr)expr) {
    PTR(AddExpr) other_expr = CAST(AddExpr)(expr);
    if ( other_expr == nullptr ) 
        return false;
    return (   this->lhs ->equals(other_expr->lhs)
            && this->rhs ->equals(other_expr->rhs) );
}

/**
 * Interpret this AddExpr object
 * -> execute the calculation to add lhs and rhs of this AddExpr object
 * @return sum of lhs and rhs
 */
PTR(Val) AddExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->add_to(rhs->interp(env));
}

void AddExpr::print(ostream& os ) {
    os << "(";
    lhs->print(os);
    os << "+";
    rhs->print(os);
    os << ")";
}

void AddExpr::pretty_print_at(std::ostream& os, precedence_t prec, streampos& pos, bool parenthesis) {
    if ( prec >= prec_add ) os << "(";
    lhs->pretty_print_at( os, prec_add, pos, true );
    os << " + ";
    rhs->pretty_print_at( os, prec_none, pos, parenthesis );
    if ( prec >= prec_add ) os << ")";
}






// MultExpr
//----------------------------------------------------------------------------------------------------------------------
MultExpr::MultExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
}
/**
 * Check whether the parameter equals to the MultExpr object
 * @param expr
 * @return boolean
 */
bool MultExpr::equals(PTR(Expr)expr) {
    PTR(MultExpr) other_expr = CAST(MultExpr)(expr);
    if ( other_expr == nullptr ) 
        return false;
    return (   this->lhs ->equals(other_expr->lhs) 
            && this->rhs ->equals(other_expr->rhs) );
}

/**
 * Interpret this AddExpr object
 * -> execute the calculation that
 * @return product of lhs and rhs
 */
PTR(Val) MultExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->mult_with(rhs->interp(env));
}

void MultExpr::print(ostream& os) {
    os << "(";
    lhs->print(os);
    os << "*";
    rhs->print(os);
    os << ")";
}

void MultExpr::pretty_print_at(std::ostream& os, precedence_t prec, streampos& pos, bool parenthesis) {
    if ( prec == prec_mult )
        os << "(";
    lhs->pretty_print_at( os, prec_mult, pos, true );
    os << " * ";
//    if ( dynamic_cast<LetExpr*>(rhs) != nullptr ) parenthesis = false;
    rhs->pretty_print_at( os, prec_add, pos, parenthesis );
    if ( prec == prec_mult )
        os << ")";
}






// VarExpr
//----------------------------------------------------------------------------------------------------------------------
VarExpr::VarExpr(string str) : str(std::move(str) ) {}
/**
 * Check whether the parameter equals to the VarExpr object
 * @param expr
 * @return boolean
 */
bool VarExpr::equals(PTR(Expr)expr) {
    PTR(VarExpr) other_expr = CAST(VarExpr)(expr);
    if ( other_expr == nullptr )
        return false;
    return this->str == other_expr->str;
}

/**
 * Show user an error message
 * @return error msg
 */
PTR(Val) VarExpr::interp(PTR(Env) env) {
    return env->lookup(this->str);
}

void VarExpr::print(ostream &os ) {
    os << this->str;
}

void VarExpr::pretty_print_at (std::ostream& os, precedence_t prec, streampos& pos, bool parenthesis) {
    os << this->str;
}






//Let_Binding
//----------------------------------------------------------------------------------------------------------------------
LetExpr::LetExpr(std::string lhs, PTR(Expr)rhs, PTR(Expr)body) {
    this->lhs = std::move( lhs );
    this->rhs = std::move(rhs);
    this->body = std::move(body);
}


bool LetExpr::equals(PTR(Expr)expr) {
    PTR(LetExpr) other_expr = CAST(LetExpr)( expr );
    if (other_expr == nullptr) return false;
    return (    this->lhs == other_expr->lhs 
             && this->rhs->equals(other_expr->rhs) 
             && this->body->equals(other_expr->body) );
}

PTR(Val) LetExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs, rhs_val, env);
    return body->interp(new_env);
}

void LetExpr::print(ostream &os) {
    os << "(_let " << lhs << "=";
    os << rhs->to_string();
    os << " _in ";
    body->print(os );
    os << ")";
}

void LetExpr::pretty_print_at(ostream &os, precedence_t prec, streampos& pos, bool parenthesis) {
    if ( parenthesis )
        os << "(";
    streampos currentPos = os.tellp();
    long currpos = currentPos;
    os << "_let " << this->lhs << " = ";
    this->rhs->pretty_print_at( os, prec, pos, parenthesis );
    os << "\n";
    streampos nextLine = os.tellp();
    long nl = pos;
    long intent = currpos - nl;
    if ( currpos > 0 ) {
        for ( int i = 0; i < intent; i++ )
            os << " ";
    }
    os << "_in  ";
    this->body->pretty_print_at( os, prec_none, nextLine, parenthesis );
    if ( parenthesis )
        os << ")";
}






// If Expr
//----------------------------------------------------------------------------------------------------------------------
IfExpr::IfExpr(PTR(Expr)condition, PTR(Expr)trueSide, PTR(Expr)falseSide) {
    this->test_part = std::move(condition);
    this->then_part = std::move(trueSide);
    this->else_part = std::move(falseSide);
}

bool IfExpr::equals(PTR(Expr)expr) {
    PTR(IfExpr) other_expr = CAST(IfExpr)(expr);
    if ( other_expr == nullptr ) return false;
    return ( this->test_part->equals(other_expr->test_part) &&
             this->then_part->equals(other_expr->then_part) &&
             this->else_part->equals(other_expr->else_part) );
}

PTR(Val) IfExpr::interp(PTR(Env) env) {
    if (test_part->interp(env)->is_true())
        return then_part->interp(env);
    else
        return else_part->interp(env);
}

void IfExpr::print(ostream &os) {
    os << "(_if ";
    test_part->print(os);
    os << " _then ";
    then_part->print(os);
    os << " _else ";
    else_part->print(os);
    os << ")";
}

void IfExpr::pretty_print_at(ostream &os, precedence_t prec, streampos &pos, bool parenthesis) {
    print(os);
}






// Bool Expr
//----------------------------------------------------------------------------------------------------------------------
BoolExpr::BoolExpr(bool value) {
    this->value = value;
}

bool BoolExpr::equals(PTR(Expr)expr) {
    PTR(BoolExpr) other_expr = CAST(BoolExpr)( expr );
    if (other_expr == nullptr) return false;
    return this->value == other_expr->value;
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->value);
}

void BoolExpr::print(ostream &os) {
    if ( this->value ) os << "_true";
    else os << "_false";
}

void BoolExpr::pretty_print_at(ostream &os, precedence_t prec, streampos &pos, bool parenthesis) {
    print(os);
}






// Equal Expr
//----------------------------------------------------------------------------------------------------------------------
EqExpr::EqExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = std::move(lhs);
    this->rhs = std::move(rhs);
}

bool EqExpr::equals(PTR(Expr)expr) {
    PTR(EqExpr) other_expr = CAST(EqExpr)( expr );
    if (other_expr == nullptr) return false;
    return this->lhs->equals(other_expr->lhs) && this->rhs->equals(other_expr->rhs);
}

PTR(Val) EqExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->lhs->interp(env)->equals(this->rhs->interp(env)));
}

void EqExpr::print(ostream &os) {
    os << "(";
    this->lhs->print(os);
    os << "==";
    this->rhs->print(os);
    os << ")";
}

void EqExpr::pretty_print_at(ostream &os, precedence_t prec, streampos &pos, bool parenthesis) {
    print(os);
}






// Func Expr
//----------------------------------------------------------------------------------------------------------------------
FunExpr::FunExpr(std::string formal_arg, PTR(Expr)skr) {
    this->formal_arg = std::move(formal_arg);
    this->body = std::move(skr);
}

bool FunExpr::equals(PTR(Expr)expr) {
    PTR(FunExpr) other_funExpr = CAST(FunExpr)(expr);
    if ( other_funExpr == nullptr ) return false;
    return this->formal_arg == other_funExpr->formal_arg
        && this->body->equals(other_funExpr->body);
}

PTR(Val) FunExpr::interp(PTR(Env)env) {
    return NEW(FunVal)(this->formal_arg, this->body, env);
}

void FunExpr::print(ostream &os) {
//    this->interp()->print(os);
    os << "[function]";
}

void FunExpr::pretty_print_at(ostream &os, precedence_t prec, streampos &pos, bool parenthesis) {
    print(os);
}






// Call Expr
//----------------------------------------------------------------------------------------------------------------------
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = std::move(to_be_called);
    this->actual_arg = std::move(actual_arg);
}

bool CallExpr::equals(PTR(Expr) expr) {
    PTR(CallExpr) other_callExpr = CAST(CallExpr)(expr);
    if ( other_callExpr == nullptr ) return false;
    else return this->actual_arg->equals(other_callExpr->actual_arg)
             && this->to_be_called->equals(other_callExpr->to_be_called);
}

PTR(Val) CallExpr::interp(PTR(Env) env) { // (_fun(x) x+1)(100)
    return this->to_be_called->interp(env)
                             ->call(actual_arg->interp(env));
}

void CallExpr::print(ostream &os) {
    os << "(" << this->to_be_called->to_string() << ")";
    os << "(" << this->actual_arg->to_string() << ")";
}

void CallExpr::pretty_print_at(ostream &os, precedence_t prec, streampos &pos, bool parenthesis) {
    print(os);
}
