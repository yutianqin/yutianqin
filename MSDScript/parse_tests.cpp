#include "headers/parse.hpp"
#include "headers/catch.h"
#include "headers/pointer.h"
#include <sstream>
using namespace std;



//----------------------------------------------------------------------------------------------------------------------
TEST_CASE("parse FunExpr test")
{
    CHECK(parse_str("(_fun (x) x+2)(1)")
                        ->equals(NEW(CallExpr)(NEW(FunExpr)("x",
                                                                           NEW(AddExpr)(NEW(VarExpr)("x"),
                                                                                             NEW(NumExpr)(2))),
                                                    NEW(NumExpr)(1))));
    CHECK(parse_str("_let f = _fun (x) x*x\n"
                        "_in f(2)")
                        ->equals(NEW(LetExpr)("f",
                                                  NEW(FunExpr)("x",
                                                                  NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                                     NEW(VarExpr)("x"))),
                                                  NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                    NEW(NumExpr)(2)))));
    CHECK(parse_str("_let f = _fun (x)\n"
                        "           _fun (y)\n"
                        "             x*x + y*y\n"
                        "_in f(2)(3)")
                        ->equals(NEW(LetExpr)("f",
                                                 NEW(FunExpr)("x",
                                                                  NEW(FunExpr)("y",
                                                                                    NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("x"),
                                                                                                                       NEW(VarExpr)("x")),
                                                                                                      NEW(MultExpr)(NEW(VarExpr)("y"),
                                                                                                                        NEW(VarExpr)("y"))))),
                                                 NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                           NEW(NumExpr)(2)),
                                                                   NEW(NumExpr)(3)))));
    CHECK(parse_str("_let y = 8\n"
                        "_in _let f = _fun (x) x*y\n"
                        "    _in f(2)")
                        ->equals(NEW(LetExpr)("y",
                                                  NEW(NumExpr)(8),
                                                   NEW(LetExpr)("f",
                                                                  NEW(FunExpr)("x",
                                                                                  NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("y"))),
                                                                   NEW(CallExpr)(NEW(VarExpr)("f"),
                                                                                    NEW(NumExpr)(2))))));
    CHECK(parse_str("_let factrl = _fun (factrl)\n"
                        "                _fun (x)\n"
                        "                  _if x == 1\n"
                        "                  _then 1\n"
                        "                  _else x * factrl(factrl)(x + -1)\n"
                        "_in  factrl(factrl)(10)")->interp(Env::empty)->to_string() == "3628800");


    SECTION("Lydia's tests")
    {
        // 1
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in f(5) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 2
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           7\n"
                                     "_in f(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));

        // 3
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           _true\n"
                                     "_in f(5) ")->interp(Env::empty)->equals(NEW(BoolVal)(true)));

        // 4
        REQUIRE_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                                 "           x + _true\n"
                                                 "_in f(5) ")->interp(Env::empty), "add to non-number");

        // 5
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           x + _true\n"
                                     "_in 5 + 1 ")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 6
        REQUIRE_THROWS_WITH(parse_str("_let f = _fun (x)\n"
                                                 "           7\n"
                                                 "_in  f(5 + _true)")->interp(Env::empty), "add to non-number");

        // 7
        REQUIRE_THROWS_WITH(parse_str("_let f = _fun (x) x+ 1\n"
                                                 "_in f + 5")->interp(Env::empty),"Cannot perform add_to for FunVal");

        // 8
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in _if _false\n"
                                     "    _then f(5)\n"
                                     "    _else f(6)")->interp(Env::empty)->equals(NEW(NumVal)(7)));

        // 9
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in _let g = _fun (y) y+ 2 \n"
                                     "_in _if _true\n"
                                     "    _then f(5)\n"
                                     "    _else g(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 10
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in _let g = _fun (y) y+ 2 \n"
                                     "_in f(g(5)) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));

        // 11
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in _let g = _fun (y)\n"
                                     "              f(y + 2)\n"
                                     "_in g(5) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));

        // 12
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in _let g = _fun (x)\n"
                                     "              f(2) + x\n"
                                     "_in g(5) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));

        // 13
        REQUIRE_THROWS_WITH(parse_str("_let f = _fun (x) x+ 1 \n"
                                                 "_in f 5 ")->interp(Env::empty), "invalid input");

        // 14
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in (f)(5) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 15
        PTR(AddExpr) add_x_1 = NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1));
        PTR(FunVal) fun_val_x_add_x_1 = NEW(FunVal)("x", add_x_1, Env::empty);
        REQUIRE(parse_str("_fun (x) x+ 1 ")->interp(Env::empty)->equals(fun_val_x_add_x_1));

        //16
        REQUIRE(parse_str("_let f = _fun (x) x+ 1 \n"
                                     "_in f ")->interp(Env::empty)->equals(fun_val_x_add_x_1));

        // 17
        REQUIRE(parse_str("(_fun (x)\n"
                                     "   x + 1)(5)")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 18
        REQUIRE(parse_str("_let f = _if _false\n"
                                     "            _then _fun (x)  \n"
                                     "                        x+ 1 \n"
                                     "           _else _fun (x)\n"
                                     "                       x+ 2\n"
                                     "_in f(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));

        // 19
        REQUIRE(parse_str("(_if _false \n"
                                     "  _then _fun (x)\n"
                                     "            x+ 1\n"
                                     "   _else _fun (x)\n"
                                     "                x + 2)(5)")->interp(Env::empty)->equals(NEW(NumVal)(7)));

        // 20
        REQUIRE(parse_str("_let f = _fun (g)\n"
                                     "           g(5)\n"
                                     "_in _let g = _fun (y)  \n"
                                     "             y + 2\n"
                                     "_in f(g) ")->interp(Env::empty)->equals(NEW(NumVal)(7)));

        // 21
        REQUIRE(parse_str("_let f = _fun (g)\n"
                                     "           g(5)\n"
                                     "_in f(_fun (y)\n"
                                     "        y + 2)")->interp(Env::empty)->equals(NEW(NumVal)(7)));

        // 22
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           _fun (y)\n"
                                     "x+ y _in (f(5))(1) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 23
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           _fun (y)\n"
                                     "x+ y _in f(5)(1) ")->interp(Env::empty)->equals(NEW(NumVal)(6)));

        // 24
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           _fun (g)\n"
                                     "             g(x + 1)\n"
                                     "_in _let g = _fun (y)\n"
                                     "              y+ 2 \n"
                                     "_in (f(5))(g) ")->interp(Env::empty)->equals(NEW(NumVal)(8)));

        // 25
        REQUIRE(parse_str("_let f = _fun (x)\n"
                                     "           _fun (g)\n"
                                     "             g(x + 1)\n"
                                     "_in _let g = _fun (y)\n"
                                     "y+ 2 _in f(5)(g)")->interp(Env::empty)->equals(NEW(NumVal)(8)));

        // 26
        REQUIRE(parse_str("_let f = _fun (f)\n"
                                     "           _fun (x)\n"
                                     "             _if x == 0\n"
                                     "             _then 0\n"
                                     "             _else x + f(f)(x + -1)\n"
                                     "_in f(f)(3)")->interp(Env::empty)->equals(NEW(NumVal)(6)));
    }
}



