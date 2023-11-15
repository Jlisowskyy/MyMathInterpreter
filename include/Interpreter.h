//
// Created by Jlisowskyy on 08/08/2023.
//
#ifndef INTERPRETER_INTERPRETER_H
#define INTERPRETER_INTERPRETER_H

#include <fstream>
#include <list>

#include "globalValues.h"
#include "../include/LexerUnit.h"

class Interpreter
{
// ------------------------------
// type creation
// ------------------------------
public:

    Interpreter() = default;
    ~Interpreter(){ delete[] fileContent; }

// ------------------------------
// type interaction
// ------------------------------

    // TODO: possibly in future will return ast trees or something
    void processFile(const char* filename);

    // TODO: there will be interactive cli interpreter in future - NOT IMPLEMENTED YET
    void processCin();

    // IMPORTANT: tries to open files stored inside class, changed by appropriate methods AND then turns on flag
    void EnableSaveToFile();

    void SetLogDest(const char* newDest){
        logDest = newDest;
    }

#ifdef DEBUG_
    void SetDebugLogDest(const char* newDest){
        debugDest = newDest;
    }
#endif

// ------------------------------
// private methods
// ------------------------------

    static void openLogFile(std::ofstream& fstr, const char* dest);
    void openReadFile(std::ifstream & fstr, const char* src);
    size_t loadReadFile(std::ifstream& fstr);

    // Used only to write line out in error messaging
    std::string getLine(size_t line) const;

    // Performs lexical analysis on loaded file
    std::list<Token> getLexTokens(size_t fSize);

// ------------------------------
// private fields
// ------------------------------

    // controls whether output should be logged to files, when debugging is enabled controls also that
    bool saveLogToFile { false };

    // logging components
    const char* logDest = logsDestDefault;
    std::ofstream logFile;

    const char* lastFilename { nullptr };
    char* fileContent { nullptr };

#ifdef DEBUG_
    const char* debugDest = debugDestDefault;
    std::ofstream debugLogFile;
#endif

};

// ------------------------------
// Helping functions
// ------------------------------

// used only inside logging and debugging
std::string getTodayDate(bool displaySeconds = false);

// ------------------------------
// debugging functions
// ------------------------------

#ifdef DEBUG_

void writeListOut(std::ofstream& dst, std::list<Token>& list);

#endif // DEBUG_

#endif //INTERPRETER_INTERPRETER_H