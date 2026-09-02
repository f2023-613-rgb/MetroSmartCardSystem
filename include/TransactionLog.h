#ifndef TRANSACTION_LOG_H
#define TRANSACTION_LOG_H

#include "OperationCounter.h"

class TransactionLog
{
public:
    enum TransactionType
    {
        REGISTER_CARD,
        TAP_IN,
        TAP_OUT,
        TOP_UP,
        UNDO_TOP_UP,
        BLOCK_CARD,
        UNBLOCK_CARD
    };

private:
    struct TransactionNode
    {
        TransactionType type;
        long long cardNumber;
        double amount;
        int stationId;
        char time[30];
        TransactionNode* next;

        TransactionNode(TransactionType transactionType,
                        long long number,
                        double transactionAmount,
                        int station,
                        const char* transactionTime);
    };

    TransactionNode* head;
    TransactionNode* tail;
    int count;

    void clear();
    void copyFrom(const TransactionLog& other);

    const char* getTypeName(TransactionType type) const;

public:
    TransactionLog();

    TransactionLog(const TransactionLog& other);

    TransactionLog& operator=(const TransactionLog& other);

    ~TransactionLog();

    bool append(TransactionType type,
                long long cardNumber,
                double amount,
                int stationId,
                const char* time,
                OperationCounter& counter);

    void replay() const;

    bool isEmpty() const;

    int getCount() const;
};

#endif