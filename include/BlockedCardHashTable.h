#ifndef BLOCKED_CARD_HASH_TABLE_H
#define BLOCKED_CARD_HASH_TABLE_H

#include "OperationCounter.h"

class BlockedCardHashTable
{
private:
    struct BlockedNode
    {
        long long cardNumber;
        BlockedNode* next;

        BlockedNode(long long number);
    };

    BlockedNode** buckets;
    int capacity;
    int count;

    int hash(long long cardNumber) const;

    void clear();
    void copyFrom(const BlockedCardHashTable& other);
    void rehash(int newCapacity);

public:
    BlockedCardHashTable(int initialCapacity = 2003);

    BlockedCardHashTable(
        const BlockedCardHashTable& other);

    BlockedCardHashTable& operator=(
        const BlockedCardHashTable& other);

    ~BlockedCardHashTable();

    bool blockCard(long long cardNumber,
                   OperationCounter& counter);

    bool unblockCard(long long cardNumber,
                     OperationCounter& counter);

    bool isBlocked(long long cardNumber,
                   OperationCounter& counter) const;

    int getCount() const;

    int getCapacity() const;

    double getLoadFactor() const;
};

#endif