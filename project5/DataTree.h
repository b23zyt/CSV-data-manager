#ifndef DATA_TREE_H
#define DATA_TREE_H

#include "SeriesList.h"
#include "TreeNode.h"
#include <string>

class DataTree
{
private:
    
    double seriesMin;
    double seriesMax;

    std::string treeSeriesCode;

    void getMinAndMax(std::string seriesCode);

    TreeNode* root;

    TreeNode* buildTree(std::string seriesCode, double min, double max, int indexes[], int nodeStringNumber);

    bool allEqual(std::string seriesCode, int indexes[], int count);

    void printLess(double value, TreeNode* node);
    void printGreat(double value, TreeNode* node);
    void printEqual(double value, TreeNode* node);
    TreeNode* deleteLeftHelper(std::string name, TreeNode* node);
    TreeNode* deleteRightHelper(std::string name, TreeNode* node);
    void leftLimit(TreeNode* node);
    void rightLimit(TreeNode* node);

    void postorderDelete(TreeNode* curr);

public:
    int countryIndex;
    SeriesList* countries[512];
    DataTree();
    void load(std::string fileName);
    void list(std::string countryName);
    void range(std::string seriesCode);
    void build(std::string seriesCode);
    //can be a a private function
   

    void find(double mean, std::string operation);
    void deleteName(std::string name);
    void limit(std::string condition);


    ~DataTree();
};

#endif