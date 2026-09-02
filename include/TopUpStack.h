#ifndef TOP_UP_STACK_H
#define TOP_UP_STACK_H

#include "OperationCounter.h"

class TopUpStack
{
private:
    struct StackNode
    {
        long long cardNumber;
        double amount;
        StackNode* next;

        StackNode(long long number, double topUpAmount);
    };

    StackNode* topNode;
    int count;

    void clear();
    void copyFrom(const TopUpStack& other);

public:
    TopUpStack();

    TopUpStack(const TopUpStack& other);

    TopUpStack& operator=(const TopUpStack& other);

    ~TopUpStack();

    bool push(long long cardNumber,
              double amount,
              OperationCounter& counter);

    bool pop(long long& cardNumber,
             double& amount,
             OperationCounter& counter);

    bool peek(long long& cardNumber,
              double& amount) const;

    bool isEmpty() const;

    int getCount() const;
};

#endif