//----------------------------------------------------------------------------------------------------------------------
TEST_CASE("parse IfExpr BoolExpr test")
{
    CHECK( parse_str("_if 4 + 1\n"
                         "_then 2\n"
                         "_else 3") ->equals(NEW(IfExpr)(NEW(AddExpr)(NEW(NumExpr)(4),NEW(NumExpr)(1)),
                                                            NEW(NumExpr)(2),
                                                            NEW(NumExpr)(3))) );
    CHECK_THROWS_WITH( parse_str("_if 4 + 1\n"
                                     "_then 2\n"
                                     "_else 3") ->interp(Env::empty), "check bool for num" );

    CHECK( parse_str("_let x = _true + 1\n"
                         "_in  _if _true\n"
                         "     _then 5\n"
                         "     _else x")
                         ->equals(NEW(LetExpr)("x",
                                                   NEW(AddExpr)(NEW(BoolExpr)(true),NEW(NumExpr)(1)),
                                                    NEW(IfExpr)(NEW(BoolExpr)(true),
                                                                  NEW(NumExpr)(5),
                                                                  NEW(VarExpr)("x")))) );

    CHECK_THROWS_WITH( parse_str("_let x = _true + 1\n"
                                     "_in  _if _true\n"
                                     "     _then 5\n"
                                     "     _else x") ->interp(Env::empty), "add to non-number" );

    SECTION( "William's parsing tests from quiz" )
    {
        CHECK( (parse_str ( "_if 1 == 2 _then 5 _else 6")) ->
                equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(5), NEW(NumExpr)(6))));

        CHECK( (((parse_str ( "_if 1 == 2 _then 5 _else 6")) -> interp(Env::empty)) -> to_string()) == "6");
        CHECK( (((parse_str ( "1 == 2")) -> interp(Env::empty)) -> to_string()) == "_false");
        CHECK( (((parse_str ( "(1 + 2) == (3 + 0)")) -> interp(Env::empty)) -> to_string()) == "_true");
        CHECK( (((parse_str ( "1 + 2 == 3 + 0")) -> interp(Env::empty)) -> to_string()) == "_true");

        // temp test
