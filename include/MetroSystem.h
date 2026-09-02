#ifndef METRO_SYSTEM_H
#define METRO_SYSTEM_H

#include "CardHashTable.h"
#include "BlockedCardHashTable.h"
#include "GateQueue.h"
#include "TopUpStack.h"
#include "TransactionLog.h"
#include "Station.h"
#include "OperationCounter.h"

class MetroSystem
{
private:
    CardHashTable cards;
    BlockedCardHashTable blockedCards;

    GateQueue gateQueue;
    TopUpStack undoStack;

    TransactionLog transactionLog;

    Station stations[30];

    static const double MINIMUM_BALANCE;
    static const double FARE_PER_STATION;

    double calculateFare(int entryStation,
                         int exitStation) const;

public:
    MetroSystem();

    bool registerCard(long long cardNumber,
                      const char* holderName,
                      const char* cnic,
                      double balance,
                      OperationCounter& counter);

    Card* findCard(long long cardNumber,
                   OperationCounter& counter);

    bool topUpCard(long long cardNumber,
                   double amount,
                   const char* time,
                   OperationCounter& counter);

    bool undoLastTopUp(OperationCounter& counter);

    bool blockCard(long long cardNumber,
                   const char* time,
                   OperationCounter& counter);

    bool unblockCard(long long cardNumber,
                     const char* time,
                     OperationCounter& counter);

    bool addPassengerToGate(long long cardNumber,
                            OperationCounter& counter);

    bool serveNextPassenger(long long& cardNumber,
                            OperationCounter& counter);

    int getRegisteredCardCount() const;

    int getBlockedCardCount() const;

    int getGateQueueCount() const;

    void replayTransactions() const;

    bool tapIn(long long cardNumber,
           int stationId,
           const char* time,
           OperationCounter& counter);

    bool tapOut(long long cardNumber,
            int stationId,
            const char* time,
            double& chargedFare,
            OperationCounter& counter);

    bool getCurrentJourney(
            long long cardNumber,
            Journey& journey,
            OperationCounter& counter);

    bool moveJourneyPrevious(
    long long cardNumber,
    OperationCounter& counter);

bool moveJourneyNext(
    long long cardNumber,
    OperationCounter& counter);

bool insertJourneyAfterCurrent(
    long long cardNumber,
    const Journey& journey,
    OperationCounter& counter);

bool deleteCurrentJourney(
    long long cardNumber,
    OperationCounter& counter);

bool moveJourneyToFirst(
    long long cardNumber,
    OperationCounter& counter);

bool moveJourneyToLast(
    long long cardNumber,
    OperationCounter& counter);

bool loadCards(const char* filename);

bool loadJourneys(const char* filename);

bool saveCards(const char* filename) const;

bool saveJourneys(const char* filename) const;
};

#endif