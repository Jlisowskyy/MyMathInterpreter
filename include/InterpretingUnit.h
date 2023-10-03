//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_INTERPRETINGUNIT_H
#define INTERPRETER_INTERPRETINGUNIT_H

#include <list>
#include <stack>

#include "token.h"
#include "AST.h"
#include "StdBasedHashTable.h"

// -------------
// Insert your hashtable class here:
    using MemoryManager = StdBasedHashTable;
//--------------

class InterpretingUnit{
    std::list<token>& tokenStream;
    std::stack<ASTNode*> nodesStack;
    std::list<AST> executionPath;
    ASTNode* currentTree { nullptr };
    token actualToken{};
    MemoryManager mm;

    // work helpers
    void getNextToken();
    void error(std::string errorMsg);

    /*  Important notes:
     *  - all processing functions are expected to leave actualToken variable at last used token,
     *      so the main loop could begin translating the next sentence by itself,
     *  - all processing functions should care to check if there is correct semicolon usage appearing,
     *
     *
     *
     */

    // Grammatical structures procedures
    inline void processProcInvocAsFirstToken();
    void processNumExpression();

    // interpreter action methods
    dataPack loadExpressionArgument();

    // TODO: test template here
    dataPack evalNumExpression(separatorType terminationSign);
    void processNumSubExpressionInParenthesis();
    dataPack evalNumSubExpressionInParenthesis();
    inline void processAssignment();


    // Token deeper analyzers
    inline void processSeparator();
    inline void processKeyword();
    inline void processUnOperator();
    inline void processVar();
    inline void processConst();

public:
    InterpretingUnit(std::list<token>& tokens);
    AST lineDispatcher();

#ifdef DEBUG_
    // temporary printing tool
    void printToken(token x);
#endif
};

#endif //INTERPRETER_INTERPRETINGUNIT_H