//        std::string test = ((parse_str("1 + 2 == 3 + 0"))->interp(Env::empty))->to_string();
//        std::cout << "result: " << test << endl;

        CHECK_THROWS_WITH(( (((parse_str ( "(1 == 2) + 3 ")) -> interp(Env::empty)) -> to_string()) == "_true"), "add to non-number");
        CHECK( (((parse_str ( "1==2+3")) -> interp(Env::empty)) -> to_string()) == "_false");
        CHECK( (((parse_str ( "_if _false\n"
                              "_then 5\n"
                              "_else 6")) -> interp(Env::empty)) -> to_string()) == "6");
        CHECK( (((parse_str ( "_if _false\n"
                              "_then _false\n"
                              "_else _true")) -> interp(Env::empty)) -> to_string()) == "_true");
        CHECK( (((parse_str ( "_if _false\n"
                              "_then 5\n"
                              "_else _false")) -> interp(Env::empty)) -> to_string()) == "_false");
        CHECK_THROWS_WITH(( (((parse_str ( "_true + _false")) -> interp(Env::empty)) -> to_string()) == "_false"), "add to non-number");
        CHECK_THROWS_WITH(( (((parse_str ( "_true + 1")) -> interp(Env::empty)) -> to_string()) == "_false"), "add to non-number");
        CHECK( (((parse_str ( "_true == _true")) -> interp(Env::empty)) -> to_string()) == "_true");
        CHECK( (((parse_str ( "1 == _true")) -> interp(Env::empty)) -> to_string()) == "_false");
        CHECK_THROWS_WITH(( (((parse_str ( "_if 1 + 2\n"
                                           "_then _false\n"
                                           "_else _true")) -> interp(Env::empty)) -> to_string()) == "_false"), "check bool for num");
        CHECK( (((parse_str ( "_if _true\n"
                              "_then 5\n"
                              "_else _true + 1")) -> interp(Env::empty)) -> to_string()) == "5");
        CHECK_THROWS_WITH(( (((parse_str ( "_if _false\n"
                                           "_then 5\n"
                                           "_else _true + 1")) -> interp(Env::empty)) -> to_string()) == "_false"), "add to non-number");
        CHECK_THROWS_WITH(( (((parse_str ( "_let x = _true + 1\n"
                                           "_in  _if _true\n"
                                           "     _then 5\n"
                                           "     _else x")) -> interp(Env::empty)) -> to_string()) == "_false"), "add to non-number");
        CHECK_THROWS_WITH(( (((parse_str ( "_let x = _true + 1\n"
                                           "_in  _if _true\n"
                                           "     _then 5\n"
                                           "     _else x")) -> interp(Env::empty)) -> to_string()) == "_false"), "add to non-number");
        CHECK( (((parse_str ( "(_if _true\n"
                              " _then 5\n"
                              " _else _true) + 1")) -> interp(Env::empty)) -> to_string()) == "6");
        CHECK( (((parse_str ( "_if (_if 1 == 2\n"
                              "     _then _false\n"
                              "     _else _true)\n"
                              "_then 5\n"
                              "_else 6")) -> interp(Env::empty)) -> to_string()) == "5");
        CHECK( (((parse_str ( "_if (_if 1 == 2\n"
                              "     _then _true\n"
                              "      _else _false)\n"
                              "_then 5\n"
                              "_else 6")) -> interp(Env::empty)) -> to_string()) == "6");
    }
}



