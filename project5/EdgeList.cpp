#include "EdgeList.h"
#include <iostream>
#include <set>
#include <vector>

EdgeList::EdgeList():head(nullptr) {
}

bool EdgeList::addRelation(std::string code, double threshold, std::string relation, int num, std::string* countries, int indexes[])
{
    //if the list is empty, initialize the list
    if (head == nullptr){
        Node* newNode = new Node();
        newNode->next = nullptr;
        newNode->relation = relation;
        newNode->threshold = threshold;
        newNode->code = code;
        for(int i = 0; i < num; i++){
            newNode->listCountries[i] = countries[i];
            newNode->listofIndex[i] = indexes[i];
        }
        newNode->numberOfCountries = num;
        head = newNode;
        return true;
    }
    
    //if the list is not empty, check if the relationship exist, if not, add it to the end of the linked list
    Node* current = head;
    Node* previous = nullptr;
    while(current != nullptr){
        if(current->relation == relation && current->code == code && current->threshold == threshold){
            return false;
        }
        previous = current;
        current = current->next;  
    }
    Node* newNode = new Node();
    newNode->next = nullptr;
    newNode->relation = relation;
    newNode->threshold = threshold;
    newNode->code = code;
    for(int i = 0; i < num; i++){
        newNode->listCountries[i] = countries[i];
        newNode->listofIndex[i] = indexes[i];
    }
    newNode->numberOfCountries = num;
    previous->next = newNode;
    return true;
}

void EdgeList::printAdjacent()
{
    //if no relationships, print none
    if(head == nullptr){
        std::cout << "none";
        return;
    }

    Node* current = head;
    std::set<std::string> countries;

    //insert all the countries in the list to the set
    while(current != nullptr){
        for(int i = 0; i < current->numberOfCountries; i++){
            countries.insert(current->listCountries[i]);
        }
        current = current->next;
    }
    if(countries.empty()){
        std::cout << "none"; 
        return;
    }

    //print all the elements in the set
    //auto represents: std::set<std::string>::iterator 
    //Reference[2]
    //https://www.geeksforgeeks.org/different-ways-to-iterate-over-a-set-in-c/
    auto cty = countries.begin();
    if(cty != countries.end()) {
        std::cout << *cty;
        ++cty;
        
        for(; cty != countries.end(); ++cty){
            std::cout << ' ' << *cty;
        }
    }
}

//update the ret vector
void EdgeList::findPath(std::vector<int>& ret){
    
    if(head == nullptr){
        // return ret;
        return;
    }
    Node* current = head;
    //push all the country index to the vector
    while (current != nullptr){
        for(int i = 0; i < current->numberOfCountries; i++){
            ret.push_back(current->listofIndex[i]);
        }
        current = current->next;
    }
    // return ret;
}


// void EdgeList::findPath(std::string name){
//     if(head == nullptr){
//         std::cout << "false";
//         return;
//     }
//     Node* current = head;
//     while(current != nullptr){
//         for(int i = 0; i < current->numberOfCountries; i++){
//             if(current->listCountries[i] == name){
//                 std::cout << "true";
//                 return;
//             }
//         }
//         current = current->next;
//     }
//     // std::cout << "hello" << std::endl;
//     std::cout << "false";
// }



void EdgeList::findRelation(std::string name) {
    if(head == nullptr){
        std::cout << "none";
        return;
    }
    bool exist = false;
    //bool first = true;
    Node* current = head;
    //print all the relationships in the linked list 
    while(current != nullptr){
        for(int i = 0; i < current->numberOfCountries; i++){
            if(current->listCountries[i] == name){
                exist = true;
                std::cout << '(' << current->code << ' ' << current->threshold << ' ' << current->relation << ") ";
                // if(first){
                //     std::cout << '(' << current->code << ' ' << current->threshold << ' ' << current->relation << ')';
                //     first = false;
                // }else{
                //     std::cout << " (" << current->code << ' ' << current->threshold << ' ' << current->relation << ')';
                // }
            }
        }
        current = current->next;
    }
    if(exist){
        return;
    }
    std::cout << "none";
}

//destructor
EdgeList::~EdgeList() {
    Node* current = head;
    while (current != nullptr){
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}
