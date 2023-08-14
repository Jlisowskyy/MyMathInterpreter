//
// Created by Jlisowskyy on 08/08/2023.
//

#include "include/ParserUnit.h"

int main(){
    ParserUnit parU{};
    parU.EnableSaveToFile();
    parU.processFile("C:\\Users\\Jlisowskyy\\Desktop\\Projekty\\Interpreter\\TestedProgram");

    return 0;
}
