//
// Created by Jlisowskyy on 17/08/2023.
//

#include <iostream>

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
        &tokenizer::processBlank,
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
        &tokenizer::processBlank,
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

void tokenizer::processBlank() {
    file[curPos] = '\0';
    isNewToken = true;
}

void tokenizer::processComment() {
    do{
        file[curPos++] = '\0';
    }while(curPos < fSize && !isEOL(file[curPos]));
    isNewToken = true;
}

void tokenizer::processConstChar() {
    file[curPos++] = '\0';
    tokens.emplace_back(file + curPos, token::tokenType::CONST);
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
    op("(");
}

void tokenizer::processParenthesisClosed() {
    op(")");
}

void tokenizer::processMult() {
    op("*");
}

void tokenizer::processAdd() {
    op("+");
}

void tokenizer::processComma() {
    op(",");
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
    op(".");
}

void tokenizer::processNumber() {

}

void tokenizer::processColon() {
    op(":");
}

void tokenizer::processInvalidChar() {
    const char* msg = "[ERROR] Encountered reserved but unused character, to ensure backward compatibility its use if prohibited!\n";

    std::cerr << msg << "Char: " << file[curPos] << "\nPosition: " << curPos;

    throw std::runtime_error(msg);
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
    if (isNewToken){
        isNewToken = false;
        tokens.emplace_back(file + curPos, token::tokenType::VAR);
    }
}

void tokenizer::processIndexOpen() {
    op("[");
}

void tokenizer::processIndexClose() {
    op("]");
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
