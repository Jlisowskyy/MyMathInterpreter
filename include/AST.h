//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_AST_H
#define INTERPRETER_AST_H

#include <exception>
#include <stdexcept>
#include <iostream>

#include "globalValues.h"
#include "operatorsProc.h"

// Ready to execute object

class AST;
class ASTNode;
class InterpretingUnit;

void printAST(ASTNode* head, int depth);

class ASTNode{
    enum class nodeType{
        data, unaryOp, binaryOp, procedure
    } nType { nodeType::data };

    binOpType bType;
    unaryOpType uType;

    dataPack data{};
    ASTNode* leftExpression { nullptr };
    ASTNode* rightExpression { nullptr };

    ASTNode** funcParams { nullptr };

    friend void printAST(ASTNode* head, int depth);
    friend AST;
    friend InterpretingUnit;

    ASTNode(binOpType Type):
        nType { nodeType::binaryOp }, bType { Type } {}
    ASTNode(dataPack dPack):
        nType { nodeType::data }, data { dPack } {}
    ASTNode(unaryOpType Type):
        nType { nodeType::unaryOp }, uType{ Type } {}
    ~ASTNode();
public:
    dataPack eval();
};

class AST{
    ASTNode* head { nullptr };
public:
    AST(ASTNode* newHead):
        head { newHead } {}
    ~AST(){
        delete head;
    }
    dataPack eval(){
        // TODO: nullptr check
        return head->eval();
    }

};

#endif //INTERPRETER_AST_H
