//
// Created by Jlisowskyy on 08/08/2023.
//

#include "include/Interpreter.h"
#include <chrono>
#include <iostream>

int main(int argc, const char** argv){
    // TODO: temporary startup procedure. CLI not implemented yet

    if (argc < 2){
        throw std::runtime_error("[ERROR] Missing file name");
    }

    Interpreter parU{};
    parU.EnableSaveToFile();

    auto start = std::chrono::steady_clock::now();
    parU.processFile(argv[1]);
    auto stop = std::chrono::steady_clock::now();

    // milliseconds
    std::cout << std::endl << (double)(stop.time_since_epoch() - start.time_since_epoch()).count() * 1e-6;

    return 0;
}
