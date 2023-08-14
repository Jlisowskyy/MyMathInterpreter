//
// Created by Jlisowskyy on 08/08/2023.
//

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <exception>
#include "../include/ParserUnit.h"

std::string getTodayDate() {
    std::stringstream stream(std::ios::out);
    const time_t now = std::time(nullptr);
    const std::tm* tStr = std::localtime(&now);

    stream << std::setfill('0');
    stream << std::setw(2) << tStr->tm_hour << tStr->tm_min << '_' << tStr->tm_mday << '_' << (1 + tStr->tm_mon) << '_'
           << std::setw(4) << (tStr->tm_year + 1900);

    return stream.str();
}

#ifdef DEBUG_

void writeBufferOut(std::ifstream& stream) {
    if (!stream) return;
    std::fpos startPos = stream.tellg();
    char buff;

    while(stream.get(buff)) {
        std::cout << buff;
    }

    stream.seekg(startPos);
    stream.clear();
}

#endif // DEBUG_

void ParserUnit::openLogFile(std::ofstream &stream, const char *dest) {
    if (stream.is_open()) stream.close();

    stream.open(std::string(dest) + getTodayDate() + ".log", std::ios::app );

    if(!stream){
        std::cerr << "[ERROR] Not able to open logging file. Proceeding further...";
    }
}

void ParserUnit::openReadFile(std::ifstream &stream, const char *src) {
    if (stream.is_open()) stream.close();
    stream.open(src);

    if(!stream){
        const char* msg = "[ERROR] Main file not available\n";

        if (saveLogToFile){
            logFile << msg;

#ifdef DEBUG_
            std::cerr << msg;
            debugLogFile << msg;
#endif
        }

        throw std::runtime_error(msg);
    }
}

std::list<std::string> ParserUnit::processFile(const char* src) {
    std::ifstream ftp;
    openReadFile(ftp, src);

#ifdef DEBUG_
    writeBufferOut(ftp);
#endif

    return std::list<std::string>();
}

std::list<std::string> ParserUnit::processCin() {
    return std::list<std::string>();
}

void ParserUnit::EnableSaveToFile() {
    saveLogToFile = true;
    openLogFile(logFile, logDest);
#ifdef DEBUG_
    openLogFile(debugLogFile, debugDest);
#endif
}
