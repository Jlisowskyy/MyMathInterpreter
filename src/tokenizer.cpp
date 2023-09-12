//
// Created by Jlisowskyy on 17/08/2023.
//

#include <iostream>
#include <cstdlib>

#include "../include/tokenizer.h"
#include "../include/errors.h"

using rProc = void(tokenizer::*)();

const rProc tokenizer::reactionMap[ASCII_SIZE] = {
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::processNewLine,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::Nothing,
        &tokenizer::processSpace,
        &tokenizer::processNegation,
        &tokenizer::processConstChar,
        &tokenizer::processComment,
        &tokenizer::processInvalidChar, // $
        &tokenizer::processModulo,
        &tokenizer::processAND,
        &tokenizer::processInvalidChar, // '
        &tokenizer::processParenthesisOpened,
        &tokenizer::processParenthesisClosed,
        &tokenizer::processMult,
        &tokenizer::processAdd,
        &tokenizer::processComma,
        &tokenizer::processSub,
        &tokenizer::processDot,
        &tokenizer::processDiv,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processNumber,
        &tokenizer::processColon,
        &tokenizer::processSemiColon, // ;
        &tokenizer::processSmaller,
        &tokenizer::processEqual,
        &tokenizer::processBigger,
        &tokenizer::processInvalidChar, // ?
        &tokenizer::processInvalidChar, // @
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processIndexOpen,
        &tokenizer::processInvalidChar, // "\"
        &tokenizer::processIndexClose,
        &tokenizer::processPow,
        &tokenizer::processEmptyChar,
        &tokenizer::processInvalidChar, // `
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processEmptyChar,
        &tokenizer::processInvalidChar, // {
        &tokenizer::processOR,
        &tokenizer::processInvalidChar, // }
        &tokenizer::processInvalidChar, // ~
        &tokenizer::Nothing
};

void tokenizer::cSep(char val) {
    tokens.emplace_back(val, token::tokenInfo(token::tokenInfo::tokenType::CSEPARATOR));

    auto lastOpenedSep = separatorStack.top();
    if (lastOpenedSep != val) [[unlikely]]{
        static const std::string msg = "[ERROR] Not correctly closed separator: ";
        throw std::runtime_error(msg + val + "\nOn line: " + std::to_string(line) + '\n');
    }
    else separatorStack.pop();

    isNewToken = true;
    file[curPos] = '\0';
}

void tokenizer::sep(char val) {
    tokens.emplace_back(val, token::tokenInfo(token::tokenInfo::tokenType::SEPARATOR));
    isNewToken = true;
    file[curPos] = '\0';
}

inline void tokenizer::op(token::tokenInfo tInfo) {
    tokens.emplace_back(tInfo);
    isNewToken= true;
    file[curPos] = '\0';
}

std::list<token> tokenizer::breakToTokens(){
    for (curPos = 0; curPos < fSize; ++curPos){
        void(tokenizer::*react)() = reactionMap[(unsigned char)file[curPos]];
        (this->*react)();
    }

    return tokens;
}

void tokenizer::processNewLine(){
    file[curPos] = '\0';
    isNewToken = true;
    ++line;
}

void tokenizer::processSpace() {
    file[curPos] = '\0';
    isNewToken = true;
}

void tokenizer::processComment() {
    do{
        file[curPos++] = '\0';
    }while(curPos < fSize && !isEOL(file[curPos]));
    isNewToken = true;
    ++line;
}

void tokenizer::processConstChar() {
    file[curPos++] = '\0';
    token temp {token::tokenInfo(token::tokenInfo::constType::CONST_CHAR) };
    temp.setConstCharVal(file + curPos);
    tokens.push_back(temp);
    isNewToken = true;

    do{
        throwIfQMLack(file[curPos]);
    }while(!isConstChar(file[++curPos]));

    file[curPos] = '\0';
}

void tokenizer::processNegation() {
    op(token::tokenInfo(token::tokenInfo::unOpType::LOGICAL_NEGATION));
}

void tokenizer::processParenthesisOpened() {
    auto prevToken = tokens.back();
    if (prevToken.getTokenInfo().tType == token::tokenType::VAR){
        prevToken.setTokenInfo().tType = token::tokenType::PROC;
    }

    sep('(');
    separatorStack.push(')');
}

void tokenizer::processParenthesisClosed() {
    cSep(')');
}

void tokenizer::processMult() {
    op(token::tokenInfo(token::tokenInfo::binOpType::MULT));
}

void tokenizer::processAdd() {
    op(token::tokenInfo(token::tokenInfo::binOpType::ADD));
}

void tokenizer::processComma() {
    sep(',');
}

void tokenizer::processSub() {
    auto prevTokenType = tokens.back().getTokenInfo().tType;

    if (prevTokenType != token::tokenType::VAR
        && prevTokenType != token::tokenType::PROC
        && prevTokenType != token::tokenType::CONST){
        op(token::tokenInfo(token::tokenInfo::unOpType::MATHEMATICAL_NEGATION));
    }
    else{
        op(token::tokenInfo(token::tokenInfo::binOpType::SUB));
    }
}

