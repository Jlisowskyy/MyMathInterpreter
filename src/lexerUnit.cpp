//
// Created by Jlisowskyy on 17/08/2023.
//

#include <iostream>
#include <cstdlib>

#include "../include/lexerUnit.h"
#include "../include/errors.h"

using rProc = void(lexerUnit::*)();

const rProc lexerUnit::reactionMap[ASCII_SIZE] {
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::processNewLine,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::Nothing,
        &lexerUnit::processSpace,
        &lexerUnit::processNegation,
        &lexerUnit::processConstChar,
        &lexerUnit::processComment,
        &lexerUnit::processInvalidChar, // $
        &lexerUnit::processModulo,
        &lexerUnit::processAND,
        &lexerUnit::processInvalidChar, // '
        &lexerUnit::processParenthesisOpened,
        &lexerUnit::processParenthesisClosed,
        &lexerUnit::processMult,
        &lexerUnit::processAdd,
        &lexerUnit::processComma,
        &lexerUnit::processSub,
        &lexerUnit::processDot,
        &lexerUnit::processDiv,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processNumber,
        &lexerUnit::processColon,
        &lexerUnit::processSemiColon, // ;
        &lexerUnit::processSmaller,
        &lexerUnit::processEqual,
        &lexerUnit::processBigger,
        &lexerUnit::processInvalidChar, // ?
        &lexerUnit::processInvalidChar, // @
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processIndexOpen,
        &lexerUnit::processInvalidChar, // "\"
        &lexerUnit::processIndexClose,
        &lexerUnit::processPow,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processInvalidChar, // `
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processEmptyChar,
        &lexerUnit::processInvalidChar, // {
        &lexerUnit::processOR,
        &lexerUnit::processInvalidChar, // }
        &lexerUnit::processInvalidChar, // ~
        &lexerUnit::Nothing
};

const std::unordered_map<const char*, keywordType, std::hash<const char*>, stringCmp> lexerUnit::keyWordMap{
    std::make_pair("if", keywordType::IF),
    std::make_pair("elif", keywordType::ELIF),
    std::make_pair("else", keywordType::ELSE),
    std::make_pair("endif", keywordType::ENDIF),
    std::make_pair("for", keywordType::FOR),
    std::make_pair("while", keywordType::WHILE),
    std::make_pair("end", keywordType::END),
};

void lexerUnit::expectNewToken() {
    isNewToken = true;

    if (auto& prevToken = tokens.back(); prevToken.getTokenInfo().tType == tokenType::VAR){
//        if (auto iter = keyWordMap.find(prevToken.getIdentifier()); iter != keyWordMap.end()){
//            prevToken.setTokenInfo(token::tokenInfo(iter->second));
//        }

        auto id = prevToken.getIdentifier();
        if (std::strcmp(id, "if") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::IF));
        else if (std::strcmp(id, "elif") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::ELIF));
        else if (std::strcmp(id, "else") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::ELSE));
        else if (std::strcmp(id, "endif") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::ENDIF));
        else if (std::strcmp(id, "for") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::FOR));
        else if (std::strcmp(id, "while") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::WHILE));
        else if (std::strcmp(id, "end") == 0) prevToken.setTokenInfo(token::tokenInfo(keywordType::END));
    }
}

void lexerUnit::cSep(separatorType type) {
    tokens.emplace_back(token::tokenInfo(type));

    auto lastOpenedSep = separatorStack.top();
    if (lastOpenedSep != type) [[unlikely]]{
        static const std::string msg = "[ERROR] Not correctly closed separator: ";
        throw std::runtime_error(msg + separatorTypeSymbols[(size_t) type] + "\nOn line: " + std::to_string(line) + '\n');
    }
    else separatorStack.pop();
    file[curPos] = '\0';

    expectNewToken();
}

void lexerUnit::sep(separatorType type) {
    tokens.emplace_back(token::tokenInfo(type));
    file[curPos] = '\0';
    expectNewToken();
}

inline void lexerUnit::op(token::tokenInfo tInfo) {
    tokens.emplace_back(tInfo);
    file[curPos] = '\0';
    expectNewToken();
}

std::list<token> lexerUnit::breakToTokens(){
    for (curPos = 0; curPos < fSize; ++curPos){
        void(lexerUnit::*react)() = reactionMap[(unsigned char)file[curPos]];
        (this->*react)();
    }

    return tokens;
}

void lexerUnit::processNewLine(){
    file[curPos] = '\0';
    ++line;
    expectNewToken();
}

void lexerUnit::processSpace() {
    file[curPos] = '\0';
    expectNewToken();
}

void lexerUnit::processComment() {
    do{
        file[curPos++] = '\0';
    }while(curPos < fSize && !isEOL(file[curPos]));
    ++line;
    expectNewToken();
}

void lexerUnit::processConstChar() {
    file[curPos++] = '\0';
    token temp {token::tokenInfo(constType::CONST_CHAR) };
    temp.setConstCharVal(file + curPos);
    tokens.push_back(temp);

    do{
        throwIfQMLack(file[curPos]);
    }while(!isConstChar(file[++curPos]));

    file[curPos] = '\0';
    expectNewToken();
}

void lexerUnit::processNegation() {
    op(token::tokenInfo(unaryOpType::LOGICAL_NEGATION));
}

void lexerUnit::processParenthesisOpened() {
    auto& prevToken = tokens.back();
    if (prevToken.getTokenInfo().tType == tokenType::VAR){
        prevToken.setTokenInfo().tType = tokenType::PROC;
    }

    sep(separatorType::PARENTHESIS_OPEN);
    separatorStack.push(separatorType::PARENTHESIS_CLOSED);
}

