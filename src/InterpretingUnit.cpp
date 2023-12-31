//
// Created by Jlisowskyy on 12/09/2023.
//
#include "../include/InterpretingUnit.h"

// ----------------------------------
// type creation implementation
// ----------------------------------

InterpretingUnit::InterpretingUnit(std::list<Token> &tokens) :
        tokenStream{ tokens }{
    if (tokenStream.empty()) [[unlikely]]{
        throw std::runtime_error("[ERROR] Illegal empty Token list passed to parser\n");
    }

    // Convention that Token list begins with semicolon separator
    if (tokenStream.front().getTokenInfo().sType != separatorType::SEMI_COLON) [[unlikely]]{
        throw std::runtime_error("[ERROR] Not valid first Token on Token list occurred. Refer to documentation\n");
    }
}

// ------------------------------------
// helping methods implementation
// ------------------------------------

void InterpretingUnit::getNextToken() {
    if (tokenStream.empty()) [[unlikely]]{
        throw std::runtime_error("[ERROR] Lack of expected Token!\n");
    }

    actualToken = tokenStream.front();
    tokenStream.pop_front();
}

void InterpretingUnit::error(const std::string& errorMsg) {
    throw std::runtime_error("[ERROR] " + errorMsg);
}

// -------------------------------------
// type interaction implementation
// -------------------------------------

AST InterpretingUnit::interpret() {
    while (!tokenStream.empty()) {
        getNextToken();
        switch (auto &tInfo = actualToken.getTokenInfo(); tInfo.tType) {
            case tokenType::VAR:
                processVar();
                break;
            case tokenType::SEPARATOR:
                processSeparator();
                break;
            case tokenType::PROC:
                processFuncCallAsFirstToken();
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
            case tokenType::UNKNOWN:
                [[unlikely]]
                        error("InterpretingUnit received UNKNOWN Token\n");
                break;
        }
    }


    return { nullptr };
}

// -----------------------------------------------------
// Grammatical structures processing implementation
// -----------------------------------------------------

AST * InterpretingUnit::processNumExpression() {
    // TODO: NOT IMPLEMENTED YET
}

dataPack InterpretingUnit::evalNumExpression(separatorType terminationSign) {
    dataPack lBuffer, rBuffer;
    lBuffer = loadExpressionArgument();
    getNextToken();

    while(true){
        if (actualToken.getTokenInfo().bOpType == binOpType::MULT) {
            while (actualToken.getTokenInfo().bOpType == binOpType::MULT) {
                getNextToken();
                rBuffer = loadExpressionArgument();
                getNextToken();
                lBuffer = processMUL(lBuffer, rBuffer);
            }
        } else if (actualToken.getTokenInfo().bOpType == binOpType::ADD) {
            getNextToken();
            auto nextTokenInf = tokenStream.front().getTokenInfo();

            if (nextTokenInf.bOpType == binOpType::ADD || nextTokenInf.sType ==  terminationSign
                || actualToken.getTokenInfo().sType == separatorType::PARENTHESIS_OPEN){
                rBuffer = loadExpressionArgument();
                getNextToken();
            }
            else if (nextTokenInf.bOpType == binOpType::MULT) {
                dataPack tempBuff = loadExpressionArgument();
                getNextToken();

                while (actualToken.getTokenInfo().bOpType == binOpType::MULT){
                    getNextToken();
                    rBuffer = loadExpressionArgument();
                    getNextToken();
                    tempBuff = processMUL(tempBuff, rBuffer);
                }
                rBuffer = tempBuff;
            }
            else{
                error("Invalid expression syntax encountered - evalNumExpression - Inner layer\n");
            }

            lBuffer = processADD(lBuffer, rBuffer);
        } else if (actualToken.getTokenInfo().sType == terminationSign) {
            break;
        }
        else {
            error("Invalid expression syntax encountered - evalNumExpression - Outer layer\n");
        }
    }

    return lBuffer;
}

