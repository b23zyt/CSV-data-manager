#include "Hashing.h"
#include <iostream>
#include <string>

int main(){
    Hashing dt;
    std::string command;

    while(std::cin >> command){
        if(command == "LOAD"){
            std::string fileName;
            std::cin >> fileName;
            dt.load(fileName);
        }
        else if(command == "LIST"){
            std::cin.ignore();
            std::string countryName;
            std::getline(std::cin, countryName);
            dt.list(countryName);
        }
        else if (command == "RANGE"){
            std::string seriesCode;
            std::cin >> seriesCode;
            dt.range(seriesCode);
        }
        else if (command == "BUILD"){
            std::string seriesCode;
            std::cin >> seriesCode;
            dt.build(seriesCode);

        }
        else if (command == "FIND"){
            double mean;
            std::string operation;
            std::cin >> mean >> operation;
            dt.find(mean, operation);
        }
        else if (command == "DELETE"){
            std::cin.ignore();
            std::string countryName;
            std::getline(std::cin, countryName);
            dt.deleteName(countryName);
        }
        else if (command == "LIMITS"){
            std::string condition;
            std::cin >> condition;
            dt.limit(condition);
        }
        else if (command == "LOOKUP"){
            std::string code;
            std::cin >> code;
            dt.lookup(code);
        }
        else if(command == "INSERT"){
            std::string code;
            std::string fileName;
            std::cin >> code >> fileName;
            dt.insert(code, fileName);
        }
        else if(command == "REMOVE"){
            std::string code;
            std::cin >> code;
            dt.remove(code);
        }
        else if(command == "EXIT"){
            break;
        }
    }
    return 0;
}