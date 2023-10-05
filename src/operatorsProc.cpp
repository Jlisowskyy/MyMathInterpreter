//
// Created by Jlisowskyy on 13/09/2023.
//

#include <iostream>

#include "../include/operatorsProc.h"
#include "../include/globalValues.h"

void printDataPack(dataPack &x) {
    switch (x.dType) {
        case dataType::floatingPoint:
            std::cout << x.dUnion.fpVal;
            break;
        case dataType::matrix:
            std::cout << *((MatrixType*)x.dUnion.dynamicDataPtr);
            break;
        case dataType::vector:
            std::cout << *((VectorType*)x.dUnion.dynamicDataPtr);
            break;
        case dataType::integer:
            std::cout << x.dUnion.intVal;
            break;
        case dataType::constChar:
            std::cout << x.dUnion.constChar;
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

dataPack getDPack(Token x) {
    switch (x.getTokenInfo().cType) {
        case dataType::floatingPoint:
            return dataPack{ x.getFpVal() };
        case dataType::integer:
            return dataPack{ x.getIntVal() };
        case dataType::constChar:
            return dataPack{ x.getConstCharVal() };
        default: [[unlikely]]
                    throw std::runtime_error("[ERROR] Encountered illegal literal usage\n");
    }
}

dataPack::dataPack(void *dataPtr, dataType type) :
        dUnion{ dataPtr }, dType{ type } {}

dataPack::dataPack(const char *constChar) :
        dataPack{}
{
    dUnion.constChar = constChar;
    dType = dataType::constChar;
}

dataPack::dataPack(IntegerType val) :
        dataPack{}
{
    dUnion.intVal = val;
    dType = dataType::integer;
}

dataPack::dataPack(FloatingPointType val) :
        dataPack{}
{
    dUnion.fpVal = val;
    dType = dataType::floatingPoint;
}

dataPack::dataPack(MatrixType val) {
    // TODO: ADD HERE SUPERVISED ALLOCATION
    dUnion.dynamicDataPtr = nullptr;
    dType = dataType::matrix;
}

dataPack::dataPack(VectorType val) {
    dUnion.dynamicDataPtr = nullptr;
    dType = dataType::vector;
}
