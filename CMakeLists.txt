cmake_minimum_required(VERSION 3.26)
project(Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(Interpreter
        main.cpp
        include/Interpreter.h
        src/MyMathInterpreter.cpp
        include/globalValues.h
        src/Interpreter.cpp
        include/Token.h
        include/errors.h
        include/LexerUnit.h
        src/LexerUnit.cpp
        include/InterpretingUnit.h
        src/InterpretingUnit.cpp
        include/AST.h
        include/operatorsProc.h
        src/AST.cpp
        src/operatorsProc.cpp
        include/VariableHashTable.h
        include/GarbageCollector.h)

target_compile_options(Interpreter PUBLIC -Wall;-Wextra;-Wformat=2;-Wunused;-march=native;-O3;)