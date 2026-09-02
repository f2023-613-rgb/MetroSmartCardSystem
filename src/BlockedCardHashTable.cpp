#include "../include/BlockedCardHashTable.h"

BlockedCardHashTable::BlockedNode::BlockedNode(
    long long number)
{
    cardNumber = number;
    next = nullptr;
}

BlockedCardHashTable::BlockedCardHashTable(
    int initialCapacity)
{
    if (initialCapacity <= 0)
    {
        initialCapacity = 2003;
    }

    capacity = initialCapacity;
    count = 0;

    buckets = new BlockedNode*[capacity];

    for (int i = 0; i < capacity; i++)
    {
        buckets[i] = nullptr;
    }
}

BlockedCardHashTable::BlockedCardHashTable(
    const BlockedCardHashTable& other)
{
    capacity = other.capacity;
    count = 0;

    buckets = new BlockedNode*[capacity];

    for (int i = 0; i < capacity; i++)
    {
        buckets[i] = nullptr;
    }

    copyFrom(other);
}

BlockedCardHashTable&
BlockedCardHashTable::operator=(
    const BlockedCardHashTable& other)
{
    if (this != &other)
    {
        clear();

        delete[] buckets;

        capacity = other.capacity;
        count = 0;

        buckets = new BlockedNode*[capacity];

        for (int i = 0; i < capacity; i++)
        {
            buckets[i] = nullptr;
        }

        copyFrom(other);
    }

    return *this;
}

BlockedCardHashTable::~BlockedCardHashTable()
{
    clear();

    delete[] buckets;
    buckets = nullptr;
}

int BlockedCardHashTable::hash(
    long long cardNumber) const
{
    unsigned long long value =
        static_cast<unsigned long long>(cardNumber);

    unsigned long long mixed =
        value ^ (value >> 23);

    mixed *= 0x2127599bf4325c37ULL;

    mixed ^= (mixed >> 47);

    return static_cast<int>(
        mixed %
        static_cast<unsigned long long>(capacity));
}

bool BlockedCardHashTable::blockCard(
    long long cardNumber,
    OperationCounter& counter)
{
    double projectedLoadFactor =
        static_cast<double>(count + 1)
        / static_cast<double>(capacity);

    if (projectedLoadFactor > 0.75)
    {
        rehash(capacity * 2 + 1);
    }

    int index = hash(cardNumber);

    counter.incrementSteps();

    BlockedNode* current =
        buckets[index];

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->cardNumber == cardNumber)
        {
            return false;
        }

        current = current->next;
    }

    BlockedNode* newNode =
        new BlockedNode(cardNumber);

    newNode->next =
        buckets[index];

    buckets[index] =
        newNode;

    count++;

    counter.addSteps(3);

    return true;
}

bool BlockedCardHashTable::unblockCard(
    long long cardNumber,
    OperationCounter& counter)
{
    int index = hash(cardNumber);

    counter.incrementSteps();

    BlockedNode* current =
        buckets[index];

    BlockedNode* previous =
        nullptr;

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->cardNumber == cardNumber)
        {
            if (previous == nullptr)
            {
                buckets[index] =
                    current->next;
            }
            else
            {
                previous->next =
                    current->next;
            }

            delete current;

            count--;

            counter.addSteps(3);

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

bool BlockedCardHashTable::isBlocked(
    long long cardNumber,
    OperationCounter& counter) const
{
    int index = hash(cardNumber);

    counter.incrementSteps();

    BlockedNode* current =
        buckets[index];

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->cardNumber == cardNumber)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

int BlockedCardHashTable::getCount() const
{
    return count;
}

int BlockedCardHashTable::getCapacity() const
{
    return capacity;
}

double BlockedCardHashTable::getLoadFactor() const
{
    if (capacity == 0)
    {
        return 0.0;
    }

    return static_cast<double>(count)
           / static_cast<double>(capacity);
}

void BlockedCardHashTable::rehash(
    int newCapacity)
{
    BlockedNode** oldBuckets =
        buckets;

    int oldCapacity =
        capacity;

    buckets =
        new BlockedNode*[newCapacity];

    for (int i = 0; i < newCapacity; i++)
    {
        buckets[i] = nullptr;
    }

    capacity = newCapacity;

    for (int i = 0;
         i < oldCapacity;
         i++)
    {
        BlockedNode* current =
            oldBuckets[i];

        while (current != nullptr)
        {
            BlockedNode* nextNode =
                current->next;

            int newIndex =
                hash(current->cardNumber);

            current->next =
                buckets[newIndex];

            buckets[newIndex] =
                current;

            current =
                nextNode;
        }
    }

    delete[] oldBuckets;
}

void BlockedCardHashTable::clear()
{
    for (int i = 0;
         i < capacity;
         i++)
    {
        BlockedNode* current =
            buckets[i];

        while (current != nullptr)
        {
            BlockedNode* nextNode =
                current->next;

            delete current;

            current = nextNode;
        }

        buckets[i] = nullptr;
    }

    count = 0;
}

void BlockedCardHashTable::copyFrom(
    const BlockedCardHashTable& other)
{
    for (int i = 0;
         i < other.capacity;
         i++)
    {
        BlockedNode* source =
            other.buckets[i];

        BlockedNode* copiedTail =
            nullptr;

        while (source != nullptr)
        {
            BlockedNode* newNode =
                new BlockedNode(
                    source->cardNumber);

            if (buckets[i] == nullptr)
            {
                buckets[i] =
                    newNode;

                copiedTail =
                    newNode;
            }
            else
            {
                copiedTail->next =
                    newNode;

                copiedTail =
                    newNode;
            }

            count++;

            source =
                source->next;
        }
    }
}