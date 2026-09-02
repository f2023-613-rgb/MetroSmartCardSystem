#include "../include/JourneyHistory.h"

JourneyHistory::JourneyNode::JourneyNode(
    const Journey& journey)
{
    data = journey;
    prev = nullptr;
    next = nullptr;
}

JourneyHistory::JourneyHistory()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    count = 0;
}

JourneyHistory::JourneyHistory(
    const JourneyHistory& other)
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    count = 0;

    copyFrom(other);
}

JourneyHistory& JourneyHistory::operator=(
    const JourneyHistory& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }

    return *this;
}

JourneyHistory::~JourneyHistory()
{
    clear();
}

bool JourneyHistory::addJourney(
    const Journey& journey,
    OperationCounter& counter)
{
    JourneyNode* newNode =
        new JourneyNode(journey);

    counter.incrementSteps();

    if (head == nullptr)
    {
        counter.incrementComparisons();

        head = newNode;
        tail = newNode;
        current = newNode;

        count = 1;

        counter.addSteps(4);

        return true;
    }

    counter.incrementComparisons();

    newNode->prev = tail;
    tail->next = newNode;
    tail = newNode;
    current = newNode;

    count++;

    counter.addSteps(5);

    if (count > 20)
    {
        removeOldest(counter);
    }

    return true;
}

bool JourneyHistory::insertAfterCurrent(
    const Journey& journey,
    OperationCounter& counter)
{
    if (current == nullptr)
    {
        counter.incrementComparisons();
        return addJourney(journey, counter);
    }

    counter.incrementComparisons();

    JourneyNode* newNode =
        new JourneyNode(journey);

    JourneyNode* after =
        current->next;

    newNode->prev = current;
    newNode->next = after;

    current->next = newNode;

    counter.addSteps(4);

    if (after != nullptr)
    {
        counter.incrementComparisons();

        after->prev = newNode;
        counter.incrementSteps();
    }
    else
    {
        counter.incrementComparisons();

        tail = newNode;
        counter.incrementSteps();
    }

    current = newNode;
    count++;

    counter.addSteps(2);

    if (count > 20)
    {
        removeOldest(counter);
    }

    return true;
}

bool JourneyHistory::deleteCurrent(
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (current == nullptr)
    {
        return false;
    }

    JourneyNode* nodeToDelete =
        current;

    JourneyNode* before =
        nodeToDelete->prev;

    JourneyNode* after =
        nodeToDelete->next;

    counter.addSteps(3);

    if (before != nullptr)
    {
        counter.incrementComparisons();

        before->next = after;
        counter.incrementSteps();
    }
    else
    {
        counter.incrementComparisons();

        head = after;
        counter.incrementSteps();
    }

    if (after != nullptr)
    {
        counter.incrementComparisons();

        after->prev = before;
        current = after;

        counter.addSteps(2);
    }
    else
    {
        counter.incrementComparisons();

        tail = before;
        current = before;

        counter.addSteps(2);
    }

    delete nodeToDelete;

    count--;

    counter.addSteps(2);

    return true;
}

bool JourneyHistory::moveNext(
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (current == nullptr ||
        current->next == nullptr)
    {
        return false;
    }

    current = current->next;

    counter.incrementSteps();

    return true;
}

bool JourneyHistory::movePrevious(
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (current == nullptr ||
        current->prev == nullptr)
    {
        return false;
    }

    current = current->prev;

    counter.incrementSteps();

    return true;
}

bool JourneyHistory::getCurrentJourney(
    Journey& journey) const
{
    if (current == nullptr)
    {
        return false;
    }

    journey = current->data;

    return true;
}

bool JourneyHistory::isEmpty() const
{
    return head == nullptr;
}

int JourneyHistory::getCount() const
{
    return count;
}

void JourneyHistory::moveToFirst()
{
    current = head;
}

void JourneyHistory::moveToLast()
{
    current = tail;
}

void JourneyHistory::removeOldest(
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (head == nullptr)
    {
        return;
    }

    JourneyNode* oldHead =
        head;

    head = head->next;

    counter.addSteps(2);

    if (head != nullptr)
    {
        counter.incrementComparisons();

        head->prev = nullptr;
        counter.incrementSteps();
    }
    else
    {
        counter.incrementComparisons();

        tail = nullptr;
        current = nullptr;

        counter.addSteps(2);
    }

    if (current == oldHead)
    {
        counter.incrementComparisons();
        current = head;
        counter.incrementSteps();
    }
    else
    {
        counter.incrementComparisons();
    }

    delete oldHead;

    count--;

    counter.addSteps(2);
}

void JourneyHistory::clear()
{
    JourneyNode* currentNode =
        head;

    while (currentNode != nullptr)
    {
        JourneyNode* nextNode =
            currentNode->next;

        delete currentNode;

        currentNode = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    current = nullptr;
    count = 0;
}

void JourneyHistory::copyFrom(
    const JourneyHistory& other)
{
    JourneyNode* source =
        other.head;

    JourneyNode* copiedCurrent =
        nullptr;

    while (source != nullptr)
    {
        JourneyNode* newNode =
            new JourneyNode(source->data);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }

        if (source == other.current)
        {
            copiedCurrent = newNode;
        }

        count++;

        source = source->next;
    }

    current = copiedCurrent;
}