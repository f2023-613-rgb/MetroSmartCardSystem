#ifndef STATION_H
#define STATION_H

class Station
{
private:
    int stationId;
    char stationCode[10];
    char stationName[50];

public:
    Station();

    int getStationId() const;
    const char* getStationCode() const;
    const char* getStationName() const;

    void setStationId(int id);
    void setStationCode(const char* code);
    void setStationName(const char* name);
};

#endif