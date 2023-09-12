//
// Created by Jlisowskyy on 08/08/2023.
//

#ifndef INTERPRETER_GLOBALVALUES_H
#define INTERPRETER_GLOBALVALUES_H

#include <cstdlib>
#include <cstdio>

#define DEBUG_

constexpr const char* parserDebugDestDefault { R"(C:/Users/Jlisowskyy\Desktop\Projekty\Interpreter\DebugLogs\)" };
constexpr const char* parserLogsDestDefault { R"(C:\Users\Jlisowskyy\Desktop\Projekty\Interpreter\Logs\)" };

constexpr char commentChar { '#' };
constexpr char EOL { '\n' };
constexpr size_t ASCII_SIZE { 128 };
constexpr size_t MAX_FILE_SIZE { 33554432 }; // 32 MiB

constexpr char SPECCHAR_BORDER {'A'};
    // value beneath which all chars are treated as special ones and needs to be interpreted as ones

constexpr bool isComment(const char x){
    return x == commentChar;
}

constexpr bool isSpace(const char x){
    return x == ' ';
}

constexpr bool isEOL(const char x){
    return x == EOL;
}

constexpr bool isNotBlank(const char x){
    return x != ' ' && x != EOL;
}

constexpr bool isBlank(const char x){
    return x == ' ' || x == EOL;
}

constexpr bool isConstChar(const char x){
    return x == '"';
}

constexpr bool isEOF(const char x){
    return x == EOF;
}

#endif //INTERPRETER_GLOBALVALUES_H
