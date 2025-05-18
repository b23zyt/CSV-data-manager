#ifndef EDGE_LIST_H
#define EDGE_LIST_H

#include "SeriesList.h"
#include <string>
#include <vector>


class EdgeList {
    class Node{
        
        
    public:
        int numberOfCountries;

        std::string code;
        double threshold;
        std::string relation;

        std::string listCountries[512];
        int listofIndex[512];
        Node* next;

        Node(){

        }
        Node(std::string c, double t, std::string r){
            code = c;
            threshold = t;
            relation = r;
        }
        
        ~Node(){
        }
    };
    
private:
    Node* head;

public:
    EdgeList();
    bool addRelation(std::string code, double threshold, std::string relation, int num, std::string* countries, int indexes[]);
    void printAdjacent();
    //void findPath(std::string name);
    // std::vector<int> findPath();
    void findPath(std::vector<int>& ret);
    void findRelation(std::string name);
    
    ~EdgeList();
};

#endif