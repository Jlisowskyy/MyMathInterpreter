//
// Created by Jlisowskyy on 15/08/2023.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include <map>

class token{
    char* tokenVal;
    unsigned tokenSize;

    struct tokenInfo{
        enum class tokenType{ UNKNOWN , VAR, CONST, PROC, OPER } tType;
        enum class varType{ UNKNOWN, NUM, CHAR} vType;
        // FUNC PTR
        // VAL PTR
    } tInfo;
public:
    using tokenType = token::tokenInfo::tokenType;
    using varType = token::tokenInfo::varType;

    explicit token(char* tokenVal, tokenType tType = tokenType::UNKNOWN, varType vType = varType::UNKNOWN ):
        tokenVal{ tokenVal }, tokenSize{ 0 }, tInfo{ tType, vType } {}

    // Used mainly in debugging options
    inline const char* getToken() const { return tokenVal; }
    inline const unsigned getSize() const { return tokenSize; }
    inline tokenType gettType() const { return tInfo.tType; }

    // Used in parser chopping
    inline void setSize(unsigned x) { tokenSize = x; }
};



#endif //INTERPRETER_TOKEN_H
