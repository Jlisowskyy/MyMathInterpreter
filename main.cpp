//
// Created by Jlisowskyy on 08/08/2023.
//

#include "include/parserUnit.h"
#include <chrono>
#include <iostream>

int main(){
    parserUnit parU{};
    parU.EnableSaveToFile();

    auto start = std::chrono::steady_clock::now();
    parU.processFile("C:\\Users\\Jlisowskyy\\Desktop\\Projekty\\Interpreter\\TestedProgram");
    auto stop = std::chrono::steady_clock::now();

    std::cout << std::endl << (double)(stop.time_since_epoch() - start.time_since_epoch()).count() * 1e-6;

    return 0;
}
