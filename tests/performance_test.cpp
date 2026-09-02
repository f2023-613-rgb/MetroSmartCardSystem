#include <iostream>
#include <chrono>
#include <iomanip>

#include "../include/Card.h"
#include "../include/CardHashTable.h"
#include "../include/OperationCounter.h"


void runHashTableTest(int numberOfCards)
{
    std::cout
        << "\n=====================================\n"
        << "Testing with "
        << numberOfCards
        << " cards\n"
        << "=====================================\n";

    CardHashTable table;

    OperationCounter insertionCounter;

    // All generated card numbers have the same first 6 digits.
    const long long baseCardNumber =
        1234560000000000LL;

    // ---------------------------------------------------------
    // INSERT CARDS
    // ---------------------------------------------------------

    auto insertStart =
        std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numberOfCards; i++)
    {
        Card card;

        card.setCardNumber(
            baseCardNumber + i);

        card.setHolderName(
            "Performance Test User");

        card.setCNIC(
            "0000000000000");

        card.setBalance(
            500.0);

        table.insert(
            card,
            insertionCounter);
    }

    auto insertEnd =
        std::chrono::high_resolution_clock::now();

    long long insertMicroseconds =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                insertEnd - insertStart)
            .count();


    // ---------------------------------------------------------
    // SEARCH TEST
    // ---------------------------------------------------------

    const int SEARCH_REPETITIONS = 10000;

    long long targetCard =
        baseCardNumber +
        numberOfCards -
        1;

    long long totalComparisons = 0;
    long long totalSteps = 0;

    int successfulSearches = 0;

    auto searchStart =
        std::chrono::high_resolution_clock::now();

    for (int i = 0;
         i < SEARCH_REPETITIONS;
         i++)
    {
        OperationCounter searchCounter;

        Card* found =
            table.search(
                targetCard,
                searchCounter);

        if (found != nullptr)
        {
            successfulSearches++;
        }

        totalComparisons +=
            searchCounter.getComparisons();

        totalSteps +=
            searchCounter.getSteps();
    }

    auto searchEnd =
        std::chrono::high_resolution_clock::now();

    long long searchMicroseconds =
        std::chrono::duration_cast<
            std::chrono::microseconds>(
                searchEnd - searchStart)
            .count();


    // ---------------------------------------------------------
    // RESULTS
    // ---------------------------------------------------------

    double averageSearchTime =
        static_cast<double>(
            searchMicroseconds)
        / SEARCH_REPETITIONS;

    double averageComparisons =
        static_cast<double>(
            totalComparisons)
        / SEARCH_REPETITIONS;

    double averageSteps =
        static_cast<double>(
            totalSteps)
        / SEARCH_REPETITIONS;


    std::cout
        << std::fixed
        << std::setprecision(4);

    std::cout
        << "Cards stored: "
        << table.getCount()
        << '\n';

    std::cout
        << "Hash table capacity: "
        << table.getCapacity()
        << '\n';

    std::cout
        << "Load factor: "
        << table.getLoadFactor()
        << '\n';

    std::cout
        << "Total insertion time: "
        << insertMicroseconds
        << " microseconds\n";

    std::cout
        << "Search repetitions: "
        << SEARCH_REPETITIONS
        << '\n';

    std::cout
        << "Successful searches: "
        << successfulSearches
        << '\n';

    std::cout
        << "Total search time: "
        << searchMicroseconds
        << " microseconds\n";

    std::cout
        << "Average search time: "
        << averageSearchTime
        << " microseconds\n";

    std::cout
        << "Average comparisons: "
        << averageComparisons
        << '\n';

    std::cout
        << "Average steps: "
        << averageSteps
        << '\n';
}


int main()
{
    std::cout
        << "=====================================\n"
        << " CARD HASH TABLE PERFORMANCE TEST\n"
        << "=====================================\n";

    runHashTableTest(5000);

    runHashTableTest(20000);

    runHashTableTest(50000);

    std::cout
        << "\n=====================================\n"
        << "Performance test complete.\n"
        << "=====================================\n";

    return 0;
}