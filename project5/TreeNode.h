#ifndef TREE_NODE_H
#define TREE_NODE_H
#include <iostream>

class TreeNode
{
public:
    double minRange;
    double maxRange;
    std::string* names;
    std::string* codes;
    int storedCountryIndex[512];
    TreeNode* left;
    TreeNode* right;
    TreeNode();
    ~TreeNode();

    int nodeCountryNumber;
};


#endif