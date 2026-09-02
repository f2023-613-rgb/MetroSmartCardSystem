#include <iostream>

#include "../include/GateQueue.h"
#include "../include/TopUpStack.h"
#include "../include/TransactionLog.h"
#include "../include/JourneyHistory.h"
#include "../include/CardHashTable.h"
#include "../include/BlockedCardHashTable.h"

int main()
{
    std::cout << "=== MEMORY / RULE OF THREE TEST ===\n";

    OperationCounter counter;

    {
        GateQueue q1;

        q1.enqueue(1234560000000001LL, counter);
        q1.enqueue(1234560000000002LL, counter);

        GateQueue q2 = q1;

        GateQueue q3;
        q3 = q1;

        std::cout << "Queue copies created.\n";
    }

    {
        TopUpStack s1;

        s1.push(1234560000000001LL, 100.0, counter);
        s1.push(1234560000000002LL, 200.0, counter);

        TopUpStack s2 = s1;

        TopUpStack s3;
        s3 = s1;

        std::cout << "Stack copies created.\n";
    }

    {
        TransactionLog log1;

        log1.append(
            TransactionLog::TOP_UP,
            1234560000000001LL,
            100.0,
            -1,
            "10:00",
            counter);

        TransactionLog log2 = log1;

        TransactionLog log3;
        log3 = log1;

        std::cout << "Transaction log copies created.\n";
    }

    {
        JourneyHistory h1;

        Journey journey;

        journey.setEntryStation(1);
        journey.setExitStation(5);
        journey.setEntryTime("11:00");
        journey.setExitTime("11:30");
        journey.setFare(40.0);

        h1.addJourney(
            journey,
            counter);

        JourneyHistory h2 = h1;

        JourneyHistory h3;
        h3 = h1;

        std::cout << "Journey history copies created.\n";
    }

    {
        CardHashTable table1;

        Card card;

        card.setCardNumber(
            1234560000000001LL);

        card.setHolderName(
            "Memory Test");

        card.setCNIC(
            "0000000000000");

        card.setBalance(
            500.0);

        table1.insert(
            card,
            counter);

        CardHashTable table2 = table1;

        CardHashTable table3;
        table3 = table1;

        std::cout << "Card hash table copies created.\n";
    }

    {
        BlockedCardHashTable blocked1;

        blocked1.blockCard(
            1234560000000001LL,
            counter);

        BlockedCardHashTable blocked2 =
            blocked1;

        BlockedCardHashTable blocked3;

        blocked3 =
            blocked1;

        std::cout << "Blocked table copies created.\n";
    }

    std::cout
        << "All test objects left scope successfully.\n";

    std::cout
        << "No crash or double-delete detected during Rule of Three test.\n";

    return 0;
}