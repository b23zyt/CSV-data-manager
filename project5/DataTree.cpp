#include "DataTree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//helper function for building the tree
//used to get the range and store min value and max vale of the given series to the class
//initially just copied from the range function
//Note: then realized that the mean of no valid data is 0, but this function cannot update invalid data as 0.
//made some changes to update 0
void DataTree::getMinAndMax(std::string seriesCode){
    if(countryIndex == 0){
        return;
    }    
    bool firstFlag = true;
    double value = countries[0]->seriesMean(seriesCode);
    if(value == -1){
        value = 0;
    }
    seriesMin = value;
    seriesMax = value;
    for(int i = 1; i < countryIndex; i++){
        value = countries[i]->seriesMean(seriesCode);
        if(value == -1){
            value = 0;
        }
        if(value < seriesMin){
            seriesMin = value;
        }
        if(value > seriesMax){
            seriesMax = value;
        }
    }
    return;
}

//helper function for building the tree: check whether the difference between countries mean is less than the tolerance value
bool DataTree::allEqual(std::string seriesCode, int indexes[], int count)
{
    if(count <= 1){
        return true;
    }
    double min = countries[indexes[0]]->seriesMean(seriesCode);
    double max = countries[indexes[0]]->seriesMean(seriesCode);
    if(min == -1){
        min = 0;
    }
    if(max == -1){
        max = 0;
    }
    double tolerance = 1e-3;

    for(int i = 1; i < count; i++) {
        double mean = countries[indexes[i]]->seriesMean(seriesCode);
        if(mean == -1){
            mean = 0;
        }
        if(mean > max){
            max = mean;
        }
        if(mean < min){
            min = mean;
        }
    }
    double difference = max -min;

    if(difference < tolerance){
        return true;
    }
    return false;
}

//helper function to build the tree 
//Note: Indexes[] stores the indexes of these countries in the 'countries' array stored in this node.
//More explanation of this helper function in the design document
TreeNode* DataTree::buildTree(std::string seriesCode, double min, double max, int indexes[], int nodeStringNumber) {
    //std::cout << min << ' ' << max << ' ' << nodeStringNumber << ' ';
    TreeNode* newNode = new TreeNode();
    newNode->minRange = min;
    newNode->maxRange = max;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->nodeCountryNumber = nodeStringNumber;

    //edge case(all equal or 1 country left)
    if(nodeStringNumber == 1 || allEqual(seriesCode, indexes, nodeStringNumber)){
        for(int i = 0; i < nodeStringNumber; i++){
            //std::cout << countries[indexes[i]].getCountryName() << std::endl;
            newNode->names[i] = countries[indexes[i]]->getCountryName();
            newNode->storedCountryIndex[i] = indexes[i];
        }
        return newNode;
    }

    int allLeft[512];
    int allRight[512];
    int leftIndex = 0;
    int rightIndex = 0;
    double medium = (min + max)/2;

    for(int i = 0; i < nodeStringNumber; i++){//country name
        int oneIndex = indexes[i];
        double mean = countries[oneIndex]->seriesMean(seriesCode);
        if(mean == -1){
            mean = 0;
        }
        if(mean >= min && mean < medium){
            allLeft[leftIndex] = oneIndex;
            leftIndex++;
        }else{
            allRight[rightIndex] =  oneIndex;
            rightIndex++;
        }
        newNode->names[i] = countries[oneIndex]->getCountryName();
        newNode->storedCountryIndex[i] = indexes[i];
        //std::cout << countries[indexes[i]].getCountryName() << std::endl;
    }
    newNode->left = buildTree(seriesCode, min, medium, allLeft, leftIndex);
    newNode->right = buildTree(seriesCode, medium, max, allRight, rightIndex);
    return newNode;
}


void DataTree::printLess(double value, TreeNode* node) {
    if(node == nullptr){
        return;
    }

    //if min > value, no countries satisfy
    if(node->minRange >= value){
        return;
    }
    
    //value > min
    if(value > node->maxRange){
        for(int i = 0; i < node->nodeCountryNumber; i++){
            std::cout << node->names[i] << ' ';
        }
    }else{
        //value <= max
        //edge case (initially didn't figure out): if left or right is nullptr, then the rest wont print!!!!
        //if left is null
        if(node->left == nullptr){
            for(int i = 0; i < node->nodeCountryNumber; i++){
                //encountered a problem at here: unable to access the country, therefore go back to 'build' and stored the index of country at each node!!!
                //now I can access it!!!
                double mean = countries[node->storedCountryIndex[i]]->seriesMean(treeSeriesCode);
                if(mean == -1){
                    mean = 0;
                }
                if(mean < value){
                    std::cout << node->names[i] << ' ';
                }
            }
        }
        //if right is null: fine, left contains all the needing countries.
        printLess(value, node->left);
        printLess(value, node->right);
    }
}

