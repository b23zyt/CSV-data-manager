#include "TreeNode.h"

TreeNode::TreeNode() : right(nullptr), left(nullptr), names(new std::string[512]), minRange(0.0), maxRange(0.0) {
}

TreeNode::~TreeNode() {
    delete[] names;
}
