#include "../include/Journey.h"
#include <cstring>

Journey::Journey()
{
    entryStation = -1;
    exitStation = -1;

    std::strcpy(entryTime, "");
    std::strcpy(exitTime, "");

    fare = 0.0;
}

int Journey::getEntryStation() const
{
    return entryStation;
}

int Journey::getExitStation() const
{
    return exitStation;
}

const char* Journey::getEntryTime() const
{
    return entryTime;
}

const char* Journey::getExitTime() const
{
    return exitTime;
}

double Journey::getFare() const
{
    return fare;
}

void Journey::setEntryStation(int station)
{
    if (station >= 0)
    {
        entryStation = station;
    }
}

void Journey::setExitStation(int station)
{
    if (station >= 0)
    {
        exitStation = station;
    }
}

void Journey::setEntryTime(const char* time)
{
    if (time == nullptr)
    {
        entryTime[0] = '\0';
        return;
    }

    std::strncpy(entryTime, time, sizeof(entryTime) - 1);
    entryTime[sizeof(entryTime) - 1] = '\0';
}

void Journey::setExitTime(const char* time)
{
    if (time == nullptr)
    {
        exitTime[0] = '\0';
        return;
    }

    std::strncpy(exitTime, time, sizeof(exitTime) - 1);
    exitTime[sizeof(exitTime) - 1] = '\0';
}

void Journey::setFare(double amount)
{
    if (amount >= 0.0)
    {
        fare = amount;
    }
}