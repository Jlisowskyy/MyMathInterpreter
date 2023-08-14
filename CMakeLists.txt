cmake_minimum_required(VERSION 3.26)
project(Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Interpreter
        main.cpp
        include/ParserUnit.h
        src/MyMathInterpreter.cpp
        include/GlobalMacros.h
        src/ParserUnit.cpp)

#target_compile_options(ParallelNumeric PUBLIC -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-mavx;-mavx2;-mfma;-O3)