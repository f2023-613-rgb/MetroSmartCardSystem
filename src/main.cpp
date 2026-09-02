#include <iostream>

#include "../include/MetroSystem.h"

int main()
{
    MetroSystem system;

    std::cout
        << "=== METRO SMART CARD SYSTEM ===\n\n";

    if (system.loadCards("data/cards.csv"))
    {
        std::cout
            << "Cards loaded successfully.\n";
    }
    else
    {
        std::cout
            << "Could not load cards.csv.\n";
    }

    if (system.loadJourneys("data/journeys.csv"))
    {
        std::cout
            << "Journeys loaded successfully.\n";
    }
    else
    {
        std::cout
            << "Could not load journeys.csv.\n";
    }

    std::cout
        << "Registered cards: "
        << system.getRegisteredCardCount()
        << '\n';

    std::cout
        << "Blocked cards: "
        << system.getBlockedCardCount()
        << '\n';

    OperationCounter counter;

    Card* card =
        system.findCard(
            1234560000000101LL,
            counter);

    if (card != nullptr)
    {
        std::cout
            << "\nLoaded card holder: "
            << card->getHolderName()
            << '\n';

        std::cout
            << "Loaded balance: "
            << card->getBalance()
            << '\n';

        std::cout
            << "Loaded journeys: "
            << card
                   ->getJourneyHistory()
                   .getCount()
            << '\n';
    }

    if (system.saveCards(
            "data/cards_saved.csv"))
    {
        std::cout
            << "\nCards saved successfully.\n";
    }

    if (system.saveJourneys(
            "data/journeys_saved.csv"))
    {
        std::cout
            << "Journeys saved successfully.\n";
    }

std::cout << "\n=== BAD FILE TEST ===\n";

MetroSystem badFileSystem;

if (badFileSystem.loadCards("data/cards_bad.csv"))
{
    std::cout << "Bad file processed without crashing.\n";
}

std::cout
    << "Valid cards loaded from bad file: "
    << badFileSystem.getRegisteredCardCount()
    << '\n';

std::cout
    << "Blocked cards loaded from bad file: "
    << badFileSystem.getBlockedCardCount()
    << '\n';

MetroSystem missingFileSystem;

if (!missingFileSystem.loadCards(
        "data/file_that_does_not_exist.csv"))
{
    std::cout
        << "Missing file handled correctly.\n";
}



    return 0;
}