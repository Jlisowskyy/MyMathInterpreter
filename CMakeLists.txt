cmake_minimum_required(VERSION 3.26)
project(Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Interpreter
        main.cpp
        include/parserUnit.h
        src/MyMathInterpreter.cpp
        include/globalValues.h
        src/parserUnit.cpp
        include/token.h
        src/token.cpp
        include/errors.h
        include/lexerUnit.h
        src/lexerUnit.cpp
        include/InterpretingUnit.h
        src/InterpretingUnit.cpp
        include/AST.h
        include/operatorsProc.h
        src/AST.cpp
        src/operatorsProc.cpp
        include/StdBasedHashTable.h)

target_compile_options(Interpreter PUBLIC -Wall;-Wextra;-Wformat=2;-Wunused;-march=native;)