#include <iostream>
#include "headers/Env.h"
#include "headers/parse.hpp"
#include "headers/cmdline.hpp"
#include "headers/pointer.h"

/**
* \mainpage MSDScript -> CS6015_project
* \author Yutian Qin
* \date 02-07-2023
*/
int main( int argc, char **argv ) {
    try {
        if ( use_arguments(argc,argv) == do_nothing ) {
            exit(EXIT_SUCCESS);
        }
        else {
            PTR(Expr) e = parse_expr(std::cin);

            switch( use_arguments(argc,argv) )
            {
                case do_nothing: break;

                case do_interp:
                    std::cout << e->interp(Env::empty)->to_string() << std::endl;
                    exit(EXIT_SUCCESS);

                case do_print:
                    std::cout << e->to_string() << std::endl;
                    exit(EXIT_SUCCESS);

                case do_pretty_print:
                    std::cout << e->to_string_pretty() << std::endl;
                    exit(EXIT_SUCCESS);
            }
        }
        return EXIT_SUCCESS;
    }
    catch ( std::runtime_error &exn )
    {
        std::cerr << exn.what() << "\n";
        return EXIT_FAILURE;
    }
}






