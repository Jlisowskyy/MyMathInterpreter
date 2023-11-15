//
// Created by Jlisowskyy on 27/09/2023.
//

#ifndef INTERPRETER_STD_BASED_HASHTABLE_H
#define INTERPRETER_STD_BASED_HASHTABLE_H

#include <unordered_map>
#include <string>
#include "operatorsProc.h"

/*                                              IMPORTANT NOTES
 * To change globally used hash table just change implementation below, without touching methods signatures.
 * This solution will be probably changed in future to my own implementation to boost up global performance
 * */

class VariableHashTable{
public:
// ------------------------------
// Storing Methods
// ------------------------------

    void addDPack(const char* const identifier, dataPack data){
        table[identifier] = data;
    }

    void addDPack(const std::string& identifier, dataPack data){
        table[identifier] = data;
    }

// ------------------------------
// Loading Methods
// ------------------------------

    dataPack getDPack(const char* identifier) const noexcept(false){
        if (auto result = table.find(identifier); result != table.end()) [[likely]]{
            return result->second;
        }
        else undefinedReferenceErr(identifier);
    }

    dataPack getDPack(const std::string& identifier) const noexcept(false){
        if (auto result = table.find(identifier); result != table.end()){
            return result->second;
        }
        else undefinedReferenceErr(identifier);
    }

// ------------------------------
// Helping methods
// ------------------------------

    static void undefinedReferenceErr(const std::string& id) noexcept(false) {
        throw std::runtime_error("[ERROR] Undefined reference error with identifier: " + id);
    }

// ------------------------------
// private variables
// ------------------------------

    std::unordered_map<std::string, dataPack> table{}; // TODO: <-- insert here built-in functions

};

#endif //INTERPRETER_STD_BASED_HASHTABLE_H