void DataTree::printGreat(double value, TreeNode *node){
    if(node == nullptr){
        return;
    }
    if(node->maxRange <= value){
        return;
    }

    //value < max
    if(value < node->minRange){
        for(int i = 0; i < node->nodeCountryNumber; i++){
            std::cout << node->names[i] << ' ';
        }
    }else{
        //value >= max
        //edge case: if right is null
        if(node->right == nullptr){
            for(int i = 0; i < node->nodeCountryNumber; i++){
                double mean = countries[node->storedCountryIndex[i]]->seriesMean(treeSeriesCode);
                if(mean == -1){
                    mean = 0;
                }
                if(mean > value){
                    std::cout << node->names[i] << ' ';
                }
            }     
        }
        printGreat(value, node->right);
        printGreat(value, node->left);
    }
}

//O(N) is fine, although O(logN) is possible
void DataTree::printEqual(double value, TreeNode *node){
    double tolerance = 1e-3;
    //not needed actually
    if (node == nullptr){
        return;
    }
    if(value < node->minRange || value > node->maxRange){
        return;
    }
    for(int i = 0; i < node->nodeCountryNumber; i++){
        double mean = countries[node->storedCountryIndex[i]]->seriesMean(treeSeriesCode);
        if(mean == -1){
            mean = 0;
        }
        if(-tolerance <= (mean - value) && (mean - value) <= tolerance){
            std::cout << node->names[i] << ' ';
        }
    }
}


//helper functions for DELETE
TreeNode* DataTree::deleteLeftHelper(std::string name, TreeNode *node){
    int deleteIndex = 0;
    if(node == nullptr){
        return nullptr;
    }
    if(node->nodeCountryNumber == 1){
        delete node;
        return nullptr;
    }
    for(int i = 0; i < node->nodeCountryNumber; i++){
        if(node->names[i] == name){
            deleteIndex = i;
            break;
        }
        //not found, return
        if(i == node->nodeCountryNumber -1){
            return node;
        }
    }
    for(int i = deleteIndex; i < node->nodeCountryNumber - 1; i++){
        node->names[i] = node->names[i+1];
        node->storedCountryIndex[i] = node->storedCountryIndex[i+1];
    }
    node->names[node->nodeCountryNumber - 1] = "";
    node->storedCountryIndex[node->nodeCountryNumber - 1] = -1;
    int newNodeCountryNumber = node->nodeCountryNumber - 1;
    node->nodeCountryNumber = newNodeCountryNumber;
    node->left = deleteLeftHelper(name, node->left);
    node->right = deleteLeftHelper(name, node->right);

    return node;
}

TreeNode* DataTree::deleteRightHelper(std::string name, TreeNode *node){
    int deleteIndex = 0;
    if(node == nullptr){
        return nullptr;
    }
    if(node->nodeCountryNumber == 1){
        delete node;
        return nullptr;
    }
    for(int i = 0; i < node->nodeCountryNumber; i++){
        if(node->names[i] == name){
            deleteIndex = i;
            break;
        }
        //not found, return
        if(i == node->nodeCountryNumber - 1){
            return node;
        }
    }
    for(int i = deleteIndex; i < node->nodeCountryNumber - 1; i++){
        node->names[i] = node->names[i+1];
        node->storedCountryIndex[i] = node->storedCountryIndex[i+1];
    }
    node->names[node->nodeCountryNumber - 1] = "";
    node->storedCountryIndex[node->nodeCountryNumber - 1] = -1;
    int newNodeCountryNumber = node->nodeCountryNumber - 1;
    node->nodeCountryNumber = newNodeCountryNumber;
    node->right = deleteRightHelper(name, node->right);
    node->left = deleteRightHelper(name, node->left);
    
    return node;
}

void DataTree::leftLimit(TreeNode *node){
    //same algorithm as the base function 
    if(node->left == nullptr){
        if(node->right == nullptr){
            for(int i = 0; i < node->nodeCountryNumber; i++){
                std::cout << node->names[i] << ' ';
            }
            return;
        }else{
            leftLimit(node->right);
        }
    }else{
        leftLimit(node->left);
    }
}

void DataTree::rightLimit(TreeNode *node){
    //same algorithm as the base function 
    if(node->right == nullptr){
        if(node->left == nullptr){
            for(int i = 0; i < node->nodeCountryNumber; i++){
                std::cout << node->names[i] << ' ';
            }
            return;
        }else{
            rightLimit(node->left);
        }
    }
    else{
        rightLimit(node->right);
    }
}

void DataTree::postorderDelete(TreeNode *curr){
    if(curr == nullptr) return;
    postorderDelete(curr->left);
    postorderDelete(curr->right);
    delete(curr);
    curr = nullptr;
}

