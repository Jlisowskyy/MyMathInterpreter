//
// Created by Jlisowskyy on 15/08/2023.
//

#ifndef INTERPRETER_TOKEN_H
#define INTERPRETER_TOKEN_H

#include "globalValues.h"

// TODO: consider using UNKNOWN = 0 to make initialisation faster???

class Token{
public:
    struct tokenInfo; // used only to allow class definition below

// ------------------------------
// type creation
// ------------------------------
#pragma region creation

    Token(): tInfo{} {}
    Token(tokenInfo info, size_t line): tInfo{ info }, line{ line }{}
    explicit Token(FloatingPointType val, tokenInfo info, size_t line): tInfo{ info }, fpVal{ val }, line{ line }{}
    explicit Token(IntegerType val, tokenInfo info, size_t line): tInfo{ info }, inVal{ val }, line{ line }{}

#pragma endregion creation
// ------------------------------
// type interaction
// ------------------------------
#pragma region interaction

    // All below inside this section will be probably changed in future

    // ------------------------------
    // getters
    // ------------------------------

    // Used mainly in debugging options
    [[nodiscard("getter used without effect")]]
    inline const char* getIdentifier() const { return identifier; }

    [[nodiscard("getter used without effect")]]
    inline const tokenInfo& getTokenInfo() const { return tInfo; }

    [[nodiscard("getter used without effect")]]
    inline FloatingPointType getFpVal() const { return fpVal; }

    [[nodiscard("getter used without effect")]]
    inline IntegerType getIntVal() const {  return inVal; }

    [[nodiscard("getter used without effect")]]
    inline const char* getConstCharVal() const { return constCharVal; }

    // ------------------------------
    // setters
    // ------------------------------

    inline tokenInfo& setTokenInfo() { return tInfo; }
    inline void setTokenInfo(tokenInfo val) { tInfo = val; }
    inline void setFpVal(FloatingPointType val) { fpVal = val; }
    inline void setIntVal(IntegerType val) { inVal = val; }
    inline void setConstCharVal(const char* val) { constCharVal = val; }
    inline void setIdentifier(const char* val) { identifier = val; }

#pragma endregion interaction
// ------------------------------
// type definitions
// ------------------------------
#pragma region type definitions

public:
    // Structure type used to hold metadata about token
    struct tokenInfo{
        // ------------------------------
        // type creation
        // ------------------------------

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


        // ------------------------------
        // type fields - TODO: replace with union
        // ------------------------------

        tokenType tType { tokenType::UNKNOWN };
        binOpType bOpType { binOpType::UNKNOWN };
        unaryOpType uOpType { unaryOpType::UNKNOWN };
        keywordType kWordType{ keywordType::UNKNOWN };
        dataType cType{ dataType::voidType };
        separatorType sType { separatorType::UNKNOWN };
    };

#pragma endregion type definitions
// ------------------------------
// private variables
// ------------------------------
#pragma region private variables

private:
    tokenInfo tInfo;

    // TODO: All below should be replaced with union again ASAp
    const char* identifier { nullptr };
    const char* constCharVal { nullptr };
    FloatingPointType fpVal { FloatingPointType{} };
    IntegerType inVal { IntegerType{} };

#pragma endregion private variables
// ------------------------------
// public variables
// ------------------------------
public:

    // Used in debugging to find the line, containing error inside file
    size_t line{};
};

#endif //INTERPRETER_TOKEN_H