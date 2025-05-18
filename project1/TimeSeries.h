#ifndef TIME_SERIES_H
#define TIME_SERIES_H

#include <string>

class TimeSeries {
private:
    std::string seriesName;
    std::string seriesCode;

    int* years;
    double* data;
    
    //'current' represents the current number of data that store inside the array (index+1)
    int current;
    int capacity;

    int checkYear(int year);
    
    // void doubleSize();
    // void halveSize();
    void resize(bool increase);
    
public:
    TimeSeries();
    ~TimeSeries();
    bool loadFromFile(std::string fileName);
    void print();
    bool add(int year, double value);
    bool update(int year, double value);
    double mean();
    bool isMonotonic();
    void bestFit(double &m, double &b);

    void setCurrent(int value);
    int getCurrent() const;
};
#endif