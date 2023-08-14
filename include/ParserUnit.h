//
// Created by Jlisowskyy on 08/08/2023.
//
#ifndef INTERPRETER_PARSERUNIT_H
#define INTERPRETER_PARSERUNIT_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "GlobalMacros.h"

std::string getTodayDate();

#ifdef DEBUG_
void writeBufferOut(std::ifstream&);
#endif // DEBUG_

class ParserUnit{
    // Logging simple messages and occurred problems
    bool saveLogToFile = false;
    const char* logDest = ParserLogsDestDefault;
    std::ofstream logFile;
    static void openLogFile(std::ofstream& stream, const char* dest);
    void openReadFile(std::ifstream & stream, const char* src);
#ifdef DEBUG_
    const char* debugDest = ParserDebugDestDefault;
    std::ofstream debugLogFile;
#endif

public:
    void EnableSaveToFile();
    void SetLogDest(const char* NewDest){
        logDest = NewDest;
    }

#ifdef DEBUG_
    void SetDebugLogDest(const char* NewDest){
        debugDest = NewDest;
    }
#endif
private:


public:
    std::list<std::string> processFile(const char*);
    std::list<std::string> processCin();
};

#endif //INTERPRETER_PARSERUNIT_H
