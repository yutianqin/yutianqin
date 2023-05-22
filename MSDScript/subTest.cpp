#include "headers/Env.h"
#include "headers/val.h"
#include "headers/Expr.hpp"
#include "headers/catch.h"
#include "headers/pointer.h"
#include <iostream>
#include <sstream>
using namespace std;



TEST_CASE( "Pretty Print based on Matthew Flatt's examples" )
{
    std::stringstream out("");
    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  x + 1");

    out.str(std::string());
    (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in  x) + 1");

    out.str(std::string());
    (NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in  x + 1");

    out.str(std::string());
    (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in  x) + 1");

    out.str(std::string());
    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
            ->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  (_let y = 3\n"
                       "      _in  y + 2) + x");

    out.str(std::string());
    (NEW(LetExpr)("x", NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in  y * 2\n"
                       "_in  x + 1");
}


//----------------------------------------------------------------------------------------------------------------------------------


TEST_CASE( "pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let" )
{
    PTR(LetExpr) tripleNestedLet1 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                           NEW(LetExpr)("y", NEW(NumExpr)(1),
                                                       NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z"))));

    PTR(LetExpr) tripleNestedLet2 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                            NEW(LetExpr)("y", NEW(NumExpr)(1),
                                                        NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                                    NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));

    PTR(LetExpr) tripleNestedLet3 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                            NEW(LetExpr)("y", NEW(NumExpr)(1),
                                                        NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                                    NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));

    CHECK(tripleNestedLet1 -> to_string_pretty() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  (x + y) * z"
    );
    CHECK(tripleNestedLet2 -> to_string_pretty() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  _let z = x + 1\n"
          "          _in  (x + y) + z"
    );
    CHECK(tripleNestedLet3 -> to_string_pretty() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  _let z = x + 1\n"
          "          _in  (x + y) * z"
    );

    PTR(LetExpr) tripleNestedLet4 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                           NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                       NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))));

    PTR(LetExpr) tripleNestedLet5 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                           NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                                   NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")));
    std::stringstream out("");
    out.str("");
    tripleNestedLet4->pretty_print(out);
    CHECK(out.str() ==
          "_let x = 5\n"
          "_in  _let y = 3\n"
          "     _in  y + 2"
    );

    CHECK(tripleNestedLet5 -> to_string_pretty() == "_let x = 5\n"
                                                          "_in  (_let y = 3\n"
                                                          "      _in  y + 2) + x");

    SECTION("assignment_examples") {
        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))-> to_string_pretty()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");

//        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr) (2),
//                                      NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") ,new  NumExpr(1)) )),
//                         NEW(NumExpr)(3) )) -> to_string_pretty()
//               == "(2 * _let x = 5\n"
//                  "     _in  x + 1) * 3");
    }

        // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
        // where _let would have needed parentheses body the surrounding context
        // (that is, if the _let used body place of the whole * would need parentheses,
        // then it still needs parentheses within the right-hand size of *).
    SECTION("new_edge") {
        CHECK( (NEW(MultExpr)(NEW(NumExpr) (2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x") , NEW(NumExpr)(1)) ) )) -> to_string_pretty()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");

        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1)))-> to_string_pretty()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");

        CHECK( (NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1))), NEW(NumExpr)(7)))-> to_string_pretty()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");

        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->to_string_pretty()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");

        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       -> to_string_pretty() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * 10 * 10" );

        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))
                       -> to_string_pretty() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}



TEST_CASE( "Let_equals_mine" )
{
    SECTION("Values_same") {
        REQUIRE( (NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) )));
    }

    SECTION("Values_same_different_rhs") {
        REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) )));
    }

    SECTION("Values_same_different_lhs") {
        REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(LetExpr)("y", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) )));
    }

    SECTION("Values_same_different_body") {
        REQUIRE( !((NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(
                NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y")) ))) );
    }

    SECTION("different_types") {
        REQUIRE( !(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("x")) ))->equals(NEW(MultExpr)(NEW(NumExpr)(3), NEW(VarExpr)("y")) ));
    }
}



TEST_CASE( "Let_print_mine" )
{
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")))) -> to_string()
           == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");

    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1),
                        NEW(LetExpr)("y", NEW(NumExpr)(1),
                                    NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")),
                                                         NEW(VarExpr)("z"))))))
                                    -> to_string() == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))" );
}



