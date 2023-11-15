//
// Created by Jlisowskyy on 08/08/2023.
//
#ifndef INTERPRETER_PARSERUNIT_H
#define INTERPRETER_PARSERUNIT_H

#include <fstream>
#include <list>

#include "globalValues.h"
#include "../include/LexerUnit.h"

class ParserUnit{
    // Logging simple messages and occurred problems
    bool saveLogToFile { false };
    const char* logDest = parserLogsDestDefault;
    std::ofstream logFile;
    const char* lastFilename { nullptr };
    char* fileContent { nullptr };

#ifdef DEBUG_
    const char* debugDest = parserDebugDestDefault;
    std::ofstream debugLogFile;
#endif

    static void openLogFile(std::ofstream& fstr, const char* dest);
    void openReadFile(std::ifstream & fstr, const char* src);
    size_t loadReadFile(std::ifstream& fstr);
    std::string getLine(size_t line); // Used only to write line out in error messaging

public:
    ~ParserUnit(){ delete[] fileContent; }
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
    std::list<Token> getLexTokens(size_t fSize);

public:
    // TODO: possibly in future will return ast trees or something
    void processFile(const char* filename);
    void processCin();
};

std::string getTodayDate(bool);
inline size_t chopComment(char*, size_t);
inline size_t chopConstChar(char*, size_t);
inline size_t getToken(std::list<std::string>&, char*);

#ifdef DEBUG_
void writeListOut(std::ofstream& dst, std::list<Token>& list);
#endif // DEBUG_

#endif //INTERPRETER_PARSERUNIT_H