//----------------------------------------------------------------------------------------------------------------------
TEST_CASE("parse LetExpr test")
{
    CHECK( parse_str("_let x=5 _in 995+x")->interp(Env::empty)->equals(NEW(NumVal)(1000)));

    CHECK( parse_str("(_let x=44 _in x) + 10")
                ->equals ( NEW(AddExpr)(NEW(LetExpr)("x",
                                           NEW(NumExpr)(44),
                                           NEW(VarExpr)("x")),
                               NEW(NumExpr)(10))) );


    PTR(LetExpr) tripleNestedLet1 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                           NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                                   NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")));

    PTR(LetExpr) tripleNestedLet2 =NEW(LetExpr)("x", NEW(NumExpr)(5),
                                           NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                       NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))));

    PTR(LetExpr) tripleNestedLet3 = NEW(LetExpr)("x", NEW(NumExpr)(1),
                                            NEW(LetExpr)("y", NEW(NumExpr)(1),
                                                        NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)),
                                                                    NEW(MultExpr)(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")), NEW(VarExpr)("z")))));
    CHECK( parse_str("_let x = 5\n"
                         "_in  (_let y = 3\n"
                         "      _in  y + 2) + x") ->equals( tripleNestedLet1 ) );

    CHECK( parse_str("_let x = 5\n"
                         "_in  _let y = 3\n"
                         "     _in  y + 2") ->equals( tripleNestedLet2 ) );

    CHECK(parse_str("_let x = 1\n"
                        "_in  _let y = 1\n"
                        "     _in  _let z = x + 1\n"
                        "          _in  (x + y) * z") ->equals( tripleNestedLet3 ) );
}

//----------------------------------------------------------------------------------------------------------------------

TEST_CASE("Nabil's test for parse")
{
//    This is some temporary debugging code:
//    std::istringstream in( "(   _let    y   = 4 _in  "
//                              "   y        )        +  5" );
//    parse_expr(in)
//    ->print(std::cout);
//    cout << endl;

  CHECK_THROWS_WITH( parse_str("()"), "invalid input" );

  CHECK( parse_str("(1)")->equals(NEW(NumExpr)(1)) );
  CHECK( parse_str("(((1)))")->equals(NEW(NumExpr)(1)) );

  CHECK_THROWS_WITH( parse_str("(1"), "missing close parenthesis" );

  CHECK( parse_str("1")->equals(NEW(NumExpr)(1)) );
  CHECK( parse_str("10")->equals(NEW(NumExpr)(10)) );
  CHECK( parse_str("-3")->equals(NEW(NumExpr)(-3)) );
  CHECK( parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)) );

  CHECK_THROWS_WITH( parse_str("-"), "invalid input" );

  CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );

  CHECK( parse_str("x")->equals(NEW(VarExpr)("x")) );
  CHECK( parse_str("xyz")->equals(NEW(VarExpr)("xyz")) );
  CHECK( parse_str("xYz")->equals(NEW(VarExpr)("xYz")) );

  CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );

  CHECK( parse_str("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );
  CHECK( parse_str("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

  CHECK( parse_str("z * x + y")
        ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                             NEW(VarExpr)("y"))) );

  CHECK( parse_str("z * (x + y)")
        ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) ));
}

