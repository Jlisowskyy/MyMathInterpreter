//
// Created by Jlisowskyy on 27/09/2023.
//

#ifndef INTERPRETER_STDBASEDHASHTABLE_H
#define INTERPRETER_STDBASEDHASHTABLE_H

#include <unordered_map>
#include <string>
#include "operatorsProc.h"

class StdBasedHashTable{
    std::unordered_map<std::string, dataPack> table{}; // <-- insert here built-in functions

    void undefinedReferenceErr(std::string id) const{
        throw std::runtime_error("[ERROR] Undefined reference error with identifier: " + id);
    }

public:
    // --------------------------------------------------------------------------------------------------

    // Perform action whatever result of indexing is, used in certain situations

    void addDPack(const char* const identifier, dataPack data){
        table[identifier] = data;
    }

    void addDPack(const std::string& identifier, dataPack data){
        table[identifier] = data;
    }

    // --------------------------------------------------------------------------------------------------

    // Checks if passed identifier exists, if no throws undefined reference error

    dataPack getDPack(const char* identifier) const{
        if (auto result = table.find(identifier); result != table.end()) [[likely]]{
            return result->second;
        }
        else undefinedReferenceErr(identifier);
    }

    dataPack getDPack(const std::string& identifier) const{
        if (auto result = table.find(identifier); result != table.end()){
            return result->second;
        }
        else undefinedReferenceErr(identifier);
    }

    // --------------------------------------------------------------------------------------------------

};

#endif //INTERPRETER_STDBASEDHASHTABLE_H
