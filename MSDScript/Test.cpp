#include "headers/Env.h"
#include "headers/val.h"
#include "headers/Expr.hpp"
#include "headers/catch.h"
#include "headers/pointer.h"
#include <sstream>
using namespace std;


//----------------------------------------------------------------------------------------------------------------------
TEST_CASE( "Functions" )
{
    SECTION( "FunVal" )
    {
        // _fun (x) x + 1
        PTR(FunExpr) funExpr1 = NEW(FunExpr)("x",
                                     NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)) );
        CHECK( funExpr1->interp(Env::empty)
                       ->equals(NEW(FunVal)("x",
                                                     NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                     Env::empty)) );
//        CHECK( funExpr1->to_string() == "_fun (x) (x+1)");
        CHECK( funExpr1->to_string() == "[function]");

        // _fun (y) y * 1
        PTR(FunExpr) funExpr2 = NEW(FunExpr)("y",
                                             NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(500)));
        CHECK( funExpr2->interp(Env::empty)
                       ->equals(NEW(FunVal)("y",
                                                    NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(500)),
                                                    Env::empty)) );
//        CHECK( funExpr2->to_string() == "_fun (y) (y*500)");
        CHECK( funExpr2->to_string() == "[function]");

        // _let f = _fun (x) x*x
        // _in f(2)
        PTR(FunExpr) funExpr3 = NEW(FunExpr)("x",
                                             NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x")));
        PTR(LetExpr) letExpr1 = NEW(LetExpr)("f",
                                             funExpr3,
                                              NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                 NEW(NumExpr)(5)) );
        CHECK( letExpr1->interp(Env::empty)->equals(NEW(NumVal)(25)) );
    }
}


//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "Booleans and Conditionals" )
{
    SECTION( "Boolean Expr" )
    {
        CHECK( (NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)));
        CHECK( (NEW(BoolExpr)(true))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
        CHECK( (NEW(BoolExpr)(true))->to_string()=="_true" );
        CHECK( (NEW(BoolExpr)(false))->to_string()=="_false" );
    }

    SECTION( "If Expr" )
    {
        CHECK_THROWS_WITH( (NEW(IfExpr)(NEW(NumExpr)(000),
                                       NEW(NumExpr)(111),
                                       NEW(NumExpr)(222)))
                                       ->interp(Env::empty), "check bool for num" );

        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(111),
                           NEW(NumExpr)(222)))
                            ->equals(
                               NEW(IfExpr)(NEW(BoolExpr)(true),
                                                NEW(NumExpr)(111),
                                                 NEW(NumExpr)(222))) );

        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(111),
                           NEW(NumExpr)(222)))
                           ->interp(Env::empty)
                           ->equals(NEW(NumVal)(111)) );

        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(false),
                           NEW(NumExpr)(111),
                           NEW(NumExpr)(222)))
                           ->interp(Env::empty)
                           ->equals(NEW(NumVal)(222)) );

        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(111),
                           NEW(NumExpr)(222)))
                       ->to_string() == "(_if _true _then 111 _else 222)");
    }

    SECTION( "Equal Expr" )
    {
        CHECK( (NEW(EqExpr)(NEW(NumExpr)(5),NEW(NumExpr)(6)))
                    ->equals(NEW(EqExpr)(NEW(NumExpr)(5),NEW(NumExpr)(6))) );

        CHECK( (NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))
                    ->interp(Env::empty)
                    ->equals(NEW(BoolVal)(true)) );

        CHECK( (NEW(EqExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))
                    ->interp(Env::empty)
                    ->equals(NEW(BoolVal)(true)) );

        CHECK( (NEW(EqExpr)(NEW(NumExpr)(888), NEW(NumExpr)(888)))
                       ->to_string()
                       == "(888==888)" );
    }
}

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "Value classes" )
{
    SECTION( "NumVal" )
    {
        PTR(NumVal) numval1 = NEW(NumVal)(2);
        CHECK( numval1->to_string() == "2" );
    }
    SECTION( "BoolVal" )
    {
        PTR(BoolVal) boolval1 = NEW(BoolVal)(true);
        CHECK( boolval1->to_string() == "_true" );
    }

    CHECK( (NEW(LetExpr)("x",
                        NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                        NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))
                   ->interp(Env::empty)
                   ->equals(NEW(NumVal)(25)) );
}


