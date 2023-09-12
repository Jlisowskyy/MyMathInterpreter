//
// Created by Jlisowskyy on 08/08/2023.
//
#ifndef INTERPRETER_PARSERUNIT_H
#define INTERPRETER_PARSERUNIT_H

#include <fstream>
#include <list>

#include "globalValues.h"
#include "../include/tokenizer.h"

class parserUnit{
    // Logging simple messages and occurred problems
    bool saveLogToFile = false;
    const char* logDest = parserLogsDestDefault;
    std::ofstream logFile;
    char* fileContent = nullptr;

#ifdef DEBUG_
    const char* debugDest = parserDebugDestDefault;
    std::ofstream debugLogFile;
#endif

    static void openLogFile(std::ofstream& fstr, const char* dest);
    void openReadFile(std::ifstream & fstr, const char* src);
    size_t loadReadFile(std::ifstream& fstr);

public:
    ~parserUnit(){ delete[] fileContent; }

public:
    void EnableSaveToFile();
    void SetLogDest(const char* newDest){
        logDest = newDest;
    }

#ifdef DEBUG_
    void SetDebugLogDest(const char* newDest){
        debugDest = newDest;
    }
#endif
private:
    std::list<token> getFirstStageTokens(size_t fSize);

public:
    std::list<token> processFile(const char*);
    std::list<token> processCin();
};

std::string getTodayDate(bool);
inline size_t chopComment(char*, size_t);
inline size_t chopConstChar(char*, size_t);
inline size_t getToken(std::list<std::string>&, char*);

#ifdef DEBUG_
void writeListOut(std::ofstream& dst, std::list<token>& list);
#endif // DEBUG_

#endif //INTERPRETER_PARSERUNIT_H