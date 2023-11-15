//
// Created by Jlisowskyy on 17/08/2023.
//

#include <iostream>
#include <cstdlib>

#include "../include/LexerUnit.h"
#include "../include/errors.h"

// ---------------------------------
// static fields initialisation
// ---------------------------------

// TODO: currently not used, should be changed in close future
const std::unordered_map<const char*, keywordType, std::hash<const char*>, stringCmp> LexerUnit::keyWordMap{
    std::make_pair("if", keywordType::IF),
    std::make_pair("elif", keywordType::ELIF),
    std::make_pair("else", keywordType::ELSE),
    std::make_pair("endif", keywordType::ENDIF),
    std::make_pair("for", keywordType::FOR),
    std::make_pair("while", keywordType::WHILE),
    std::make_pair("end", keywordType::END),
};

// ------------------------------
// state changing methods
// ------------------------------

std::list<Token> LexerUnit::breakToTokens(){
    for (curPos = 0; curPos < fSize; ++curPos){
        reactProc react = reactionMap[file[curPos]];
        (this->*react)();
    }

    return tokens;
}


void LexerUnit::expectNewToken() {
    isNewToken = true;

    // TODO: change this awful lines of code ASAP
    if (auto& prevToken = tokens.back(); prevToken.getTokenInfo().tType == tokenType::VAR){

        auto id = prevToken.getIdentifier();
        if (std::strcmp(id, "if") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::IF));
        else if (std::strcmp(id, "elif") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::ELIF));
        else if (std::strcmp(id, "else") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::ELSE));
        else if (std::strcmp(id, "endif") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::ENDIF));
        else if (std::strcmp(id, "for") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::FOR));
        else if (std::strcmp(id, "while") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::WHILE));
        else if (std::strcmp(id, "end") == 0) prevToken.setTokenInfo(Token::tokenInfo(keywordType::END));
    }
}

// --------------------------------------------------------------------------
// methods used to imply correct logic on specific character occurrence
// --------------------------------------------------------------------------

// Used to not repeat same logic in all closing separators
void LexerUnit::cSep(separatorType type)
{
    tokens.emplace_back(Token::tokenInfo(type), line);

    auto lastOpenedSep = separatorStack.top();
    if (lastOpenedSep != type) [[unlikely]]{
        static const std::string msg = "[ERROR] Not correctly closed separator: ";
        throw std::runtime_error(msg + separatorTypeSymbols[(size_t) type] + "\nOn line: " + std::to_string(line) + '\n');
    }
    else separatorStack.pop();
    file[curPos] = '\0';

    expectNewToken();
}

// Used to not repeat same logic in all separators
void LexerUnit::sep(separatorType type) {
    tokens.emplace_back(Token::tokenInfo(type), line);
    file[curPos] = '\0';
    expectNewToken();
}

// Used to not repeat same logic in all operators
inline void LexerUnit::op(Token::tokenInfo tInfo) {
    tokens.emplace_back(tInfo, line);
    file[curPos] = '\0';
    expectNewToken();
}

void LexerUnit::processNewLine(){
    file[curPos] = '\0';
    ++line;
    expectNewToken();
}

void LexerUnit::processSpace() {
    file[curPos] = '\0';
    expectNewToken();
}

void LexerUnit::processComment() {
    do{
        file[curPos++] = '\0';
    }while(curPos < fSize && !isEOL(file[curPos]));
    ++line;
    expectNewToken();
}

// TODO: '"' inside const char sequence?
void LexerUnit::processConstChar() {
    file[curPos++] = '\0';
    Token temp {Token::tokenInfo(dataType::constChar), line };
    temp.setConstCharVal(file + curPos);
    tokens.push_back(temp);

    do{
        throwIfQMLack(file[curPos]);
    }while(!isConstChar(file[++curPos]));

    file[curPos] = '\0';
    expectNewToken();
}

void LexerUnit::processNegation() {
    op(Token::tokenInfo(unaryOpType::LOGICAL_NEGATION));
}

void LexerUnit::processParenthesisOpened() {
    auto& prevToken = tokens.back();
    if (prevToken.getTokenInfo().tType == tokenType::VAR){
        prevToken.setTokenInfo().tType = tokenType::PROC;
    }

    sep(separatorType::PARENTHESIS_OPEN);
    separatorStack.push(separatorType::PARENTHESIS_CLOSED);
}

void LexerUnit::processParenthesisClosed() {
    cSep(separatorType::PARENTHESIS_CLOSED);
}

void LexerUnit::processMult() {
    op(Token::tokenInfo(binOpType::MULT));
}

