#include "../include/MetroSystem.h"

const double MetroSystem::MINIMUM_BALANCE = 50.0;

const double MetroSystem::FARE_PER_STATION = 10.0;

MetroSystem::MetroSystem()
{
    for (int i = 0; i < 30; i++)
    {
        stations[i].setStationId(i);
    }
}

bool MetroSystem::registerCard(
    long long cardNumber,
    const char* holderName,
    const char* cnic,
    double balance,
    OperationCounter& counter)
{
    if (cardNumber <= 0 || balance < 0.0)
    {
        counter.incrementComparisons();
        return false;
    }

    counter.incrementComparisons();

    Card newCard;

    newCard.setCardNumber(cardNumber);
    newCard.setHolderName(holderName);
    newCard.setCNIC(cnic);
    newCard.setBalance(balance);
    newCard.setBlocked(false);
    newCard.setActive(true);

    counter.addSteps(6);

    if (!cards.insert(newCard, counter))
    {
        return false;
    }

    transactionLog.append(
        TransactionLog::REGISTER_CARD,
        cardNumber,
        0.0,
        -1,
        "",
        counter);

    return true;
}

Card* MetroSystem::findCard(
    long long cardNumber,
    OperationCounter& counter)
{
    return cards.search(cardNumber, counter);
}

bool MetroSystem::topUpCard(
    long long cardNumber,
    double amount,
    const char* time,
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (amount <= 0.0)
    {
        return false;
    }

    Card* card =
        cards.search(cardNumber, counter);

    counter.incrementComparisons();

    if (card == nullptr)
    {
        return false;
    }

    if (!card->addBalance(amount))
    {
        return false;
    }

    counter.incrementSteps();

    if (!undoStack.push(
            cardNumber,
            amount,
            counter))
    {
        card->deductBalance(amount);
        return false;
    }

    transactionLog.append(
        TransactionLog::TOP_UP,
        cardNumber,
        amount,
        -1,
        time,
        counter);

    return true;
}

bool MetroSystem::undoLastTopUp(
    OperationCounter& counter)
{
    long long cardNumber = 0;
    double amount = 0.0;

    if (!undoStack.pop(
            cardNumber,
            amount,
            counter))
    {
        return false;
    }

    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    if (!card->deductBalance(amount))
    {
        return false;
    }

    transactionLog.append(
        TransactionLog::UNDO_TOP_UP,
        cardNumber,
        amount,
        -1,
        "",
        counter);

    return true;
}

bool MetroSystem::blockCard(
    long long cardNumber,
    const char* time,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    counter.incrementComparisons();

    if (card->isBlocked())
    {
        return false;
    }

    if (!blockedCards.blockCard(
            cardNumber,
            counter))
    {
        return false;
    }

    card->setBlocked(true);

    counter.incrementSteps();

    transactionLog.append(
        TransactionLog::BLOCK_CARD,
        cardNumber,
        0.0,
        -1,
        time,
        counter);

    return true;
}

bool MetroSystem::unblockCard(
    long long cardNumber,
    const char* time,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    counter.incrementComparisons();

    if (!card->isBlocked())
    {
        return false;
    }

    if (!blockedCards.unblockCard(
            cardNumber,
            counter))
    {
        return false;
    }

    card->setBlocked(false);

    counter.incrementSteps();

    transactionLog.append(
        TransactionLog::UNBLOCK_CARD,
        cardNumber,
        0.0,
        -1,
        time,
        counter);

    return true;
}

bool MetroSystem::addPassengerToGate(
    long long cardNumber,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    counter.incrementComparisons();

    if (!card->isActive())
    {
        return false;
    }

    return gateQueue.enqueue(
        cardNumber,
        counter);
}

bool MetroSystem::serveNextPassenger(
    long long& cardNumber,
    OperationCounter& counter)
{
    return gateQueue.dequeue(
        cardNumber,
        counter);
}

int MetroSystem::getRegisteredCardCount() const
{
    return cards.getCount();
}

int MetroSystem::getBlockedCardCount() const
{
    return blockedCards.getCount();
}

int MetroSystem::getGateQueueCount() const
{
    return gateQueue.getCount();
}

void MetroSystem::replayTransactions() const
{
    transactionLog.replay();
}

