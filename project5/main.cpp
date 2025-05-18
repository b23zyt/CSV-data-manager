#include "CountryGraph.h"
#include <iostream>
#include <string>

int main(){
    CountryGraph cg;
    std::string command;

    while(std::cin >> command){
        if(command == "LOAD"){
            std::string fileName;
            std::cin >> fileName;
            cg.load(fileName);
        }
        else if(command == "INITIALIZE"){
            cg.initialize();
        }
        else if(command == "UPDATE_EDGES"){
            std::string code;
            double threshold;
            std::string relation;
            std::cin >> code >> threshold >> relation;
            cg.update(code, threshold, relation);
        }else if(command == "ADJACENT"){
            std::string countryCode;
            std::cin >> countryCode;
            cg.adjacent(countryCode);
        }else if(command == "PATH"){
            std::string firstCode;
            std::string secondCode;
            std::cin >> firstCode >> secondCode;
            cg.path(firstCode, secondCode);
        }else if(command == "RELATIONSHIPS"){
            std::string firstCode;
            std::string secondCode;
            std::cin >> firstCode >> secondCode;
            cg.relations(firstCode, secondCode);
        }
        


        // else if(command == "LIST"){
        //     std::cin.ignore();
        //     std::string countryName;
        //     std::getline(std::cin, countryName);
        //     dt.list(countryName);
        // }
        // else if (command == "RANGE"){
        //     std::string seriesCode;
        //     std::cin >> seriesCode;
        //     dt.range(seriesCode);
        // }
        // else if (command == "BUILD"){
        //     std::string seriesCode;
        //     std::cin >> seriesCode;
        //     dt.build(seriesCode);

        // }
        // else if (command == "FIND"){
        //     double mean;
        //     std::string operation;
        //     std::cin >> mean >> operation;
        //     dt.find(mean, operation);
        // }
        // else if (command == "DELETE"){
        //     std::cin.ignore();
        //     std::string countryName;
        //     std::getline(std::cin, countryName);
        //     dt.deleteName(countryName);
        // }
        // else if (command == "LIMITS"){
        //     std::string condition;
        //     std::cin >> condition;
        //     dt.limit(condition);
        // }
        // else if (command == "LOOKUP"){
        //     std::string code;
        //     std::cin >> code;
        //     dt.lookup(code);
        // }
        // else if(command == "INSERT"){
        //     std::string code;
        //     std::string fileName;
        //     std::cin >> code >> fileName;
        //     dt.insert(code, fileName);
        // }
        // else if(command == "REMOVE"){
        //     std::string code;
        //     std::cin >> code;
        //     dt.remove(code);
        // }
        else if(command == "EXIT"){
            break;
        }
    }
    return 0;
}