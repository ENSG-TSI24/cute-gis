#ifndef DataProvider_H
#define DataProvider_H

#include <string>
#include <iostream>
#include <fstream>

class DataProvider
{
private:
    const char* url;
    
public:

    virtual GDALDataset* getDataset() = 0;
    virtual void displayMetadata() = 0;
    virtual bool isEmpty();


};

#endif // DataProvider_H