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
        &tokenizer::processInvalidChar, // &
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
        &tokenizer::processInvalidChar, // ;
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
    tokens.emplace_back(file + curPos, token::tokenType::CONST, token::varType::CHAR);
    isNewToken = true;

    do{
        throwIfQMLack(file[curPos]);
    }while(!isConstChar(file[++curPos]));

    file[curPos] = '\0';
}

void tokenizer::processNegation() {
    op("!");
}

void tokenizer::processParenthesisOpened() {
    abandonIfEmpty();

    auto prevToken = tokens.back();
    if (prevToken.gettType() == token::tokenType::VAR){
        prevToken.settType(token::tokenType::PROC);
    }

    sep("(");
}

void tokenizer::processParenthesisClosed() {
    csep(")");
}

void tokenizer::processMult() {
    op("*");
}

void tokenizer::processAdd() {
    op("+");
}

void tokenizer::processComma() {
    sep(",");
}

void tokenizer::processSub() {
    op("-");
}

void tokenizer::processDiv() {
    op("/");
}

void tokenizer::processModulo() {
    op("%");
}

void tokenizer::processDot() {
    // for now:
    throw std::runtime_error("[ERROR] Dot character ('.') should only be used during literal values typing\nIn future use of classes");
//    sep(".");
}

void tokenizer::processNumber() {
    abandonIfEmpty();

    auto prevToken = tokens.back();
    if (prevToken.gettType() == token::tokenType::OPER
        || prevToken.gettType() == token::tokenType::SEPARATOR)
        // goes through only legal numerical literals possibilities
    {
        const char* const begin = file + curPos;
        char* end;

        double val = strtod(begin, &end);

        if (end != begin)
            // strtod return end == begin when error happened
        {
            isNewToken = true;

            // curPos should indicate an already used character because of the main tokenizer loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(begin, token::tokenType::CONST, token::varType::NUM);
            tokens.back().setNumVal(val);
        }
        else [[unlikely]]{
            const std::string msg = "[ERROR] Invalid literal passed on line: ";
            throw std::runtime_error(msg + std::to_string(line ) + '\n');
        }
    }
    else if (file[curPos-1] != '\0' && (prevToken.gettType() == token::tokenType::VAR
            || prevToken.gettType() == token::tokenType::PROC))
    {
        // Does nothing, cuz its only part of some name
    }
    else [[unlikely]]{
        const std::string msg = "[ERROR] Invalid use of literal number on line: ";
        throw std::runtime_error(msg + std::to_string(line) + '\n');
    }

}

void tokenizer::processColon() {
    sep(":");
}

void tokenizer::processInvalidChar() {
    const std::string msg = "[ERROR] Encountered reserved but unused character, to ensure backward compatibility its use is prohibited!\n";
    const std::string whatChar = std::string("Char: ") + file[curPos] + '\n';
    const std::string position = std::string ("On line: ") + std::to_string(line) + '\n';

    throw std::runtime_error(msg + whatChar + position);
}

void tokenizer::processSmaller() {
    op("<");
}

void tokenizer::processEqual() {
    op("=");
}

void tokenizer::processBigger() {
    op(">");
}

void tokenizer::processEmptyChar() {
    if (isNewToken)[[unlikely]]{
        isNewToken = false;
        tokens.emplace_back(file + curPos, token::tokenType::VAR);
    }
}

void tokenizer::processIndexOpen() {
    sep("[");
}

void tokenizer::processIndexClose() {
    csep("]");
}

void tokenizer::processPow() {
    op("^");
}

void tokenizer::processOR() {
    op("|");
}

void tokenizer::processAND() {
    op("&");
}