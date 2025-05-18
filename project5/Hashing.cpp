#include "Hashing.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//helper function for building the tree
//used to get the range and store min value and max vale of the given series to the class
//initially just copied from the range function
//Note: then realized that the mean of no valid data is 0, but this function cannot update invalid data as 0.
//made some changes to update 0
void Hashing::getMinAndMax(std::string seriesCode){
    if(countryIndex == 0){
        return;
    }    
    int i = 0;
    for(; i < countryIndex; i++){
        double value = countries[countriesUsedIndex[i]]->seriesMean(seriesCode);
        if(countries[countriesUsedIndex[i]] == nullptr){
            continue;
        }
        else if(countries[countriesUsedIndex[i]]->Deleted == true){
            continue;
        }else{
            double value = countries[countriesUsedIndex[i]]->seriesMean(seriesCode);
            if(value == -1){
                value = 0;
            }
            seriesMin = value;
            seriesMax = value;
            break;
        }
    }
    
    for(; i < countryIndex; i++){
        //std::cout << "test" << std::endl;
        double value = countries[countriesUsedIndex[i]]->seriesMean(seriesCode);
        if(countries[countriesUsedIndex[i]] == nullptr){
            continue;
        }
        else if(countries[countriesUsedIndex[i]]->Deleted == true){
            continue;
        }else{
            double value = countries[countriesUsedIndex[i]]->seriesMean(seriesCode);
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
    }
    return;
}

//helper function for building the tree: check whether the difference between countries mean is less than the tolerance value
bool Hashing::allEqual(std::string seriesCode, int indexes[], int count)
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
TreeNode* Hashing::buildTree(std::string seriesCode, double min, double max, int indexes[], int nodeStringNumber) {
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
            newNode->codes[i] = countries[indexes[i]]->getCountryCode();
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
        newNode->codes[i] = countries[oneIndex]->getCountryCode();
        newNode->storedCountryIndex[i] = indexes[i];
        //std::cout << countries[indexes[i]].getCountryName() << std::endl;
    }
    newNode->left = buildTree(seriesCode, min, medium, allLeft, leftIndex);
    newNode->right = buildTree(seriesCode, medium, max, allRight, rightIndex);
    return newNode;
}


void Hashing::printLess(double value, TreeNode* node, std::vector<std::string>& ret) {
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
            ret.push_back(node->codes[i]);
            //std::cout << node->names[i] << ' ';
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
                    ret.push_back(node->codes[i]);
                    //std::cout << node->names[i] << ' ';
                }
            }
        }
        //if right is null: fine, left contains all the needing countries.
        printLess(value, node->left, ret);
        printLess(value, node->right, ret);
    }
}

void Hashing::printGreat(double value, TreeNode *node, std::vector<std::string>& ret){
    if(node == nullptr){
        return;
    }
    if(node->maxRange <= value){
        return;
    }

    //value < max
    if(value < node->minRange){
        for(int i = 0; i < node->nodeCountryNumber; i++){
            ret.push_back(node->codes[i]);
            //std::cout << node->names[i] << ' ';
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
                    ret.push_back(node->codes[i]);
                    //std::cout << node->names[i] << ' ';
                }
            }     
        }
        printGreat(value, node->right, ret);
        printGreat(value, node->left, ret);
    }
}

//O(N) is fine, although O(logN) is possible
void Hashing::printEqual(double value, TreeNode *node, std::vector<std::string>& ret){
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
            ret.push_back(node->codes[i]);
            //std::cout << node->names[i] << ' ';
        }
    }
}


