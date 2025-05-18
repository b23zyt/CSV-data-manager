#include "TimeSeries.h"
#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

TimeSeries::TimeSeries() : seriesName(""), seriesCode(""), years(new int[2]), data(new double[2]), current(0), capacity(2){

}

TimeSeries::~TimeSeries() {
    delete[] years;
    delete[] data;
}

//helper function: Check year existence: used in ADD and UPDATE
int TimeSeries::checkYear(int year){
    for (int i = 0; i < current; i++){
        if (years[i] == year){
            return i;
        }
    }
    return -1;
}

//helper function: Combined double and halve into one function. Used to resize the array
void TimeSeries::resize(bool increase){
    int newCapacity;
    if(increase){
        newCapacity = capacity*2;
    }else{
        newCapacity = capacity/2;
    }
    if(newCapacity < 2){
        newCapacity = 2;
    }
    int* newYears = new int[newCapacity];
    double* newData = new double[newCapacity];

    for (int i = 0; i < current; i++) {
        newYears[i] = years[i];
        newData[i] = data[i];
    }

    delete[] years;
    delete[] data;

    years = newYears;
    data = newData;
    capacity = newCapacity;
}


//passed
bool TimeSeries::loadFromFile(std::string fileName)
{
    // delete all previous data(copy from the constructor)
    if(current > 0) {
        int* newYears = new int[2];
        double* newData = new double[2];
        delete[] years;
        delete[] data;

        years = newYears;
        data = newData;
        
        current = 0;
        capacity = 2;
        seriesName = "";
        seriesCode = "";
    }

    //project 0
    /*
    CITATION:
    Learned about how to load data from csv files from the following forum.
    I learned I can use ifstream function 
    https://cplusplus.com/forum/beginner/157129/
    */
    std::ifstream myFile(fileName);
    if(!myFile) return false;
    std::string line;

    // if(!std::getline(myFile, line)) return false; //changed if statement to while to handle multiple lines of data in further projects.
    while(std::getline(myFile, line)) {
        std::stringstream lineStream(line);
        std::string dataInString;

        //first and second columns country and country code (discard them now)
        std::getline(lineStream, dataInString , ',');
        std::getline(lineStream, dataInString , ',');

        //third and fourth columns
        std::getline(lineStream, seriesName, ',');
        std::getline(lineStream, seriesCode, ',');

        int year = 1959;
        while(std::getline(lineStream, dataInString, ',')){
            year++;
            double dataInDouble = std::stod(dataInString);
            //std::cout << dataInDouble << std::endl;

            //discard the missing data
            if(dataInDouble == -1)continue;

            //increase the capacity when it is full
            if(capacity <= current) resize(true);

            data[current] = dataInDouble;
            years[current] = year;
            current++;
        }
    }

    //test
    // for(int i = 0; i < current; i++){
    //     std::cout << years[i] << std::endl;
    // }
    // for(int i = 0; i < current; i++){
    //     std::cout << data[i] << std::endl;
    // }
    myFile.close();
    return true;
}

//passed
void TimeSeries::print(){
    if(current == 0){
        std::cout << "failure" << std::endl;
        return;
    }
    for (int i = 0; i < current; i++){
        std::cout << '(' << years[i] << ',' << data[i] << ") ";
    }
    std::cout << std::endl;

    //test
    // std::cout << current << std::endl;
    // std::cout << capacity << std::endl;

    return;
}

//passed
bool TimeSeries::add(int year, double value){
    if (checkYear(year) != -1) return false;
    if (value == -1) return true; //I don't need to store -1, but should return false
    if(current >= capacity) resize(true); //increase the capacity if it is full

    int position = 0;
    while((position < current) && years[position] < year){
        position++;
    }

    for(int i = current; i > position; i--){
        years[i] = years[i-1];
        data[i] = data[i-1];
    }

    years[position] = year;
    data[position] = value;
    current++;

    return true;
}

bool TimeSeries::update(int year, double value){
    int yearInyears = checkYear(year);
    if(yearInyears == -1){
        return false;
    }

    //check already exists (possibly reduce running time) not needed!
    // if(years[yearInyears] == year && data[yearInyears] == value){
    //     //std::cout << "Hello World" << std::endl;
    //     return true;
    // }

    //handle deletion v1
    //(unsolved issue: some data are assigned to xxxxe-311, very weird)

    // if(value == -1){
    //     int* newYears = new int[capacity]; //can also use current here, but it will break the pattern of capacity
    //     double* newData = new double[capacity];
    //     int j = 0;
    //     for(int i = 0; i < current; i++){
    //         //std::cout << i << ' ' << j << std::endl;
    //         if(i == yearInyears){
    //             continue;
    //         }
    //         newYears[j] = years[i];
    //         newData[j] = data[i];
    //         j++;
    //     }
    //     delete[] years;
    //     delete[] data;
    //     years = newYears;
    //     data = newData;
    //     current--;
    //     if(capacity/4 <= current){
    //         resize(false);
    //     }
    //     return true;
    // }

//v2 (potential problem: cannot delete the last data in the array) 
    if(value == -1){
        for(int i = yearInyears; i < current-1; i++){
            years[i] = years[i+1];
            data[i] = data[i+1];
        }
        current--;
        //leave the last data inside the array and it doesn't matter. it will be replaced by the next update
        return true;
    }
    
    //when year exists && value ~= -1, then update the new value
    data[yearInyears] = value;
    return true;
}

//passed
double TimeSeries::mean(){
    if(current <= 0) return 0;
    //previously declare tot as int, caused the test to fail
    double tot = 0.0;
    for(int i = 0; i < current; i++){
        tot += data[i];
        //test (error: forgot to declare tot as double...)
        // std::cout << data[i] << std::endl;
        // std::cout << tot << std::endl;
    }
    //test
    // std::cout << current << std::endl;

    double ret = tot/current;
    return ret;
}

//passed
bool TimeSeries::isMonotonic()
{
    if(current <= 0) return false;

    bool increase = true;
    bool decrease = true;

    for(int i = 0; i < current -1; i++){
        if(data[i+1] > data[i]) decrease = false;
        if(data[i+1] < data[i]) increase = false;
    }

    bool ret = increase || decrease;
    return ret;
}

void TimeSeries::bestFit(double &m, double &b){
    if(current <= 0) {
        m = 0;
        b = 0;
        return;
    }
    long double sumX = 0;
    long double sumY = 0;
    long double sumXY = 0;
    long double sumXX = 0;
    for(int i = 0; i < current; i++){
        sumX += years[i];
        sumY += data[i];
        sumXX += years[i]*years[i];
        sumXY += years[i]*data[i];
    }

    m = (current*sumXY - sumX*sumY)/(current*sumXX - sumX*sumX);
    b = (sumY - m*sumX)/current;
}

void TimeSeries::setCurrent(int value){
    current = value;
}

//enable main to get the data size
int TimeSeries::getCurrent() const {
    return current;
}