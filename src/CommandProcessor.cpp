#include "../include/CommandProcessor.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>

CommandProcessor::CommandProcessor(
    MetroSystem& metroSystem)
    : system(metroSystem)
{
}


bool CommandProcessor::processFile(
    const char* filename)
{
    std::ifstream input(filename);

    if (!input.is_open())
    {
        std::cout
            << "Could not open command file: "
            << filename
            << '\n';

        return false;
    }

    std::string line;

    int lineNumber = 0;

    while (std::getline(input, line))
    {
        lineNumber++;

        if (line.empty())
        {
            continue;
        }

        // Allow comment lines.
        if (line[0] == '#')
        {
            continue;
        }

        std::cout
            << "Command "
            << lineNumber
            << ": ";

        try
        {
            processLine(line.c_str());
        }
        catch (...)
        {
            std::cout
                << "ERROR - invalid command data\n";
        }
    }

    input.close();

    return true;
}


bool CommandProcessor::processLine(
    const char* rawLine)
{
    if (rawLine == nullptr)
    {
        std::cout
            << "ERROR - empty command\n";

        return false;
    }

    std::string line(rawLine);

    std::stringstream stream(line);

    std::string command;

    std::getline(
        stream,
        command,
        '|');

    OperationCounter counter;

    auto start =
        std::chrono::high_resolution_clock::now();


    // =========================================================
    // REGISTER
    // =========================================================
    if (command == "REGISTER")
    {
        std::string cardText;
        std::string holderName;
        std::string cnic;
        std::string balanceText;

        std::getline(stream, cardText, '|');
        std::getline(stream, holderName, '|');
        std::getline(stream, cnic, '|');
        std::getline(stream, balanceText);

        long long cardNumber =
            std::stoll(cardText);

        double balance =
            std::stod(balanceText);

        bool success =
            system.registerCard(
                cardNumber,
                holderName.c_str(),
                cnic.c_str(),
                balance,
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        std::cout
            << (success
                    ? "REGISTER OK"
                    : "REGISTER FAILED")
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // FIND
    // =========================================================
    if (command == "FIND")
    {
        std::string cardText;

        std::getline(
            stream,
            cardText);

        long long cardNumber =
            std::stoll(cardText);

        Card* card =
            system.findCard(
                cardNumber,
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        if (card != nullptr)
        {
            std::cout
                << "FOUND "
                << card->getCardNumber()
                << " "
                << card->getHolderName();
        }
        else
        {
            std::cout
                << "NOT FOUND";
        }

        std::cout
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return card != nullptr;
    }


    // =========================================================
    // TOPUP
    // =========================================================
    if (command == "TOPUP")
    {
        std::string cardText;
        std::string amountText;
        std::string time;

        std::getline(stream, cardText, '|');
        std::getline(stream, amountText, '|');
        std::getline(stream, time);

        long long cardNumber =
            std::stoll(cardText);

        double amount =
            std::stod(amountText);

        bool success =
            system.topUpCard(
                cardNumber,
                amount,
                time.c_str(),
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        std::cout
            << (success
                    ? "TOPUP OK"
                    : "TOPUP FAILED")
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


 // =========================================================
// UNDO
// Format: UNDO|N
// =========================================================
if (command == "UNDO")
{
    std::string countText;

    std::getline(
        stream,
        countText);

    int n = 1;

    if (!countText.empty())
    {
        n = std::stoi(countText);
    }

    int undone =
        system.undoLastNTopUps(
            n,
            counter);

    auto end =
        std::chrono::high_resolution_clock::now();

    long long microseconds =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                end - start)
            .count();

    if (undone > 0)
    {
        std::cout
            << "UNDO "
            << undone
            << "/"
            << n
            << " OK";
    }
    else
    {
        std::cout
            << "UNDO 0/"
            << n
            << " FAILED";
    }

    std::cout
        << " | Time: "
        << microseconds
        << " us"
        << " | Steps: "
        << counter.getSteps()
        << " | Comparisons: "
        << counter.getComparisons()
        << '\n';

    return undone > 0;
}
    // =========================================================
    // BLOCK
    // =========================================================
    if (command == "BLOCK")
    {
        std::string cardText;
        std::string time;

        std::getline(stream, cardText, '|');
        std::getline(stream, time);

        long long cardNumber =
            std::stoll(cardText);

        bool success =
            system.blockCard(
                cardNumber,
                time.c_str(),
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        std::cout
            << (success
                    ? "BLOCK OK"
                    : "BLOCK FAILED")
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // UNBLOCK
    // =========================================================
    if (command == "UNBLOCK")
    {
        std::string cardText;
        std::string time;

        std::getline(stream, cardText, '|');
        std::getline(stream, time);

        long long cardNumber =
            std::stoll(cardText);

        bool success =
            system.unblockCard(
                cardNumber,
                time.c_str(),
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        std::cout
            << (success
                    ? "UNBLOCK OK"
                    : "UNBLOCK FAILED")
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // TAPIN
    // =========================================================
    if (command == "TAPIN")
    {
        std::string cardText;
        std::string stationText;
        std::string time;

        std::getline(stream, cardText, '|');
        std::getline(stream, stationText, '|');
        std::getline(stream, time);

        long long cardNumber =
            std::stoll(cardText);

        int stationId =
            std::stoi(stationText);

        bool success =
            system.tapIn(
                cardNumber,
                stationId,
                time.c_str(),
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        std::cout
            << (success
                    ? "TAPIN OK"
                    : "TAPIN FAILED")
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // TAPOUT
    // =========================================================
    if (command == "TAPOUT")
    {
        std::string cardText;
        std::string stationText;
        std::string time;

        std::getline(stream, cardText, '|');
        std::getline(stream, stationText, '|');
        std::getline(stream, time);

        long long cardNumber =
            std::stoll(cardText);

        int stationId =
            std::stoi(stationText);

        double fare = 0.0;

        bool success =
            system.tapOut(
                cardNumber,
                stationId,
                time.c_str(),
                fare,
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        if (success)
        {
            std::cout
                << "TAPOUT OK"
                << " | Fare: "
                << fare;
        }
        else
        {
            std::cout
                << "TAPOUT FAILED";
        }

        std::cout
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // ENQUEUE
    // =========================================================
    if (command == "ENQUEUE")
    {
        std::string cardText;

        std::getline(
            stream,
            cardText);

        long long cardNumber =
            std::stoll(cardText);

        bool success =
            system.addPassengerToGate(
                cardNumber,
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        std::cout
            << (success
                    ? "ENQUEUE OK"
                    : "ENQUEUE FAILED")
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // SERVE
    // =========================================================
    if (command == "SERVE")
    {
        long long cardNumber = 0;

        bool success =
            system.serveNextPassenger(
                cardNumber,
                counter);

        auto end =
            std::chrono::high_resolution_clock::now();

        long long microseconds =
            std::chrono::duration_cast<
                std::chrono::microseconds>(
                    end - start)
                .count();

        if (success)
        {
            std::cout
                << "SERVE OK"
                << " | Card: "
                << cardNumber;
        }
        else
        {
            std::cout
                << "SERVE FAILED";
        }

        std::cout
            << " | Time: "
            << microseconds
            << " us"
            << " | Steps: "
            << counter.getSteps()
            << " | Comparisons: "
            << counter.getComparisons()
            << '\n';

        return success;
    }


    // =========================================================
    // REPLAY
    // =========================================================
    if (command == "REPLAY")
    {
        std::cout
            << "REPLAY\n";

        system.replayTransactions();

        return true;
    }


    // =========================================================
    // SAVE
    // =========================================================
    if (command == "SAVE")
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
                << "SAVE OK\n";

            return true;
        }

        std::cout
            << "SAVE FAILED\n";

        return false;
    }


    std::cout
        << "UNKNOWN COMMAND\n";

    return false;
}