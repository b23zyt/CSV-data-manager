#ifndef TIME_SERIES_H
#define TIME_SERIES_H

#include <string>

//Assume TimeSeries is a node in the Series List 
class TimeSeries {
private:

//change to getter and setter after finish the project
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

    void loadSeries(int year, double data, int index);
    
    void print();
    bool add(int year, double value);
    bool update(int year, double value);
    double mean();
    bool isMonotonic();
    void bestFit(double &m, double &b);

    void setCurrent(int value);
    int getCurrent() const;

    void setSeriesName(std::string value);
    std::string getSeriesName() const;

    void setSeriesCode(std::string value);
    std::string getSeriesCode() const;

    //stores the address of the next series (linker)
    TimeSeries* next;

    //https://documents.uow.edu.au/~lukes/textbook/notes-cpp/oop-condestructors/shallowdeepcopy.html

    TimeSeries( TimeSeries& other);
    TimeSeries& operator=(TimeSeries& other);
};
#endif