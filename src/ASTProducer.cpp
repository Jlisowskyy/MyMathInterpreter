//
// Created by Jlisowskyy on 12/09/2023.
//
#include "../include/ASTProducer.h"

ASTProducer::ASTProducer(std::list<token> &&tokens) :
        tokenStream{ tokens }{
    if (tokenStream.empty()) [[unlikely]]{
        throw std::runtime_error("[ERROR] Illegal empty token list passed to parser\n");
    }

    prevToken = tokenStream.front(); // <-- Convention that token list begins with semicolon separator
    tokenStream.pop_front();
}

void ASTProducer::processIF() {

}

void ASTProducer::processELIF() {

}

void ASTProducer::processELSE() {

}

void ASTProducer::processENDIF() {

}

void ASTProducer::processWHILE() {

}

void ASTProducer::processFOR() {

}

void ASTProducer::processEND() {

}

void ASTProducer::processKeyWord(keywordType type) {
    switch (type){
        case keywordType::IF:
            processIF();
            break;
        case keywordType::ENDIF:
            processENDIF();
            break;
        case keywordType::FOR:
            processFOR();
            break;
        case keywordType::END:
            processEND();
            break;
        case keywordType::ELSE:
            processELSE();
            break;
        case keywordType::ELIF:
            processELIF();
            break;
        case keywordType::WHILE:
            processWHILE();
            break;
        case keywordType::UNKNOWN:
            throw std::runtime_error("[ERROR] KeyWord processing unit received UNKNOWN keyword\n");
    }
}

void ASTProducer::processANGLE_BRACKETS_CLOSED() {

}

void ASTProducer::processANGLE_BRACKETS_OPEN() {

}

void ASTProducer::processPARENTHESIS_CLOSED() {

}

void ASTProducer::processPARENTHESIS_OPEN() {

}

void ASTProducer::processSEMI_COLON() {
    executionPath.emplace_back(currentTree);
}

void ASTProducer::processCOLON() {

}

void ASTProducer::processCOMMA() {

}

void ASTProducer::processSeparator(separatorType type) {
    switch (type) {
        case separatorType::SEMI_COLON:
            processSEMI_COLON();
            break;
        case separatorType::PARENTHESIS_OPEN:
            processPARENTHESIS_OPEN();
            break;
        case separatorType::PARENTHESIS_CLOSED:
            processPARENTHESIS_CLOSED();
            break;
        case separatorType::COMMA:
            processCOMMA();
            break;
        case separatorType::ANGLE_BRACKETS_OPEN:
            processANGLE_BRACKETS_OPEN();
            break;
        case separatorType::ANGLE_BRACKETS_CLOSED:
            processANGLE_BRACKETS_CLOSED();
            break;
        case separatorType::COLON:
            processCOLON();
            break;
        case separatorType::UNKNOWN:
            break;
    }
}

AST ASTProducer::produceAST() {
    while(!tokenStream.empty()){
        token actualToken = tokenStream.front();
        tokenStream.pop_front();

        switch(auto& tInfo = actualToken.getTokenInfo(); tInfo.tType){
            case tokenType::VAR:
                // NOT YET
                break;
            case tokenType::SEPARATOR:
                processSeparator(tInfo.sType);
                break;
            case tokenType::PROC:
                // NOT YET
                break;
            case tokenType::BIN_OP:
                break;
            case tokenType::KEYWORD:
                processKeyWord(tInfo.kWordType);
                break;
            case tokenType::UN_OP:
                // NOT YET
                break;
            case tokenType::CONST:
                break;
            case tokenType::UNKNOWN:
                throw std::runtime_error("[ERROR] ASTProducer received UNKNOWN token\n");
        }
    }

    return AST(nullptr);
}