void lexerUnit::processParenthesisClosed() {
    cSep(separatorType::PARENTHESIS_CLOSED);
}

void lexerUnit::processMult() {
    op(token::tokenInfo(binOpType::MULT));
}

void lexerUnit::processAdd() {
    op(token::tokenInfo(binOpType::ADD));
}

void lexerUnit::processComma() {
    sep(separatorType::COMMA);
}

void lexerUnit::processSub() {
    auto prevTokenType = tokens.back().getTokenInfo().tType;

    if (prevTokenType != tokenType::VAR
        && prevTokenType != tokenType::PROC
        && prevTokenType != tokenType::CONST){
        op(token::tokenInfo(unaryOpType::MATHEMATICAL_NEGATION));
    }
    else{
        op(token::tokenInfo(binOpType::SUB));
    }
}

void lexerUnit::processDiv() {
    op(token::tokenInfo(binOpType::DIVIDE));
}

void lexerUnit::processModulo() {
    op(token::tokenInfo(binOpType::MOD));
}

void lexerUnit::processDot() {
    // for now:
    throw std::runtime_error("[ERROR] Dot character ('.') should only be used during literal values typing\nIn future use of classes");
//    sep(".");
}

void lexerUnit::processNumber() {
    abandonIfEmpty();

    if (!isNewToken) return;
    else if (auto prevTokenType = tokens.back().setTokenInfo().tType;
        prevTokenType == tokenType::BIN_OP
        || prevTokenType == tokenType::UN_OP
        || prevTokenType == tokenType::SEPARATOR
        || prevTokenType == tokenType::KEYWORD)
        // goes through only legal numerical literals possibilities
    {
        const char* const begin { file + curPos };
        char* end { nullptr };

        // TODO: sensitive to change of IntegerType
        IntegerType intVal = strtoll(begin, &end, 10);
        if (end != begin)
            // strtod return end == begin when error happened
        {
            // curPos should indicate an already used character because of the main lexerUnit loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(intVal, token::tokenInfo(constType::INTEGER));
            expectNewToken();
        }
        else{
            // TODO: sensitive to change of FloatingPointType
            FloatingPointType fpVal = strtod(begin, &end);

            if (end == begin) [[unlikely]]{
                static const std::string msg = "[ERROR] Invalid literal passed on line: ";
                throw std::runtime_error(msg + std::to_string(line ) + '\n');
            }

            // curPos should indicate an already used character because of the main lexerUnit loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(fpVal, token::tokenInfo(constType::FLOATING_POINT));
            expectNewToken();
        }
    }
    else [[unlikely]]{
        static const std::string msg = "[ERROR] Invalid use of literal number on line: ";
        throw std::runtime_error(msg + std::to_string(line) + '\n');
    }

}

void lexerUnit::processColon() {
    sep(separatorType::COLON);
}

void lexerUnit::processInvalidChar() {
    static const std::string msg = "[ERROR] Encountered reserved but unused character, to ensure backward compatibility its use is prohibited!\n";
    const std::string whatChar = std::string("Char: ") + file[curPos] + '\n';
    const std::string position = std::string ("On line: ") + std::to_string(line) + '\n';

    throw std::runtime_error(msg + whatChar + position);
}

void lexerUnit::processSmaller() {
    op(token::tokenInfo(binOpType::SMALLER_THAN));
}

void lexerUnit::processEqual() {
    auto prevToken = tokens.back();

    switch(prevToken.getTokenInfo().bOpType){
        case(binOpType::SMALLER_THAN):
            prevToken.setTokenInfo().bOpType = binOpType::SMALLER_EQUAL_THAN;
            break;
        case(binOpType::BIGGER_THAN):
            prevToken.setTokenInfo().bOpType = binOpType::BIGGER_EQUAL_THAN;
            break;
        case(binOpType::ASSIGN):
            prevToken.setTokenInfo().bOpType = binOpType::EQUAL;
            break;
        case(binOpType::UNKNOWN):
            op(token::tokenInfo(binOpType::ASSIGN));
            break;
        default:
            static const std::string msg = "[ERROR] Encountered unexpected operand, on line: ";
            throw std::runtime_error(msg + std::to_string(line) + '\n');
    }
}

void lexerUnit::processBigger() {
    op(token::tokenInfo(binOpType::BIGGER_THAN));
}

void lexerUnit::processEmptyChar() {
    if (isNewToken)[[unlikely]]{
        isNewToken = false;
        token temp { token::tokenInfo(tokenType::VAR) };
        temp.setIdentifier(file + curPos);
        tokens.emplace_back(temp);
    }
}

void lexerUnit::processIndexOpen() {
    sep(separatorType::ANGLE_BRACKETS_OPEN);
    separatorStack.push(separatorType::ANGLE_BRACKETS_CLOSED);
}

void lexerUnit::processIndexClose() {
    cSep(separatorType::ANGLE_BRACKETS_CLOSED);
}

void lexerUnit::processPow() {
    op(token::tokenInfo(binOpType::POW));
}

void lexerUnit::processOR() {
    op(token::tokenInfo(binOpType::OR));
}

void lexerUnit::processAND() {
    op(token::tokenInfo(binOpType::AND));
}

void lexerUnit::processSemiColon() {
    if (!separatorStack.empty()) [[unlikely]]{
        static const std::string msg = "[ERROR] Not closed separator. Lack of: \"";
        throw std::runtime_error(msg + separatorTypeSymbols[(size_t) separatorStack.top()] + "\", on line: " + std::to_string(line) + '\n');
    }
    sep(separatorType::SEMI_COLON);
}