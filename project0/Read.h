#ifndef READ_H_
#define READ_H_
#include <iostream>
#include <string>

//write the constructor, getter and setter for practice
class Read
{
    private:
        std::string file;
    public:
        Read(std::string file);

        void readFile();
        std::string getFile();
        void setFile(std::string newFile);
};

#endif