//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_INTERPRETINGUNIT_H
#define INTERPRETER_INTERPRETINGUNIT_H

#include <list>
#include <stack>

#include "Token.h"
#include "AST.h"
#include "StdBasedHashTable.h"

// -------------
// Insert your hashtable class here:
    using MemoryManager = StdBasedHashTable;
//--------------

class InterpretingUnit{
    std::list<Token>& tokenStream;
    std::stack<ASTNode*> nodesStack;
    std::list<AST> executionPath;
    ASTNode* currentTree { nullptr };
    Token actualToken{};
    MemoryManager mm;

    dataPack lastResult{};

    // work helpers
    void getNextToken();
    static void error(const std::string& errorMsg);

    /*  Important notes:
     *  - all processing functions are expected to leave actualToken variable at last used Token,
     *      so the main loop could begin translating the next sentence by itself,
     *  - all processing functions should care to check if there is correct semicolon usage appearing,
     *
     *
     *
     */

    // Grammatical structures procedures
    inline void processProcInvocAsFirstToken();
    AST * processNumExpression();

    // interpreter action methods
    dataPack loadExpressionArgument();

    // TODO: test template here
    dataPack evalNumExpression(separatorType terminationSign);
    inline void processAssignment();


    // Token deeper analyzers
    inline void processSeparator();
    inline void processKeyword();
    inline void processUnOperator();
    inline void processVar();
    inline void processConst();

public:
    InterpretingUnit(std::list<Token>& tokens);
    AST lineDispatcher();

#ifdef DEBUG_
    // temporary printing tool
    void printToken(Token x);
#endif
};

#endif //INTERPRETER_INTERPRETINGUNIT_H
