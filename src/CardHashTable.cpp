#include "../include/CardHashTable.h"
#include <fstream>
#include <iomanip>

CardHashTable::HashNode::HashNode(const Card& card)
{
    data = card;
    next = nullptr;
}

CardHashTable::CardHashTable(int initialCapacity)
{
    if (initialCapacity <= 0)
    {
        initialCapacity = 10007;
    }

    capacity = initialCapacity;
    count = 0;

    buckets = new HashNode*[capacity];

    for (int i = 0; i < capacity; i++)
    {
        buckets[i] = nullptr;
    }
}

CardHashTable::CardHashTable(
    const CardHashTable& other)
{
    capacity = other.capacity;
    count = 0;

    buckets = new HashNode*[capacity];

    for (int i = 0; i < capacity; i++)
    {
        buckets[i] = nullptr;
    }

    copyFrom(other);
}

CardHashTable& CardHashTable::operator=(
    const CardHashTable& other)
{
    if (this != &other)
    {
        clear();

        delete[] buckets;

        capacity = other.capacity;
        count = 0;

        buckets = new HashNode*[capacity];

        for (int i = 0; i < capacity; i++)
        {
            buckets[i] = nullptr;
        }

        copyFrom(other);
    }

    return *this;
}

CardHashTable::~CardHashTable()
{
    clear();

    delete[] buckets;
    buckets = nullptr;
}

int CardHashTable::hash(
    long long cardNumber) const
{
    unsigned long long value =
        static_cast<unsigned long long>(cardNumber);

    unsigned long long mixed =
        value ^ (value >> 23);

    mixed *= 0x2127599bf4325c37ULL;

    mixed ^= (mixed >> 47);

    return static_cast<int>(
        mixed % static_cast<unsigned long long>(capacity));
}

bool CardHashTable::insert(
    const Card& card,
    OperationCounter& counter)
{
    double projectedLoadFactor =
    static_cast<double>(count + 1)
    / static_cast<double>(capacity);

if (projectedLoadFactor > 0.75)
{
    rehash(capacity * 2 + 1);
}

    long long cardNumber =
        card.getCardNumber();

    int index =
        hash(cardNumber);

    counter.addSteps(2);

    HashNode* current =
        buckets[index];

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->data.getCardNumber()
            == cardNumber)
        {
            return false;
        }

        current = current->next;
    }

    HashNode* newNode =
        new HashNode(card);

    newNode->next =
        buckets[index];

    buckets[index] =
        newNode;

    count++;

    counter.addSteps(3);

    return true;
}

Card* CardHashTable::search(
    long long cardNumber,
    OperationCounter& counter)
{
    int index =
        hash(cardNumber);

    counter.incrementSteps();

    HashNode* current =
        buckets[index];

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->data.getCardNumber()
            == cardNumber)
        {
            return &(current->data);
        }

        current =
            current->next;
    }

    return nullptr;
}

const Card* CardHashTable::search(
    long long cardNumber,
    OperationCounter& counter) const
{
    int index =
        hash(cardNumber);

    counter.incrementSteps();

    HashNode* current =
        buckets[index];

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->data.getCardNumber()
            == cardNumber)
        {
            return &(current->data);
        }

        current =
            current->next;
    }

    return nullptr;
}

bool CardHashTable::remove(
    long long cardNumber,
    OperationCounter& counter)
{
    int index =
        hash(cardNumber);

    counter.incrementSteps();

    HashNode* current =
        buckets[index];

    HashNode* previous =
        nullptr;

    while (current != nullptr)
    {
        counter.incrementComparisons();
        counter.incrementSteps();

        if (current->data.getCardNumber()
            == cardNumber)
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

        previous =
            current;

        current =
            current->next;
    }

    return false;
}

bool CardHashTable::contains(
    long long cardNumber,
    OperationCounter& counter) const
{
    return search(cardNumber, counter)
           != nullptr;
}

int CardHashTable::getCount() const
{
    return count;
}

int CardHashTable::getCapacity() const
{
    return capacity;
}

double CardHashTable::getLoadFactor() const
{
    if (capacity == 0)
    {
        return 0.0;
    }

    return static_cast<double>(count)
           / static_cast<double>(capacity);
}

void CardHashTable::clear()
{
    for (int i = 0;
         i < capacity;
         i++)
    {
        HashNode* current =
            buckets[i];

        while (current != nullptr)
        {
            HashNode* nextNode =
                current->next;

            delete current;

            current =
                nextNode;
        }

        buckets[i] =
            nullptr;
    }

    count = 0;
}

void CardHashTable::copyFrom(
    const CardHashTable& other)
{
    for (int i = 0;
         i < other.capacity;
         i++)
    {
        HashNode* source =
            other.buckets[i];

        HashNode* copiedTail =
            nullptr;

        while (source != nullptr)
        {
            HashNode* newNode =
                new HashNode(
                    source->data);

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

void CardHashTable::rehash(int newCapacity)
{
    HashNode** oldBuckets = buckets;
    int oldCapacity = capacity;

    buckets = new HashNode*[newCapacity];

    for (int i = 0; i < newCapacity; i++)
    {
        buckets[i] = nullptr;
    }

    capacity = newCapacity;

    for (int i = 0; i < oldCapacity; i++)
    {
        HashNode* current = oldBuckets[i];

        while (current != nullptr)
        {
            HashNode* nextNode = current->next;

            int newIndex =
                hash(current->data.getCardNumber());

            current->next = buckets[newIndex];
            buckets[newIndex] = current;

            current = nextNode;
        }
    }

    delete[] oldBuckets;
}

bool CardHashTable::saveCards(
    const char* filename) const
{
    std::ofstream output(filename);

    if (!output.is_open())
    {
        return false;
    }

    output
        << "cardNumber,holderName,cnic,balance,"
        << "blocked,active,openJourney,"
        << "entryStation,entryTime\n";

    output << std::fixed << std::setprecision(2);

    for (int i = 0; i < capacity; i++)
    {
        HashNode* current =
            buckets[i];

        while (current != nullptr)
        {
            const Card& card =
                current->data;

            output
                << card.getCardNumber() << ','
                << card.getHolderName() << ','
                << card.getCNIC() << ','
                << card.getBalance() << ','
                << (card.isBlocked() ? 1 : 0) << ','
                << (card.isActive() ? 1 : 0) << ','
                << (card.hasOpenJourney() ? 1 : 0) << ','
                << card.getEntryStation() << ','
                << card.getEntryTime()
                << '\n';

            current =
                current->next;
        }
    }

    output.close();

    return true;
}

bool CardHashTable::saveJourneys(
    const char* filename) const
{
    std::ofstream output(filename);

    if (!output.is_open())
    {
        return false;
    }

    output
        << "cardNumber,entryStation,exitStation,"
        << "entryTime,exitTime,fare\n";

    output << std::fixed << std::setprecision(2);

    for (int i = 0; i < capacity; i++)
    {
        HashNode* current =
            buckets[i];

        while (current != nullptr)
        {
            const Card& card =
                current->data;

            card.getJourneyHistory()
                .writeCSV(
                    output,
                    card.getCardNumber());

            current =
                current->next;
        }
    }

    output.close();

    return true;
}