void LexerUnit::processAdd() {
    op(Token::tokenInfo(binOpType::ADD));
}

void LexerUnit::processComma() {
    sep(separatorType::COMMA);
}

void LexerUnit::processSub() {
    auto prevTokenType = tokens.back().getTokenInfo().tType;

    if (prevTokenType != tokenType::VAR
        && prevTokenType != tokenType::PROC
        && prevTokenType != tokenType::CONST){
        op(Token::tokenInfo(unaryOpType::MATHEMATICAL_NEGATION));
    }
    else{
        op(Token::tokenInfo(binOpType::SUB));
    }
}

void LexerUnit::processDiv() {
    op(Token::tokenInfo(binOpType::DIVIDE));
}

void LexerUnit::processModulo() {
    op(Token::tokenInfo(binOpType::MOD));
}

void LexerUnit::processDot()
    // TODO: Not implemented yet, used only in future inside classes
{
    throw std::runtime_error("[ERROR] Dot character ('.') should only be used during literal values typing\nIn future use of classes");
}

void LexerUnit::processNumber() {
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
            // strtoll return end == begin when error happened
        {
            // curPos should indicate an already used character because of the main LexerUnit loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(intVal, Token::tokenInfo(dataType::integer), line);
            expectNewToken();
        }
        else{
            // TODO: sensitive to change of FloatingPointType
            FloatingPointType fpVal = strtod(begin, &end);

            if (end == begin) [[unlikely]]{
                static const std::string msg = "[ERROR] Invalid literal passed on line: ";
                throw std::runtime_error(msg + std::to_string(line ) + '\n');
            }

            // curPos should indicate an already used character because of the main LexerUnit loop
            curPos += (end - begin) - 1;

            tokens.emplace_back(fpVal, Token::tokenInfo(dataType::floatingPoint), line);
            expectNewToken();
        }
    }
    else [[unlikely]]{
        static const std::string msg = "[ERROR] Invalid use of literal number on line: ";
        throw std::runtime_error(msg + std::to_string(line) + '\n');
    }

}

void LexerUnit::processColon() {
    sep(separatorType::COLON);
}

void LexerUnit::processInvalidChar()
    // used to prevent usage of reserved characters, maybe not used yet but possibly in the future
{
    static const std::string msg = "[ERROR] Encountered reserved but unused character, to ensure backward compatibility its use is prohibited!\n";
    const std::string whatChar = std::string("Char: ") + file[curPos] + '\n';
    const std::string position = std::string ("On line: ") + std::to_string(line) + '\n';

    throw std::runtime_error(msg + whatChar + position);
}

void LexerUnit::processSmaller() {
    op(Token::tokenInfo(binOpType::SMALLER_THAN));
}

void LexerUnit::processEqual() {
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
            op(Token::tokenInfo(binOpType::ASSIGN));
            break;
        default:
            static const std::string msg = "[ERROR] Encountered unexpected operand, on line: ";
            throw std::runtime_error(msg + std::to_string(line) + '\n');
    }
}

void LexerUnit::processBigger() {
    op(Token::tokenInfo(binOpType::BIGGER_THAN));
}

void LexerUnit::processEmptyChar() {
    if (isNewToken)[[unlikely]]{
        isNewToken = false;
        Token temp {Token::tokenInfo(tokenType::VAR), line };
        temp.setIdentifier(file + curPos);
        tokens.emplace_back(temp);
    }
}

void LexerUnit::processIndexOpen() {
    sep(separatorType::ANGLE_BRACKETS_OPEN);
    separatorStack.push(separatorType::ANGLE_BRACKETS_CLOSED);
}

void LexerUnit::processIndexClose() {
    cSep(separatorType::ANGLE_BRACKETS_CLOSED);
}

void LexerUnit::processPow() {
    op(Token::tokenInfo(binOpType::POW));
}

void LexerUnit::processOR() {
    op(Token::tokenInfo(binOpType::OR));
}

void LexerUnit::processAND() {
    op(Token::tokenInfo(binOpType::AND));
}

void LexerUnit::processSemiColon() {
    if (!separatorStack.empty()) [[unlikely]]{
        static const std::string msg = "[ERROR] Not closed separator. Lack of: \"";
        throw std::runtime_error(msg + separatorTypeSymbols[(size_t) separatorStack.top()] + "\", on line: " + std::to_string(line) + '\n');
    }
    sep(separatorType::SEMI_COLON);
}

void LexerUnit::abandonIfEmpty() noexcept(false) {
    if (tokens.empty()) [[unlikely]]
    {
        throw std::runtime_error("[ERROR] Invalid use of operator or separator Token on first line\n");
    }
}
