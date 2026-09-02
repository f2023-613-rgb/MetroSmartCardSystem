#include "../include/GateQueue.h"

GateQueue::QueueNode::QueueNode(long long number)
{
    cardNumber = number;
    next = nullptr;
}

GateQueue::GateQueue()
{
    frontNode = nullptr;
    rearNode = nullptr;
    count = 0;
}

GateQueue::GateQueue(const GateQueue& other)
{
    frontNode = nullptr;
    rearNode = nullptr;
    count = 0;

    copyFrom(other);
}

GateQueue& GateQueue::operator=(const GateQueue& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }

    return *this;
}

GateQueue::~GateQueue()
{
    clear();
}

bool GateQueue::enqueue(long long cardNumber,
                        OperationCounter& counter)
{
    counter.incrementSteps();

    QueueNode* newNode = new QueueNode(cardNumber);

    if (newNode == nullptr)
    {
        return false;
    }

    counter.incrementSteps();

    if (rearNode == nullptr)
    {
        counter.incrementComparisons();

        frontNode = newNode;
        rearNode = newNode;

        counter.addSteps(2);
    }
    else
    {
        counter.incrementComparisons();

        rearNode->next = newNode;
        rearNode = newNode;

        counter.addSteps(2);
    }

    count++;
    counter.incrementSteps();

    return true;
}

bool GateQueue::dequeue(long long& removedCardNumber,
                        OperationCounter& counter)
{
    counter.incrementComparisons();

    if (frontNode == nullptr)
    {
        return false;
    }

    QueueNode* oldFront = frontNode;

    removedCardNumber = oldFront->cardNumber;

    counter.addSteps(2);

    frontNode = frontNode->next;

    counter.incrementSteps();

    if (frontNode == nullptr)
    {
        counter.incrementComparisons();

        rearNode = nullptr;

        counter.incrementSteps();
    }
    else
    {
        counter.incrementComparisons();
    }

    delete oldFront;

    count--;

    counter.addSteps(2);

    return true;
}

bool GateQueue::peek(long long& frontCardNumber) const
{
    if (frontNode == nullptr)
    {
        return false;
    }

    frontCardNumber = frontNode->cardNumber;

    return true;
}

bool GateQueue::isEmpty() const
{
    return frontNode == nullptr;
}

int GateQueue::getCount() const
{
    return count;
}

void GateQueue::clear()
{
    QueueNode* current = frontNode;

    while (current != nullptr)
    {
        QueueNode* nextNode = current->next;

        delete current;

        current = nextNode;
    }

    frontNode = nullptr;
    rearNode = nullptr;
    count = 0;
}

void GateQueue::copyFrom(const GateQueue& other)
{
    QueueNode* current = other.frontNode;

    while (current != nullptr)
    {
        QueueNode* newNode =
            new QueueNode(current->cardNumber);

        if (rearNode == nullptr)
        {
            frontNode = newNode;
            rearNode = newNode;
        }
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }

        count++;

        current = current->next;
    }
}