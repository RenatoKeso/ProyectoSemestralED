#ifndef AIR_RECORD_H
#define AIR_RECORD_H


#include <string>
using namespace std;

struct AirRecord{
    int uniqueID;
    int indicatorID;
    string name;
    string measure;
    string measureInfo;
    string geoTypeName;
    string geoJoinID;
    string geoPlaceName;
    string timePeriod;
    string startDate;
    double dataValue;
    string message;
};

#endif