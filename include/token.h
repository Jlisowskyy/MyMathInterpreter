//
// Created by Jlisowskyy on 15/08/2023.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include "globalMacros.h"

class token{
    const char* tokenVal;

    struct tokenInfo{
        enum class tokenType{ UNKNOWN , VAR, CONST, PROC, OPER } tType;
        enum class varType{ UNKNOWN, NUM, CHAR} vType;
        // FUNC PTR
        // VAL PTR
    } tInfo;
public:
    using tokenType = token::tokenInfo::tokenType;
    using varType = token::tokenInfo::varType;

    explicit token(const char* tokenVal, tokenType tType = tokenType::UNKNOWN, varType vType = varType::UNKNOWN ):
        tokenVal{ tokenVal }, tInfo{ tType, vType } {}

    // Used mainly in debugging options
    inline const char* getToken() const { return tokenVal; }
    inline tokenType gettType() const { return tInfo.tType; }
};



#endif //INTERPRETER_TOKEN_H