//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "LetExpr" )
{
    SECTION( "let1" )
    {
        PTR(LetExpr) let1 = NEW(LetExpr)("x",
                                    NEW(NumExpr)(5),
                                    NEW(AddExpr)(NEW(NumExpr)(995), NEW(VarExpr)("x")));
        PTR(LetExpr) toCompare = NEW(LetExpr)("x",
                                         NEW(NumExpr)(5),
                                         NEW(AddExpr)(NEW(NumExpr)(995), NEW(VarExpr)("x")));

        CHECK( let1->equals(toCompare) );
        CHECK( let1->to_string() == "(_let x=5 _in (995+x))" );
        CHECK( let1->to_string_pretty() == "_let x = 5\n"
                                           "_in  995 + x");
    }

    SECTION( "let2" )
    {
        PTR(LetExpr) let2 = NEW(LetExpr)("x",
                                    NEW(NumExpr)(5),
                                    NEW(AddExpr)(NEW(LetExpr)("y",
                                                            NEW(NumExpr)(3),
                                                            NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                                        NEW(VarExpr)("x")));

        CHECK( let2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))" );
        CHECK( let2->to_string_pretty() == "_let x = 5\n"
                                           "_in  (_let y = 3\n"
                                           "      _in  y + 2) + x" );
    }

    SECTION( "let body mult" )
    {
        PTR(MultExpr) mult = NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2),
                                                   NEW(LetExpr)("x",
                                                               NEW(NumExpr)(5),
                                                               NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))),
                                      NEW(NumExpr)(3));

        CHECK( mult->to_string() == "((2*(_let x=5 _in (x+1)))*3)" );
        CHECK( mult->to_string_pretty() == "(2 * (_let x = 5\n"
                                           "      _in  x + 1)) * 3");
    }
}


//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "pretty_print by using to_string_pretty" )
{
    SECTION( "NumExpr" )
    {
        CHECK((NEW(NumExpr)(9999))->to_string() == "9999" );
    }

    SECTION( "VarExpr" )
    {
        CHECK((NEW(VarExpr)("Arigato"))->to_string() == "Arigato" );
    }

    SECTION( "AddExpr" )
    {
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))
                        ->to_string_pretty() == "10 + 10" );

        CHECK( (NEW(AddExpr)(NEW(NumExpr)(99), NEW(VarExpr)("Sandwiches")))
                        ->to_string_pretty() == "99 + Sandwiches" );

        CHECK( (NEW(AddExpr)(NEW(NumExpr)(1),
                            NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))
                       ->to_string_pretty() == "1 + 2 * 3" );

        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(212)),
                            NEW(MultExpr)(NEW(NumExpr)(1325), NEW(NumExpr)(52))))
                            ->to_string_pretty() == "8 * 212 + 1325 * 52" );
    }

    SECTION( "MultExpr" )
    {
        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))
                        ->to_string_pretty()
                                    == "(2 * 3) * 4" );

        CHECK( (NEW(MultExpr) (NEW(NumExpr)(2), NEW(MultExpr) (NEW(NumExpr)(3), NEW(NumExpr)(4))))
                        ->to_string_pretty()
                                    == "2 * 3 * 4" );

        CHECK( (NEW(MultExpr)(NEW(NumExpr)(1),
                             NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))
                        ->to_string_pretty()
                                    == "1 * (2 + 3)" );

        CHECK( (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(66), NEW(NumExpr)(77)),
                             NEW(AddExpr)(NEW(NumExpr)(88), NEW(NumExpr)(99))))
                       ->to_string_pretty()
                                    == "(66 + 77) * (88 + 99)" );

        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(12), NEW(NumExpr)(25)), NEW(VarExpr)("x")),
                             NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(63), NEW(VarExpr)("y")), NEW(NumExpr)(80))))
                       ->to_string_pretty()
                                    == "((12 + 25) * x) * (63 * y + 80)" );
    }

    SECTION("Mult_Mults_Mults_Mults")
    {
        CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))))
                      ->to_string_pretty()
                                    == "(10 * (10 * 10) * 10) * 10 * 10");
    }
}


