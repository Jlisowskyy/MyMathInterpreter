//
// Created by Jlisowskyy on 12/09/2023.
//

#ifndef INTERPRETER_OPERATORS_PROC_H
#define INTERPRETER_OPERATORS_PROC_H

#include "globalValues.h"
#include "Token.h"

// ------------------------------------------------
// dynamic programming - most basic data unit
// ------------------------------------------------

struct dataPack
    // just contains data union with corresponding data enum - tagged union
{
// ------------------------------
// type creation
// ------------------------------

    dataPack();
    explicit dataPack(void* dataPtr, dataType type);
    explicit dataPack(const char* constChar);
    explicit dataPack(IntegerType val);
    explicit dataPack(FloatingPointType val);
    explicit dataPack(MatrixType val);
    explicit dataPack(VectorType val);

// ------------------------------
// public fields
// ------------------------------

    dataType dType;

    union{
        void* dynamicDataPtr;
        const char* constChar;
        FloatingPointType fpVal;
        IntegerType intVal;
    } dUnion;
};

// --------------------------------
// dataPack helping functions
// --------------------------------

dataPack getDPack(Token x);
void printDataPack(dataPack& x);

// ------------------------------
// operator templates
// ------------------------------

template<
        typename NumTypeLeft,
        typename NumTypeRight
        >
struct operADD{
    auto operator()(NumTypeLeft& lOp, NumTypeRight& rOp) const{
        return lOp + rOp;
    }
};

template<
        typename NumTypeLeft,
        typename NumTypeRight
        >
struct operSUB{
    auto operator()(NumTypeLeft& lOp, NumTypeRight& rOp) const{
        return lOp - rOp;
    }
};

template<
        typename NumTypeLeft,
        typename NumTypeRight
        >
struct operMUL{
    auto operator()(NumTypeLeft& lOp, NumTypeRight& rOp) const{
        return lOp * rOp;
    }
};

template<
        typename NumTypeLeft,
        typename NumTypeRight
        >
struct operDIV{
    auto operator()(NumTypeLeft& lOp, NumTypeRight& rOp) const{
        return lOp / rOp;
    }
};

// ------------------------------------
// processing operators templates
// ------------------------------------

template<
        typename numType, // expected one from dataType enum class
        template< typename, typename> typename operatorClass
        >
dataPack processOperatorRType(numType& lOperand, dataPack rOperand);

template<
        template<typename, typename> typename operatorClass
        >
dataPack processOperator(dataPack lOperand, dataPack rOperand);

inline static constexpr dataPack (*processADD)(dataPack, dataPack) { &processOperator<operADD> };
inline static constexpr dataPack (*processSUB)(dataPack, dataPack) { &processOperator<operSUB> };
inline static constexpr dataPack (*processMUL)(dataPack, dataPack) { &processOperator<operMUL> };
inline static constexpr dataPack (*processDIV)(dataPack, dataPack) { &processOperator<operDIV> };

// ------------------------------
// operator packs
// ------------------------------

struct binPack{
    binOpType type { binOpType::UNKNOWN };
    dataPack leftArg;
    dataPack rightArg;
};

struct unaryPack{
    unaryOpType type { unaryOpType::UNKNOWN };
    dataPack Arg;
};

dataPack processBinaryOperand(binPack x);
dataPack processUnaryOperand(unaryPack x);

// ---------------------------------------------------
// processing operators templates implementation
// ---------------------------------------------------

template<
        typename numType, // expected one from dataType enum class
        template< typename, typename> typename operatorClass
        >
dataPack processOperatorRType(numType& lOperand, dataPack rOperand){
    static const operatorClass<numType, FloatingPointType> fpOper;
    static const operatorClass<numType, IntegerType> intOper;
    static const operatorClass<numType, MatrixType> matOper;
    static const operatorClass<numType, VectorType> vectOper;
    static const operatorClass<numType, const char*> charOper;

    switch (rOperand.dType) {
        case dataType::floatingPoint:
            return dataPack{ (double)fpOper(lOperand, rOperand.dUnion.fpVal) };
        case dataType::matrix:
            return dataPack{ (double)matOper(lOperand, *(static_cast<MatrixType*>(rOperand.dUnion.dynamicDataPtr))) };
        case dataType::vector:
            return dataPack{ (double)vectOper(lOperand, *(static_cast<VectorType*>(rOperand.dUnion.dynamicDataPtr))) };
        case dataType::integer:
            return dataPack{ (double)intOper(lOperand, rOperand.dUnion.intVal) };
        case dataType::constChar:
            throw std::runtime_error("[ERROR] Character operations not done yet\n");
        case dataType::voidType:
            throw std::runtime_error("[ERROR] Void type is not acceptable inside expression\n");
    }
}

template<
        template<typename, typename> typename operatorClass
        >
dataPack processOperator(dataPack lOperand, dataPack rOperand){
    switch (lOperand.dType) {

        case dataType::floatingPoint:
            return processOperatorRType<FloatingPointType, operatorClass>(lOperand.dUnion.fpVal, rOperand);
        case dataType::matrix:
            return processOperatorRType<MatrixType , operatorClass>(*static_cast<MatrixType*>(lOperand.dUnion.dynamicDataPtr), rOperand);
        case dataType::vector:
            return processOperatorRType<VectorType , operatorClass>(*static_cast<VectorType*>(lOperand.dUnion.dynamicDataPtr), rOperand);
        case dataType::integer:
            return processOperatorRType<IntegerType , operatorClass>(lOperand.dUnion.intVal, rOperand);
        case dataType::constChar:
            throw std::runtime_error("[ERROR] Character operations not done yey\n");
        case dataType::voidType:
            throw std::runtime_error("[ERROR] Void type is not acceptable inside expression\n");
    }
}

#endif //INTERPRETER_OPERATORS_PROC_H