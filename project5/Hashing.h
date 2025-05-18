#ifndef HASHING_H
#define HASHING_H

#include "SeriesList.h"
#include "TreeNode.h"
#include <string>
#include <vector>

class Hashing
{
    private:
    
    
    double seriesMin;
    double seriesMax;

    std::string treeSeriesCode;
    int countriesUsedIndex[512];

    void getMinAndMax(std::string seriesCode);

    TreeNode* root;

    TreeNode* buildTree(std::string seriesCode, double min, double max, int indexes[], int nodeStringNumber);

    bool allEqual(std::string seriesCode, int indexes[], int count);

    void printLess(double value, TreeNode* node, std::vector<std::string>& ret);
    void printGreat(double value, TreeNode* node, std::vector<std::string>& ret);
    void printEqual(double value, TreeNode* node, std::vector<std::string>& ret);
    TreeNode* deleteLeftHelper(std::string name, TreeNode* node);
    TreeNode* deleteRightHelper(std::string name, TreeNode* node);
    void leftLimit(TreeNode* node);
    void rightLimit(TreeNode* node);

    void postorderDelete(TreeNode* curr);

    //hashing functions:
    int primaryHash(std::string code);
    int secondaryHash(std::string code);

public:
    SeriesList* countries[512];
    int countryIndex; //represents number of elements in the array (countryNumber)

    Hashing();
    void load(std::string fileName);
    void list(std::string countryName);
    void range(std::string seriesCode);
    void build(std::string seriesCode);
    //can be a a private function
   

    void find(double mean, std::string operation, std::vector<std::string>& ret);
    void deleteName(std::string name);
    void limit(std::string condition);

    //commands for project4
    void lookup(std::string code);
    void insert(std::string code, std::string fileName);
    void remove(std::string code);

    int getHashCode(std::string code);
    ~Hashing();

};

#endif