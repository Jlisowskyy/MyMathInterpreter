//
// Created by Jlisowskyy on 15/08/2023.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include "globalValues.h"

// TODO: consider using UNKNOWN = 0 to make initialisation easier !!!

class token{
public:
    struct tokenInfo{
        enum class tokenType{
            VAR,
            CONST,
            PROC,
            BIN_OP,
            UN_OP,
            KEYWORD,
            SEPARATOR,
            CSEPARATOR,
            UNKNOWN
        } tType { tokenType::UNKNOWN };

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
            UNKNOWN
        } bOpType { binOpType::UNKNOWN };

        enum class unOpType{
            LOGICAL_NEGATION,
            MATHEMATICAL_NEGATION,
            UNKNOWN
        } uOpType { unOpType::UNKNOWN };

        enum class keywordType{
            IF,
            ELIF,
            ELSE,
            ENDIF,
            FOR,
            WHILE,
            END,
            UNKNOWN
        } kWordType{ keywordType::UNKNOWN };

        enum class constType{
            FLOATING_POINT,
            INTEGER,
            CONST_CHAR,
            UNKNOWN
        } cType { constType::UNKNOWN };
        // FUNC PTR

        explicit tokenInfo(binOpType Type):
            tType{ tokenType::BIN_OP }, bOpType{ Type } {}

        explicit tokenInfo(unOpType Type):
            tType{ tokenType::UN_OP }, uOpType{ Type } {}

        explicit tokenInfo(keywordType Type):
            tType{ tokenType::KEYWORD }, kWordType{ Type } {}

        explicit tokenInfo(constType Type):
            tType{ tokenType::CONST }, cType{ Type } {}

        explicit tokenInfo(tokenType Type):
            tType{ Type } {}
    };
private:
    tokenInfo tInfo;
    char tokenVal { 0 };
    const char* identifier { nullptr };
    const char* constCharVal { nullptr };
    FloatingPointType fpVal { FloatingPointType{} };
    IntegerType inVal { IntegerType{} };
public:
    using tokenType = tokenInfo::tokenType;
    explicit token(tokenInfo info): tInfo{ info } {}
    explicit token(char val, tokenInfo info): tInfo{ info }, tokenVal{ val }{}
    explicit token(FloatingPointType val, tokenInfo info): tInfo{ info }, fpVal{ val }{}
    explicit token(IntegerType val, tokenInfo info): tInfo{ info }, inVal{ val }{}

    // Used mainly in debugging options
    inline const char* getIdentifier() const { return identifier; }
    inline void setIdentifier(const char* val) { identifier = val; }
    inline const tokenInfo& getTokenInfo() const { return tInfo; }
    inline tokenInfo& setTokenInfo() { return tInfo; }
    inline void setTokenInfo(tokenInfo val) { tInfo = val; }
    inline void setFpVal(FloatingPointType val) { fpVal = val; }
    inline void setIntVal(IntegerType val) { inVal = val; }
    inline void setConstCharVal(const char* val) { constCharVal = val; }
    inline FloatingPointType getFpVal() const { return fpVal; }
    inline IntegerType getIntVal() const {  return inVal; }
    inline const char* getConstCharVal() const { return constCharVal; }
    inline char getTokenVal() const { return tokenVal; }
};

#endif //INTERPRETER_TOKEN_H