//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "print by using to_string" )
{
    SECTION( "NumExpr" )
    {
        CHECK((NEW(NumExpr)(10))->to_string() == "10" );
        CHECK((NEW(NumExpr)(-10))->to_string() == "-10" );
        CHECK((NEW(NumExpr)(0))->to_string() == "0" );
        CHECK((NEW(NumExpr)(1))->to_string() == "1" );
        CHECK((NEW(NumExpr)(-1))->to_string() == "-1" );

        stringstream max, min;
        max << INT32_MAX;
        min << INT32_MIN;
        string maxInt = max.str(), minInt = min.str();
        CHECK((NEW(NumExpr)(INT32_MAX))->to_string() == maxInt );
        CHECK((NEW(NumExpr)(INT32_MIN))->to_string() == minInt );
    }

    SECTION( "VarExpr" )
    {
        CHECK((NEW(VarExpr)("lol"))->to_string() == "lol" );
        CHECK((NEW(VarExpr)("value:)"))->to_string() == "value:)" );
    }

    SECTION( "AddExpr" )
    {
        CHECK((NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))->to_string() == "(10+10)" );
        CHECK((NEW(AddExpr)(NEW(NumExpr)(99), NEW(VarExpr)("Sandwiches")))->to_string() == "(99+Sandwiches)" );
        CHECK((NEW(AddExpr)(NEW(VarExpr)("U"), NEW(VarExpr)("T")))->to_string() == "(U+T)" );

        CHECK( (NEW(AddExpr)((NEW(AddExpr)(NEW(NumExpr)(10),
                                         NEW(NumExpr)(10))),
                            NEW(NumExpr)(100)))
                        ->to_string()
                        == "((10+10)+100)" );

        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")),
                            NEW(VarExpr)("z")) )
                       ->to_string()
                       == "((x*y)+z)" );

        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")),
                            NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y"))))
                        ->to_string()
                        == "((2*x)+(3*y))" );
    }

    SECTION( "MultExpr" )
    {
        CHECK( (NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))
                            ->to_string()
                            == "(10*10)" );

        CHECK( (NEW(MultExpr)(NEW(NumExpr)(10), NEW(VarExpr)("Burgers")))
                            ->to_string()
                            == "(10*Burgers)" );

        CHECK( (NEW(MultExpr)(NEW(VarExpr)("~"), NEW(VarExpr)("~")))
                            ->to_string()
                            == "(~*~)" );

        CHECK( (NEW(MultExpr)((NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))), NEW(NumExpr)(100)))
                            ->to_string()
                            == "((10+10)*100)" );

        CHECK( (NEW(MultExpr)(
                NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")),
                NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y"))))
                            ->to_string()
                            == "((2+x)*(3*y))" );
    }
}


