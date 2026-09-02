#include "../include/OperationCounter.h"

OperationCounter::OperationCounter()
{
    comparisons = 0;
    steps = 0;
}

void OperationCounter::reset()
{
    comparisons = 0;
    steps = 0;
}

void OperationCounter::incrementComparisons()
{
    comparisons++;
}

void OperationCounter::incrementSteps()
{
    steps++;
}

void OperationCounter::addComparisons(long long amount)
{
    if (amount > 0)
    {
        comparisons += amount;
    }
}

void OperationCounter::addSteps(long long amount)
{
    if (amount > 0)
    {
        steps += amount;
    }
}

long long OperationCounter::getComparisons() const
{
    return comparisons;
}

long long OperationCounter::getSteps() const
{
    return steps;
}