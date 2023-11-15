//
// Created by Jlisowskyy on 15/08/2023.
//

#ifndef INTERPRETER_ERRORS_H
#define INTERPRETER_ERRORS_H

inline void throwIfQMLack(char x) noexcept(false)
    // throws if occurred EOF or COM or EOL, without a closing question mark
{
    if (isEOF(x) || isComment(x) || isEOL(x)){
        throw std::runtime_error("[ERROR] quotation marks not closed!\n");
    }
}

// TODO: will be populated in future

#endif //INTERPRETER_ERRORS_H
