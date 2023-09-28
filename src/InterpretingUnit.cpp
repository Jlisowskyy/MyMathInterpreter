//
// Created by Jlisowskyy on 12/09/2023.
//
#include "../include/InterpretingUnit.h"

// -------------------------------------------
// Class structural methods and helping methods
// -------------------------------------------

InterpretingUnit::InterpretingUnit(std::list<token> &&tokens) :
        tokenStream{ tokens }{
    if (tokenStream.empty()) [[unlikely]]{
        throw std::runtime_error("[ERROR] Illegal empty token list passed to parser\n");
    }

    // Convention that token list begins with semicolon separator
    if (tokenStream.front().getTokenInfo().sType != separatorType::SEMI_COLON) [[unlikely]]{
        throw std::runtime_error("[ERROR] Not valid first token on token list occurred. Refer to documentation\n");
    }
}

void InterpretingUnit::getNextToken() {
    actualToken = tokenStream.front();
    tokenStream.pop_front();
}

void InterpretingUnit::error(std::string errorMsg) {
    throw std::runtime_error("[ERROR] " + errorMsg);
}

// -------------------------------------------
// Main decoding loop implementation
// -------------------------------------------

AST InterpretingUnit::lineDispatcher() {
    while(!tokenStream.empty()){
        getNextToken();
        switch(auto& tInfo = actualToken.getTokenInfo(); tInfo.tType){
            case tokenType::VAR:
                processVar();
                break;
            case tokenType::SEPARATOR:
                processSeparator();
                break;
            case tokenType::PROC:
                processProcInvocAsFirstToken();
                break;
            case tokenType::BIN_OP:
                error("Invalid use of binary operator: missing left operand\n");
                break;
            case tokenType::KEYWORD:
                processKeyword();
                break;
            case tokenType::UN_OP:
                processUnOperator();
                break;
            case tokenType::CONST:
                processConst();
                break;
            case tokenType::UNKNOWN: [[unlikely]]
                error("InterpretingUnit received UNKNOWN token\n");
                break;
        }
    }

    return AST(nullptr);
}

// -------------------------------------------
// token deeper analyzers
// -------------------------------------------

void InterpretingUnit::processUnOperator() {
    // TODO
}

void InterpretingUnit::processSeparator() {
    switch (actualToken.getTokenInfo().sType) {
        case separatorType::SEMI_COLON:
            break;
        case separatorType::PARENTHESIS_OPEN:

            break;
        default: [[unlikely]]
            error("invalid use of separator appeared\n");
            break;
    }
}

void InterpretingUnit::processKeyword() {
    // TODO
}

void InterpretingUnit::processVar() {
    if (tokenStream.empty()) [[unlikely]] {
        error("missing semi-colon\n");
    }

    if (tokenStream.front().getTokenInfo().bOpType == binOpType::ASSIGN) {
        processAssignment();
    } else {
        processNumExpression();
    }
}

void InterpretingUnit::processConst() {
    switch (actualToken.getTokenInfo().cType) {
        case dataType::constChar: [[unlikely]]

            getNextToken();
            if (actualToken.getTokenInfo().sType != separatorType::SEMI_COLON)[[unlikely]]{
                error("invalid use of const char literal\n");
            }

            break;
        case dataType::voidType: [[unlikely]]
            error("UNKNOWN const type-appeared: lexer bug encountered'\n");
            break;
        default:
            [[likely]]
                    processNumExpression();
            break;
    }
}

// -------------------------------------------
// Grammatical structures procedures
// -------------------------------------------

void InterpretingUnit::processNumExpression() {

}

dataPack InterpretingUnit::evalNumExpression() {
    while(actualToken.getTokenInfo().sType != separatorType::SEMI_COLON){
        dataPack lOperand;


    }
}

void InterpretingUnit::processNumSubExpressionInParenthesis() {

}

dataPack InterpretingUnit::evalNumSubExpressionInParenthesis() {
    return dataPack();
}

void InterpretingUnit::processAssignment()
{
    auto identifier { actualToken.getIdentifier() };
    getNextToken(); // Consumes '=' token
    getNextToken();
    auto result = evalNumExpression();
    mm.addDPack(identifier, result);
}

void InterpretingUnit::processProcInvocAsFirstToken() {
    // TODO: temp used to printing
    if (std::strcmp("print", actualToken.getIdentifier()) == 0)[[unlikely]]{
        getNextToken(); // gets '('
        getNextToken(); // gets argument
#ifdef DEBUG_
        printToken(actualToken);
#endif
        getNextToken(); // gets ')'

        if (getNextToken(); actualToken.getTokenInfo().sType != separatorType::SEMI_COLON) [[unlikely]]{
            error("missing semi-colon\n");
        }
    }
}

// -------------------------------------------
// Debug tools
// -------------------------------------------

#ifdef DEBUG_

void InterpretingUnit::printToken(token x) {
    dataPack var{};
    
    switch ( x.getTokenInfo().tType) {
        case tokenType::VAR:
            var = mm.getDPack(x.getIdentifier());
            switch (var.dType) {
                case dataType::floatingPoint:
                    std::cout << *(static_cast<FloatingPointType*>(var.dataPtr));
                    break;
                case dataType::matrix:
                    std::cout << *(static_cast<MatrixType*>(var.dataPtr));
                    break;
                case dataType::vector:
                    std::cout << *(static_cast<VectorType*>(var.dataPtr));
                    break;
                case dataType::integer:
                    std::cout << *(static_cast<IntegerType*>(var.dataPtr));
                    break;
                case dataType::constChar:
                    std::cout << (static_cast<const char*>(var.dataPtr));
                    break;
                case dataType::voidType: [[unlikely]]
                    error("Void data type is not printable\n");
                    break;
            }
            break;
        case tokenType::CONST:

            switch (x.getTokenInfo().cType) {
                case dataType::floatingPoint:
                    std::cout << x.getFpVal();
                    break;
                case dataType::integer:
                    std::cout << x.getIntVal();
                    break;
                case dataType::constChar:
                    std::cout << x.getConstCharVal();
                    break;
                case dataType::voidType: [[unlikely]]
                    error("Passed UNKNOWN const type\n");
                    break;
            }
            
            break;
        default: [[unlikely]]
                    error("Invalid debug print argument\n");
    }
}

#endif