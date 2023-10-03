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
        // TODO: CONSIDER UNION HERE!!!
        tokenType tType { tokenType::UNKNOWN };
        binOpType bOpType { binOpType::UNKNOWN };
        unaryOpType uOpType { unaryOpType::UNKNOWN };
        keywordType kWordType{ keywordType::UNKNOWN };
        dataType cType{ dataType::voidType };
        separatorType sType { separatorType::UNKNOWN };

        explicit tokenInfo() = default;
        explicit tokenInfo(binOpType Type):
            tType{ tokenType::BIN_OP }, bOpType{ Type } {}

        explicit tokenInfo(unaryOpType Type):
            tType{ tokenType::UN_OP }, uOpType{ Type } {}

        explicit tokenInfo(keywordType Type):
            tType{ tokenType::KEYWORD }, kWordType{ Type } {}

        explicit tokenInfo(dataType Type):
            tType{ tokenType::CONST }, cType{ Type } {}

        explicit tokenInfo(separatorType Type):
            tType{ tokenType::SEPARATOR }, sType{ Type } {}

        explicit tokenInfo(tokenType Type):
            tType{ Type } {}
    };
private:
    tokenInfo tInfo;
    const char* identifier { nullptr };
    const char* constCharVal { nullptr };
    FloatingPointType fpVal { FloatingPointType{} };
    IntegerType inVal { IntegerType{} };

public:
    size_t line{}; // Used in debugging to find the line, containing error inside file
    explicit token(): tInfo{} {}
    explicit token(tokenInfo info, size_t line): tInfo{ info }, line{ line }{}
    explicit token(FloatingPointType val, tokenInfo info, size_t line): tInfo{ info }, fpVal{ val }, line{ line }{}
    explicit token(IntegerType val, tokenInfo info, size_t line): tInfo{ info }, inVal{ val }, line{ line }{}

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
};

#endif //INTERPRETER_TOKEN_H