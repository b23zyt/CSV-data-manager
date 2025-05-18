#include "CountryGraph.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>

//This is the helper function I used when I didn't use the binary tree to update edges:
// bool CountryGraph::relationHoldOrNot(double mean, double threshold, std::string relation) {
//     bool ret = false;
//     if(relation == "greater"){
//         if(mean > threshold){
//             ret = true;
//         }
//     }else if(relation == "equal"){
//         if(std::abs(threshold - mean) <= 1e-3){
//             ret = true;
//         }
//     }else if(relation == "less"){
//         if(mean < threshold){
//             ret = true;
//         }
//     }
//     return ret;
// }

CountryGraph::CountryGraph() {
    //initialize the hash table with nullptr in all indexes
    for(int i = 0; i < 512; i++) {
        keys[i] = nullptr;
    }
}

CountryGraph::~CountryGraph(){
}

void CountryGraph::load(std::string fileName){
    //Hash the all country objects into the hash table
    hashing.load(fileName);


    //check if data is loaded
    // for(int i = 0; i < 512; i++){
    //     if(hashing.countries[i] != nullptr){
    //         if(hashing.countries[i]->Deleted == false){
    //             std::cout << hashing.countries[i]->getCountryName()<< ' ';
    //         }
    //     }
        
    // }
    // std::cout << std::endl;
}

void CountryGraph::initialize(){
    //My design is to have two arrays (each of a size of 512)
    //The first array stores all countr objects, named as keys[];
    //The second array stores a list of edges objects, named as edges[];

    //here, I initialize the graph by storing all country object into the graph(keys array)
    for(int i = 0; i < 512; i++){
        if(hashing.countries[i] != nullptr){
            if(hashing.countries[i]->Deleted == false){
                keys[i] = hashing.countries[i];
            }else{
                keys[i] = nullptr;
            }
        }else{
            keys[i] = nullptr;
        }
    }

    //check if the process is successful
    // for(int i = 0; i < 512; i++){
    //     if(keys[i] != nullptr){
    //         std::cout << hashing.countries[i]->getCountryName()<< ' ';
    //     }
        
    // }
    std::cout << "success" << std::endl;
}

void CountryGraph::update(std::string code, double threshold, std::string relation){
    std::vector<std::string> allCodes;

    //build the binary tree based in on the in the hashing class
    hashing.build(code);

    //Modified the FIND function in project3 such that it won't print the countryName that satisfy the condition
    //Instead, it will append the all the countryCode that satisfy the condition into a vector I initialized as AllCodes
    //stores all the codes inside allCode that satisfy the relation;
    hashing.find(threshold, relation, allCodes);

    //if less than 1 country satisfies this relationship, then simply return failure;
    if(allCodes.size() <= 1){
        std::cout << "failure" << std::endl;
        return;
    }

    //check all the value country codes that satisfy the condition.
    // for (const auto& str : allCodes) {
    //     std::cout << str << " ";
    // }
    // std::cout << std::endl;

    //more than 2 codes satisfy the condition
    for (const auto& key : allCodes) {
        //get the index of code in order to update the corresponding entry in the edge array
        int keyIndex = hashing.getHashCode(key);

        //The number of countries that fit this relationship(except the key country)
        int num = 0;
        //a list of countries that fit this relationship(except the key country)
        std::string valueCountries[512];

        //a list of countries index(in hash table) in which its country fit this relationship(except the key country)
        int indexes[512];

        for (const auto& value : allCodes) {
            if(key != value){
                int valueIndex = hashing.getHashCode(value);
                valueCountries[num] = keys[valueIndex]->getCountryName();
                indexes[num] = valueIndex;
                num++;
            }
        }

        //update the edge by adding all the relationships to the linked list in edge.
        //returns false if the relationship already exists in the list.
        bool result = edges[keyIndex].addRelation(code, threshold, relation, num, valueCountries, indexes);

        //if the relation already exist in the list
        if(result == false){
            std::cout << "failure" << std::endl;
            return;
        }
    }
    std::cout << "success" << std::endl;

    //Initial design
    //Learned that I need to use the binary tree to find the edge, changed it to the above version...
    //keep it since both the two methods work

    // bool flag = false;
    // for(int i = 0; i < 512; i++){
    //     if(keys[i] != nullptr){
    //         double keyMean = keys[i]->seriesMean(code);
    //         bool keyRelationHolds = relationHoldOrNot(keyMean, threshold, relation);
    //         if(!keyRelationHolds){
    //             continue;
    //         }

    //         //now the key relation holds

    //         //number of 
    //         int num = 0;

    //         //the countries 
    //         std::string valueCountries[512];
    //         int indexes[512];
    //         for(int j = 0; j < 512; j++){
    //             if(i == j){
    //                 continue;
    //             }
                
    //             if(keys[j] != nullptr){
    //                 double valueMean = keys[j]->seriesMean(code);
    //                 bool valueRelationHolds = relationHoldOrNot(valueMean, threshold, relation);
    //                 if(!valueRelationHolds){
    //                     continue;
    //                 }
    //                 flag = true;
    //                 valueCountries[num] = keys[j]->getCountryName();
    //                 indexes[num] = j;
    //                 num++;
    //             }
    //         }

    //         if(num == 0){
    //             continue;
    //         }

    //         bool result = edges[i].addRelation(code, threshold, relation, num, valueCountries, indexes);

    //         //if the relation already exist
    //         if(result == false){
    //             std::cout << "failure" << std::endl;
    //             return;
    //         }
    //     }      
    // }
    // if(flag == false){
    //     std::cout << "failure" << std::endl;
    // }else{
    //     std::cout << "success" << std::endl;
    // }
}