TEST_CASE ( "Let_interp_mine" )
{
    SECTION("hw_examples")
    {
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))
                -> interp(Env::empty)->equals(NEW(NumVal)(26)));

        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))
                -> interp(Env::empty)->equals(NEW(NumVal)(30)));
    }

    SECTION( "from_pretty_print_edge" )
    {
        CHECK( (NEW(MultExpr)((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))), NEW(NumExpr)(1))),
                              NEW(NumExpr)(7)))
                        ->interp(Env::empty)->equals(NEW(NumVal)(357))); // 51 * 7

        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(NumExpr)(10))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(1000000)));

        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(VarExpr)("x"))), NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(10000)));

        CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(1),
                            NEW(LetExpr)("y", NEW(NumExpr)(1),
                                        NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                    NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z"))))))
                                                -> interp(Env::empty)->equals(NEW(NumVal)(4)));

        CHECK_THROWS_WITH( ((NEW(LetExpr)("x",
                                         NEW(NumExpr)(1),
                                         NEW(MultExpr)(NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                                                                             NEW(VarExpr)("x"))),
                                                   NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))))
                                    -> interp(Env::empty)->equals(NEW(NumVal)(10000))), "free variable: y" );
    }

    SECTION("bypass_middle_let")
    {
        CHECK ((NEW(LetExpr)("x",
                            NEW(NumExpr)(2),
                            NEW(LetExpr)("z",
                                        NEW(NumExpr)(4),
                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(10)))))
                                     -> interp(Env::empty)->equals(NEW(NumVal)(12)));
    }
}



//----------------------------------------------------------------------------------------------------------------------------------


TEST_CASE( "Thomas' test for pretty_print for LetExpr" )
{
    SECTION( "pretty_print" )
    {
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(
                NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(
                        NEW(VarExpr)("y"), NEW(NumExpr)(2))),
                NEW(VarExpr)("x"))))->to_string_pretty() == "_let x = 5\n"
                                                           "_in  (_let y = 3\n"
                                                           "      _in  y + 2) + x");

        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)(
                "x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(
                1)))->to_string_pretty() == "5 * (_let x = 5\n"
                                            "     _in  x) + 1");

        CHECK((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(
                1)))->to_string_pretty() == "(_let x = 5\n"
                                            " _in  x) + 1");

        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(
                NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->to_string_pretty() == "5 * _let x = 5\n"
                                                                                      "    _in  x + 1");
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(
                NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")),
                NEW(NumExpr)(1))))->to_string_pretty() == "5 * ((_let x = 5\n"
                                                              "      _in  x) + 1)");

        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(LetExpr)("y", NEW(NumExpr)(2), NEW(LetExpr)(
                "z", NEW(NumExpr)(3), NEW(AddExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)(
                        "y")), NEW(VarExpr)("z"))))))->to_string_pretty() == "_let x = 1\n"
                                                                            "_in  _let y = 2\n"
                                                                            "     _in  _let z = 3\n"
                                                                            "          _in  (x + y) + z");

        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1),
                           NEW(LetExpr)("x", NEW(NumExpr)(2),
                                       NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(
                                               NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")),
                                               NEW(VarExpr)("x"))))))->to_string_pretty() == "_let x = 1\n"
                                                                                             "_in  _let x = 2\n"
                                                                                            "     _in  _let x = 3\n"
                                                                                            "          _in  (x + x) + x");
    }
}



