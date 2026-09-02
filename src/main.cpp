#include <iostream>

#include "../include/MetroSystem.h"

int main()
{
    MetroSystem system;

    OperationCounter counter;

    std::cout << "=== METRO SYSTEM TEST ===\n";

    bool registered =
        system.registerCard(
            1234560000000001LL,
            "Ali Khan",
            "3520212345671",
            500.0,
            counter);

    if (registered)
    {
        std::cout
            << "Card registered successfully.\n";
    }

    counter.reset();

    bool secondRegistered =
        system.registerCard(
            1234560000000002LL,
            "Ahmed Raza",
            "3520212345672",
            300.0,
            counter);

    if (secondRegistered)
    {
        std::cout
            << "Second card registered successfully.\n";
    }

    std::cout
        << "Registered cards: "
        << system.getRegisteredCardCount()
        << '\n';

    counter.reset();

    Card* card =
        system.findCard(
            1234560000000001LL,
            counter);

    if (card != nullptr)
    {
        std::cout
            << "Found holder: "
            << card->getHolderName()
            << '\n';

        std::cout
            << "Current balance: "
            << card->getBalance()
            << '\n';
    }

    counter.reset();

    if (system.topUpCard(
            1234560000000001LL,
            200.0,
            "12:00",
            counter))
    {
        std::cout
            << "Top-up successful.\n";
    }

    counter.reset();

    card =
        system.findCard(
            1234560000000001LL,
            counter);

    if (card != nullptr)
    {
        std::cout
            << "Balance after top-up: "
            << card->getBalance()
            << '\n';
    }

    counter.reset();

    if (system.undoLastTopUp(counter))
    {
        std::cout
            << "Top-up undo successful.\n";
    }

    counter.reset();

    card =
        system.findCard(
            1234560000000001LL,
            counter);

    if (card != nullptr)
    {
        std::cout
            << "Balance after undo: "
            << card->getBalance()
            << '\n';
    }

    counter.reset();

    if (system.blockCard(
            1234560000000002LL,
            "12:10",
            counter))
    {
        std::cout
            << "Card blocked successfully.\n";
    }

    std::cout
        << "Blocked cards: "
        << system.getBlockedCardCount()
        << '\n';

    counter.reset();

    if (system.unblockCard(
            1234560000000002LL,
            "12:15",
            counter))
    {
        std::cout
            << "Card unblocked successfully.\n";
    }

    std::cout
        << "Blocked cards after unblock: "
        << system.getBlockedCardCount()
        << '\n';

    counter.reset();

    system.addPassengerToGate(
        1234560000000001LL,
        counter);

    system.addPassengerToGate(
        1234560000000002LL,
        counter);

    std::cout
        << "Passengers in gate queue: "
        << system.getGateQueueCount()
        << '\n';

    long long servedCard = 0;

    counter.reset();

    if (system.serveNextPassenger(
            servedCard,
            counter))
    {
        std::cout
            << "Served card: "
            << servedCard
            << '\n';
    }

    std::cout
        << "Passengers remaining: "
        << system.getGateQueueCount()
        << '\n';

    std::cout
        << "\n=== DAILY TRANSACTION REPLAY ===\n";

    counter.reset();

    system.registerCard(
        1234560000000003LL,
        "Sara Ahmed",
        "3520212345673",
        500.0,
        counter);

    std::cout
    << "\n=== TAP IN / TAP OUT TEST ===\n";

counter.reset();

if (system.tapIn(
        1234560000000003LL,
        3,
        "13:00",
        counter))
{
    std::cout
        << "Tap-in successful.\n";
}
else
{
    std::cout
        << "Tap-in failed.\n";
}

std::cout
    << "Tap-in steps: "
    << counter.getSteps()
    << '\n';

std::cout
    << "Tap-in comparisons: "
    << counter.getComparisons()
    << '\n';

    counter.reset();

double fare = 0.0;

if (system.tapOut(
        1234560000000003LL,
        8,
        "13:30",
        fare,
        counter))
{
    std::cout
        << "Tap-out successful.\n";

    std::cout
        << "Fare charged: "
        << fare
        << '\n';
}
else
{
    std::cout
        << "Tap-out failed.\n";
}

counter.reset();

Card* sara =
    system.findCard(
        1234560000000003LL,
        counter);

if (sara != nullptr)
{
    std::cout
        << "Balance after journey: "
        << sara->getBalance()
        << '\n';

    std::cout
        << "Journey history count: "
        << sara
               ->getJourneyHistory()
               .getCount()
        << '\n';
}

counter.reset();

system.tapIn(
    1234560000000003LL,
    2,
    "14:00",
    counter);

counter.reset();

if (!system.tapIn(
        1234560000000003LL,
        4,
        "14:05",
        counter))
{
    std::cout
        << "Duplicate tap-in rejected correctly.\n";
}

counter.reset();

double secondFare = 0.0;

system.tapOut(
    1234560000000003LL,
    5,
    "14:20",
    secondFare,
    counter);

counter.reset();

system.blockCard(
    1234560000000003LL,
    "15:00",
    counter);

counter.reset();

if (!system.tapIn(
        1234560000000003LL,
        1,
        "15:05",
        counter))
{
    std::cout
        << "Blocked card tap-in rejected correctly.\n";
}

counter.reset();

system.unblockCard(
    1234560000000003LL,
    "15:10",
    counter);

counter.reset();

system.registerCard(
    1234560000000004LL,
    "Low Balance User",
    "3520212345674",
    20.0,
    counter);

counter.reset();

if (!system.tapIn(
        1234560000000004LL,
        1,
        "16:00",
        counter))
{
    std::cout
        << "Low-balance tap-in rejected correctly.\n";
}

std::cout
    << "\n=== ADVANCED B JOURNEY HISTORY TEST ===\n";

counter.reset();

Journey selectedJourney;

if (system.getCurrentJourney(
        1234560000000003LL,
        selectedJourney,
        counter))
{
    std::cout
        << "Current fare: "
        << selectedJourney.getFare()
        << '\n';
}

counter.reset();

if (system.moveJourneyPrevious(
        1234560000000003LL,
        counter))
{
    system.getCurrentJourney(
        1234560000000003LL,
        selectedJourney,
        counter);

    std::cout
        << "Previous journey fare: "
        << selectedJourney.getFare()
        << '\n';
}

counter.reset();

if (system.moveJourneyNext(
        1234560000000003LL,
        counter))
{
    system.getCurrentJourney(
        1234560000000003LL,
        selectedJourney,
        counter);

    std::cout
        << "Next journey fare: "
        << selectedJourney.getFare()
        << '\n';
}

counter.reset();

system.moveJourneyToFirst(
    1234560000000003LL,
    counter);

Journey insertedJourney;

insertedJourney.setEntryStation(8);
insertedJourney.setExitStation(9);
insertedJourney.setEntryTime("13:35");
insertedJourney.setExitTime("13:45");
insertedJourney.setFare(10.0);

counter.reset();

if (system.insertJourneyAfterCurrent(
        1234560000000003LL,
        insertedJourney,
        counter))
{
    std::cout
        << "Middle journey inserted successfully.\n";
}

counter.reset();

if (system.getCurrentJourney(
        1234560000000003LL,
        selectedJourney,
        counter))
{
    std::cout
        << "Inserted journey fare: "
        << selectedJourney.getFare()
        << '\n';
}

counter.reset();

if (system.deleteCurrentJourney(
        1234560000000003LL,
        counter))
{
    std::cout
        << "Current journey deleted successfully.\n";
}

counter.reset();

if (system.getCurrentJourney(
        1234560000000003LL,
        selectedJourney,
        counter))
{
    std::cout
        << "Fare after deletion: "
        << selectedJourney.getFare()
        << '\n';
}

counter.reset();

Card* historyCard =
    system.findCard(
        1234560000000003LL,
        counter);

if (historyCard != nullptr)
{
    std::cout
        << "Final journey history count: "
        << historyCard
               ->getJourneyHistory()
               .getCount()
        << '\n';
}


    system.replayTransactions();

    return 0;
}