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

    stream << std::setw(2) << tStr->tm_hour <<  std::setw(2)<< tStr->tm_min << '_' << tStr->tm_mday
        << '_' << (1 + tStr->tm_mon) << '_' << std::setw(4) << (tStr->tm_year + 1900);

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
    static const char* tTypeNames[5] =
            {"UNKNOWN", "VAR", "CONST", "PROC", "OPER"};

    unsigned acc = 0;
    for (auto& i : list){
        dst << i.getToken() << "{ "<< (tTypeNames[(int)i.gettType()]) << " }";

        if (++acc % 5 == 0) dst << '\n';
        dst << " --> ";
    }
}

#endif

void parserUnit::openLogFile(std::ofstream &fstr, const char *dest) {
    if (fstr.is_open()) fstr.close();

    fstr.open(std::string(dest) + getTodayDate() + ".log", std::ios::app );

    if(!fstr){
        std::cerr << "[ERROR] Not able to open logging file. Proceeding further...\n";
        return;
    }

    fstr << getTodayDate(true) << "\n------------------------------------------------\n";
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

    fileContent = new (std::nothrow) char[fSize];

    if(fileContent == nullptr){
        throw std::runtime_error("[ERROR] Not able to allocate memory big enough to fit whole file\n");
    }

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