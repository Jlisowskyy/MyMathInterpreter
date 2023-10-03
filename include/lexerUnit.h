//
// Created by Jlisowskyy on 17/08/2023.
//

#ifndef INTERPRETER_LEXERUNIT_H
#define INTERPRETER_LEXERUNIT_H

#include "../include/globalValues.h"
#include "../include/token.h"
#include <list>
#include <stack>
#include <unordered_map>

class lexerUnit{
    std::list<token> tokens {};
    std::stack<separatorType> separatorStack {};
    char* file { nullptr };
    size_t fSize { 0 };
    size_t curPos { 0 };
    size_t line { 1 };
    bool isNewToken { false };

    using reactProc = void(lexerUnit::*)();
    // TODO: optimise gain possible: look for constexpr reacitonMap, keywordMap
    static const reactProc reactionMap[ASCII_SIZE];
    static const std::unordered_map<const char*, keywordType, std::hash<const char*>, stringCmp> keyWordMap;
private:
    inline void abandonIfEmpty() noexcept(false){
        if (tokens.empty()) [[unlikely]]
        {
            throw std::runtime_error("[ERROR] Invalid use of operator or separator token on first line\n");
        }
    }

    inline void expectNewToken();

//    inline void op(char val);;
    inline void sep(separatorType type);
    inline void cSep(separatorType type);
    inline void op(token::tokenInfo tInfo);

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
    lexerUnit(char* file, size_t size): file{file }, fSize {size }{
        tokens.emplace_back(token::tokenInfo(separatorType::SEMI_COLON), line);
    }
    std::list<token> breakToTokens();
};

#endif //INTERPRETER_LEXERUNIT_H
