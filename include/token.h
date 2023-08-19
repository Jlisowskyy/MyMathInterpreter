//
// Created by Jlisowskyy on 15/08/2023.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include "globalMacros.h"

class token{
    const char* tokenVal;
    double numVal;

    struct tokenInfo{
        enum class tokenType{ UNKNOWN , VAR, CONST, PROC, OPER, KEYWORD, SEPARATOR , CSEPARATOR} tType;
        enum class varType{ UNKNOWN, NUM, CHAR } vType;
        // FUNC PTR
    } tInfo;
public:
    using tokenType = token::tokenInfo::tokenType;
    using varType = token::tokenInfo::varType;

    explicit token(const char* tokenVal, tokenType tType = tokenType::UNKNOWN, varType vType = varType::UNKNOWN ):
        tokenVal{ tokenVal }, numVal{ 0 }, tInfo{ tType, vType} {}

    // Used mainly in debugging options
    inline const char* getToken() const { return tokenVal; }
    inline tokenType gettType() const { return tInfo.tType; }
    inline void settType(tokenType val) { tInfo.tType = val; }
    inline void setNumVal(double val) { numVal = val; }
};



#endif //INTERPRETER_TOKEN_H
