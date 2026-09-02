#ifndef CARD_H
#define CARD_H

#include "JourneyHistory.h"

class Card
{
private:
    long long cardNumber;

    char holderName[100];
    char cnic[20];

    double balance;

    bool blocked;
    bool active;

    // Current open journey
    bool openJourney;
    int entryStation;
    char entryTime[30];

    // Advanced B: last 20 journeys
    JourneyHistory journeyHistory;

public:
    Card();

    // Get functions
    long long getCardNumber() const;

    const char* getHolderName() const;
    const char* getCNIC() const;

    double getBalance() const;

    bool isBlocked() const;
    bool isActive() const;

    bool hasOpenJourney() const;

    int getEntryStation() const;
    const char* getEntryTime() const;

    JourneyHistory& getJourneyHistory();
    const JourneyHistory& getJourneyHistory() const;

    // Set functions
    void setCardNumber(long long number);

    void setHolderName(const char* name);
    void setCNIC(const char* cnicValue);

    void setBalance(double amount);

    void setBlocked(bool value);
    void setActive(bool value);

    // Balance operations
    bool addBalance(double amount);
    bool deductBalance(double amount);

    // Journey operations
    bool openNewJourney(int stationId,
                        const char* time);

    void closeOpenJourney();
};

#endif