//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "equals" )
{
    SECTION( "AddExpr" )
    {
        CHECK((NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(5)))
        -> equals(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(5))) == true);

        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))
        -> equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(4))) == false);

        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))
        -> equals(NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(4))) == false);

        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))
        -> equals(NEW(VarExpr)("x")) == false);

        CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))
        -> equals(NEW(NumExpr)(4)) == false);
    }

    SECTION( "MultExpr" )
    {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(5))) -> equals(NEW(MultExpr)(NEW(NumExpr)(3),
                                                                                    NEW(NumExpr)(5))) == true);

        CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))) -> equals(NEW(MultExpr)(NEW(NumExpr)(1),
                                                                                    NEW(NumExpr)(4))) == false);

        CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))) -> equals(NEW(AddExpr)(NEW(NumExpr)(1),
                                                                                   NEW(NumExpr)(2))) == false);
    }

    SECTION( "variable" )
    {
        CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true );
        CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false );
        CHECK((NEW(VarExpr)("-8"))->equals(NEW(VarExpr)("x")) == false );
        CHECK((NEW(VarExpr)(""))->equals(NEW(VarExpr)("x")) == false );
    }

    SECTION( "dynamic_cast" )
    {
        auto v1 = NEW(VarExpr)("str" );
        auto n1 = NEW(NumExpr)(5);
        auto n2 = NEW(NumExpr)(10);
        auto a1 = NEW(AddExpr)(n1, n1);
        auto m1 = NEW(MultExpr)(n1, n1);

        CHECK(CAST(VarExpr)( n1 ) == nullptr );
        CHECK(CAST(NumExpr)( n1 ) != nullptr );
        CHECK(CAST(AddExpr)( n1 ) == nullptr );
        CHECK(CAST(MultExpr)( n1 ) == nullptr );
        CHECK(CAST(NumExpr)( a1 ) == nullptr );
        CHECK(CAST(AddExpr)( a1 ) != nullptr );
        CHECK(CAST(MultExpr)( a1 ) == nullptr );
        CHECK(CAST(NumExpr)( m1 ) == nullptr );
        CHECK(CAST(AddExpr)( m1 ) == nullptr );
        CHECK(CAST(MultExpr)( m1 ) != nullptr );
    }
}


//----------------------------------------------------------------------------------------------------------------------

TEST_CASE( "interp" )
{
    SECTION( "NumExpr" )
    {
        CHECK((NEW(NumExpr)(3))->interp(Env::empty)->equals(NEW(NumVal)(3)));
        CHECK(!((NEW(NumExpr)(3))->interp(Env::empty)->equals(NEW(NumVal)(4))));
    }

    SECTION( "AddExpr" )
    {
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))
                       ->interp(Env::empty)
                       ->equals(NEW(NumVal)(5)) );

        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(15)),
                            NEW(AddExpr)(NEW(NumExpr)(20), NEW(NumExpr)(20))))
                       ->interp(Env::empty)
                       ->equals(NEW(NumVal)(65)));

        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                            NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(5))))
                       ->interp(Env::empty)
                       ->equals(NEW(NumVal)(15)));
    }

    SECTION( "MultExpr" )
    {
        CHECK( (NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))
                       ->interp(Env::empty)
                       ->equals(NEW(NumVal)(6)) );

        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                             NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(5))))
                       ->interp(Env::empty)
                       ->equals(NEW(NumVal)(120)));

        CHECK( (NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
                             NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(2))))
                       ->interp(Env::empty)
                       ->equals(NEW(NumVal)(30)));
    }

    SECTION( "VarExpr" )
    {
        CHECK_THROWS_WITH( (NEW(VarExpr)("x"))->interp(Env::empty), "free variable: x" );
    }
}


// Substitution function removed
//----------------------------------------------------------------------------------------------------------------------
//TEST_CASE( "subst" )
//{
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
//                   ->subst("x", NEW(VarExpr)("y"))
//                   ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) == true );
//
//    CHECK(((NEW(NumExpr)(7))
//            ->subst("x", NEW(VarExpr)("y")))
//                   ->equals(NEW(NumExpr)(7)) == true );
//
//    CHECK((NEW(VarExpr)("x"))
//                   ->subst("W", NEW(VarExpr)("y"))
//                   ->equals(NEW(VarExpr)("x")) == true );
//
//    CHECK((NEW(AddExpr)(NEW(NumExpr)(8), NEW(NumExpr)(2)))
//                   ->subst("x", NEW(VarExpr)("y"))
//                   ->equals(NEW(AddExpr)(NEW(NumExpr)(8), NEW(NumExpr)(2))) == true );
//
//    CHECK((NEW(MultExpr)(NEW(VarExpr)("h"), NEW(NumExpr)(2)))
//                   ->subst("h", NEW(VarExpr)("y"))
//                   ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))) == true );
//}

//----------------------------------------------------------------------------------------------------------------------






