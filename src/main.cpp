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

    system.replayTransactions();

    return 0;
}