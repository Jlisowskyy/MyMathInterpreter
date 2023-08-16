//
// Created by Jlisowskyy on 08/08/2023.
//
#ifndef INTERPRETER_PARSERUNIT_H
#define INTERPRETER_PARSERUNIT_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "../include/token.h"
#include "GlobalMacros.h"

std::string getTodayDate(bool);
inline size_t chopComment(char*, size_t); // check for alternative
inline size_t chopConstChar(char*, size_t);
inline size_t getToken(std::list<std::string>&, char*);

#ifdef BUFFERED_
inline void breakToTokens(std::list<token>&, std::ifstream&);
#else
inline char* breakToTokens(std::list<token>&, std::ifstream &);
#endif

#ifdef DEBUG_
void writeListOut(std::ofstream& dst, std::list<token>& list);
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

#ifndef BUFFERED_
    char* fileContent = nullptr;
public:
    ~ParserUnit(){ delete[] fileContent; }
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
    std::list<token> getFirstStageTokens(std::ifstream&);

public:
    std::list<token> processFile(const char*);
    std::list<token> processCin();
};

#endif //INTERPRETER_PARSERUNIT_H