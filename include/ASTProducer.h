//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_ASTPRODUCER_H
#define INTERPRETER_ASTPRODUCER_H

#include "token.h"
#include "AST.h"
#include <list>
#include <stack>

class ASTProducer{
    std::list<token> tokenStream;
    std::stack<ASTNode*> nodesStack;
    std::list<AST> executionPath;
    ASTNode* currentTree { nullptr };
    token prevToken{};

    // keyword processors
    void processIF();
    void processELIF();
    void processELSE();
    void processENDIF();
    void processWHILE();
    void processFOR();
    void processEND();
    void processKeyWord(keywordType type);

    // separators processors
    void processSeparator(separatorType type);
    void processCOMMA();
    void processCOLON();
    void processSEMI_COLON();
    void processPARENTHESIS_OPEN();
    void processPARENTHESIS_CLOSED();
    void processANGLE_BRACKETS_OPEN();
    void processANGLE_BRACKETS_CLOSED();

public:
    ASTProducer(std::list<token>&& tokens);

    AST produceAST();
};

#endif //INTERPRETER_ASTPRODUCER_H
