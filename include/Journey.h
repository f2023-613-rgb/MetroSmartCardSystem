#ifndef JOURNEY_H
#define JOURNEY_H

class Journey
{
private:
    int entryStation;
    int exitStation;

    char entryTime[30];
    char exitTime[30];

    double fare;

public:
    Journey();

    int getEntryStation() const;
    int getExitStation() const;

    const char* getEntryTime() const;
    const char* getExitTime() const;

    double getFare() const;

    void setEntryStation(int station);
    void setExitStation(int station);

    void setEntryTime(const char* time);
    void setExitTime(const char* time);

    void setFare(double amount);
};

#endif