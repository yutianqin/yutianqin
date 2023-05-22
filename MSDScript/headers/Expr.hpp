#ifndef Expr_hpp
#define Expr_hpp

#include <iostream>
#include "pointer.h"


//----------------------------------------------------------------------------------------------------------------------
class Env;
class Val;


//----------------------------------------------------------------------------------------------------------------------
typedef enum {
    prec_none = 0,      // = 0
    prec_add = 1,       // = 1
    prec_mult = 2       // = 2
} precedence_t;

/**
 * The parent class that will be inherited by
 * all the other classes body this project
 */
CLASS(Expr) {
public:
    virtual ~Expr() = default;

    virtual bool equals( PTR(Expr) expr ) = 0;
    virtual PTR(Val) interp(PTR(Env) env) = 0;

    virtual void print( std::ostream& os ) = 0;
    std::string to_string();
    std::string to_string_pretty();
    void pretty_print( std::ostream& os );
    virtual void pretty_print_at( std::ostream& os, precedence_t, std::streampos& pos, bool parenthesis ) = 0;
};



//----------------------------------------------------------------------------------------------------------------------
class NumExpr : public Expr {
public:
    explicit NumExpr( int val );
    int rep; //!< Value of this NumExpr object

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};



//----------------------------------------------------------------------------------------------------------------------
class AddExpr : public Expr {
public:
    PTR(Expr) lhs; //!< Left hand side
    PTR(Expr) rhs; //!< Right hand side
    AddExpr( PTR(Expr) lhs, PTR(Expr) rhs );

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class MultExpr : public Expr {
public:
    PTR(Expr) lhs; //!< Left hand side
    PTR(Expr) rhs; //!< Right hand side
    MultExpr( PTR(Expr) lhs, PTR(Expr) rhs );

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class VarExpr : public Expr {
public:
    std::string str; //!< Value(String) of this VarExpr object
    explicit VarExpr( std::string str );

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class LetExpr : public Expr {
public:
    std::string lhs; //!< name
    PTR(Expr) rhs;  //!< value of name
    PTR(Expr) body;  //!< actual calculation
    LetExpr( std::string lhs, PTR(Expr) rhs, PTR(Expr) in );

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class IfExpr : public Expr {
public:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class BoolExpr : public Expr {
public:
    bool value; //!< Value of this BoolExpr object
    explicit BoolExpr( bool value );

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    EqExpr(PTR(Expr)lhs, PTR(Expr)rhs);

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;
    FunExpr(std::string formal_arg, PTR(Expr)body);

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};


//----------------------------------------------------------------------------------------------------------------------
class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    CallExpr(PTR(Expr)to_be_called, PTR(Expr)actual_arg);

    bool equals( PTR(Expr) expr ) override;
    PTR(Val) interp( PTR(Env) env ) override;

    void print( std::ostream& os ) override;
    void pretty_print_at( std::ostream& os, precedence_t prec, std::streampos& pos, bool parenthesis ) override;
};

#endif /* Expr_hpp */