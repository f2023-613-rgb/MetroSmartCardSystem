#include "../include/Card.h"

#include <cstring>

Card::Card()
{
    cardNumber = 0;

    holderName[0] = '\0';
    cnic[0] = '\0';

    balance = 0.0;

    blocked = false;
    active = true;

    openJourney = false;
    entryStation = -1;
    entryTime[0] = '\0';
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

bool Card::hasOpenJourney() const
{
    return openJourney;
}

int Card::getEntryStation() const
{
    return entryStation;
}

const char* Card::getEntryTime() const
{
    return entryTime;
}

JourneyHistory& Card::getJourneyHistory()
{
    return journeyHistory;
}

const JourneyHistory& Card::getJourneyHistory() const
{
    return journeyHistory;
}

void Card::setCardNumber(long long number)
{
    if (number >= 1000000000000000LL &&
        number <= 9999999999999999LL)
    {
        cardNumber = number;
    }
}

void Card::setHolderName(const char* name)
{
    if (name == nullptr)
    {
        holderName[0] = '\0';
        return;
    }

    std::strncpy(
        holderName,
        name,
        sizeof(holderName) - 1);

    holderName[sizeof(holderName) - 1] = '\0';
}

void Card::setCNIC(const char* cnicValue)
{
    if (cnicValue == nullptr)
    {
        cnic[0] = '\0';
        return;
    }

    std::strncpy(
        cnic,
        cnicValue,
        sizeof(cnic) - 1);

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
    if (amount < 0.0 ||
        amount > balance)
    {
        return false;
    }

    balance -= amount;

    return true;
}

bool Card::openNewJourney(
    int stationId,
    const char* time)
{
    if (openJourney)
    {
        return false;
    }

    if (stationId < 0 ||
        stationId >= 30)
    {
        return false;
    }

    entryStation = stationId;

    if (time == nullptr)
    {
        entryTime[0] = '\0';
    }
    else
    {
        std::strncpy(
            entryTime,
            time,
            sizeof(entryTime) - 1);

        entryTime[sizeof(entryTime) - 1] = '\0';
    }

    openJourney = true;

    return true;
}

void Card::closeOpenJourney()
{
    openJourney = false;

    entryStation = -1;
    entryTime[0] = '\0';
}