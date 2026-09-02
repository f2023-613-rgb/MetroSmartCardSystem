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
};

#endif