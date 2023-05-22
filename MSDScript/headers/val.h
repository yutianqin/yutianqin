#ifndef MSD_CS6015_PROJECT_VAL_H
#define MSD_CS6015_PROJECT_VAL_H

#include "iostream"
#include "pointer.h"


//----------------------------------------------------------------------------------------------------------------------
class Expr;
class Env;


//----------------------------------------------------------------------------------------------------------------------
CLASS(Val) {
public:
    virtual bool equals(PTR(Val) other_val) = 0;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other_val) = 0;
    std::string to_string();
    virtual void print(std::ostream &os) = 0;
    virtual bool is_true() = 0;

    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};


//----------------------------------------------------------------------------------------------------------------------
class NumVal : public Val {
public:
    int value;
    explicit NumVal(int value);

    bool equals(PTR(Val) expr) override;
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_with(PTR(Val) other_val) override;
    bool is_true() override;
    void print(std::ostream &os) override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};


//----------------------------------------------------------------------------------------------------------------------
class BoolVal : public Val {
public:
    bool boolean;
    explicit BoolVal(bool boolean);

    bool equals(PTR(Val) expr) override;
    PTR(Val) add_to(PTR(Val) other_bool) override;
    PTR(Val) mult_with(PTR(Val) other_val) override;
    bool is_true() override;
    void print(std::ostream &os) override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};


//----------------------------------------------------------------------------------------------------------------------
class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);

    bool equals(PTR(Val) expr) override;
    PTR(Val) add_to(PTR(Val) other_bool) override;
    PTR(Val) mult_with(PTR(Val) other_val) override;
    bool is_true() override;
    void print(std::ostream &os) override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};

#endif //MSD_CS6015_PROJECT_VAL_H