//
// Created by Jlisowskyy on 08/08/2023.
//
#ifndef INTERPRETER_PARSERUNIT_H
#define INTERPRETER_PARSERUNIT_H

#include <iostream>
#include <fstream>
#include <chrono>
#include "../GlobalMacros.h"

class ParserUnit{
    std::istream& MaintInputStream;
    std::ifstream MainFileStream;

    // Logging simple messages and occurred problems
    std::ostream& LoggingStream = std::cerr;
    bool SaveLogToFile = false;
    const char* LogDest = ParserLogsDestDefault;
    std::ofstream LogFile;
    void OpenLogFile();

#ifdef DEBUG_
    std::ostream& DebugStream = std::cout;
    bool SaveDebugToFile = false;
    const char* DebugDest = ParserDebugDestDefault;
    std::ofstream DebugLogFile;
    void OpenDebugFile();
#endif

public:

    ParserUnit(std::istream& InputStream): MaintInputStream{ InputStream }
    {}

    ParserUnit(const char* InputFile): MaintInputStream { MainFileStream }
    {}

    void EnableSaveToFile(){
        SaveLogToFile = true;

#ifdef DEBUG_
        SaveDebugToFile = true;
#endif
    }

    void SetLogDest(const char* NewDest){
        LogDest = NewDest;
    }

#ifdef DEBUG_
    void SetDebugLogDest(const char* NewDest){
        DebugDest = NewDest;
    }
#endif

    bool ProcessInput();
};

#endif //INTERPRETER_PARSERUNIT_H
