#include "SeriesList.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SeriesList::SeriesList() : countryName(""), countryCode(""), head(nullptr), Deleted(false) {
}

//Learned the implementation of destructing a linked list from the slide
SeriesList::~SeriesList() {
    TimeSeries* current = head;
    while(current != nullptr){
        TimeSeries* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
}

bool SeriesList::load(std::string country) {

    //clear all data whenever the function is called (when the command is load)
    if(head != nullptr){
        TimeSeries* current = head;
        while(current != nullptr){
            TimeSeries* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr; // reset head after clearing
        countryName = "";
        countryCode = "";
    }

    //initialize a flag signal to check whether there's a match with the input and a country name in the dataset
    bool flag = false;
    TimeSeries* current = nullptr;

    std::ifstream myFile(fileName);
    if(!myFile) return false;
    std::string line;

    while(std::getline(myFile, line)) {
        std::stringstream lineStream(line);
        std::string dataInString;

        std::string countryNameTemp;
        std::string countryCodeTemp;

        std::getline(lineStream, countryNameTemp , ',');
        std::getline(lineStream, countryCodeTemp , ',');

        if(countryNameTemp != country) continue;

        flag = true;
        countryName = countryNameTemp;
        countryCode = countryCodeTemp;

        //initialize an 'new node'
        TimeSeries* newSeries = new TimeSeries();
        newSeries->next = nullptr; 

        std::string seriesName;
        std::string seriesCode;
        std::getline(lineStream, seriesName, ',');
        std::getline(lineStream, seriesCode, ',');
        // std::cout << seriesName << ' ';
        newSeries->setSeriesCode(seriesCode);
        newSeries->setSeriesName(seriesName);

        int year = 1959;
        int index = -1;
        while(std::getline(lineStream, dataInString, ',')){
            year++;
            double dataInDouble = std::stod(dataInString);

            if(dataInDouble == -1) continue;

            //new version (O(n), much faster) Introduced a new loading function in TimeSeries class
            index++;
            newSeries->loadSeries(year, dataInDouble, index);
        }

        if(head == nullptr){
            head = newSeries;
            current = head;
        } 
        else {
            current->next = newSeries;
            current = newSeries;
        }
    }
    myFile.close();
    return flag;
}

void SeriesList::newLoad(std::string lines[], int lineCount){
    //make some slightly changes from the 'old' load function.
    TimeSeries* current = nullptr;
    for (int i = 0; i < lineCount; i++){
        std::stringstream lineStream(lines[i]);
    
        //dummy variables
        std::string countryNameTemp;
        std::string countryCodeTemp;

        if (i == 0)
        {
            std::getline(lineStream, countryName , ',');
            std::getline(lineStream, countryCode , ',');
        }else{
            std::getline(lineStream, countryNameTemp , ',');
            std::getline(lineStream, countryCodeTemp , ',');
        }

        //initialize an 'new node' (TimeSeries)
        TimeSeries* newSeries = new TimeSeries();
        newSeries->next = nullptr; 
        std::string dataInString;

        std::string seriesName;
        std::string seriesCode;
        std::getline(lineStream, seriesName, ',');
        std::getline(lineStream, seriesCode, ',');
        // std::cout << seriesName << ' ';
        newSeries->setSeriesCode(seriesCode);
        newSeries->setSeriesName(seriesName);

        int year = 1959;
        int index = -1;
        while(std::getline(lineStream, dataInString, ',')){
            year++;
            double dataInDouble = std::stod(dataInString);

            if(dataInDouble == -1)continue;
            index++;
            newSeries->loadSeries(year, dataInDouble, index);
        }

        if(head == nullptr){
            head = newSeries;
            current = head;
        } 
        else {
            current->next = newSeries;
            current = newSeries;
        }
    }
}

bool SeriesList::loadByCode(std::string code, std::string file)
{
    if(head != nullptr){
        TimeSeries* current = head;
        while(current != nullptr){
            TimeSeries* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr; // reset head after clearing
        countryName = "";
        countryCode = "";
    }

    //initialize a flag signal to check whether there's a match with the input and a country name in the dataset
    bool flag = false;
    TimeSeries* current = nullptr;

    std::ifstream myFile(file);
    if(!myFile) return false;
    std::string line;

    while(std::getline(myFile, line)) {
        std::stringstream lineStream(line);
        std::string dataInString;

        std::string countryNameTemp;
        std::string countryCodeTemp;

        std::getline(lineStream, countryNameTemp , ',');
        std::getline(lineStream, countryCodeTemp , ',');

        if(countryCodeTemp!= code) continue;

        flag = true;
        countryName = countryNameTemp;
        countryCode = countryCodeTemp;

        //initialize an 'new node'
        TimeSeries* newSeries = new TimeSeries();
        newSeries->next = nullptr; 

        std::string seriesName;
        std::string seriesCode;
        std::getline(lineStream, seriesName, ',');
        std::getline(lineStream, seriesCode, ',');
        // std::cout << seriesName << ' ';
        newSeries->setSeriesCode(seriesCode);
        newSeries->setSeriesName(seriesName);

        int year = 1959;
        int index = -1;
        while(std::getline(lineStream, dataInString, ',')){
            year++;
            double dataInDouble = std::stod(dataInString);

            if(dataInDouble == -1)continue;

            //new version (O(n), much faster) Introduced a new loading function in TimeSeries class
            index++;
            newSeries->loadSeries(year, dataInDouble, index);
        }

        if(head == nullptr){
            head = newSeries;
            current = head;
        } 
        else {
            current->next = newSeries;
            current = newSeries;
        }
    }
    myFile.close();
    return flag;
}

// print the seriesName of all the elements in the linked list
void SeriesList::list() {
    /*
    Modification after passing all tests 
    I cleared countryName and countryCode in the destructor, allowing me to know the list is empty if countryName and countryCode are empty strings.
    Without 'if(countryName == "") return;' If no data has been stored, it will print "  ".
    Possible Alternative: Check if head == nullptr
    */
    if(countryName == "") return;

    std::cout << countryName << ' ' << countryCode << ' ';

    TimeSeries* current = head;
    while(current != nullptr){
        std::cout << current->getSeriesName() << ' ';
        current = current->next;
    }
    std::cout << std::endl;
}

//traverse to the appropriate series and perform add
bool SeriesList::add(std::string code, int year, double data)
{
    if(head == nullptr) return false;
    TimeSeries* current = head;
    while(current != nullptr){
        if(current->getSeriesCode() == code){
            bool ret = current->add(year, data);
            return ret;
        }
        current = current->next;
    }
    return false;
}

//traverse to the appropriate series and perform update
bool SeriesList::update(std::string code, int year, double data)
{
    if(head == nullptr) return false;
    TimeSeries* current = head;
    while (current != nullptr){
        if(current->getSeriesCode() == code){
            bool ret = current->update(year, data);
            return ret;
        }
        current = current->next;
    }
    return false;   
}

//traverse to the appropriate series and print the series
bool SeriesList::print(std::string code){
    if(head == nullptr) return false;
    TimeSeries* current = head;
    while(current != nullptr){
        if(current->getSeriesCode() == code){
            current->print();
            return true;
        }
        current = current->next;
    }
    return false;
}

//basic algorithm of deleting a node in a linked list
bool SeriesList::deleteSeries(std::string code)
{
    if(head == nullptr) return false;
    TimeSeries* current = head;
    TimeSeries* prev = nullptr;
    while(current != nullptr){
        if(current->getSeriesCode() == code){
            if(current == head){
                head = current->next;
                delete current;
                return true;
            }
            prev->next = current->next;
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

//Notice from the manual: In the case of a tie, print the one that is first in the dataset (store the name of the first series with greatest value)
std::string SeriesList::biggest()
{
    std::string retCode;
    double max = 0.0;
    if(head == nullptr) return "failure";
    TimeSeries* current = head;
    while(current != nullptr){
        double mean = current->mean();
        if(mean > max){
            retCode = current->getSeriesCode();
            max = mean;
        }
        current = current->next;
    }
    if(max == 0){
        return "failure"; //might cause a problem if the mean is 0
    }
    return retCode;
}

double SeriesList::seriesMean(std::string code)
{
    if(head == nullptr) return -1;
    TimeSeries* current = head;
    while(current != nullptr){
        //std::cout << current->getSeriesCode() << std::endl;
        if(current->getSeriesCode() == code){
             return current->mean();
        }
        current = current->next;
    }
    return -1;
}

//getters and setters
void SeriesList::setCountryName(std::string value){
    countryName = value;
}

std::string SeriesList::getCountryName() const
{
    return countryName;
}

void SeriesList::setCountryCode(std::string value){
    countryCode = value;
}

std::string SeriesList::getCountryCode() const
{
    return countryCode;
}
