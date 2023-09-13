//
// Created by Jlisowskyy on 13/09/2023.
//

#include "../include/AST.h"
#include "string"

void printAST(ASTNode *head, int depth) {
    static constexpr int blanksPerSingleDepth { 5 };

    if (!head) return;
    printAST(head->leftExpression, depth + 1);

    std::cout << '\n' << std::string(depth * blanksPerSingleDepth, ' ') << "( ";

    switch(head->nType){
        case ASTNode::nodeType::data:
            printDataPack(head->data);
            break;
        case ASTNode::nodeType::unaryOp:
            std::cout << unaryOpTypeSymbols[(size_t)head->uType];
            break;
        case ASTNode::nodeType::binaryOp:
            std::cout << binOpTypeSymbols[(size_t)head->bType];
            break;
        case ASTNode::nodeType::procedure:
            std::cout << "NOT YET"; // TODO
            break;
    }

    std::cout << " )" << std::endl;
    printAST(head->rightExpression, depth + 1);
}


dataPack ASTNode::eval() {
    switch (nType) {
        case nodeType::data:
            return data;
        case nodeType::unaryOp:
            if (leftExpression == nullptr) [[unlikely]]{
                throw std::runtime_error("[ERROR] During creation of AST occurred some error: not complete tree\n");
            }
            return processUnaryOperand(unaryPack(uType, leftExpression->eval()));
        case nodeType::binaryOp:
            if (leftExpression == nullptr || rightExpression == nullptr){
                throw std::runtime_error("[ERROR] During creation of AST occurred some error: not complete tree\n");
            }

            return processBinaryOperand(binPack(bType,
                                                leftExpression->eval(),
                                                rightExpression->eval())
            );
        case nodeType::procedure:
            std::cout << "NOT YET";
            return dataPack{};
    }
}

ASTNode::~ASTNode() {
    // TODO: data linking check
    delete leftExpression;
    delete rightExpression;
}