void InterpretingUnit::processAssignment()
    // Just evaluates expression on the rvalue to get data needed to perform assignment and loads it into memory
{
    auto identifier { actualToken.getIdentifier() };
    getNextToken(); // Consumes '=' Token
    getNextToken();
    auto result = evalNumExpression(separatorType::SEMI_COLON);
    mm.addDPack(identifier, result);
}

void InterpretingUnit::processFuncCallAsFirstToken() {
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

dataPack InterpretingUnit::loadExpressionArgument()
    // function is responsible for loading argument needed to some part of expressions. When parenthesis is found,
    // the expression inside has to be evaluated before.
{
    if (actualToken.getTokenInfo().sType == separatorType::PARENTHESIS_OPEN){
        getNextToken();
        return evalNumExpression(separatorType::PARENTHESIS_CLOSED);
    }
    else if (actualToken.getTokenInfo().tType == tokenType::VAR){
        return mm.getDPack(actualToken.getIdentifier());
    }
    else if (actualToken.getTokenInfo().tType == tokenType::CONST){
        return getDPack(actualToken);
    }
    else{
        // TODO: BETTER CONTEXT ERROR HERE
        error("Invalid expression syntax encountered - loadExpressionArgument\n");
    }
}

// ---------------------------------------------
// Token processing methods implementation
// ---------------------------------------------

void InterpretingUnit::processUnOperator()
    // TODO: NOT IMPLEMENTED YET
{
    switch (actualToken.getTokenInfo().uOpType) {

        case unaryOpType::LOGICAL_NEGATION:
            break;
        case unaryOpType::MATHEMATICAL_NEGATION:
            break;
        case unaryOpType::UNKNOWN:
            break;
    }
}

void InterpretingUnit::processSeparator()
    // TODO: PARTIALLY IMPLEMENTED
{
    switch (actualToken.getTokenInfo().sType) {
        case separatorType::SEMI_COLON:
            break;
        case separatorType::PARENTHESIS_OPEN:
            lastResult = evalNumExpression(separatorType::PARENTHESIS_CLOSED);
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
        lastResult = evalNumExpression(separatorType::SEMI_COLON);
    }
}

void InterpretingUnit::processConst() {
    switch (actualToken.getTokenInfo().cType) {
        case dataType::constChar: [[unlikely]] // TODO: ADD CONST CHAR OPERATIONS

            lastResult = getDPack(actualToken);
            getNextToken();
            if (actualToken.getTokenInfo().sType != separatorType::SEMI_COLON)[[unlikely]]{
                error("invalid use of const char literal\n");
            }

            break;
        case dataType::voidType: [[unlikely]]
            error("UNKNOWN const type-appeared: lexer bug encountered'\n");
            break;
        default: [[likely]]
            lastResult = evalNumExpression(separatorType::SEMI_COLON);
            break;
    }
}

// -------------------------------------------
// Debug tools
// -------------------------------------------

#ifdef DEBUG_

void InterpretingUnit::printToken(Token x) const {
    dataPack var{};
    
    switch ( x.getTokenInfo().tType) {
        case tokenType::VAR:
            var = mm.getDPack(x.getIdentifier());
            switch (var.dType) {
                case dataType::floatingPoint:
                    std::cout << var.dUnion.fpVal;
                    break;
                case dataType::matrix:
                    std::cout << *(static_cast<MatrixType*>(var.dUnion.dynamicDataPtr));
                    break;
                case dataType::vector:
                    std::cout << *(static_cast<VectorType*>(var.dUnion.dynamicDataPtr));
                    break;
                case dataType::integer:
                    std::cout << var.dUnion.intVal;
                    break;
                case dataType::constChar:
                    std::cout << var.dUnion.constChar;
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
                case dataType::matrix:
                    error("Passed MATRIX const type - not implemented\n"); //TODO:
                    break;
                case dataType::vector:
                    error("Passed VECTOR const type - not implemented\n"); //TODO:
                    break;
            }
            
            break;
        default: [[unlikely]]
                    error("Invalid debug print argument\n");
    }
}

#endif