//
// Created by Jlisowskyy on 08/08/2023.
//

#ifndef INTERPRETER_GLOBALMACROS_H
#define INTERPRETER_GLOBALMACROS_H

//#define DEBUG_
//#define BUFFERED_

#define ParserDebugDestDefault "C:\\Users\\Jlisowskyy\\Desktop\\Projekty\\Interpreter\\DebugLogs\\"
#define ParserLogsDestDefault "C:\\Users\\Jlisowskyy\\Desktop\\Projekty\\Interpreter\\Logs\\"

#define comment '#'

#define isComment(x) (x == comment)
#define isSpace(x) (x == ' ')
#define isEOL(x) (x == '\n')
#define isNotBlank(x) (x != ' ' && x != '\n')
#define isBlank(x) (x == ' ' || x == '\n')
#define isConstChar(x) (x == '"')
#define isEOF(x) (x == EOF)

#endif //INTERPRETER_GLOBALMACROS_H
