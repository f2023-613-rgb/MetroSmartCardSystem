#ifndef OPERATION_COUNTER_H
#define OPERATION_COUNTER_H

class OperationCounter
{
private:
    long long comparisons;
    long long steps;

public:
    OperationCounter();

    void reset();

    void incrementComparisons();
    void incrementSteps();

    void addComparisons(long long amount);
    void addSteps(long long amount);

    long long getComparisons() const;
    long long getSteps() const;
};

#endif