#ifndef SERIES_LIST_H
#define SERIES_LIST_H

#include "TimeSeries.h"
#include <string>

class SeriesList {
private: 
    std::string countryName;
    std::string countryCode;

    TimeSeries* head;
    TimeSeries* curr;

    std::string fileName = "lab2_multidata.csv";

public: 
    SeriesList();
    ~SeriesList();

    //later set it to getter and setter
    bool Deleted;

    bool load(std::string country);

    //project3
    void newLoad(std::string lines[], int lineCount);

    //project 4
    bool loadByCode(std::string code, std::string file);

    void list();
    bool add(std::string code, int year, double data);
    bool update(std::string code, int year, double data);
    bool print(std::string code);
    bool deleteSeries(std::string code);
    std::string biggest();

    double seriesMean(std::string code);

    void setCountryName(std::string value);
    std::string getCountryName() const;   
    void setCountryCode(std::string value);
    std::string getCountryCode() const;


    SeriesList(const SeriesList& other);
    SeriesList& operator=(const SeriesList& other);

};

#endif