double MetroSystem::calculateFare(
    int entryStation,
    int exitStation) const
{
    int distance =
        exitStation - entryStation;

    if (distance < 0)
    {
        distance = -distance;
    }

    return distance * FARE_PER_STATION;
}

bool MetroSystem::tapIn(
    long long cardNumber,
    int stationId,
    const char* time,
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (stationId < 0 ||
        stationId >= 30)
    {
        return false;
    }

    Card* card =
        cards.search(
            cardNumber,
            counter);

    counter.incrementComparisons();

    if (card == nullptr)
    {
        return false;
    }

    counter.incrementComparisons();

    if (!card->isActive())
    {
        return false;
    }

    bool blocked =
        blockedCards.isBlocked(
            cardNumber,
            counter);

    counter.incrementComparisons();

    if (blocked)
    {
        return false;
    }

    counter.incrementComparisons();

    if (card->getBalance()
        < MINIMUM_BALANCE)
    {
        return false;
    }

    counter.incrementComparisons();

    if (card->hasOpenJourney())
    {
        return false;
    }

    if (!card->openNewJourney(
            stationId,
            time))
    {
        return false;
    }

    counter.incrementSteps();

    transactionLog.append(
        TransactionLog::TAP_IN,
        cardNumber,
        0.0,
        stationId,
        time,
        counter);

    return true;
}

bool MetroSystem::tapOut(
    long long cardNumber,
    int stationId,
    const char* time,
    double& chargedFare,
    OperationCounter& counter)
{
    chargedFare = 0.0;

    counter.incrementComparisons();

    if (stationId < 0 ||
        stationId >= 30)
    {
        return false;
    }

    Card* card =
        cards.search(
            cardNumber,
            counter);

    counter.incrementComparisons();

    if (card == nullptr)
    {
        return false;
    }

    counter.incrementComparisons();

    if (!card->hasOpenJourney())
    {
        return false;
    }

    int entryStation =
        card->getEntryStation();

    const char* entryTime =
        card->getEntryTime();

    double fare =
        calculateFare(
            entryStation,
            stationId);

    counter.addSteps(3);

    counter.incrementComparisons();

    if (fare > card->getBalance())
    {
        return false;
    }

    Journey completedJourney;

    completedJourney.setEntryStation(
        entryStation);

    completedJourney.setExitStation(
        stationId);

    completedJourney.setEntryTime(
        entryTime);

    completedJourney.setExitTime(
        time);

    completedJourney.setFare(
        fare);

    counter.addSteps(5);

    if (!card->deductBalance(fare))
    {
        return false;
    }

    JourneyHistory& history =
        card->getJourneyHistory();

    if (!history.addJourney(
            completedJourney,
            counter))
    {
        return false;
    }

    card->closeOpenJourney();

    counter.incrementSteps();

    transactionLog.append(
        TransactionLog::TAP_OUT,
        cardNumber,
        fare,
        stationId,
        time,
        counter);

    chargedFare = fare;

    return true;
}

bool MetroSystem::getCurrentJourney(
    long long cardNumber,
    Journey& journey,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    return card
        ->getJourneyHistory()
        .getCurrentJourney(journey);
}

bool MetroSystem::moveJourneyPrevious(
    long long cardNumber,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    return card
        ->getJourneyHistory()
        .movePrevious(counter);
}

bool MetroSystem::moveJourneyNext(
    long long cardNumber,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    return card
        ->getJourneyHistory()
        .moveNext(counter);
}

bool MetroSystem::insertJourneyAfterCurrent(
    long long cardNumber,
    const Journey& journey,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    return card
        ->getJourneyHistory()
        .insertAfterCurrent(
            journey,
            counter);
}

bool MetroSystem::deleteCurrentJourney(
    long long cardNumber,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    return card
        ->getJourneyHistory()
        .deleteCurrent(counter);
}

bool MetroSystem::moveJourneyToFirst(
    long long cardNumber,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    card->getJourneyHistory()
        .moveToFirst();

    counter.incrementSteps();

    return true;
}

bool MetroSystem::moveJourneyToLast(
    long long cardNumber,
    OperationCounter& counter)
{
    Card* card =
        cards.search(
            cardNumber,
            counter);

    if (card == nullptr)
    {
        return false;
    }

    card->getJourneyHistory()
        .moveToLast();

    counter.incrementSteps();

    return true;
}