TEST_CASE( "Thomas' test " )
{
    SECTION( "num" )
    {
        CHECK((NEW(NumExpr)(10))->to_string() == "10" );
    }

    SECTION( "var" )
    {
        CHECK((NEW(VarExpr)("x"))->to_string() == "x");
    }

    SECTION( "add" )
    {
        CHECK((NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))) ->to_string() == "(10+10)" );

        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                            NEW(NumExpr)(10)))
                             ->to_string()
                             == "((10+10)+10)" );

        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)), NEW(AddExpr)(
                NEW(NumExpr)(10), NEW(NumExpr)(10))))
                ->to_string()
                == "((10+10)+(10+10))" );

        CHECK( (NEW(AddExpr)(NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                                        NEW(NumExpr)(10)), NEW(NumExpr)(10)))
                                         ->to_string()
                                         == "(((10+10)+10)+10)" );
    }

    SECTION( "mult" )
    {
        CHECK((NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)))->to_string() == "(10*10)" );

        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                             NEW(NumExpr)(10)))
                              ->to_string()
                              == "((10*10)*10)" );

        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                             NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))))
                          ->to_string()
                          == "((10*10)*(10*10))");

        CHECK( (NEW(MultExpr)(NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10)),
                                          NEW(NumExpr)(10)),
                             NEW(NumExpr)(10)))
                          ->to_string()== "(((10*10)*10)*10)" );
    }

    SECTION( "mix" )
    {
        CHECK( (NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(VarExpr)("x")),
                            NEW(NumExpr)(10)))
                            ->to_string()
                            == "((10*x)+10)" );

        CHECK( (NEW(MultExpr)(NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(10), NEW(VarExpr)("x")),
                                         NEW(NumExpr)(10)), NEW(NumExpr)(10)))
                                         ->to_string()
                                         == "(((10*x)+10)*10)" );
    }
}



//----------------------------------------------------------------------------------------------------------------------------------



TEST_CASE( "Kevin Tests NumExpr Equality" )
{
    auto numValueZero = NEW(NumExpr)(0);
    auto numValueOne = NEW(NumExpr)(1);
    auto numValueFive1 = NEW(NumExpr)(5);
    auto numValueFive2 = NEW(NumExpr)(5);
    auto numValueNegativeTen1 = NEW(NumExpr)(-10);
    auto numValueNegativeTen2 = NEW(NumExpr)(-10);
    auto numValueOneMillion1 = NEW(NumExpr)(1000000);
    auto numValueOneMillion2 = NEW(NumExpr)(1000000);
    auto numValueNegOneMillion1 = NEW(NumExpr)(1000000);
    auto numValueNegOneMillion2 = NEW(NumExpr)(1000000);

    SECTION( "Equality with same values" )
    {
        SECTION( "Same positive value" )
        {
            REQUIRE( numValueFive1->equals(numValueFive2) == true);
            REQUIRE( numValueOneMillion1->equals(numValueOneMillion2) == true);
        }

        SECTION( "Same negative value" )
        {
            REQUIRE( numValueNegativeTen1->equals(numValueNegativeTen2) == true);
            REQUIRE( numValueNegOneMillion1->equals(numValueNegOneMillion2) == true);
        }

        SECTION( "Both values 0" )
        {
            REQUIRE( numValueZero->equals(numValueZero) == true);
        }
    }

    SECTION( "Equality with different values" )
    {
        SECTION( "Different values with same sign" )
        {
            REQUIRE( numValueOne->equals(numValueFive1) == false);
            REQUIRE( numValueNegOneMillion1->equals(numValueNegativeTen1) == false);
        }

        SECTION( "Different values with different sign" )
        {
            REQUIRE( numValueOne->equals(numValueNegOneMillion1) == false);
            REQUIRE( numValueNegativeTen1->equals(numValueFive1) == false);
        }

        SECTION( "Non-zero compared to zero" )
        {
            REQUIRE( numValueOne->equals(numValueZero) == false);
        }
    }
}



TEST_CASE( "Kevin Tests AddExpr Equality" )
{
    auto addDoubleZero = NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0));
    auto addDoubleZero2 = NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0));
    auto add11 = NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1));
    auto addNegativeOnes = NEW(AddExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1));
    auto addNegativeOnes2 = NEW(AddExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1));
    auto addPosAndNeg = NEW(AddExpr)(NEW(NumExpr)(1000), NEW(NumExpr)(-1999));

    SECTION( "Equivalent AddExpr Expressions" )
    {
        REQUIRE( addDoubleZero->equals(addDoubleZero2) == true);
        REQUIRE( add11->equals(add11) == true);
        REQUIRE( addNegativeOnes->equals(addNegativeOnes2) == true);
    }

    SECTION( "Nonequivalent AddExpr Expressions" )
    {
        REQUIRE( addDoubleZero->equals(add11) == false);
        REQUIRE( addPosAndNeg->equals(addNegativeOnes) == false);
        REQUIRE( add11->equals(addNegativeOnes2) == false);
    }

    SECTION( "Comparison with Non-AddExpr Expressions" )
    {
        REQUIRE( add11->equals(NEW(NumExpr)
                                      (8)) == false);
        REQUIRE( add11->equals( NEW(MultExpr)(NEW(NumExpr)
                                                (0), NEW(NumExpr)
                                                (1))) == false);
        REQUIRE(add11->equals(NEW(VarExpr)((std::string &)"test" )) == false);
    }
}



