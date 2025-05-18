#include "Read.h"
#include <iostream>


int main(){
    std::string file = "../project0_test_data.csv";
    Read read(file);
    read.readFile();

    return 0;
}