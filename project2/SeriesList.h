#ifndef SERIES_LIST_H
#define SERIES_LIST_H

#include "TimeSeries.h"
#include <string>

class SeriesList {
private: 
    std::string countryName;
    std::string countryCode;

    TimeSeries* head;

    std::string fileName = "lab2_multidata.csv";

public: 
    SeriesList();
    ~SeriesList();

    bool load(std::string country);
    void list();
    bool add(std::string code, int year, double data);
    bool update(std::string code, int year, double data);
    bool print(std::string code);
    bool deleteSeries(std::string code);
    std::string biggest();

    void setCountryName(std::string value);
    std::string getCountryName() const;   
    void setCountryCode(std::string value);
    std::string getCountryCode() const;
};

#endif
