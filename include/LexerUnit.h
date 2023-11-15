//
// Created by Jlisowskyy on 17/08/2023.
//

#ifndef INTERPRETER_LEXER_UNIT_H
#define INTERPRETER_LEXER_UNIT_H

#include "../include/globalValues.h"
#include "../include/Token.h"
#include <list>
#include <stack>
#include <unordered_map>

// Simple state machine transforming char[] into inner token representation of input
class LexerUnit
{
// -----------------------------------
// type creation and interaction
// -----------------------------------
public:

    // Expects whole file to be loaded to memory
    LexerUnit(char* file, size_t size): file{ file }, fSize { size }{
        tokens.emplace_back(Token::tokenInfo(separatorType::SEMI_COLON), line); // Sentinel
    }

    // Processes file and returns token output
    std::list<Token> breakToTokens();

private:

    // used to just flip isNewToken flag and currently, nobody knows why,
    // also does some strange checks of keyword presence
    // TODO: change ASAP
    inline void expectNewToken();


// --------------------------------------------------------------------------
// methods used to imply correct logic on specific character occurrence
// --------------------------------------------------------------------------

    /*          IMPORTANT NOTES
     *  - All separators, operators, special and blank characters are replaced with null character
     *  - Comments are also null filled
     *  - literal values like strings are not copied, there is only pointer to specific file offset
     *  - identifiers of variables are also stored inside file, probably should be changed due to data locality
     *  - numeric literals are not cleaned from file
     *  - WHOLE MODULE IS NOT FULLY TESTED YET
     * */

    // TODO: Consider templates here?
    inline void sep(separatorType type);
    inline void cSep(separatorType type);
    inline void op(Token::tokenInfo tInfo);

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
    inline void abandonIfEmpty() noexcept(false);

// --------------------------------
// private instance variables
// --------------------------------

private:
    // final stream of tokens
    std::list<Token> tokens {};

    // stack used to count and validate usage of operators, which needs to be closed for example '(' and ')'
    std::stack<separatorType> separatorStack {};

    // whole file loaded to memory to be processed, get modified during execution, possibly try
    // to move out variables and data inside this array to boost up data locality
    char* file { nullptr };
    const size_t fSize { 0 };

    // index of current processed char inside array, may be modified by methods sequentially
    size_t curPos { 0 };

    // used mostly to debugging, gathering information of token location inside the file
    size_t line { 1 };

    // flag used for communication between methods
    bool isNewToken { false };

    // alias for method type used to jump to specified code adequate to actually occurred character
    using reactProc = void(LexerUnit::*)();

// ------------------------------
// private static variables
// ------------------------------

    // map used to get correct method address to jump to, possibly better packaging in future
    // Strongly simplifies logic of whole module to just correct reaction inside methods
    static constexpr reactProc reactionMap[ASCII_SIZE]{
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::processNewLine,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::Nothing,
            &LexerUnit::processSpace,
            &LexerUnit::processNegation,
            &LexerUnit::processConstChar,
            &LexerUnit::processComment,
            &LexerUnit::processInvalidChar, // $
            &LexerUnit::processModulo,
            &LexerUnit::processAND,
            &LexerUnit::processInvalidChar, // '
            &LexerUnit::processParenthesisOpened,
            &LexerUnit::processParenthesisClosed,
            &LexerUnit::processMult,
            &LexerUnit::processAdd,
            &LexerUnit::processComma,
            &LexerUnit::processSub,
            &LexerUnit::processDot,
            &LexerUnit::processDiv,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processNumber,
            &LexerUnit::processColon,
            &LexerUnit::processSemiColon, // ;
            &LexerUnit::processSmaller,
            &LexerUnit::processEqual,
            &LexerUnit::processBigger,
            &LexerUnit::processInvalidChar, // ?
            &LexerUnit::processInvalidChar, // @
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processIndexOpen,
            &LexerUnit::processInvalidChar, // "\"
            &LexerUnit::processIndexClose,
            &LexerUnit::processPow,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processInvalidChar, // `
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processEmptyChar,
            &LexerUnit::processInvalidChar, // {
            &LexerUnit::processOR,
            &LexerUnit::processInvalidChar, // }
            &LexerUnit::processInvalidChar, // ~
            &LexerUnit::Nothing
    };

    // map used to speed up keyword detection process, try to create faster map in future
    // TODO: currently not used, should be changed in close future
    static const std::unordered_map<const char*, keywordType, std::hash<const char*>, stringCmp> keyWordMap;
};

#endif //INTERPRETER_LEXER_UNIT_H