//PUBLIC FUNCTIONS
DataTree::DataTree() : countryIndex(-1), root(nullptr), treeSeriesCode(""){
}

//load function
void DataTree::load(std::string fileName)
{   
    std::ifstream myFile(fileName);
    if (!myFile.is_open()) {
        return;
    }
    //countryIndex++;
    countryIndex = 0;
    int currentLineCount = 0;
    std::string line;

    //std::string lines[512];
    
    //create an array to store the all the series the same country
    //since the number of series of each series is unknown, begin with 2 and double it when the size is full
    int capacity = 2;
    std::string* lines = new std::string[capacity];

    //used to check if the next series's country name changes
    std::string countryNamePrev = "";

    while(std::getline(myFile, line)){

        std::string currentLine = line;

        std::stringstream lineStream(line);
        std::string countryName;

        std::getline(lineStream, countryName, ',');

        //when the country name changes, we need to store the data for the previous country
        //
        if (countryNamePrev != "" && countryName != countryNamePrev){
            //In the previous version, I stored a SeriesList object in the countries array, which required implementing the assignment operator for SeriesList and TimeSeries.
            // SeriesList sl;
            // sl.newLoad(lines, currentLineCount);
            // countries[countryIndex] = &sl;

            //I now store pointers to `SeriesList` objects
            //This way, each country gets its own dynamically allocated SeriesList
            //Note: it requires the destructor to delete every element in countries array
            //more comments in 'newLoad' ... 
            countries[countryIndex] = new SeriesList();
            countries[countryIndex]->newLoad(lines, currentLineCount);
            countryIndex++;

            //clear the lines array for the next country's data
            for (int i = 0; i < currentLineCount; i++) {
                lines[i] = ""; 
            }

            currentLineCount = 0;
        }

        //copied the double algorithm from project 1
        if (currentLineCount >= capacity) {
            int newCapcity = capacity * 2;
            std::string* newLines = new std::string[newCapcity];

            for (int i = 0; i < capacity; i++) {
                newLines[i] = lines[i];
            }

            delete[] lines;
            lines = newLines;
            capacity = newCapcity;
        }
        //store the new line to 'lines' array
        lines[currentLineCount] = currentLine;
        countryNamePrev = countryName;
        currentLineCount++;
    }

    //for the last country, if there are lines remaining 
    if (currentLineCount > 0) {
        // SeriesList sl; 
        // sl.newLoad(lines, currentLineCount);
        // countries[countryIndex] = &sl;
        countries[countryIndex] = new SeriesList();
        countries[countryIndex]->newLoad(lines, currentLineCount);
        countryIndex++;
    }
    
    if (lines != nullptr) {
        delete[] lines;
        lines = nullptr;
    }
    std::cout << "success" << std::endl;
    myFile.close();

    // std::cout << countries[0].getCountryName();
    // std::cout << std::endl;
    // std::cout << countries[100].biggest();
}

void DataTree::list(std::string countryName){
    //find the country in countries and call its list function
    for(int i = 0; i < countryIndex; i++){
        //std::cout << countries[i].getCountryName() << ' ' << std::endl;
        if (countryName == countries[i]->getCountryName()){
            countries[i]->list();
        }
    }
    //std::cout << countryIndex << std::endl;
}

void DataTree::range(std::string seriesCode){
    // std::cout << countryIndex; 266
    // std::cout << seriesCode;
    double min = 0;
    double max = 0;
    bool firstFlag = true;
    //a flag to check if valid data exists for at least one country
    bool retFlag = false;
    for(int i = 0; i < countryIndex; i++){
        //std::cout << countries[i].seriesMean(seriesCode) << std::endl;
        if(countries[i]->seriesMean(seriesCode) == -1){
            continue;
        }

        //the first valid data
        if(firstFlag){
            //std::cout << countries[i].seriesMean(seriesCode) << std::endl;
            min = countries[i]->seriesMean(seriesCode);
            max = countries[i]->seriesMean(seriesCode);
            firstFlag = false;
            retFlag = true;
            continue;
        } 
        //std::cout << countries[i].seriesMean(seriesCode) << std::endl;
        if(countries[i]->seriesMean(seriesCode) < min){
            min = countries[i]->seriesMean(seriesCode);
        }
        if(countries[i]->seriesMean(seriesCode) > max){
            max = countries[i]->seriesMean(seriesCode);
        }
    }
    
    if(retFlag){
        std::cout << min << ' ' << max << std::endl;
        return;
    }
    return;
}


