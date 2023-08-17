cmake_minimum_required(VERSION 3.26)
project(Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Interpreter
        main.cpp
        include/parserUnit.h
        src/MyMathInterpreter.cpp
        include/globalMacros.h
        src/parserUnit.cpp
        include/token.h
        src/token.cpp
        include/errors.h
        include/tokenizer.h
        src/tokenizer.cpp)

target_compile_options(Interpreter PUBLIC -Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;)