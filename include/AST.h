//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_AST_H
#define INTERPRETER_AST_H

#include <exception>
#include <stdexcept>
// Ready to execute object

class AST;

enum class dataType {
    floatingPoint, matrix, vector, integer, constChar, voidType
};

struct dataPack{
    void* dataPtr { nullptr };
    dataType dType { dataType::floatingPoint };
};

class ASTNode{
    enum class nodeType{
        data, unaryOp, binaryOp, procedure
    } nType { nodeType::data };



    dataPack data{};
    ASTNode* leftExpression { nullptr };
    ASTNode* rightExpression { nullptr };

    ASTNode** funcParams { nullptr };

    dataPack eval(){
        switch (nType) {
            case nodeType::data:
                return data;
            case nodeType::unaryOp:
                if (leftExpression == nullptr) [[unlikely]]{
                    throw std::runtime_error("[ERROR] During creation of AST occurred some error: not complete tree\n");
                }
                leftExpression->eval();
                break;
            case nodeType::binaryOp:
                break;
            case nodeType::procedure:
                break;
        }
    }
};

class AST{

};

#endif //INTERPRETER_AST_H
