//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_OPERATORSPROC_H
#define INTERPRETER_OPERATORSPROC_H

#include "globalValues.h"
#include "token.h"

struct dataPack{
    bool isConst = false;
    void* dataPtr;
    dataType dType;

    dataPack():
        dataPtr{ nullptr }, dType{ dataType::voidType } {}

    dataPack(void* dataPtr, dataType type, bool isConst = false):
        dataPtr{ dataPtr }, dType{ type } {}

    ~dataPack(){
    }
};

dataPack getDPack(token x){
    switch (x.getTokenInfo().cType) {
        case dataType::floatingPoint:
            return dataPack{ new FloatingPointType{ x.getFpVal() }, dataType::floatingPoint, true };
        case dataType::integer:
            return dataPack{ new IntegerType{ x.getIntVal() }, dataType::integer, true };
        case dataType::constChar:
            return dataPack{ new const char* { x.getConstCharVal() }, dataType::constChar, true };
        default: [[unlikely]]
            throw std::runtime_error("[ERROR] Encountered illegal literal usage\n");
    }
}

void printDataPack(dataPack& x);

struct binPack{
    binOpType type;
    dataPack leftArg;
    dataPack rightArg;
};

struct unaryPack{
    unaryOpType type;
    dataPack Arg;
};

dataPack processBinaryOperand(binPack x);
dataPack processUnaryOperand(unaryPack x);

#endif //INTERPRETER_OPERATORSPROC_H