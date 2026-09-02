#include "../include/MetroSystem.h"
#include <fstream>
#include <sstream>
#include <string>

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
if (cardNumber < 1000000000000000LL ||
    cardNumber > 9999999999999999LL ||
    balance < 0.0)
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

int MetroSystem::undoLastNTopUps(
    int n,
    OperationCounter& counter)
{
    counter.incrementComparisons();

    if (n <= 0)
    {
        return 0;
    }

    int undoneCount = 0;

    for (int i = 0; i < n; i++)
    {
        counter.incrementSteps();

        if (!undoLastTopUp(counter))
        {
            break;
        }

        undoneCount++;
    }

    return undoneCount;
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

void MetroSystem::replayTransactions(
    OperationCounter& counter) const
{
    transactionLog.replay(counter);
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

bool MetroSystem::saveCards(
    const char* filename) const
{
    return cards.saveCards(filename);
}

bool MetroSystem::saveJourneys(
    const char* filename) const
{
    return cards.saveJourneys(filename);
}

bool MetroSystem::loadCards(
    const char* filename)
{
    std::ifstream input(filename);

    if (!input.is_open())
    {
        return false;
    }

    std::string line;

    // Skip header
    std::getline(input, line);

    while (std::getline(input, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream stream(line);

        std::string cardNumberText;
        std::string holderName;
        std::string cnic;
        std::string balanceText;
        std::string blockedText;
        std::string activeText;
        std::string openJourneyText;
        std::string entryStationText;
        std::string entryTime;

        std::getline(stream, cardNumberText, ',');
        std::getline(stream, holderName, ',');
        std::getline(stream, cnic, ',');
        std::getline(stream, balanceText, ',');
        std::getline(stream, blockedText, ',');
        std::getline(stream, activeText, ',');
        std::getline(stream, openJourneyText, ',');
        std::getline(stream, entryStationText, ',');
        std::getline(stream, entryTime);

        try
        {
            long long cardNumber =
                std::stoll(cardNumberText);

            double balance =
                std::stod(balanceText);

            bool blocked =
                (std::stoi(blockedText) != 0);

            bool active =
                (std::stoi(activeText) != 0);

            bool openJourney =
                (std::stoi(openJourneyText) != 0);

            int entryStation =
                std::stoi(entryStationText);

            Card card;

            card.setCardNumber(cardNumber);
            card.setHolderName(holderName.c_str());
            card.setCNIC(cnic.c_str());
            card.setBalance(balance);
            card.setBlocked(blocked);
            card.setActive(active);

            if (openJourney)
            {
                card.openNewJourney(
                    entryStation,
                    entryTime.c_str());
            }

            OperationCounter counter;

            if (!cards.insert(
                    card,
                    counter))
            {
                continue;
            }

            if (blocked)
            {
                OperationCounter blockCounter;

                blockedCards.blockCard(
                    cardNumber,
                    blockCounter);
            }
        }
        catch (...)
        {
            // Invalid record: skip it safely.
            continue;
        }
    }

    input.close();

    return true;
}

bool MetroSystem::loadJourneys(
    const char* filename)
{
    std::ifstream input(filename);

    if (!input.is_open())
    {
        return false;
    }

    std::string line;

    // Skip header
    std::getline(input, line);

    while (std::getline(input, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream stream(line);

        std::string cardNumberText;
        std::string entryStationText;
        std::string exitStationText;
        std::string entryTime;
        std::string exitTime;
        std::string fareText;

        std::getline(stream, cardNumberText, ',');
        std::getline(stream, entryStationText, ',');
        std::getline(stream, exitStationText, ',');
        std::getline(stream, entryTime, ',');
        std::getline(stream, exitTime, ',');
        std::getline(stream, fareText);

        try
        {
            long long cardNumber =
                std::stoll(cardNumberText);

            int entryStation =
                std::stoi(entryStationText);

            int exitStation =
                std::stoi(exitStationText);

            double fare =
                std::stod(fareText);

            OperationCounter searchCounter;

            Card* card =
                cards.search(
                    cardNumber,
                    searchCounter);

            if (card == nullptr)
            {
                continue;
            }

            Journey journey;

            journey.setEntryStation(entryStation);
            journey.setExitStation(exitStation);
            journey.setEntryTime(entryTime.c_str());
            journey.setExitTime(exitTime.c_str());
            journey.setFare(fare);

            OperationCounter historyCounter;

            card->getJourneyHistory()
                .addJourney(
                    journey,
                    historyCounter);
        }
        catch (...)
        {
            continue;
        }
    }

    input.close();

    return true;
}