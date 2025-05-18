#include "TimeSeries.h"
#include <iostream>
#include <string>

int main(){
    TimeSeries ts;
    std::string command;
    
    while(std::cin >> command){
        if(command == "LOAD"){
            std::string filename;
            std::cin >> filename;
            if(ts.loadFromFile(filename)){
                std::cout << "success" << std::endl;
            } else{
                std::cout << "failure" << std::endl;
            }
        }
        else if(command == "PRINT"){
            ts.print();
        }
        else if(command == "ADD"){
            int year;
            double value;
            std::cin >> year >> value;
            if(ts.add(year, value)){
                std::cout << "success" << std::endl;
            } else{
                std::cout << "failure" << std::endl;
            }
        }
        else if (command == "UPDATE"){
            int year;
            double value;
            std::cin >> year >> value;
            if(ts.getCurrent() ==0){
                std::cout << "failure" << std::endl;
            }
            else if(ts.update(year, value)){
                std::cout << "success" << std::endl;
            } else{
                std::cout << "failure" << std::endl;
            }
        }
        else if(command == "MEAN"){
            if(ts.mean() == 0){
                std::cout << "failure" << std::endl;
            } else{
                std::cout << "mean is " << ts.mean() << std::endl;
            }
        }
        else if(command == "MONOTONIC"){
            if(ts.isMonotonic() == true){
                std::cout << "series is monotonic" << std::endl;
            } else {
                if(ts.getCurrent() == 0){
                    std::cout << "failure" << std::endl;
                } else {
                    std::cout << "series is not monotonic" << std::endl;
                }
            }
        }
        else if(command == "FIT"){
            if(ts.getCurrent() == 0){
                std::cout << "failure" << std::endl;
            } else {
                double m;
                double b;
                ts.bestFit(m, b);
                std::cout << "slope is " << m << " intercept is " << b << std::endl;
            }
        }
        else if(command == "EXIT"){
            break;
        }
    }

    return 0;
}