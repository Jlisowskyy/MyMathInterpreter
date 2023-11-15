//
// Created by Jlisowskyy on 13/09/2023.
//

#include <iostream>

#include "../include/operatorsProc.h"

// --------------------------------------
// dataPack creation implementation
// --------------------------------------

dataPack::dataPack() :
    dType{ dataType::voidType }, dUnion{ nullptr }
{}

dataPack::dataPack(void *dataPtr, dataType type) :
        dType{ type }, dUnion{ dataPtr } {}

dataPack::dataPack(const char *constChar)
{
    dUnion.constChar = constChar;
    dType = dataType::constChar;
}

dataPack::dataPack(IntegerType val)
{
    dUnion.intVal = val;
    dType = dataType::integer;
}

dataPack::dataPack(FloatingPointType val)
{
    dUnion.fpVal = val;
    dType = dataType::floatingPoint;
}

dataPack::dataPack(MatrixType val) {
    // TODO: COMPLETE AND ADD SUPERVISED ALLOCATION
    dUnion.dynamicDataPtr = nullptr;
    dType = dataType::matrix;
}

dataPack::dataPack(VectorType val) {

    dUnion.dynamicDataPtr = nullptr;
    dType = dataType::vector;
}

// -----------------------------------------------
// dataPack helping functions implementation
// -----------------------------------------------

void printDataPack(dataPack &x)
    // Used mainly during debugging, just prints dataPack on stdout
{
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

dataPack getDPack(Token x)
    // TODO: not fully done yet
{
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

dataPack processBinaryOperand(binPack x)
    // TODO: NOT IMPLEMENTED YET
{
    return x.leftArg;
}

dataPack processUnaryOperand(unaryPack x)
    // TODO: NOT IMPLEMENTED YET
{
    return x.Arg;
}

