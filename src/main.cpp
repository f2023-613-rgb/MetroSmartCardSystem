#include <iostream>
#include <limits>
#include <chrono>

#include "../include/MetroSystem.h"
#include "../include/CommandProcessor.h"

void clearInput()
{
    std::cin.clear();

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n');
}


long long getElapsedMicroseconds(
    const std::chrono::high_resolution_clock::time_point& start,
    const std::chrono::high_resolution_clock::time_point& end)
{
    return std::chrono::duration_cast<
        std::chrono::microseconds>(
            end - start)
        .count();
}


void printOperationStats(
    const OperationCounter& counter,
    long long microseconds)
{
    std::cout
        << "Time: "
        << microseconds
        << " microseconds\n";

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


int main(int argc, char* argv[])
{
    MetroSystem system;

    std::cout
        << "=====================================\n"
        << "       METRO SMART CARD SYSTEM\n"
        << "=====================================\n\n";

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
            << "cards.csv not found. "
            << "Starting with empty cards.\n";
    }

    if (system.loadJourneys("data/journeys.csv"))
    {
        std::cout
            << "Journeys loaded successfully.\n";
    }
    else
    {
        std::cout
            << "journeys.csv not found. "
            << "Starting with empty journeys.\n";
    }

    if (argc > 1)
{
    std::cout
        << "\nRunning command-file mode.\n";

    CommandProcessor processor(system);

    bool success =
        processor.processFile(argv[1]);

    system.saveCards(
        "data/cards.csv");

    system.saveJourneys(
        "data/journeys.csv");

    if (!success)
    {
        return 1;
    }

    return 0;
}

    std::cout
        << "\nRegistered cards: "
        << system.getRegisteredCardCount()
        << '\n';

    std::cout
        << "Blocked cards: "
        << system.getBlockedCardCount()
        << '\n';

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
            // =====================================================
            // 1. REGISTER CARD
            // =====================================================
            case 1:
            {
                long long cardNumber;
                char holderName[100];
                char cnic[20];
                double balance;

                std::cout
                    << "Enter 16-digit card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

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

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid balance.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.registerCard(
                        cardNumber,
                        holderName,
                        cnic,
                        balance,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Card registered successfully.\n";
                }
                else
                {
                    std::cout
                        << "Card registration failed.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 2. FIND / SHOW CARD
            // =====================================================
            case 2:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                Card* card =
                    system.findCard(
                        cardNumber,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (card == nullptr)
                {
                    std::cout
                        << "Card not found.\n";
                }
                else
                {
                    std::cout
                        << "\n--- CARD DETAILS ---\n";

                    std::cout
                        << "Card Number: "
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

                    if (card->hasOpenJourney())
                    {
                        std::cout
                            << "Entry Station: "
                            << card->getEntryStation()
                            << '\n';

                        std::cout
                            << "Entry Time: "
                            << card->getEntryTime()
                            << '\n';
                    }

                    std::cout
                        << "Journey Count: "
                        << card
                               ->getJourneyHistory()
                               .getCount()
                        << '\n';
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 3. TOP UP CARD
            // =====================================================
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

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid input.\n";

                    break;
                }

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.topUpCard(
                        cardNumber,
                        amount,
                        time,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Top-up successful.\n";
                }
                else
                {
                    std::cout
                        << "Top-up failed.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 4. UNDO LAST TOP-UP
            // =====================================================
            case 4:
            {
                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.undoLastTopUp(
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Last top-up undone successfully.\n";
                }
                else
                {
                    std::cout
                        << "No top-up could be undone.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 5. BLOCK CARD
            // =====================================================
            case 5:
            {
                long long cardNumber;
                char time[30];

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.blockCard(
                        cardNumber,
                        time,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Card blocked successfully.\n";

                    std::cout
                        << "Total blocked cards: "
                        << system.getBlockedCardCount()
                        << '\n';
                }
                else
                {
                    std::cout
                        << "Could not block card.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 6. UNBLOCK CARD
            // =====================================================
            case 6:
            {
                long long cardNumber;
                char time[30];

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.unblockCard(
                        cardNumber,
                        time,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Card unblocked successfully.\n";

                    std::cout
                        << "Total blocked cards: "
                        << system.getBlockedCardCount()
                        << '\n';
                }
                else
                {
                    std::cout
                        << "Could not unblock card.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 7. TAP IN
            // =====================================================
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

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid input.\n";

                    break;
                }

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.tapIn(
                        cardNumber,
                        stationId,
                        time,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Tap-in successful.\n";
                }
                else
                {
                    std::cout
                        << "Tap-in failed.\n";

                    std::cout
                        << "Possible reasons: card missing, "
                        << "inactive, blocked, low balance, "
                        << "invalid station, or journey already open.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 8. TAP OUT
            // =====================================================
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

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid input.\n";

                    break;
                }

                clearInput();

                std::cout
                    << "Enter time: ";

                std::cin.getline(
                    time,
                    sizeof(time));

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.tapOut(
                        cardNumber,
                        stationId,
                        time,
                        fare,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
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

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 9. ADD PASSENGER TO GATE QUEUE
            // =====================================================
            case 9:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.addPassengerToGate(
                        cardNumber,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
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

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 10. SERVE NEXT PASSENGER
            // =====================================================
            case 10:
            {
                long long servedCard = 0;

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.serveNextPassenger(
                        servedCard,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
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

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 11. SHOW CURRENT JOURNEY
            // =====================================================
            case 11:
            {
                long long cardNumber;
                Journey journey;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.getCurrentJourney(
                        cardNumber,
                        journey,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "\n--- CURRENT JOURNEY ---\n";

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

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 12. MOVE JOURNEY PREVIOUS
            // =====================================================
            case 12:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.moveJourneyPrevious(
                        cardNumber,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Moved to previous journey.\n";
                }
                else
                {
                    std::cout
                        << "Cannot move to previous journey.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 13. MOVE JOURNEY NEXT
            // =====================================================
            case 13:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.moveJourneyNext(
                        cardNumber,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Moved to next journey.\n";
                }
                else
                {
                    std::cout
                        << "Cannot move to next journey.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 14. DELETE CURRENT JOURNEY
            // =====================================================
            case 14:
            {
                long long cardNumber;

                std::cout
                    << "Enter card number: ";

                std::cin >> cardNumber;

                if (std::cin.fail())
                {
                    clearInput();

                    std::cout
                        << "Invalid card number.\n";

                    break;
                }

                auto start =
                    std::chrono::high_resolution_clock::now();

                bool success =
                    system.deleteCurrentJourney(
                        cardNumber,
                        counter);

                auto end =
                    std::chrono::high_resolution_clock::now();

                if (success)
                {
                    std::cout
                        << "Current journey deleted.\n";
                }
                else
                {
                    std::cout
                        << "No journey deleted.\n";
                }

                printOperationStats(
                    counter,
                    getElapsedMicroseconds(
                        start,
                        end));

                break;
            }


            // =====================================================
            // 15. REPLAY DAILY TRANSACTIONS
            // =====================================================
            case 15:
            {
                std::cout
                    << "\n=== DAILY TRANSACTION REPLAY ===\n";

                auto start =
                    std::chrono::high_resolution_clock::now();

                system.replayTransactions();

                auto end =
                    std::chrono::high_resolution_clock::now();

                std::cout
                    << "Replay time: "
                    << getElapsedMicroseconds(
                        start,
                        end)
                    << " microseconds\n";

                break;
            }


            // =====================================================
            // 16. SAVE DATA
            // =====================================================
            case 16:
            {
                auto start =
                    std::chrono::high_resolution_clock::now();

                bool cardsSaved =
                    system.saveCards(
                        "data/cards.csv");

                bool journeysSaved =
                    system.saveJourneys(
                        "data/journeys.csv");

                auto end =
                    std::chrono::high_resolution_clock::now();

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

                std::cout
                    << "Save time: "
                    << getElapsedMicroseconds(
                        start,
                        end)
                    << " microseconds\n";

                break;
            }


            // =====================================================
            // 0. EXIT
            // =====================================================
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


            // =====================================================
            // INVALID MENU CHOICE
            // =====================================================
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