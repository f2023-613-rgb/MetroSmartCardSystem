#include "../include/TransactionLog.h"

#include <cstring>
#include <iostream>

TransactionLog::TransactionNode::TransactionNode(
    TransactionType transactionType,
    long long number,
    double transactionAmount,
    int station,
    const char* transactionTime)
{
    type = transactionType;
    cardNumber = number;
    amount = transactionAmount;
    stationId = station;
    next = nullptr;

    if (transactionTime == nullptr)
    {
        time[0] = '\0';
    }
    else
    {
        std::strncpy(time,
                     transactionTime,
                     sizeof(time) - 1);

        time[sizeof(time) - 1] = '\0';
    }
}

TransactionLog::TransactionLog()
{
    head = nullptr;
    tail = nullptr;
    count = 0;
}

TransactionLog::TransactionLog(const TransactionLog& other)
{
    head = nullptr;
    tail = nullptr;
    count = 0;

    copyFrom(other);
}

TransactionLog& TransactionLog::operator=(
    const TransactionLog& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }

    return *this;
}

TransactionLog::~TransactionLog()
{
    clear();
}

bool TransactionLog::append(TransactionType type,
                            long long cardNumber,
                            double amount,
                            int stationId,
                            const char* time,
                            OperationCounter& counter)
{
    TransactionNode* newNode =
        new TransactionNode(type,
                            cardNumber,
                            amount,
                            stationId,
                            time);

    counter.incrementSteps();

    if (head == nullptr)
    {
        counter.incrementComparisons();

        head = newNode;
        tail = newNode;

        counter.addSteps(2);
    }
    else
    {
        counter.incrementComparisons();

        tail->next = newNode;
        tail = newNode;

        counter.addSteps(2);
    }

    count++;
    counter.incrementSteps();

    return true;
}

void TransactionLog::replay() const
{
    TransactionNode* current = head;

    while (current != nullptr)
    {
        std::cout
            << getTypeName(current->type)
            << " | Card: "
            << current->cardNumber;

        if (current->amount > 0.0)
        {
            std::cout
                << " | Amount: "
                << current->amount;
        }

        if (current->stationId >= 0)
        {
            std::cout
                << " | Station: "
                << current->stationId;
        }

        if (current->time[0] != '\0')
        {
            std::cout
                << " | Time: "
                << current->time;
        }

        std::cout << '\n';

        current = current->next;
    }
}

bool TransactionLog::isEmpty() const
{
    return head == nullptr;
}

int TransactionLog::getCount() const
{
    return count;
}

void TransactionLog::clear()
{
    TransactionNode* current = head;

    while (current != nullptr)
    {
        TransactionNode* nextNode =
            current->next;

        delete current;

        current = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

void TransactionLog::copyFrom(
    const TransactionLog& other)
{
    TransactionNode* current =
        other.head;

    while (current != nullptr)
    {
        TransactionNode* newNode =
            new TransactionNode(
                current->type,
                current->cardNumber,
                current->amount,
                current->stationId,
                current->time);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        count++;

        current = current->next;
    }
}

const char* TransactionLog::getTypeName(
    TransactionType type) const
{
    switch (type)
    {
        case REGISTER_CARD:
            return "REGISTER";

        case TAP_IN:
            return "TAP_IN";

        case TAP_OUT:
            return "TAP_OUT";

        case TOP_UP:
            return "TOP_UP";

        case UNDO_TOP_UP:
            return "UNDO_TOP_UP";

        case BLOCK_CARD:
            return "BLOCK_CARD";

        case UNBLOCK_CARD:
            return "UNBLOCK_CARD";

        default:
            return "UNKNOWN";
    }
}