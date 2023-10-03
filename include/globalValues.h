//
// Created by Jlisowskyy on 08/08/2023.
//

#ifndef INTERPRETER_GLOBALVALUES_H
#define INTERPRETER_GLOBALVALUES_H

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <complex>

#define DEBUG_

// TODO: CHECK FOR BETTER ORDER OF TOKENS TYPES

constexpr const char* parserDebugDestDefault { R"(DebugLogs/)" };
constexpr const char* parserLogsDestDefault { R"(Logs/)" };

using FloatingPointType = double;
using IntegerType = long long int;
using VectorType = char; // TODO
using MatrixType = float; // TODO

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

struct stringCmp{
    bool operator()(const char* a, const char* b) const {
        return std::strcmp(a, b) == 0;
    }
};

// TODO: ordered by usage frequency???
enum class tokenType{
    VAR,
    SEPARATOR,
    PROC,
    BIN_OP,
    KEYWORD,
    UN_OP,
    CONST,
    UNKNOWN,
};

enum class separatorType{
    SEMI_COLON,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSED,
    COMMA,
    ANGLE_BRACKETS_OPEN,
    ANGLE_BRACKETS_CLOSED,
    COLON,
    UNKNOWN,
};

enum class keywordType{
    IF,
    ENDIF,
    FOR,
    END,
    ELSE,
    ELIF,
    WHILE,
    UNKNOWN
};

enum class binOpType{
    ADD,
    SUB,
    MULT,
    POW,
    DIVIDE,
    MOD,
    AND,
    OR,
    SMALLER_THAN,
    BIGGER_THAN,
    EQUAL,
    SMALLER_EQUAL_THAN,
    BIGGER_EQUAL_THAN,
    ASSIGN,
    UNKNOWN,
};

enum class unaryOpType{
    LOGICAL_NEGATION,
    MATHEMATICAL_NEGATION,
    UNKNOWN,
};

enum class dataType {
    floatingPoint,
    matrix,
    vector,
    integer,
    constChar,
    voidType,
};

// TODO: ARRAYS

static const char* tTypeNames[] ={
        "VAR",
        "SEPARATOR",
        "PROC",
        "BIN_OP",
        "KEYWORD",
        "UN_OP",
        "CONST",
        "UNKNOWN",
};

static const char* keyWordTypeNames[] = {
        "IF",
        "ENDIF",
        "FOR",
        "END",
        "ELSE",
        "ELIF",
        "WHILE",
        "UNKNOWN KEYWORD",
};

static const char* constTypeNames[] = {
        "FLOATING_POINT",
        "INTEGER",
        "CONST_CHAR",
        "ERROR_CONST"
};

static const char* separatorTypeNames[] ={
        "SEMI_COLON",
        "PARENTHESIS_OPEN",
        "PARENTHESIS_CLOSED",
        "COMMA",
        "ANGLE_BRACKETS_OPEN",
        "ANGLE_BRACKETS_CLOSED",
        "COLON",
        "UNKNOWN",
};

static const char* separatorTypeSymbols[]={
        ";",
        "(",
        ")",
        ",",
        "[",
        "]",
        ":",
        "ERROR SEP TYPE",
};

static const char* binOpTypeNames[] = {
        "ADD",
        "SUB",
        "MULT",
        "POW",
        "DIVIDE",
        "MOD",
        "AND",
        "OR",
        "SMALLER_THAN",
        "BIGGER_THAN",
        "EQUAL",
        "SMALLER_EQUAL_THAN",
        "BIGGER_EQUAL_THAN",
        "ASSIGN",
        "ERROR_BINOP"
};

static const char* binOpTypeSymbols[]={
        "+",
        "-",
        "*",
        "^",
        "/",
        "%",
        "&",
        "|",
        "<",
        ">",
        "==",
        "<=",
        ">=",
        "=",
        "ERROR_UNOP",
};

static const char* unaryOpTypeNames[] = {
        "LOGICAL_NEGATION",
        "MATHEMATICAL_NEGATION",
        "ERROR_UNOP"
};

static const char* unaryOpTypeSymbols[] = {
        "!",
        "-",
        "ERROR_UNOP",
};

#endif //INTERPRETER_GLOBALVALUES_H