//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_INTERPRETING_UNIT_H
#define INTERPRETER_INTERPRETING_UNIT_H

#include <list>
#include <stack>

#include "Token.h"
#include "AST.h"
#include "VariableHashTable.h"

/*  Important notes:
 *  - all processing functions are expected to leave actualToken variable at last used Token,
 *      so the main loop could begin translating the next sentence by itself,
 *  - all processing functions should care to check if there is correct semicolon usage appearing,
 *
 *
 *
 */

// ---------------------------------------------------
// IMPORTANT: class should have methods with same signature as default class
// Insert your hashtable class here:
    using MemoryManager = VariableHashTable;
//-----------------------------------------------------

class InterpretingUnit{
// ------------------------------
// type interaction
// ------------------------------
public:
    InterpretingUnit(std::list<Token>& tokens);

// ------------------------------
// type interaction
// ------------------------------

    // contains main loop, TODO: AST USAGE IS TEMPORARY AND NOT IMPLEMENTED YET
    AST interpret();

// ------------------------------
// private methods
// ------------------------------

    // ------------------------------
    // helping methods
    // ------------------------------

    void getNextToken();
    static void error(const std::string& errorMsg);

    // --------------------------------------
    // Grammatical structures processing
    // --------------------------------------

    inline void processFuncCallAsFirstToken();
    AST * processNumExpression();
    dataPack evalNumExpression(separatorType terminationSign);     // TODO: test template here
    inline void processAssignment();

    dataPack loadExpressionArgument();

    // ------------------------------
    // Token processing methods
    // ------------------------------


    inline void processSeparator();
    inline void processKeyword();
    inline void processUnOperator();
    inline void processVar();
    inline void processConst();

    // ----------------------------------
    // Language elements processing
    // ----------------------------------

    // TODO: GC, VARIABLES OPERATION, OPTIMISATIONS HERE

#ifdef DEBUG_
    // temporary printing tool
    void printToken(Token x) const;
#endif


// ------------------------------
// private fields
// ------------------------------

    std::list<Token>& tokenStream;
    std::stack<ASTNode*> nodesStack;
    std::list<AST> executionPath;
    ASTNode* currentTree { nullptr };
    Token actualToken{};
    MemoryManager mm;

    dataPack lastResult{};

};

#endif //INTERPRETER_INTERPRETING_UNIT_H
