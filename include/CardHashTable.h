#ifndef CARD_HASH_TABLE_H
#define CARD_HASH_TABLE_H

#include "Card.h"
#include "OperationCounter.h"

class CardHashTable
{
private:
    struct HashNode
    {
        Card data;
        HashNode* next;

        HashNode(const Card& card);
    };

    HashNode** buckets;
    int capacity;
    int count;

    int hash(long long cardNumber) const;

    void clear();
    void copyFrom(const CardHashTable& other);
    void rehash(int newCapacity);

public:
    CardHashTable(int initialCapacity = 10007);

    CardHashTable(const CardHashTable& other);

    CardHashTable& operator=(const CardHashTable& other);

    ~CardHashTable();

    bool insert(const Card& card,
                OperationCounter& counter);

    Card* search(long long cardNumber,
                 OperationCounter& counter);

    const Card* search(long long cardNumber,
                       OperationCounter& counter) const;

    bool remove(long long cardNumber,
                OperationCounter& counter);

    bool contains(long long cardNumber,
                  OperationCounter& counter) const;

    int getCount() const;

    int getCapacity() const;

    double getLoadFactor() const;
};

#endif