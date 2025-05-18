#include "SeriesList.h"
#include <iostream>
#include <string>

int main(){
    SeriesList sl;
    std::string command;

    while(std::cin >> command){
        if(command == "LOAD"){
            /*
            CITATION
            problem encountered: 'cin' can only read the strings before a space. It cannot read a country with space inside it e.g. American Samoa
            I learned from stackoverflow and solve the problem
            Reference Link: https://stackoverflow.com/questions/40984752/c-input-string-with-spaces
            */
            std::cin.ignore();
            std::string country;
            std::getline(std::cin, country);

            if(sl.load(country)){
                std::cout << "success" << std::endl;
            } else{
                std::cout << "failure" << std::endl;
            }
        }
        else if(command == "LIST"){
            sl.list();
        }
        else if(command == "ADD"){
            std::string code; 
            int year; 
            double data; 
            std::cin >> code >> year >> data;
            if(!(sl.add(code, year, data))){
                std::cout << "failure" << std::endl;
            }else {
                std::cout << "success" << std::endl;
            }
        }
        else if(command == "UPDATE") {
            std::string code; 
            int year; 
            double data; 
            std::cin >> code >> year >> data;

            if(!(sl.update(code, year, data))){
                std::cout << "failure" << std::endl;
            }else {
                std::cout << "success" << std::endl;
            }
        }
        else if(command == "PRINT"){
            std::string code; 
            std::cin >> code;
            if(!(sl.print(code))){
                std::cout << "failure" << std::endl;
            }
        }
        else if(command == "DELETE"){
            std::string code;
            std::cin >> code;
            if(!(sl.deleteSeries(code))){
                std::cout << "failure" << std::endl;
            }else{
                std::cout << "success" << std::endl;
            }
        }
        else if(command == "BIGGEST"){
            std::cout << sl.biggest() << std::endl;
        }
        else if(command == "EXIT"){
            break;
        }
    }
}