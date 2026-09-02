#include "../include/Card.h"
#include <cstring>

Card::Card()
{
    cardNumber = 0;

    std::strcpy(holderName, "");
    std::strcpy(cnic, "");

    balance = 0.0;
    blocked = false;
    active = true;
}

long long Card::getCardNumber() const
{
    return cardNumber;
}

const char* Card::getHolderName() const
{
    return holderName;
}

const char* Card::getCNIC() const
{
    return cnic;
}

double Card::getBalance() const
{
    return balance;
}

bool Card::isBlocked() const
{
    return blocked;
}

bool Card::isActive() const
{
    return active;
}

void Card::setCardNumber(long long number)
{
    cardNumber = number;
}

void Card::setHolderName(const char* name)
{
    if (name == nullptr)
    {
        holderName[0] = '\0';
        return;
    }

    std::strncpy(holderName, name, sizeof(holderName) - 1);
    holderName[sizeof(holderName) - 1] = '\0';
}

void Card::setCNIC(const char* cnicValue)
{
    if (cnicValue == nullptr)
    {
        cnic[0] = '\0';
        return;
    }

    std::strncpy(cnic, cnicValue, sizeof(cnic) - 1);
    cnic[sizeof(cnic) - 1] = '\0';
}

void Card::setBalance(double amount)
{
    if (amount >= 0.0)
    {
        balance = amount;
    }
}

void Card::setBlocked(bool value)
{
    blocked = value;
}

void Card::setActive(bool value)
{
    active = value;
}

bool Card::addBalance(double amount)
{
    if (amount <= 0.0)
    {
        return false;
    }

    balance += amount;
    return true;
}

bool Card::deductBalance(double amount)
{
    if (amount <= 0.0 || amount > balance)
    {
        return false;
    }

    balance -= amount;
    return true;
}