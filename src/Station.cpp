#include "../include/Station.h"
#include <cstring>

Station::Station()
{
    stationId = -1;

    std::strcpy(stationCode, "");
    std::strcpy(stationName, "");
}

int Station::getStationId() const
{
    return stationId;
}

const char* Station::getStationCode() const
{
    return stationCode;
}

const char* Station::getStationName() const
{
    return stationName;
}

void Station::setStationId(int id)
{
    if (id >= 0 && id < 30)
    {
        stationId = id;
    }
}

void Station::setStationCode(const char* code)
{
    if (code == nullptr)
    {
        stationCode[0] = '\0';
        return;
    }

    std::strncpy(stationCode, code, sizeof(stationCode) - 1);
    stationCode[sizeof(stationCode) - 1] = '\0';
}

void Station::setStationName(const char* name)
{
    if (name == nullptr)
    {
        stationName[0] = '\0';
        return;
    }

    std::strncpy(stationName, name, sizeof(stationName) - 1);
    stationName[sizeof(stationName) - 1] = '\0';
}