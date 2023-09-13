//
// Created by Jlisowskyy on 13/09/2023.
//

#include <iostream>

#include "../include/operatorsProc.h"
#include "../include/globalValues.h"

void printDataPack(dataPack &x) {
    switch (x.dType) {
        case dataType::floatingPoint:
            std::cout << *((FloatingPointType*)x.dataPtr);
            break;
        case dataType::matrix:
            std::cout << *((MatrixType*)x.dataPtr);
            break;
        case dataType::vector:
            std::cout << *((VectorType*)x.dataPtr);
            break;
        case dataType::integer:
            std::cout << *((IntegerType*)x.dataPtr);
            break;
        case dataType::constChar:
            std::cout << *((const char*)x.dataPtr);
            break;
        case dataType::voidType:
            std::cout << "VOID TYPE";
            break;
    }
}

dataPack processBinaryOperand(binPack x) {
    return x.leftArg;
}

dataPack processUnaryOperand(unaryPack x) {
    return x.Arg;
}
