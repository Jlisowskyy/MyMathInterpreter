//
// Created by Jlisowskyy on 08/08/2023.
//

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <exception>
#include <limits>
#include <functional>

#include "../include/ParserUnit.h"
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

void breakToTokens(std::ifstream &fstr, std::list<std::string> &tokens) {
    char buff;
    std::string tokenBuff;
    while(fstr.get(buff)){
        if (isComment(buff)) fstr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        else if (isNotBlank(buff)) tokenBuff += buff;
        else if (!tokenBuff.empty()){
            tokens.push_back(tokenBuff);
            tokenBuff.clear();
        }
    }
}

char* breakToTokensNotBuffered(std::list<token> &tokenDst, std::ifstream &fstr){
    char* file;
    unsigned tokenSize = 0;

    fstr.ignore(std::numeric_limits<std::streamsize>::max());
    const std::streamsize fSize = fstr.gcount();
    fstr.clear();
    fstr.seekg(0, std::ios::beg);

    file = new char[fSize];
    fstr.read(file, fSize);
    fstr.close();
    bool isNewToken = true;

    for (size_t i = 0; i < fSize; ++i){
        if (isComment(file[i])){
            i = chopComment(file, i);
            isNewToken = true;
        }
        else if (isBlank(file[i])){
            file[i] = '\0';
            isNewToken = true;
        }
        else if (isConstChar(file[i])){
            tokenSize = i;
            file[i] = '\0';
            tokenDst.emplace_back(file + i + 1, token::tokenType::CONST);
            i = chopConstChar(file, i);
            file[i] = '\0';

            tokenDst.back().setSize(i - tokenSize);
            isNewToken = true;
        }
        else if (isNewToken){
            tokenSize = 0;
            tokenDst.back().setSize(tokenSize);
            tokenDst.emplace_back(file + i);
            isNewToken = false;
        }
        else ++tokenSize;
    }

    return file;
}

size_t chopComment(char * file, size_t curPos) {
    do{
        file[curPos++] = '\0';
    }while(file[curPos] != '\n' && file[curPos] != EOF);

    return curPos;
}

size_t getToken(std::list<std::string> &tokenList, char *ptr) {
    tokenList.emplace_back(ptr);
    return tokenList.back().size();
}

size_t chopConstChar(char * file, size_t curPos) {

    do{
        throwIfQMLack(file[++curPos]);
    }while(!isConstChar(file[curPos]));

    return curPos;
}

#ifdef DEBUG_

void writeListOut(std::ofstream &dst, std::list<token> &list){
    unsigned acc = 0;
    for (auto& i : list){
        dst << i.getToken() << "{ " << i.getSize() << ", "
        << (i.gettType() == token::tokenType::CONST ? "CONST" : "") << " }";

        if (++acc % 5 == 0) dst << '\n';
        dst << " --> ";
    }
}

#endif

void ParserUnit::openLogFile(std::ofstream &stream, const char *dest) {
    if (stream.is_open()) stream.close();

    stream.open(std::string(dest) + getTodayDate() + ".log", std::ios::app );

    if(!stream){
        std::cerr << "[ERROR] Not able to open logging file. Proceeding further...\n";
        return;
    }

    stream << getTodayDate(true) << "\n------------------------------------------------\n";
}

void ParserUnit::openReadFile(std::ifstream &stream, const char *src) {
    if (stream.is_open()) stream.close();
    stream.open(src);

    if(!stream){
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

std::list<token> ParserUnit::getFirstStageTokens(std::ifstream & fStream) {
    std::list<token> tokenList;
#ifdef BUFFERED_
    breakToTokens(ftp, tokenList);
#else

    fileContent = breakToTokensNotBuffered(tokenList, fStream);

    if (saveLogToFile){
        logFile << "[  OK  ] Parser correctly retrieved first stage tokens from file\n";
    }

#endif

#ifdef DEBUG_
    if (saveLogToFile){
        debugLogFile << "First stage tokens:\n\n";
        writeListOut(debugLogFile, tokenList);
    }
#endif

    return tokenList;
}

std::list<token> ParserUnit::processFile(const char* src) {
    std::ifstream ftp;
    std::list<token> tokenList;

    try{
        openReadFile(ftp, src);
        tokenList = getFirstStageTokens(ftp);
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

std::list<token> ParserUnit::processCin() {
    return {};
}

void ParserUnit::EnableSaveToFile() {
    saveLogToFile = true;
    openLogFile(logFile, logDest);
#ifdef DEBUG_
    openLogFile(debugLogFile, debugDest);
#endif
}