//START TO BUILD THE TREE!!!
void DataTree::build(std::string seriesCode) {
    //delete the previous tree(if exist) when the command is called
    if(root != nullptr){
        postorderDelete(root);
        root = nullptr;
    }
    //store the code in the class
    treeSeriesCode = seriesCode;

    //helper function to update seriesMin and seriesMax. (note: invalid data = 0)
    getMinAndMax(seriesCode);

    int nodeCountryIndexes[512];
    int nodeCountriesNumber = 0;

    for(int i = 0; i < countryIndex; i++){
        //the index of country in 'countries' array
        nodeCountryIndexes[i] = i;

        //number of country in the root of the tree
        nodeCountriesNumber++;
    }
    root = buildTree(seriesCode, seriesMin, seriesMax, nodeCountryIndexes , nodeCountriesNumber);
    std::cout << "success" << std::endl;
}


void DataTree::find(double mean, std::string operation){
    //edge case: return failure if no binary tree exists
    if(root == nullptr){
        std::cout << "failure" << std::endl;
        return;
    }
    //recursive helper functions
    if(operation == "less") {
        printLess(mean, root);
    }
    else if(operation == "greater"){
        printGreat(mean, root);
    }
    else if(operation == "equal"){
        printEqual(mean, root);
    }
    std::cout << std::endl;
}

void DataTree::deleteName(std::string name){
    int deleteIndex = 0;
    //edge cases: if no binary tree exists or the country does not appear in the binary tree
    if(root == nullptr){
        std::cout << "failure" << std::endl;
        return;
    }
    for(int i = 0; i < root->nodeCountryNumber; i++){
        if(root->names[i] == name){
            deleteIndex = i;
            break;
        }
        //country not found
        if(i == root->nodeCountryNumber -1){
            std::cout << "failure" << std::endl;
            return;
        }
    }
    //delete the country in root.
    for(int i = deleteIndex; i < root->nodeCountryNumber - 1; i++){
        root->names[i] = root->names[i+1];
        root->storedCountryIndex[i] = root->storedCountryIndex[i+1];
    }
    root->names[root->nodeCountryNumber - 1] = "";
    root->storedCountryIndex[root->nodeCountryNumber -1] = -1;

    int newNodeCountryNumber = root->nodeCountryNumber - 1;
    root->nodeCountryNumber = newNodeCountryNumber;

    //recursively delete the country on the left and right subtrees of root
    root->left = deleteLeftHelper(name, root->left);
    root->right = deleteRightHelper(name, root->right);
    std::cout << "success" << std::endl;
}

void DataTree::limit(std::string condition){
    //edge case: if no tree exists
    if (root == nullptr){
        std::cout << "failure" << std::endl;
        return;
    }
    
    if(condition == "lowest"){
        
        if(root->left == nullptr){
            //case 1: if root is the leaf, then print the countries in root.
            if(root->right == nullptr){
                for(int i = 0; i < root->nodeCountryNumber; i++){
                    std::cout << root->names[i] << ' ';
                }
            }else{
                //case 2, if right is not null but left is null, find the smallest node in the right subtree
                leftLimit(root->right);
            }
        }else{
            //case 3: if left is not null, find the smallest node in the left subtree
            leftLimit(root->left);
        }
    }else if(condition == "highest"){
        //same algorithm as above
        if(root->right == nullptr){
            if(root->left == nullptr){
                for(int i = 0; i < root->nodeCountryNumber; i++){
                    std::cout << root->names[i] << ' ';
                }
            }else{
                rightLimit(root->left);
            }
        }else{
            rightLimit(root->right);
        }
    }
    std::cout << std::endl;
}

DataTree::~DataTree(){
    for(int i = 0; i < countryIndex; i++) {
        delete countries[i];
    }
    postorderDelete(root);
    root = nullptr;
}


//old load: too slow
// void DataTree::load(std::string fileName)
// {
//     std::ifstream myFile(fileName);
//     // if(!myFile) return false;
//     std::string line;

//     while(std::getline(myFile, line)) {
//         std::cout << "hello1";
//         std::stringstream lineStream(line);
//         std::string countryName;
//         std::string countryCode;
//         std::string prev = "";

//         std::getline(lineStream, countryName , ',');
//         std::getline(lineStream, countryCode , ',');

//         if(countryName != prev){
//             SeriesList sl;
//             sl.load(countryName);     
//             countries[index] = sl;
//             index++;
//             prev = countryName;
//         }
//         else{
//             continue;
//         }
//     }
//     for(int i = 0; i < 2; i++){
//         std::cout << countries[i].biggest();
//     }
// }

// std::ifstream myFile(fileName);
//     std::string line;
//     std::string prev = " ";
//     std::string arr[512];

//     while(std::getline(myFile, line)) {
//         std::stringstream lineStream(line);
//         std::string countryName;
//         std::string countryCode;

//         std::getline(lineStream, countryName , ',');
//         std::getline(lineStream, countryCode, ',');
//         //std::cout << countryName << std::endl;

        
//         if(countryName != prev){
//             index++; 
//             arr[index] = countryName;
//             prev = countryName;
//         }
//         else{
//             continue;
//         }
//         helper(arr);
//     }