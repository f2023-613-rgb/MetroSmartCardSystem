#include <iostream>

#include "../include/MetroSystem.h"

int main()
{
    std::cout
        << "=====================================\n"
        << " METRO SYSTEM FINAL FUNCTIONAL TEST\n"
        << "=====================================\n";

    MetroSystem system;
    OperationCounter counter;

    int passed = 0;
    int failed = 0;

    // ---------------------------------------------------------
    // TEST 1 - REGISTER CARD
    // ---------------------------------------------------------
    counter.reset();

    if (system.registerCard(
            1234560000010001LL,
            "Test User One",
            "3520200000001",
            500.0,
            counter))
    {
        std::cout << "[PASS] Register valid card\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Register valid card\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 2 - DUPLICATE CARD
    // ---------------------------------------------------------
    counter.reset();

    if (!system.registerCard(
            1234560000010001LL,
            "Duplicate",
            "3520200000002",
            500.0,
            counter))
    {
        std::cout << "[PASS] Reject duplicate card\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Reject duplicate card\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 3 - INVALID NON-16-DIGIT CARD
    // ---------------------------------------------------------
    counter.reset();

    if (!system.registerCard(
            12345LL,
            "Invalid",
            "3520200000003",
            500.0,
            counter))
    {
        std::cout << "[PASS] Reject invalid card number\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Reject invalid card number\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 4 - CARD LOOKUP
    // ---------------------------------------------------------
    counter.reset();

    Card* card =
        system.findCard(
            1234560000010001LL,
            counter);

    if (card != nullptr)
    {
        std::cout << "[PASS] Find existing card\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Find existing card\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 5 - TOP UP
    // ---------------------------------------------------------
    counter.reset();

    if (system.topUpCard(
            1234560000010001LL,
            100.0,
            "09:00",
            counter))
    {
        card =
            system.findCard(
                1234560000010001LL,
                counter);

        if (card != nullptr &&
            card->getBalance() == 600.0)
        {
            std::cout << "[PASS] Top-up card\n";
            passed++;
        }
        else
        {
            std::cout << "[FAIL] Top-up balance incorrect\n";
            failed++;
        }
    }
    else
    {
        std::cout << "[FAIL] Top-up card\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 6 - UNDO TOP UP
    // ---------------------------------------------------------
    counter.reset();

    if (system.undoLastNTopUps(
            1,
            counter) == 1)
    {
        card =
            system.findCard(
                1234560000010001LL,
                counter);

        if (card != nullptr &&
            card->getBalance() == 500.0)
        {
            std::cout << "[PASS] Undo top-up\n";
            passed++;
        }
        else
        {
            std::cout << "[FAIL] Undo balance incorrect\n";
            failed++;
        }
    }
    else
    {
        std::cout << "[FAIL] Undo top-up\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 7 - TAP IN
    // ---------------------------------------------------------
    counter.reset();

    if (system.tapIn(
            1234560000010001LL,
            2,
            "10:00",
            counter))
    {
        std::cout << "[PASS] Tap in\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Tap in\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 8 - DUPLICATE TAP IN
    // ---------------------------------------------------------
    counter.reset();

    if (!system.tapIn(
            1234560000010001LL,
            3,
            "10:05",
            counter))
    {
        std::cout << "[PASS] Reject duplicate tap-in\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Reject duplicate tap-in\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 9 - BLOCKED TAP OUT
    // ---------------------------------------------------------
    counter.reset();

    system.blockCard(
        1234560000010001LL,
        "10:10",
        counter);

    double fare = 0.0;

    counter.reset();

    if (!system.tapOut(
            1234560000010001LL,
            6,
            "10:30",
            fare,
            counter))
    {
        std::cout << "[PASS] Reject blocked tap-out\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Reject blocked tap-out\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 10 - UNBLOCK AND TAP OUT
    // ---------------------------------------------------------
    counter.reset();

    system.unblockCard(
        1234560000010001LL,
        "10:31",
        counter);

    counter.reset();

    if (system.tapOut(
            1234560000010001LL,
            6,
            "10:32",
            fare,
            counter))
    {
        if (fare == 40.0)
        {
            std::cout
                << "[PASS] Unblock and tap-out with correct fare\n";

            passed++;
        }
        else
        {
            std::cout << "[FAIL] Fare incorrect\n";
            failed++;
        }
    }
    else
    {
        std::cout << "[FAIL] Tap-out after unblock\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 11 - JOURNEY SAVED
    // ---------------------------------------------------------
    card =
        system.findCard(
            1234560000010001LL,
            counter);

    if (card != nullptr &&
        card->getJourneyHistory().getCount() == 1)
    {
        std::cout << "[PASS] Journey stored in history\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Journey history\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 12 - GATE QUEUE FIFO
    // ---------------------------------------------------------

    system.registerCard(
        1234560000010002LL,
        "Test User Two",
        "3520200000004",
        500.0,
        counter);

    counter.reset();

    system.addPassengerToGate(
        1234560000010001LL,
        counter);

    system.addPassengerToGate(
        1234560000010002LL,
        counter);

    long long servedCard = 0;

    system.serveNextPassenger(
        servedCard,
        counter);

    if (servedCard ==
        1234560000010001LL)
    {
        std::cout << "[PASS] Gate queue FIFO\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Gate queue FIFO\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 13 - EMPTY UNDO
    // ---------------------------------------------------------

    MetroSystem emptySystem;

    counter.reset();

    if (emptySystem.undoLastNTopUps(
            5,
            counter) == 0)
    {
        std::cout << "[PASS] Empty undo stack handled\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Empty undo stack\n";
        failed++;
    }

    // ---------------------------------------------------------
    // TEST 14 - EMPTY GATE QUEUE
    // ---------------------------------------------------------

    long long emptyServed = 0;

    counter.reset();

    if (!emptySystem.serveNextPassenger(
            emptyServed,
            counter))
    {
        std::cout << "[PASS] Empty gate queue handled\n";
        passed++;
    }
    else
    {
        std::cout << "[FAIL] Empty gate queue\n";
        failed++;
    }

    // ---------------------------------------------------------
    // FINAL RESULT
    // ---------------------------------------------------------

    std::cout
        << "\n=====================================\n"
        << "Passed: "
        << passed
        << '\n'
        << "Failed: "
        << failed
        << '\n'
        << "=====================================\n";

    if (failed == 0)
    {
        std::cout
            << "ALL FUNCTIONAL TESTS PASSED\n";

        return 0;
    }

    std::cout
        << "SOME TESTS FAILED\n";

    return 1;
}