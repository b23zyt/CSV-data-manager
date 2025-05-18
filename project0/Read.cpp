#include "Read.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Read::Read(std::string file):file{file}{}

void Read::readFile(){
    std::ifstream myFile(file);
    std::string line;

    while(std::getline(myFile, line)){
        std::stringstream s_stream(line);
        std::string text;
        while(std::getline(s_stream, text, ',')){
            std::cout << text << std::endl;
        }
    }
    myFile.close();
}

std::string Read::getFile(){
    return file;
}

void Read::setFile(std::string newFile){
    file = newFile;
}