//helper functions for DELETE
TreeNode* Hashing::deleteLeftHelper(std::string name, TreeNode *node){
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
        root->codes[i] = root->codes[i+1];
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

TreeNode* Hashing::deleteRightHelper(std::string name, TreeNode *node){
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
        root->codes[i] = root->codes[i+1];
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

void Hashing::leftLimit(TreeNode *node){
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

void Hashing::rightLimit(TreeNode *node){
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

void Hashing::postorderDelete(TreeNode *curr){
    if(curr == nullptr) return;
    postorderDelete(curr->left);
    postorderDelete(curr->right);
    delete(curr);
    curr = nullptr;
}

int Hashing::primaryHash(std::string code)
{
    int ret;
    ret = code[0]- 'A';
    int i = 1;
    for(int i = 1; i < 3; i++){
        //horner's rule
        ret = ret*26 + (code[i] - 'A');
    }
    ret = ret%512;
    return ret;
}

int Hashing::secondaryHash(std::string code)
{
    int ret;
    ret = code[0]- 'A';
    int i = 1;
    for(int i = 1; i < 3; i++){
        //horner's rule
        ret = ret*26 + (code[i] - 'A');
    }
    ret = ret/512;
    if(ret % 2 == 0){
        return (ret + 1);
    }else{
        return ret;
    }
}

//PUBLIC FUNCTIONS
Hashing::Hashing() : countryIndex(0), root(nullptr), treeSeriesCode(""){
    //empty the table
    for(int i = 0; i < 512; i++){
        countries[i] = nullptr;
    }
}

//load function
void Hashing::load(std::string fileName)
{   
    //empty the table before load
    // for(int i = 0; i < 512; i++){
    //     countries[i] = nullptr;
    // }

    std::ifstream myFile(fileName);
    if (!myFile.is_open()) {
        return;
    }
    // countryIndex = 0;
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
        if (countryNamePrev != "" && countryName != countryNamePrev){
            std::string line = lines[0];
            std::stringstream lineStream(line);
            std::string dummy;
            std::string countryCode;
            std::getline(lineStream, dummy, ',');
            std::getline(lineStream, countryCode, ',');

            int hashedIndex = primaryHash(countryCode);
            bool countryExist = false;

            //updates after finished INSERT
            int i = 1;
            int deletedIndex = -1;
            bool firstDeleted = true;
            bool wasDeleted = false;

            while(true){
                if (countries[hashedIndex] == nullptr){
                    break;
                }
                else if(countries[hashedIndex]->getCountryCode() != countryCode && countries[hashedIndex]->Deleted == true && firstDeleted){
                    deletedIndex = hashedIndex;
                    firstDeleted = false;
                }
                else if(countries[hashedIndex]->getCountryCode() == countryCode){
                    if(countries[hashedIndex]->Deleted == true){
                        if(firstDeleted){
                            wasDeleted = true;
                            break;
                        }
                    }
                    else{
                        countryExist = true;
                        break;
                    }
                }
                hashedIndex = (hashedIndex + secondaryHash(countryCode)) % 512;
                //std::cout << hashedIndex << " ";
                i++;
            }

            if(countryExist){
                //ignore existed countries
                for (int i = 0; i < currentLineCount; i++) {
                    lines[i] = ""; 
                }
                currentLineCount = 0;
            }else if(wasDeleted){
                for (int i = 0; i < currentLineCount; i++) {
                    lines[i] = ""; 
                }
                currentLineCount = 0;
                countries[hashedIndex]->Deleted = false;
            }
            else{
                if(deletedIndex != -1){
                    hashedIndex = deletedIndex;
                }

                countriesUsedIndex[countryIndex] = hashedIndex;
                //std::cout << countryCode << " " << hashedIndex << ' ';
                countryIndex++;

                countries[hashedIndex] = new SeriesList();
                countries[hashedIndex]->newLoad(lines, currentLineCount);
                //std::cout << countries[hashedIndex]->getCountryName() << ' ' << countries[hashedIndex]->getCountryCode() << std::endl;
                

                //clear the lines array for the next country's data
                for (int i = 0; i < currentLineCount; i++) {
                    lines[i] = ""; 
                }
                currentLineCount = 0;
            }
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
        std::string line = lines[0];
        std::stringstream lineStream(line);
        std::string countryName;
        std::string countryCode;
        std::getline(lineStream, countryName, ',');
        std::getline(lineStream, countryCode, ',');

        int hashedIndex = primaryHash(countryCode);
        int i = 1;
        bool exist = false;

        int deletedIndex = -1;
        bool firstDeleted = true;
        bool wasDeleted = false;

        while(true){
            if (countries[hashedIndex] == nullptr){
                break;
            }else if(countries[hashedIndex]->getCountryCode()!= countryCode && countries[hashedIndex]->Deleted == true && firstDeleted){
                deletedIndex = hashedIndex;
                firstDeleted = false;
            }
            else if(countries[hashedIndex]->getCountryCode() == countryCode){
                if(countries[hashedIndex]->Deleted ==true){
                    if(firstDeleted){
                        wasDeleted = true;
                        break;
                    }
                }else{
                    exist = true;
                    break;
                }
            }
            hashedIndex = (hashedIndex + secondaryHash(countryCode)) % 512;
            i++;
        }

        if(exist){
            for (int i = 0; i < currentLineCount; i++) {
                lines[i] = ""; 
            }
            currentLineCount = 0;
        }
        else if(wasDeleted){
            for (int i = 0; i < currentLineCount; i++) {
                lines[i] = ""; 
            }
            currentLineCount = 0;
            countries[hashedIndex]->Deleted = false;
        }
        else{
            if(deletedIndex != -1){
                hashedIndex = deletedIndex;
            }
            countriesUsedIndex[countryIndex] = hashedIndex;
            //std::cout << countryCode << " " << hashedIndex << std::endl;
            countries[hashedIndex] = new SeriesList();
            countries[hashedIndex]->newLoad(lines, currentLineCount);
            countryIndex++;
        }
    }
    
    if (lines != nullptr) {
        delete[] lines;
        lines = nullptr;
    }
    std::cout << "success" << std::endl;
    myFile.close();

    // std::cout << countries[0].getCountryName();
    // std::cout << std::endl;
    // std::cout << countries[100].biggest();d
}

void Hashing::list(std::string countryName){
    //find the country in countries and call its list function
    for(int i = 0; i < 512; i++){
        //std::cout << countries[i].getCountryName() << ' ' << std::endl;
        if(countries[i] == nullptr){
            continue;
        }
        else if(countries[i]->Deleted == true){
            continue;
        }
        else if (countryName == countries[i]->getCountryName()){
            countries[i]->list();
        }
    }
    //std::cout << countryIndex << std::endl;
}

void Hashing::range(std::string seriesCode){
    // std::cout << countryIndex; 266
    // std::cout << seriesCode;
    double min = 0;
    double max = 0;
    bool firstFlag = true;
    //a flag to check if valid data exists for at least one country
    bool retFlag = false;
    for(int i = 0; i < 512; i++){
        //std::cout << countries[i].seriesMean(seriesCode) << std::endl;
        if(countries[i] == nullptr){
            continue;
        }
        else if(countries[i]->Deleted == true){
            continue;
        }
        else if(countries[i]->seriesMean(seriesCode) == -1){
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
void Hashing::build(std::string seriesCode) {
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
        //for safety reason, although I don't need it. 
        if(countries[countriesUsedIndex[i]] == nullptr){
            continue;
        }

        if(countries[countriesUsedIndex[i]]->Deleted == true){
            continue;
        }
        //the index of country in 'countries' array
        nodeCountryIndexes[nodeCountriesNumber] = countriesUsedIndex[i];
        //number of country in the root of the tree
        nodeCountriesNumber++;
    }
    
    root = buildTree(seriesCode, seriesMin, seriesMax, nodeCountryIndexes , nodeCountriesNumber);
    //std::cout << "success" << std::endl;
}


void Hashing::find(double mean, std::string operation, std::vector<std::string>& ret){
    //edge case: return failure if no binary tree exists
    if(root == nullptr){
        //std::cout << "failure" << std::endl;
        return;
    }
    //recursive helper functions
    if(operation == "less") {
        printLess(mean, root, ret);
    }
    else if(operation == "greater"){
        printGreat(mean, root, ret);
    }
    else if(operation == "equal"){
        printEqual(mean, root, ret);
    }
    //std::cout << std::endl;
}

void Hashing::deleteName(std::string name){
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
        root->codes[i] = root->codes[i+1];
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

void Hashing::limit(std::string condition){
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

void Hashing::lookup(std::string code){
    //if the array has no element
    if (countryIndex < 0)
    {
        std::cout << "failure" << std::endl;
        return; 
    }
    
    int index = primaryHash(code);
    int i = 1;

    //if there was 512 elements in the table and we are searching the element that we removed
    //there are no spot with nullptr, so we need to give it a maximum number of iteration
    while (i < 512)
    {   
        if(countries[index] == nullptr){
            std::cout << "failure" << std::endl; 
            return;
        }
        //std::cout << countries[index]->getCountryCode() << ' ' << code << std::endl;
        else if(countries[index]->getCountryCode() == code){
            if(countries[index]->Deleted == true){
            }
            else{
                std::cout << "index " << index << " searches " << i << std::endl;
                return;  
            }
        }
        index = (index + secondaryHash(code)) % 512;
        i++;
    }
    std::cout << "failure" << std::endl; 
}

void Hashing::insert(std::string code, std::string fileName){
    SeriesList* newCountry = new SeriesList();
    if(!newCountry->loadByCode(code, fileName)){
        delete newCountry;
        std::cout << "failure" << std::endl;
        return;
    }
    //successfully loaded (file name is correct and country exists)
    int hashedIndex = primaryHash(code);
    int deletedIndex = -1;
    bool wasDeleted = false;
    int i = 1;
    bool firstDeleted = true;

    //assume countries are always less than 512, there's always an empty spot
    while(true){
        if (countries[hashedIndex] == nullptr){
            break;
        }

        //problem: the first deleted index is the code (solved)
        else if(countries[hashedIndex]->getCountryCode() != code && countries[hashedIndex]->Deleted == true && firstDeleted){
            //get the first deleted index;
            deletedIndex = hashedIndex;
            firstDeleted = false;
        }
        //problem: if first Deleted index < this index: add firstDeleted to the condition (solved)
        //first deleted has a higher priority than was deleted
        else if(countries[hashedIndex]->getCountryCode() == code){
            if(countries[hashedIndex]->Deleted == true){
                if(firstDeleted){
                    wasDeleted = true;
                    break;
                }
            }
            else{
                //country exists
                std::cout << "failure" << std::endl;
                //prevent memory leak
                delete newCountry;
                return;
            }
        }
        hashedIndex = (hashedIndex + secondaryHash(code)) % 512;
        i++;
    }

    // std::cout << deletedIndex << std::endl; used to check if deleted
    if (wasDeleted){
        countries[hashedIndex]->Deleted = false;
        countriesUsedIndex[countryIndex] = hashedIndex;
        delete newCountry;

        std::cout << "success" << std::endl;
        return;
    }

    if(deletedIndex != -1){
        hashedIndex = deletedIndex;
        delete countries[hashedIndex];
    }

    countries[hashedIndex] = newCountry;
    countriesUsedIndex[countryIndex] = hashedIndex;
    countryIndex++;
    std::cout << "success" << std::endl;
}


void Hashing::remove(std::string code){
    int hashedIndex = primaryHash(code);

    int i = 1;

    while(i < 512){
        //if the ccountry doesn't exist
        if (countries[hashedIndex] == nullptr){
            std::cout << "failure" << std::endl;
            return;
        }else if(countries[hashedIndex]->getCountryCode() == code){
            //things need to fix in load after finish this part: 
            //check in the countries[currentIndex].deleted = true;
            if(countries[hashedIndex]->Deleted == true){
                //it's been deleted, continue searching
            }
            else{
                countries[hashedIndex]->Deleted = true;
                if(root != nullptr){
                    deleteName(countries[hashedIndex]->getCountryName());
                    return;
                    //return since deleteName can print success
                }
                std::cout << "success" << std::endl;
                return;
            }
        }
        hashedIndex = (hashedIndex + secondaryHash(code)) % 512;
        i++;
    }
    std::cout << "failure" << std::endl;
}

int Hashing::getHashCode(std::string code)
{
    if (countryIndex < 0)
    {
        //std::cout << "failure" << std::endl;
        return -1; 
    }
    
    int index = primaryHash(code);
    int i = 1;

    //if there was 512 elements in the table and we are searching the element that we removed
    //there are no spot with nullptr, so we need to give it a maximum number of iteration
    while (i < 512)
    {   
        if(countries[index] == nullptr){
            //std::cout << "failure" << std::endl; 
            return -1;
        }
        //std::cout << countries[index]->getCountryCode() << ' ' << code << std::endl;
        else if(countries[index]->getCountryCode() == code){
            if(countries[index]->Deleted == true){
            }
            else{
                //std::cout << "index " << index << " searches " << i << std::endl;
                return index;
            }
        }
        index = (index + secondaryHash(code)) % 512;
        i++;
    }
    //std::cout << "failure" << std::endl;
    return -1;
}

Hashing::~Hashing()
{
    for(int i = 0; i < 512; i++) {
        delete countries[i];
    }
    postorderDelete(root);
    root = nullptr;
}

//potential problems 
//problem: insert the removed element: (solved)
//problem: encounter DELETED index in load (2 places)
//problem: didn't store the first deleted spot in insert (fixed)
//problem: priority of wasDeleted and firstDeleted (fixed)
//problem: didn't update build