TEST_CASE( "Kevin Tests MultExpr Equality" )
{
    auto  multDoubleZero = NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0));
    auto  multDoubleZero2 = NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0));
    auto  mult11 = NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1));
    auto  multNegativeOnes = NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1));
    auto  multNegativeOnes2 = NEW(MultExpr)(NEW(NumExpr)(-1), NEW(NumExpr)(-1));
    auto  multPosAndNeg = NEW(MultExpr)(NEW(NumExpr)(1000), NEW(NumExpr)(-1999));
    SECTION( "Equivalent MultExpr Expressions" )
    {
        REQUIRE( multDoubleZero->equals(multDoubleZero2) == true);
        REQUIRE( mult11->equals(mult11) == true);
        REQUIRE( multNegativeOnes->equals(multNegativeOnes2) == true);
    }

    SECTION( "Nonequivalent MultExpr Expressions" )
    {
        REQUIRE( multDoubleZero->equals(mult11) == false);
        REQUIRE( multPosAndNeg->equals(multNegativeOnes) == false);
        REQUIRE( mult11->equals(multNegativeOnes2) == false);
    }

    SECTION( "Comparison with Non-MultExpr Expressions" )
    {
        REQUIRE( mult11->equals(NEW(NumExpr)
                                       (8)) == false);
        REQUIRE( mult11->equals( NEW(AddExpr)
                                        (NEW(NumExpr)
                                                 (0), NEW(NumExpr)
                                                 (1))) == false);
        REQUIRE(mult11->equals(NEW(VarExpr)((std::string &)"test" )) == false);
    }
}



TEST_CASE( "Kevin Tests VarExpr Equality" )
{
    std::string emptyString;
    std::string spaceString = " ";
    std::string shortString1 = "dog";
    std::string shortString2 = "cat";
    std::string longString1 = "abcdefghijklmnop";
    std::string longString2 = "This is a looooong string. Holy moly can you believe how long this string is?";
    std::string escapeCharString = "\n\t\"\\\r";
    auto emptyStringVar1 = NEW(VarExpr)(emptyString);
    auto emptyStringVar2 = NEW(VarExpr)(emptyString);
    auto spaceStringVar1 = NEW(VarExpr)(spaceString);
    auto spaceStringVar2 = NEW(VarExpr)(spaceString);
    auto shortStringVar1 = NEW(VarExpr)(shortString1);
    auto shortStringVar2 = NEW(VarExpr)(shortString2);
    auto longStringVar1 = NEW(VarExpr)(longString1);
    auto longStringVar2 = NEW(VarExpr)(longString1);
    auto longStringVar3 = NEW(VarExpr)(longString2);
    auto longStringVar4 = NEW(VarExpr)(longString2);
    auto escapeCharVar1 = NEW(VarExpr)(escapeCharString);
    auto escapeCharVar2 = NEW(VarExpr)(escapeCharString);
    auto var99("test");

    SECTION( "Equivalent string variables" )
    {
        REQUIRE( emptyStringVar1->equals(emptyStringVar2) == true);
        REQUIRE( spaceStringVar1->equals(spaceStringVar2) == true);
        REQUIRE( emptyStringVar1->equals(emptyStringVar2) == true);
        REQUIRE( shortStringVar1->equals(shortStringVar1) == true);
        REQUIRE( longStringVar3->equals(longStringVar4) == true);
        REQUIRE( escapeCharVar1->equals(escapeCharVar2) == true);
    }

    SECTION( "Nonequivalent string variables" )
    {
        REQUIRE( emptyStringVar1->equals(spaceStringVar1) == false);
        REQUIRE( longStringVar1->equals(longStringVar3) == false);
        REQUIRE( escapeCharVar1->equals(shortStringVar2) == false);
        REQUIRE( shortStringVar1->equals(shortStringVar2) == false);
    }
}



