#ifndef CSV_READER_H
#define CSV_READER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "AirRecord.h"

using namespace std;

bool loadAirQualityCSV(const string& filename, vector<AirRecord>& records) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo CSV\n";
        return false;
    }

    string line;
    getline(file, line); // saltar header

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string field;
        AirRecord r;

        getline(ss, field, ',');
        r.uniqueID = stoi(field);

        getline(ss, field, ',');
        r.indicatorID = stoi(field);

        getline(ss, r.name, ',');
        getline(ss, r.measure, ',');
        getline(ss, r.measureInfo, ',');
        getline(ss, r.geoTypeName, ',');
        getline(ss, r.geoJoinID, ',');
        getline(ss, r.geoPlaceName, ',');
        getline(ss, r.timePeriod, ',');
        getline(ss, r.startDate, ',');

        getline(ss, field, ',');
        r.dataValue = field.empty() ? 0.0 : stod(field);

        getline(ss, r.message);

        records.push_back(r);
    }

    return true;
}

#endif