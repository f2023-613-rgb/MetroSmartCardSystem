#ifndef JOURNEY_HISTORY_H
#define JOURNEY_HISTORY_H

#include "Journey.h"
#include "OperationCounter.h"
#include <ostream>

class JourneyHistory
{
private:
    struct JourneyNode
    {
        Journey data;
        JourneyNode* prev;
        JourneyNode* next;

        JourneyNode(const Journey& journey);
    };

    JourneyNode* head;
    JourneyNode* tail;
    JourneyNode* current;
    int count;

    void clear();
    void copyFrom(const JourneyHistory& other);
    void removeOldest(OperationCounter& counter);

public:
    JourneyHistory();

    JourneyHistory(const JourneyHistory& other);

    JourneyHistory& operator=(const JourneyHistory& other);

    ~JourneyHistory();

    bool addJourney(const Journey& journey,
                    OperationCounter& counter);

    bool insertAfterCurrent(const Journey& journey,
                            OperationCounter& counter);

    bool deleteCurrent(OperationCounter& counter);

    bool moveNext(OperationCounter& counter);

    bool movePrevious(OperationCounter& counter);

    bool getCurrentJourney(Journey& journey) const;

    bool isEmpty() const;

    int getCount() const;

    void moveToFirst();

    void moveToLast();

    void writeCSV(std::ostream& output,
              long long cardNumber) const;
};

#endif