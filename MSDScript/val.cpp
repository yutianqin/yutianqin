#include <sstream>
#include <utility>
#include "headers/Env.h"
#include "headers/Expr.hpp"
#include "headers/val.h"
#include "headers/pointer.h"



//----------------------------------------------------------------------------------------------------------------------
std::string Val::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}





// Num Value
//----------------------------------------------------------------------------------------------------------------------
NumVal::NumVal(int value) {
    this->value = value;
}

bool NumVal::equals(PTR(Val) expr) {
    PTR(NumVal)other_num = CAST(NumVal)(expr);
    if ( other_num == nullptr )
        return false;
    else return this->value == other_num->value;
}

PTR(Val) NumVal::add_to(PTR(Val) other_val)  {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr)
        throw std::runtime_error("add to non-number");
    return NEW(NumVal)((unsigned)value + (unsigned)other_num->value);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr)
        throw std::runtime_error("mult of non-number");
    return NEW(NumVal)((unsigned) value * (unsigned)other_num->value);
}

bool NumVal::is_true() {
    throw std::runtime_error("check bool for num");
}

void NumVal::print(std::ostream &os) {
    os << this->value;
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("Cannot call the function for NumVal");
}





// Bool Value
//----------------------------------------------------------------------------------------------------------------------
BoolVal::BoolVal(bool boolean) {
    this->boolean = boolean;
}

PTR(Val) BoolVal::add_to(PTR(Val)  other_bool) {
    throw std::runtime_error("add to non-number");
}

PTR(Val) BoolVal::mult_with(PTR(Val)  other_val) {
    throw std::runtime_error("mult of non-number");
}

bool BoolVal::is_true() {
    return this->boolean;
}

bool BoolVal::equals(PTR(Val)  expr) {
    PTR(BoolVal)  other_bool = CAST(BoolVal)(expr);
    if ( other_bool == nullptr ) return false;
    else return this->boolean == other_bool->boolean;
}

void BoolVal::print(std::ostream &os) {
    if ( this->boolean ) os << "_true";
    else os << "_false";
}

PTR(Val) BoolVal::call(PTR(Val)  actual_arg) {
    throw std::runtime_error("Cannot call the function for BoolVal");
}





// Fun Value
//----------------------------------------------------------------------------------------------------------------------
FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = std::move(formal_arg);
    this->body = std::move(body);
    this->env = std::move(env);
}

bool FunVal::equals(PTR(Val) expr) {
    PTR(FunVal) other_func = CAST(FunVal)(expr);
    if ( other_func == nullptr ) return false;
    else return this->formal_arg == other_func->formal_arg
             && this->body->equals(other_func->body);
}

PTR(Val) FunVal::add_to(PTR(Val) other_bool) {
    throw std::runtime_error("Cannot perform add_to for FunVal");
}

PTR(Val) FunVal::mult_with(PTR(Val) other_val) {
    throw std::runtime_error("Cannot perform mult_with for FunVal");
}

bool FunVal::is_true() {
    throw std::runtime_error("Cannot perform is_true for FunVal");
}

void FunVal::print(std::ostream &os) {
//    os << "_fun (";
//    os << this->formal_arg << ") ";
//    os << this->body->to_string();
    os << "[function]";
}

// (_fun(x) x+x)(5)
PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
//    PTR(Expr) value = actual_arg->to_expr();
//    return this->body->subst(this->formal_arg,value)->interp();
}
