//
// Created by Jlisowskyy on 08/08/2023.
//

#ifndef INTERPRETER_GLOBALMACROS_H
#define INTERPRETER_GLOBALMACROS_H

#define DEBUG_

#define ParserDebugDestDefault "C:\\Users\\Jlisowskyy\\Desktop\\Projekty\\Interpreter\\DebugLogs\\"
#define ParserLogsDestDefault "C:\\Users\\Jlisowskyy\\Desktop\\Projekty\\Interpreter\\Logs\\"

#define COM '#'
#define EOL '\n'
#define ASCII_SIZE 128
#define MAX_FILE_SIZE 33554432 // 32 MiB

#define SPECCHAR_BORDER 'A'
    // value beneath which all chars are treated as special ones and needs to be interpreted as ones
#define isComment(x) (x == COM)
#define isSpace(x) (x == ' ')
#define isEOL(x) (x == EOL)
#define isNotBlank(x) (x != ' ' && x != EOL)
#define isBlank(x) (x == ' ' || x == EOL)
#define isConstChar(x) (x == '"')
#define isEOF(x) (x == EOF)

#endif //INTERPRETER_GLOBALMACROS_H
