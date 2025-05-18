#ifndef COUNTRY_GRAPH_H
#define COUNTRY_GRAPH_H

#include <iostream>
#include <string>
#include <set>
#include "Hashing.h"
#include "SeriesList.h"
#include "EdgeList.h"
#include "DataTree.h"
#include <vector>


class CountryGraph {
private:
    Hashing hashing;
    
    SeriesList* keys[512];
    EdgeList edges[512];

    // int visited[512];
    std::set<int> visited;
    int numVisited = 0;
    bool pathFound = false;

    //bool relationHoldOrNot(double mean, double threshold, std::string relation);

    void pathHelper(int index, std::string secondCode);
    
public:
    CountryGraph();
    ~CountryGraph();

    void load(std::string fileName);
    void initialize();
    void update(std::string code, double threshold, std::string relation);
    void adjacent(std::string code);
    void path(std::string firstCode, std::string secondCode);
    void relations(std::string firstCode, std::string secondCode);
};

#endif