void CountryGraph::adjacent(std::string code){

    //can optimize it by using hashing.getIndex() to change to O(1)
    //Find the index of the country in the hash table, then print all the countries in that index in edge.
    for(int i = 0; i < 512; i++){
        if(keys[i] != nullptr){
            if(keys[i]->getCountryCode() == code){
                //call the function in edgeList to print all the countries in the list.
                edges[i].printAdjacent();
                std::cout << std::endl;
                return;
            }
        }
    }
    std::cout << "failure" << std::endl;
}

void CountryGraph::pathHelper(int index, std::string secondCode){
    if (pathFound){
        return;
    }
    //get all the countries index stored in the bucket
    std::vector<int> allValues;
    //append all countries in edge[this index] in the vector
    edges[index].findPath(allValues);
    
    //if the vector is empty, then just return
    if(allValues.empty()){
        return;
    }

    //Recursive DFS
    for(int num: allValues){
        //set.insert.second returns true if the set originally doesn't contain the element and the element is successfully added to the set

        //Reference[1]
        //https://www.geeksforgeeks.org/set-insert-function-in-c-stl/
        bool result = visited.insert(num).second;
        if(!result){
            continue;
        }
        //return true if the country matches the second country code
        if(keys[num]->getCountryCode() == secondCode){
            pathFound = true;
            return;
        }else{
            pathHelper(num, secondCode);
        }
    }

    // for(int num: allValues){
    //     if(keys[num]->getCountryCode() == secondCode){
    //         pathFound = true;
    //         return;
    //     // }else{
    //     //     pathHelper(num, secondCode);
    //     // }
    //     }
    // }

    return;
}


void CountryGraph::path(std::string firstCode, std::string secondCode) {
    //clear the visited set every time path is called
    visited.clear();
    pathFound = false;
    //the 2 countries will exist in the graph
    for(int i = 0; i < 512; i++){
        if(keys[i] != nullptr){
            //find the index of the first code
            if(keys[i]->getCountryCode() == firstCode){
                //visited[numVisited] = i;
                //insert the key country to the set
                visited.insert(i);
                numVisited++;
                //check path recursively
                pathHelper(i, secondCode);
            }
        }
    }
    if(pathFound){
        std::cout << "true" << std::endl;
        return;
    }
    std::cout << "false" << std::endl;
}

void CountryGraph::relations(std::string firstCode, std::string secondCode) {
    for(int i = 0; i < 512; i++){
        if(keys[i] != nullptr){
            if(keys[i]->getCountryCode() == firstCode){
                for(int j = 0; j < 512; j++){
                    if(keys[j] != nullptr){
                        if(keys[j]->getCountryCode() == secondCode){
                            //first check if the two countries exist in the graph and get their name
                            std::string countryName = keys[j]->getCountryName();
                            //check if the linked list contain the secondcode's country name
                            edges[i].findRelation(countryName);
                            std::cout << std::endl;
                            return;
                        }
                    }
                }
            }
        }
    }
    std::cout << "none" << std::endl;
}
