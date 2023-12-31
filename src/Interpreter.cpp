//
// Created by Jlisowskyy on 08/08/2023.
//

#include <ctime>
#include <sstream>
#include <iomanip>
#include <exception>
#include <limits>
#include <iostream>
#include <fstream>

#include "../include/Interpreter.h"
#include "../include/errors.h"
#include "../include/InterpretingUnit.h"

// -------------------------------------
// type interaction implementation
// -------------------------------------

void Interpreter::processFile(const char* filename) {
    lastFilename = filename;
    std::list<Token> tokenList;
    bool isParsingStarted { false };

    try{
        std::ifstream ftp;
        size_t fSize;

        openReadFile(ftp, filename);
        fSize = loadReadFile(ftp);
        tokenList = getLexTokens(fSize);

        InterpretingUnit interpreter(tokenList);
        isParsingStarted = true;
        interpreter.interpret();
    }
    catch(const std::exception& err){
        static const std::string lineErr("\nOccurred on line: ");
        std::cerr << err.what();

        if (isParsingStarted){
            const size_t errorLineNumber { tokenList.front().line };

            std::string errorLine = getLine(errorLineNumber);
            std::cerr << lineErr + std::to_string(errorLineNumber) + " \n" << errorLine << std::endl;
        }

        if (saveLogToFile){
            logFile << err.what();

            if (isParsingStarted){
                const size_t errorLineNumber { tokenList.front().line };

                std::string errorLine = getLine(errorLineNumber);
                logFile << lineErr + std::to_string(errorLineNumber) + " \n" << errorLine << std::endl;
            }
        }

        exit(EXIT_FAILURE);
    }
}
void Interpreter::processCin() {
    // TODO: Not implemented
}

void Interpreter::EnableSaveToFile() {
    saveLogToFile = true;
    openLogFile(logFile, logDest);
#ifdef DEBUG_
    openLogFile(debugLogFile, debugDest);
#endif
}

// ------------------------------------
// private methods implementation
// ------------------------------------

void Interpreter::openLogFile(std::ofstream &fstr, const char *dest) {
    if (fstr.is_open()) fstr.close();

    fstr.open(std::string(dest) + getTodayDate() + ".log", std::ios::app );

    if(!fstr){
        std::cerr << "[ERROR] Not able to open logging file. Proceeding further...\n";
        return;
    }

    static constexpr const char* WideSep { "------------------------------------------------\n" };
    fstr << WideSep << getTodayDate(true) << '\n' << WideSep ;
}

void Interpreter::openReadFile(std::ifstream &fstr, const char *src) {
    if (fstr.is_open()) fstr.close();
    fstr.open(src);

    if(!fstr){
        const char* msg = "[ERROR] Main file not available\n";
        std::cerr << msg;

        if (saveLogToFile){
            logFile << msg;
        }

        throw std::runtime_error(msg);
    }
    else{
        if (saveLogToFile){
            logFile << "[  OK  ] Parser correctly got access to passed file\n";
        }
    }
}

size_t Interpreter::loadReadFile(std::ifstream& fstr) {
    std::streamsize fSize;

    // Reads file size from stream
    fstr.ignore(std::numeric_limits<std::streamsize>::max());
    fSize = fstr.gcount();
    fstr.clear();
    fstr.seekg(0, std::ios::beg);

    // Sanity checks
    if (fSize == 0){
        throw std::runtime_error("[ERROR] Passed file is empty\n");
    }
    else if(fSize > MAX_FILE_SIZE){
        throw std::runtime_error("[ERROR] Passed file is bigger than quota\n");
    }

    fileContent = new (std::nothrow) char[fSize + 1];

    if(fileContent == nullptr){
        throw std::runtime_error("[ERROR] Not able to allocate memory big enough to fit whole file\n");
    }
    fileContent[fSize] = '\0';

    fstr.read(fileContent, fSize);
    fstr.close();

    if(saveLogToFile){
        logFile << "[  OK  ] File correctly loaded to memory\n";
    }

    return fSize;
}

std::string Interpreter::getLine(const size_t line) const {
    std::ifstream file(lastFilename);
    std::string buffer;
    size_t actLine { 1 };

    while(actLine != line && file){
        getline(file, buffer, '\n');
        ++actLine;
    }

    if (actLine != line){
        throw std::runtime_error(std::string("[ERROR] Not able to reach passed line inside file: \n")
                                 + lastFilename + ", and line: " + std::to_string(line));
    }

    getline(file, buffer, '\n');
    return buffer;
}

std::list<Token> Interpreter::getLexTokens(size_t fSize) {
    std::list<Token> tokenList;

    LexerUnit tkn(fileContent, fSize);
    tokenList = tkn.breakToTokens();

    if (saveLogToFile){
        logFile << "[  OK  ] Parser correctly retrieved first stage tokens from file\n";
    }

#ifdef DEBUG_
    if (saveLogToFile){
        debugLogFile << "First stage tokens:\n\n";
        writeListOut(debugLogFile, tokenList);
    }
#endif

    return tokenList;
}

// ------------------------------------------------------------
// Debugging and helping functions/methods implementation
// ------------------------------------------------------------

std::string getTodayDate(bool displaySeconds) {
    std::stringstream stream(std::ios::out);
    const time_t now = std::time(nullptr);
    const std::tm* tStr = std::localtime(&now);

    stream << std::setfill('0');

    if (displaySeconds) stream << std::setw(2) << tStr->tm_sec << '_';

    stream  << tStr->tm_mday << '_' << (1 + tStr->tm_mon) << '_' << std::setw(4) << (tStr->tm_year + 1900)
            << '_' << std::setw(2) << tStr->tm_hour <<  std::setw(2)<< tStr->tm_min;

    return stream.str();
}

#ifdef DEBUG_

void writeListOut(std::ofstream &dst, std::list<Token> &list){
    auto printTokenName = [&](Token& x) -> void {
        switch (auto tInfo = x.getTokenInfo(); tInfo.tType) {
            case tokenType::VAR:
                dst << x.getIdentifier();
                break;
            case tokenType::SEPARATOR:
                dst << separatorTypeNames[(size_t)(x.getTokenInfo().sType)];
                break;
            case tokenType::PROC:
                dst << x.getIdentifier();
                break;
            case tokenType::BIN_OP:
                dst << binOpTypeNames[(size_t)(x.getTokenInfo().bOpType)];
                break;
            case tokenType::KEYWORD:
                dst << keyWordTypeNames[(size_t)(x.getTokenInfo().kWordType)];
                break;
            case tokenType::UN_OP:
                dst << unaryOpTypeNames[(size_t)(x.getTokenInfo().uOpType)];
                break;
            case tokenType::CONST:
                if (tInfo.cType == dataType::floatingPoint)
                    dst << x.getFpVal();
                else if (tInfo.cType == dataType::integer)
                    dst << x.getIntVal();
                else if (tInfo.cType == dataType::constChar)
                    dst << x.getConstCharVal();
                else
                    dst << "[ERROR]";
                break;
            case tokenType::UNKNOWN:
                dst << "ERROR_NAME";
                break;
        }
    };

    unsigned acc = 0;
    for (auto& i : list){
        printTokenName(i);
        dst << "{ " << (tTypeNames[(size_t)(i.getTokenInfo().tType)]) << " }";

        if (++acc % 5 == 0) dst << '\n';
        if (acc != list.size()) dst << " --> ";
    }
    dst << std::endl;
}

#endif