#include <iostream>
#include <limits>

#include "../include/MetroSystem.h"

void clearInput()
{
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n');
}

void printOperationStats(
    const OperationCounter& counter)
{
    std::cout
        << "Steps: "
        << counter.getSteps()
        << '\n';

    std::cout
        << "Comparisons: "
        << counter.getComparisons()
        << '\n';
}

void showMenu()
{
    std::cout
        << "\n=====================================\n"
        << "       METRO SMART CARD SYSTEM\n"
        << "=====================================\n"
        << "1. Register card\n"
        << "2. Find/show card\n"
        << "3. Top up card\n"
        << "4. Undo last top-up\n"
        << "5. Block card\n"
        << "6. Unblock card\n"
        << "7. Tap in\n"
        << "8. Tap out\n"
        << "9. Add passenger to gate queue\n"
        << "10. Serve next passenger\n"
        << "11. Show current journey history record\n"
        << "12. Move journey previous\n"
        << "13. Move journey next\n"
        << "14. Delete current journey\n"
        << "15. Replay today's transactions\n"
        << "16. Save data\n"
        << "0. Exit\n"
        << "=====================================\n"
        << "Enter choice: ";
}

int main()
{
    MetroSystem system;

    std::cout
        << "Loading data...\n";

    if (system.loadCards("data/cards.csv"))
    {
        std::cout
            << "Cards loaded successfully.\n";
    }
    else
    {
        std::cout
            << "cards.csv not found. Starting with empty cards.\n";
    }

    if (system.loadJourneys("data/journeys.csv"))
    {
        std::cout
            << "Journeys loaded successfully.\n";
    }
    else
    {
        std::cout
            << "journeys.csv not found. Starting with empty journeys.\n";
    }

    int choice = -1;

    do
    {
        showMenu();

        std::cin >> choice;

        if (std::cin.fail())
        {
            clearInput();

            std::cout
                << "Invalid menu input.\n";

            continue;
        }

        OperationCounter counter;

        switch (choice)
        {
            case 1:
            {
                long long cardNumber;
                char holderName[100];
                char cnic[20];
                double balance;

                std::cout
                    << "Enter 16-digit card number: ";

                std::cin >> cardNumber;

                clearInput();

                std::cout
                    << "Enter holder name: ";

                std::cin.getline(
                    holderName,
                    sizeof(holderName));

                std::cout
                    << "Enter CNIC: ";

                std::cin.getline(
                    cnic,
                    sizeof(cnic));

                std::cout
                    << "Enter opening balance: ";

                std::cin >> balance;

                if (system.registerCard(
                        cardNumber,
                        holderName,
                        cnic,
                        balance,
                        counter))
                {
                    std::cout
                        << "Card registered successfully.\n";
                }
                else
                {
                    std::cout
                        << "Card registration failed.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 2:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                Card* card =
                    system.findCard(
                        cardNumber,
                        counter);

                if (card == nullptr)
                {
                    std::cout
                        << "Card not found.\n";
                }
                else
                {
                    std::cout
                        << "\nCard Number: "
                        << card->getCardNumber()
                        << '\n';

                    std::cout
                        << "Holder: "
                        << card->getHolderName()
                        << '\n';

                    std::cout
                        << "CNIC: "
                        << card->getCNIC()
                        << '\n';

                    std::cout
                        << "Balance: "
                        << card->getBalance()
                        << '\n';

                    std::cout
                        << "Blocked: "
                        << (card->isBlocked()
                            ? "Yes"
                            : "No")
                        << '\n';

                    std::cout
                        << "Active: "
                        << (card->isActive()
                            ? "Yes"
                            : "No")
                        << '\n';

                    std::cout
                        << "Open Journey: "
                        << (card->hasOpenJourney()
                            ? "Yes"
                            : "No")
                        << '\n';

                    std::cout
                        << "Journey Count: "
                        << card
                               ->getJourneyHistory()
                               .getCount()
                        << '\n';
                }

                printOperationStats(counter);

                break;
            }

            case 3:
            {
                long long cardNumber;
                double amount;
                char time[30];

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                std::cout
                    << "Enter top-up amount: ";

                std::cin >> amount;

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                if (system.topUpCard(
                        cardNumber,
                        amount,
                        time,
                        counter))
                {
                    std::cout
                        << "Top-up successful.\n";
                }
                else
                {
                    std::cout
                        << "Top-up failed.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 4:
            {
                if (system.undoLastTopUp(
                        counter))
                {
                    std::cout
                        << "Last top-up undone successfully.\n";
                }
                else
                {
                    std::cout
                        << "No top-up could be undone.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 5:
            {
                long long cardNumber;
                char time[30];

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                if (system.blockCard(
                        cardNumber,
                        time,
                        counter))
                {
                    std::cout
                        << "Card blocked successfully.\n";
                }
                else
                {
                    std::cout
                        << "Could not block card.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 6:
            {
                long long cardNumber;
                char time[30];

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                if (system.unblockCard(
                        cardNumber,
                        time,
                        counter))
                {
                    std::cout
                        << "Card unblocked successfully.\n";
                }
                else
                {
                    std::cout
                        << "Could not unblock card.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 7:
            {
                long long cardNumber;
                int stationId;
                char time[30];

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                std::cout
                    << "Enter station ID (0-29): ";

                std::cin >> stationId;

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                if (system.tapIn(
                        cardNumber,
                        stationId,
                        time,
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

                printOperationStats(counter);

                break;
            }

            case 8:
            {
                long long cardNumber;
                int stationId;
                char time[30];
                double fare = 0.0;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                std::cout
                    << "Enter exit station ID (0-29): ";

                std::cin >> stationId;

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                if (system.tapOut(
                        cardNumber,
                        stationId,
                        time,
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

                printOperationStats(counter);

                break;
            }

            case 9:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (system.addPassengerToGate(
                        cardNumber,
                        counter))
                {
                    std::cout
                        << "Passenger added to gate queue.\n";

                    std::cout
                        << "Passengers waiting: "
                        << system.getGateQueueCount()
                        << '\n';
                }
                else
                {
                    std::cout
                        << "Could not add passenger.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 10:
            {
                long long servedCard = 0;

                if (system.serveNextPassenger(
                        servedCard,
                        counter))
                {
                    std::cout
                        << "Served card: "
                        << servedCard
                        << '\n';

                    std::cout
                        << "Passengers remaining: "
                        << system.getGateQueueCount()
                        << '\n';
                }
                else
                {
                    std::cout
                        << "Gate queue is empty.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 11:
            {
                long long cardNumber;
                Journey journey;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (system.getCurrentJourney(
                        cardNumber,
                        journey,
                        counter))
                {
                    std::cout
                        << "Entry Station: "
                        << journey.getEntryStation()
                        << '\n';

                    std::cout
                        << "Exit Station: "
                        << journey.getExitStation()
                        << '\n';

                    std::cout
                        << "Entry Time: "
                        << journey.getEntryTime()
                        << '\n';

                    std::cout
                        << "Exit Time: "
                        << journey.getExitTime()
                        << '\n';

                    std::cout
                        << "Fare: "
                        << journey.getFare()
                        << '\n';
                }
                else
                {
                    std::cout
                        << "No current journey available.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 12:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (system.moveJourneyPrevious(
                        cardNumber,
                        counter))
                {
                    std::cout
                        << "Moved to previous journey.\n";
                }
                else
                {
                    std::cout
                        << "Cannot move to previous journey.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 13:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (system.moveJourneyNext(
                        cardNumber,
                        counter))
                {
                    std::cout
                        << "Moved to next journey.\n";
                }
                else
                {
                    std::cout
                        << "Cannot move to next journey.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 14:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (system.deleteCurrentJourney(
                        cardNumber,
                        counter))
                {
                    std::cout
                        << "Current journey deleted.\n";
                }
                else
                {
                    std::cout
                        << "No journey deleted.\n";
                }

                printOperationStats(counter);

                break;
            }

            case 15:
            {
                std::cout
                    << "\n=== DAILY TRANSACTION REPLAY ===\n";

                system.replayTransactions();

                break;
            }

            case 16:
            {
                bool cardsSaved =
                    system.saveCards(
                        "data/cards.csv");

                bool journeysSaved =
                    system.saveJourneys(
                        "data/journeys.csv");

                if (cardsSaved &&
                    journeysSaved)
                {
                    std::cout
                        << "Data saved successfully.\n";
                }
                else
                {
                    std::cout
                        << "Error while saving data.\n";
                }

                break;
            }

            case 0:
            {
                std::cout
                    << "Saving before exit...\n";

                bool cardsSaved =
                    system.saveCards(
                        "data/cards.csv");

                bool journeysSaved =
                    system.saveJourneys(
                        "data/journeys.csv");

                if (cardsSaved &&
                    journeysSaved)
                {
                    std::cout
                        << "Data saved successfully.\n";
                }
                else
                {
                    std::cout
                        << "Warning: data could not be fully saved.\n";
                }

                std::cout
                    << "Goodbye.\n";

                break;
            }

            default:
            {
                std::cout
                    << "Invalid menu choice.\n";

                break;
            }
        }

    } while (choice != 0);

    return 0;
}