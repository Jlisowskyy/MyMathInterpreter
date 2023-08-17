//
// Created by Jlisowskyy on 17/08/2023.
//

#ifndef INTERPRETER_TOKENIZER_H
#define INTERPRETER_TOKENIZER_H

#include "../include/globalMacros.h"
#include "../include/token.h"
#include <list>

class tokenizer{
    std::list<token> tokens;
    char* file = nullptr;
    size_t fSize = 0;
    size_t curPos = 0;
    bool isNewToken = false;

    using reactProc = void(tokenizer::*)();
    const static reactProc reactionMap[ASCII_SIZE];

private:
    inline void op(const char* ptr){
        tokens.emplace_back(ptr, token::tokenType ::OPER);
        isNewToken= true;
        file[curPos] = '\0';
    };

    inline void Nothing() {}
    inline void processComment();
    inline void processConstChar();
    inline void processBlank();
    inline void processNegation();
    inline void processParenthesisOpened();
    inline void processParenthesisClosed();
    inline void processMult();
    inline void processAdd();
    inline void processComma();
    inline void processSub();
    inline void processDiv();
    inline void processModulo();
    inline void processDot();
    inline void processNumber();
    inline void processColon();
    inline void processInvalidChar();
    inline void processSmaller();
    inline void processEqual();
    inline void processBigger();
    inline void processEmptyChar();
    inline void processIndexOpen();
    inline void processIndexClose();
    inline void processPow();
    inline void processOR();
    inline void processAND();
public:
    tokenizer(char* file, size_t size): file{ file }, fSize { size }{}
    std::list<token> breakToTokens();
};

#endif //INTERPRETER_TOKENIZER_H