void tokenizer::processDiv() {
    op(token::tokenInfo(token::tokenInfo::binOpType::DIVIDE));
}

void tokenizer::processModulo() {
    op(token::tokenInfo(token::tokenInfo::binOpType::MOD));
}

void tokenizer::processDot() {
    // for now:
    throw std::runtime_error("[ERROR] Dot character ('.') should only be used during literal values typing\nIn future use of classes");
//    sep(".");
}

void tokenizer::processNumber() {
    abandonIfEmpty();

    if (!isNewToken) return;
    else if (auto prevTokenType = tokens.back().setTokenInfo().tType;
        prevTokenType == token::tokenType::BIN_OP
        || prevTokenType == token::tokenType::UN_OP
        || prevTokenType == token::tokenType::SEPARATOR
        || prevTokenType == token::tokenType::KEYWORD)
        // goes through only legal numerical literals possibilities
    {
        const char* const begin { file + curPos };
        char* end { nullptr };

        // TODO: sensitive to change of IntegerType
        IntegerType intVal = strtoll(begin, &end, 10);
        if (end != begin)
            // strtod return end == begin when error happened
        {
            isNewToken = true;
            // curPos should indicate an already used character because of the main tokenizer loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(intVal, token::tokenInfo(token::tokenInfo::constType::INTEGER));
        }
        else{
            // TODO: sensitive to change of FloatingPointType
            FloatingPointType fpVal = strtod(begin, &end);

            if (end == begin) [[unlikely]]{
                static const std::string msg = "[ERROR] Invalid literal passed on line: ";
                throw std::runtime_error(msg + std::to_string(line ) + '\n');
            }

            isNewToken = true;
            // curPos should indicate an already used character because of the main tokenizer loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(fpVal, token::tokenInfo(token::tokenInfo::constType::FLOATING_POINT));
        }
    }
    else [[unlikely]]{
        static const std::string msg = "[ERROR] Invalid use of literal number on line: ";
        throw std::runtime_error(msg + std::to_string(line) + '\n');
    }

}

void tokenizer::processColon() {
    sep(':');
}

void tokenizer::processInvalidChar() {
    static const std::string msg = "[ERROR] Encountered reserved but unused character, to ensure backward compatibility its use is prohibited!\n";
    const std::string whatChar = std::string("Char: ") + file[curPos] + '\n';
    const std::string position = std::string ("On line: ") + std::to_string(line) + '\n';

    throw std::runtime_error(msg + whatChar + position);
}

void tokenizer::processSmaller() {
    op(token::tokenInfo(token::tokenInfo::binOpType::SMALLER_THAN));
}

void tokenizer::processEqual() {
    auto prevToken = tokens.back();

    switch(prevToken.getTokenInfo().bOpType){
        case(token::tokenInfo::binOpType::SMALLER_THAN):
            prevToken.setTokenInfo().bOpType = token::tokenInfo::binOpType::SMALLER_EQUAL_THAN;
            break;
        case(token::tokenInfo::binOpType::BIGGER_THAN):
            prevToken.setTokenInfo().bOpType = token::tokenInfo::binOpType::BIGGER_EQUAL_THAN;
            break;
        case(token::tokenInfo::binOpType::ASSIGN):
            prevToken.setTokenInfo().bOpType = token::tokenInfo::binOpType::EQUAL;
            break;
        case(token::tokenInfo::binOpType::UNKNOWN):
            op(token::tokenInfo(token::tokenInfo::binOpType::ASSIGN));
            break;
        default:
            static const std::string msg = "[ERROR] Encountered unexpected operand, on line: ";
            throw std::runtime_error(msg + std::to_string(line) + '\n');
    }
}

void tokenizer::processBigger() {
    op(token::tokenInfo(token::tokenInfo::binOpType::BIGGER_THAN));
}

void tokenizer::processEmptyChar() {
    if (isNewToken)[[unlikely]]{
        isNewToken = false;
        token temp { token::tokenInfo(token::tokenType::VAR) };
        temp.setIdentifier(file + curPos);
        tokens.emplace_back(temp);
    }
}

void tokenizer::processIndexOpen() {
    sep('[');
    separatorStack.push(']');
}

void tokenizer::processIndexClose() {
    cSep(']');
}

void tokenizer::processPow() {
    op(token::tokenInfo(token::tokenInfo::binOpType::POW));
}

void tokenizer::processOR() {
    op(token::tokenInfo(token::tokenInfo::binOpType::OR));
}

void tokenizer::processAND() {
    op(token::tokenInfo(token::tokenInfo::binOpType::AND));
}

void tokenizer::processSemiColon() {
    if (!separatorStack.empty()) [[unlikely]]{
        static const std::string msg = "[ERROR] Not closed separator. Lack of: \"";
        throw std::runtime_error(msg + separatorStack.top() + "\", on line: " + std::to_string(line) + '\n');
    }
    sep(';');
}