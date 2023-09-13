//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_OPERATORSPROC_H
#define INTERPRETER_OPERATORSPROC_H

#include "globalValues.h"

struct dataPack{
    void* dataPtr { nullptr };
    dataType dType { dataType::floatingPoint };
};

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