TEST_CASE( "Kevin Tests Interpret Tests" )
{
    SECTION( "NumExpr interp(Env::empty) tests" )
    {
        CHECK( (NEW(NumExpr)
                (0))->interp(Env::empty)->equals(NEW(NumVal)(0)));
        CHECK( (NEW(NumExpr)
                (-1))->interp(Env::empty)->equals(NEW(NumVal)(-1)));
        CHECK( (NEW(NumExpr)
                (1))->interp(Env::empty)->equals(NEW(NumVal)(1)));
        CHECK( (NEW(NumExpr)
                (123456789))->interp(Env::empty)->equals(NEW(NumVal)(123456789)));
        CHECK( (NEW(NumExpr)
                (-66665555))->interp(Env::empty)->equals(NEW(NumVal)(-66665555)));
    }

    SECTION( "AddExpr interp(Env::empty) tests" )
    {
        CHECK( (NEW(AddExpr)
                (NEW(AddExpr)
                         (NEW(NumExpr)
                                  (10), NEW(NumExpr)
                                  (15)),NEW(AddExpr)(NEW(NumExpr)
                                                        (20), NEW(NumExpr)
                                                        (20))))
                       ->interp(Env::empty)->equals(NEW(NumVal)(65)));
        CHECK( (NEW(AddExpr)
                (NEW(NumExpr)
                         (-99), NEW(NumExpr)
                         (99)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
        CHECK( (NEW(AddExpr)
                ( NEW(NumExpr)
                          (0), NEW(NumExpr)
                          (-123456789)))->interp(Env::empty)->equals(NEW(NumVal)(-123456789)));
    }

    SECTION( "MultExpr interp(Env::empty) tests" )
    {
        CHECK( (NEW(MultExpr)(NEW(NumExpr)
                                 (3), NEW(NumExpr)
                                 (2)))
                       ->interp(Env::empty)->equals(NEW(NumVal)(6)) );

        CHECK( (NEW(MultExpr)(NEW(AddExpr)(
                NEW(NumExpr)
                        (0),NEW(MultExpr)(NEW(NumExpr)
                                             (99),
                                         NEW(NumExpr)
                                             (2))), NEW(NumExpr)
                                  (-1)))->interp(Env::empty)->equals(NEW(NumVal)(-198)));

        CHECK( (NEW(MultExpr)(NEW(NumExpr)
                                 (12355661), NEW(NumExpr)
                                 (0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    }

//    SECTION( "VarExpr interp(Env::empty) tests" )
//    {
//        CHECK_THROWS_WITH( (NEW(VarExpr)("x"))->interp(Env::empty), "Error occurs -> CANNOT interpret value of \"Variable\"" );
//        CHECK_THROWS_WITH( (NEW(VarExpr)("\n"))->interp(Env::empty), "Error occurs -> CANNOT interpret value of \"Variable\"" );
//    }
}




// Subst func removed

//TEST_CASE( "Kevin Tests subst() test cases" )
//{
//    SECTION( "Tests where substitution occurs" )
//    {
//        CHECK( (NEW(VarExpr)("x"))
//                       ->subst("x", NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(-99)))
//                       ->equals(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(-99))) );
//        CHECK( (NEW(AddExpr)
//                (NEW(VarExpr)("x"), NEW(NumExpr)
//                        (7)))
//                       ->subst("x", NEW(VarExpr)("y"))
//                       ->equals(NEW(AddExpr)
//                                        (NEW(VarExpr)("y"), NEW(NumExpr)
//                                                (7))) );
//        CHECK( (NEW(VarExpr)("x"))
//                       ->subst("x", NEW(AddExpr)
//                               (NEW(VarExpr)("y"), NEW(NumExpr)
//                                       (7)))
//                       ->equals(NEW(AddExpr)
//                                        (NEW(VarExpr)("y"), NEW(NumExpr)
//                                                (7))) );
//    }
//}