//
// Created by Jlisowskyy on 17/08/2023.
//

#ifndef INTERPRETER_TOKENIZER_H
#define INTERPRETER_TOKENIZER_H

#include "../include/globalValues.h"
#include "../include/token.h"
#include <list>
#include <stack>

class tokenizer{
    std::list<token> tokens;
    std::stack<char> separatorStack;
    char* file = nullptr;
    size_t fSize = 0;
    size_t curPos = 0;
    size_t line = 1;
    bool isNewToken = false;

    using reactProc = void(tokenizer::*)();
    const static reactProc reactionMap[ASCII_SIZE];

private:
    inline void abandonIfEmpty() noexcept(false){
        if (tokens.empty()) [[unlikely]]
        {
            throw std::runtime_error("[ERROR] Invalid use of operator or separator token on first line\n");
        }
    }


    inline void op(const char* ptr);;
    inline void sep(const char* ptr);
    inline void oSep(const char* ptr);
    inline void cSep(const char* ptr);

    inline void Nothing() {}
    inline void processComment();
    inline void processConstChar();
    inline void processNewLine();
    inline void processSpace();
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
    inline void processSemiColon();
public:
    tokenizer(char* file, size_t size): file{ file }, fSize { size }{}
    std::list<token> breakToTokens();
};

#endif //INTERPRETER_TOKENIZER_H
