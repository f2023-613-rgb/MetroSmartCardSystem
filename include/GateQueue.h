#ifndef GATE_QUEUE_H
#define GATE_QUEUE_H

#include "OperationCounter.h"

class GateQueue
{
private:
    struct QueueNode
    {
        long long cardNumber;
        QueueNode* next;

        QueueNode(long long number);
    };

    QueueNode* frontNode;
    QueueNode* rearNode;
    int count;

    void copyFrom(const GateQueue& other);
    void clear();

public:
    GateQueue();

    GateQueue(const GateQueue& other);

    GateQueue& operator=(const GateQueue& other);

    ~GateQueue();

    bool enqueue(long long cardNumber, OperationCounter& counter);

    bool dequeue(long long& removedCardNumber,
                 OperationCounter& counter);

    bool peek(long long& frontCardNumber) const;

    bool isEmpty() const;

    int getCount() const;
};

#endif