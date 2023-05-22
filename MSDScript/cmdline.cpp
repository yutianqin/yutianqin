#define CATCH_CONFIG_RUNNER
#include "headers/catch.h"
#include "headers/parse.hpp"
#include "headers/cmdline.hpp"
#include "iostream"
using namespace std;


run_mode_t use_arguments( int argc, char **argv )
{
    if ( argc > 0 ) {
        for ( int i = 1; i < argc; i++ ) {
            if ( strcmp( argv[i],"--help" ) == 0 ) {
                cout << "do it later" << endl;
                exit(EXIT_SUCCESS);
            }
            else if ( strcmp( argv[i],"--test" ) == 0 ) {
                if ( Catch::Session().run(1, argv) == EXIT_SUCCESS ) {
                    cout << "test passed :)" << endl;
                    exit(EXIT_SUCCESS);
                } else {
                    cout << "test failed :(" << endl;
                    exit(EXIT_FAILURE);
                }
            }

            if ( strcmp( argv[i],"--interp" ) == 0 ) {
                return do_interp;
            }
            else if ( strcmp( argv[i],"--print" ) == 0 ) {
                return do_print;
            }
            else if ( strcmp( argv[i],"--pretty_print" ) == 0 ) {
                return do_pretty_print;
            }
            else {
                cerr << R"(Error occurs, try "--test" and "--help")" << endl;
                return do_nothing;
            }
        }
        return do_nothing;
    }
    else return do_nothing;
}































//void use_arguments( int argc, char **argv )
//{
//    bool testAlready = false;
//    if ( argc > 0 ) {
//        for ( int i = 1; i < argc; i++ ) {
//            if ( strcmp( argv[i],"--help" ) == 0 ) {
//                cout << "Try \"./msdscript --test\"" << endl;
//                exit(0);
//            } else if ( strcmp( argv[i],"--test" ) == 0 ) {
//                if ( !testAlready ) {
//                    testAlready = true;
//                    if ( Catch::Session().run(1, argv) == EXIT_SUCCESS ){
//                        cout << "test passed" << endl;
//                        exit(EXIT_SUCCESS);
//                    }
//                    else {
//                        cout << "test failed" << endl;
//                        exit(EXIT_FAILURE);
//                    }
//                }
//                else {
//                    cerr << "Already Tested" << endl;
//                    exit(EXIT_FAILURE);
//                }
//            } else {
//                cerr << R"(Error occurs, try "--test" and "--help")" << endl;
//                exit(1);
//            }
//        }
//    }
//    else return;
//}