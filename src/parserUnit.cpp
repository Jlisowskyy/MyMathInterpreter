//
// Created by Jlisowskyy on 08/08/2023.
//

#include <ctime>
#include <sstream>
#include <iomanip>
#include <exception>
#include <limits>
#include <iostream>

#include "../include/parserUnit.h"
#include "../include/errors.h"

std::string getTodayDate(bool displaySeconds = false) {
    std::stringstream stream(std::ios::out);
    const time_t now = std::time(nullptr);
    const std::tm* tStr = std::localtime(&now);

    stream << std::setfill('0');

    if (displaySeconds) stream << std::setw(2) << tStr->tm_sec << '_';

    stream  << tStr->tm_mday << '_' << (1 + tStr->tm_mon) << '_' << std::setw(4) << (tStr->tm_year + 1900)
            << '_' << std::setw(2) << tStr->tm_hour <<  std::setw(2)<< tStr->tm_min;

    return stream.str();
}

size_t chopComment(char * file, size_t curPos) {
    do{
        file[curPos++] = '\0';
    }while(file[curPos] != '\n' && file[curPos] != EOF);

    return curPos;
}

size_t chopConstChar(char * file, size_t curPos) {

    do{
        throwIfQMLack(file[++curPos]);
    }while(!isConstChar(file[curPos]));

    return curPos;
}

#ifdef DEBUG_

void writeListOut(std::ofstream &dst, std::list<token> &list){
    static const char* tTypeNames[] ={
    "VAR",
    "CONST",
    "PROC",
    "BIN_OP",
    "UN_OP",
    "KEYWORD",
    "SEPARATOR",
    "CSEPARATOR",
    "ERROR_TYPE"
    };

    static const char* binOpTypeNames[] = {
        "ADD",
        "SUB",
        "MULT",
        "POW",
        "DIVIDE",
        "MOD",
        "AND",
        "OR",
        "SMALLER_THAN",
        "BIGGER_THAN",
        "EQUAL",
        "SMALLER_EQUAL_THAN",
        "BIGGER_EQUAL_THAN",
        "ASSIGN",
        "ERROR_BINOP"
    };

    static const char* unaryOpTypeNames[] = {
        "LOGICAL_NEGATION",
        "MATHEMATICAL_NEGATION",
        "ERROR_UNOP"
    };

    static const char* keyWordTypeNames[] = {
        "IF",
        "ELIF",
        "ELSE",
        "ENDIF",
        "FOR",
        "WHILE",
        "END",
        "ERROR_KEYWORD"
    };

    static const char* constTypeNames[] = {
        "FLOATING_POINT",
        "INTEGER",
        "CONST_CHAR",
        "ERROR_CONST"
    };

    auto printTokenName = [&](token& x) -> void {
        switch (auto tInfo = x.getTokenInfo(); tInfo.tType) {
            case token::tokenInfo::tokenType::VAR:
                dst << x.getIdentifier();
                break;
            case token::tokenInfo::tokenType::CONST:
                if (tInfo.cType == token::tokenInfo::constType::FLOATING_POINT)
                    dst << x.getFpVal();
                else if (tInfo.cType == token::tokenInfo::constType::INTEGER)
                    dst << x.getIntVal();
                else if (tInfo.cType == token::tokenInfo::constType::CONST_CHAR)
                    dst << x.getConstCharVal();
                else
                    dst << "[ERROR]";
                break;
            case token::tokenInfo::tokenType::PROC:
                dst << x.getIdentifier();
                break;
            case token::tokenInfo::tokenType::BIN_OP:
                dst << binOpTypeNames[(size_t)(x.getTokenInfo().bOpType)];
                break;
            case token::tokenInfo::tokenType::UN_OP:
                dst << unaryOpTypeNames[(size_t)(x.getTokenInfo().uOpType)];
                break;
            case token::tokenInfo::tokenType::KEYWORD:
                dst << keyWordTypeNames[(size_t)(x.getTokenInfo().kWordType)];
                break;
            case token::tokenInfo::tokenType::SEPARATOR:
                dst << x.getTokenVal();
                break;
            case token::tokenInfo::tokenType::CSEPARATOR:
                dst << x.getTokenVal();
                break;
            case token::tokenInfo::tokenType::UNKNOWN:
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

void parserUnit::openLogFile(std::ofstream &fstr, const char *dest) {
    if (fstr.is_open()) fstr.close();

    fstr.open(std::string(dest) + getTodayDate() + ".log", std::ios::app );

    if(!fstr){
        std::cerr << "[ERROR] Not able to open logging file. Proceeding further...\n";
        return;
    }

    static constexpr const char* WideSep { "------------------------------------------------\n" };
    fstr << WideSep<< getTodayDate(true) << '\n' << WideSep ;
}

void parserUnit::openReadFile(std::ifstream &fstr, const char *src) {
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

size_t parserUnit::loadReadFile(std::ifstream& fstr) {
    std::streamsize fSize;

    fstr.ignore(std::numeric_limits<std::streamsize>::max());
    fSize = fstr.gcount();
    fstr.clear();
    fstr.seekg(0, std::ios::beg);

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

std::list<token> parserUnit::getFirstStageTokens(size_t fSize) {
    std::list<token> tokenList;

    tokenizer tkn(fileContent, fSize);
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

std::list<token> parserUnit::processFile(const char* src) {
    std::list<token> tokenList;

    try{
        std::ifstream ftp;
        size_t fSize;

        openReadFile(ftp, src);
        fSize = loadReadFile(ftp);
        tokenList = getFirstStageTokens(fSize);
    }
    catch(const std::exception& err){
        std::cerr << err.what();

        if (saveLogToFile){
            logFile << err.what();
        }

        exit(1);
    }
    return tokenList;
}

std::list<token> parserUnit::processCin() {
    return {};
}

void parserUnit::EnableSaveToFile() {
    saveLogToFile = true;
    openLogFile(logFile, logDest);
#ifdef DEBUG_
    openLogFile(debugLogFile, debugDest);
#endif
}