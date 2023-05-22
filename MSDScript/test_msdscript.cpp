#include "headers/test_msdscript.h"
#include <cstdlib>
#include <ctime>
#include "headers/exec.h"


int main( int argc, char **argv )
{
    srand(time(nullptr));
//    srand(0);// for testing

    const char * const interp1_argv[] = { "msdscript", "--interp" };
    const char * const interp2_argv[] = { "test_executables/msdscript3", "--interp" };

    const char * const print1_argv[] = { "msdscript", "--print" };
    const char * const print2_argv[] = { "test_executables/msdscript3", "--print" };

    const char * const pretty_print1_argv[] = { "msdscript", "--pretty_print" };
    const char * const pretty_print2_argv[] = { "test_executables/msdscript3", "--pretty_print" };


    for ( int i = 0; i < 100; i++ )
    {
        std::string in = random_expr_string();
        std::cout<< "Trying " << in << "\n";

        // Interp
        ExecResult interp1_result = exec_program( 2, interp1_argv, in );
        ExecResult interp2_result = exec_program( 2, interp2_argv, in );
        if ( interp1_result.out != interp2_result.out ){
            std::cout << "interp:" << std::endl
                << "'" << interp1_result.out << "' != '" << interp2_result.out << "'" << std::endl;
//            throw std::runtime_error("different results");
        }

        // Print
        ExecResult print1_result = exec_program( 2, print1_argv, in );
        ExecResult print2_result = exec_program( 2, print2_argv, in );
        if ( print1_result.out != print2_result.out ){
            std::cout << "print:" << std::endl
                << "'" << print1_result.out << "' != '" << print2_result.out << "'" << std::endl;
//            throw std::runtime_error("different results");
        }

        // Pretty Print
        ExecResult pretty_print1_result = exec_program( 2, pretty_print1_argv, in );
        ExecResult pretty_print2_result = exec_program( 2, pretty_print2_argv, in );
        if ( pretty_print1_result.out != pretty_print2_result.out ){
            std::cout << "pretty print:" << std::endl
                      << "'" << pretty_print1_result.out << "' != '" << pretty_print2_result.out << "'" << std::endl;
//            throw std::runtime_error("different results");
        }
    }
    return 0;
}


std::string random_expr_string()
{
    // Generate a random number between 0 and 9 (inclusive)
    int probability = rand() % 100;

    // NumExpr expr  // 0-59 50%
    if ( probability < 68 )
        return std::to_string(rand()); // return an int

    // AddExpr expr  // 60-69 10%
    else if ( probability < 75 )
        return "(" + random_expr_string() + "+" + random_expr_string() + ")";

    // MultExpr expr // 70-90 21%
    else if ( probability < 95 )
        return "(" + random_expr_string() + "*" + random_expr_string() + ")";

    // LetExpr expr // 95-99 10% -> this probability should be <= 10%
    else return "_let " + random_expr_var() + "=" + random_expr_string()
              + "_in  " + random_expr_string();

}


std::string random_expr_var()
{
    // Generate a random number between 0 and 25 (inclusive)
    int randomNumber = rand() % 26;

    // AddExpr 65 to the random number to get a value between 65 and 90,
    // which correspond to the ASCII codes for uppercase letters A-Z
    std::string randomChar= reinterpret_cast<const char *>( static_cast<char>(randomNumber + 65) );

    return randomChar;
}