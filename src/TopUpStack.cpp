#include "../include/TopUpStack.h"

TopUpStack::StackNode::StackNode(long long number,
                                double topUpAmount)
{
    cardNumber = number;
    amount = topUpAmount;
    next = nullptr;
}

TopUpStack::TopUpStack()
{
    topNode = nullptr;
    count = 0;
}

TopUpStack::TopUpStack(const TopUpStack& other)
{
    topNode = nullptr;
    count = 0;

    copyFrom(other);
}

TopUpStack& TopUpStack::operator=(const TopUpStack& other)
{
    if (this != &other)
    {
        clear();
        copyFrom(other);
    }

    return *this;
}

TopUpStack::~TopUpStack()
{
    clear();
}

bool TopUpStack::push(long long cardNumber,
                      double amount,
                      OperationCounter& counter)
{
    if (amount <= 0.0)
    {
        counter.incrementComparisons();
        return false;
    }

    counter.incrementComparisons();

    StackNode* newNode =
        new StackNode(cardNumber, amount);

    newNode->next = topNode;
    topNode = newNode;

    count++;

    counter.addSteps(3);

    return true;
}

bool TopUpStack::pop(long long& cardNumber,
                     double& amount,
                     OperationCounter& counter)
{
    counter.incrementComparisons();

    if (topNode == nullptr)
    {
        return false;
    }

    StackNode* oldTop = topNode;

    cardNumber = oldTop->cardNumber;
    amount = oldTop->amount;

    topNode = topNode->next;

    delete oldTop;

    count--;

    counter.addSteps(4);

    return true;
}

bool TopUpStack::peek(long long& cardNumber,
                      double& amount) const
{
    if (topNode == nullptr)
    {
        return false;
    }

    cardNumber = topNode->cardNumber;
    amount = topNode->amount;

    return true;
}

bool TopUpStack::isEmpty() const
{
    return topNode == nullptr;
}

int TopUpStack::getCount() const
{
    return count;
}

void TopUpStack::clear()
{
    StackNode* current = topNode;

    while (current != nullptr)
    {
        StackNode* nextNode = current->next;

        delete current;

        current = nextNode;
    }

    topNode = nullptr;
    count = 0;
}

void TopUpStack::copyFrom(const TopUpStack& other)
{
    if (other.topNode == nullptr)
    {
        return;
    }

    // First copy the source nodes into a temporary reversed chain.
    StackNode* reversed = nullptr;
    StackNode* current = other.topNode;

    while (current != nullptr)
    {
        StackNode* newNode =
            new StackNode(current->cardNumber,
                          current->amount);

        newNode->next = reversed;
        reversed = newNode;

        current = current->next;
    }

    // Push them back into this stack in the correct order.
    while (reversed != nullptr)
    {
        StackNode* nextNode = reversed->next;

        reversed->next = topNode;
        topNode = reversed;

        reversed = nextNode;

        count++;
    }
}