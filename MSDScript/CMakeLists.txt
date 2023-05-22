cmake_minimum_required(VERSION 3.24)
project(msd_cs6015_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(msd_cs6015_project
        main.cpp
        Test.cpp
        subTest.cpp
        cmdline.cpp
        headers/cmdline.hpp
        Expr.cpp
        headers/Expr.hpp
        parse_tests.cpp
        parse.cpp
        headers/parse.hpp
        val.cpp
        headers/val.h
        headers/pointer.h Env.cpp headers/Env.h main.cpp Expr.cpp Expr.cpp Expr.cpp)

add_executable(test_msdscript
        exec.cpp
        headers/exec.h
        test_msdscript.cpp
        headers/test_msdscript.h)

