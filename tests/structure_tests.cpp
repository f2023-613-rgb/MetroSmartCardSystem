#include <iostream>
#include "../include/Card.h"
#include "../include/Journey.h"
#include "../include/Station.h"
#include "../include/OperationCounter.h"
#include "../include/GateQueue.h"
#include "../include/TopUpStack.h"
#include "../include/TransactionLog.h"
#include "../include/JourneyHistory.h"
#include "../include/CardHashTable.h"
#include "../include/BlockedCardHashTable.h"
int main()
{
    Card card;

    card.setCardNumber(1234560000000001LL);
    card.setHolderName("Ali Khan");
    card.setCNIC("3520212345671");
    card.setBalance(500.0);

    Journey journey;

    journey.setEntryStation(3);
    journey.setExitStation(8);
    journey.setEntryTime("08:15");
    journey.setExitTime("08:42");
    journey.setFare(45.0);

    Station station;

    station.setStationId(3);
    station.setStationCode("ST03");
    station.setStationName("Central Station");

    std::cout << "=== CARD ===\n";
    std::cout << "Card Number: " << card.getCardNumber() << '\n';
    std::cout << "Holder Name: " << card.getHolderName() << '\n';
    std::cout << "Balance: " << card.getBalance() << '\n';

    std::cout << "\n=== JOURNEY ===\n";
    std::cout << "Entry Station: " << journey.getEntryStation() << '\n';
    std::cout << "Exit Station: " << journey.getExitStation() << '\n';
    std::cout << "Entry Time: " << journey.getEntryTime() << '\n';
    std::cout << "Exit Time: " << journey.getExitTime() << '\n';
    std::cout << "Fare: " << journey.getFare() << '\n';

    std::cout << "\n=== STATION ===\n";
    std::cout << "Station ID: " << station.getStationId() << '\n';
    std::cout << "Station Code: " << station.getStationCode() << '\n';
    std::cout << "Station Name: " << station.getStationName() << '\n';

    OperationCounter counter;

    counter.incrementSteps();
    counter.incrementSteps();

    counter.incrementComparisons();

    std::cout << "\n=== COUNTER TEST ===\n";
    std::cout << "Steps: " << counter.getSteps() << '\n';
    std::cout << "Comparisons: " << counter.getComparisons() << '\n';

    std::cout << "\n=== GATE QUEUE TEST ===\n";

GateQueue gateQueue;
OperationCounter queueCounter;

gateQueue.enqueue(1234560000000001LL, queueCounter);
gateQueue.enqueue(1234560000000002LL, queueCounter);
gateQueue.enqueue(1234560000000003LL, queueCounter);

std::cout << "Passengers waiting: "
          << gateQueue.getCount()
          << '\n';

long long frontCard = 0;

if (gateQueue.peek(frontCard))
{
    std::cout << "Front card: "
              << frontCard
              << '\n';
}

long long removedCard = 0;

if (gateQueue.dequeue(removedCard, queueCounter))
{
    std::cout << "Served card: "
              << removedCard
              << '\n';
}

if (gateQueue.peek(frontCard))
{
    std::cout << "New front card: "
              << frontCard
              << '\n';
}

std::cout << "Passengers remaining: "
          << gateQueue.getCount()
          << '\n';

std::cout << "Queue steps: "
          << queueCounter.getSteps()
          << '\n';

std::cout << "Queue comparisons: "
          << queueCounter.getComparisons()
          << '\n';

GateQueue emptyQueue;
OperationCounter emptyCounter;

long long emptyRemoved = 0;

if (!emptyQueue.dequeue(emptyRemoved, emptyCounter))
{
    std::cout << "Empty queue handled correctly.\n";
}

GateQueue copiedQueue = gateQueue;

std::cout << "Copied queue count: "
          << copiedQueue.getCount()
          << '\n';

GateQueue assignedQueue;

assignedQueue = gateQueue;

std::cout << "Assigned queue count: "
          << assignedQueue.getCount()
          << '\n';          


std::cout << "\n=== TOP-UP STACK TEST ===\n";

TopUpStack undoStack;
OperationCounter stackCounter;

undoStack.push(1234560000000001LL, 100.0, stackCounter);
undoStack.push(1234560000000002LL, 200.0, stackCounter);
undoStack.push(1234560000000003LL, 300.0, stackCounter);

std::cout << "Transactions stored: "
          << undoStack.getCount()
          << '\n';

long long topCard = 0;
double topAmount = 0.0;

if (undoStack.peek(topCard, topAmount))
{
    std::cout << "Latest top-up card: "
              << topCard
              << '\n';

    std::cout << "Latest top-up amount: "
              << topAmount
              << '\n';
}

long long undoCard = 0;
double undoAmount = 0.0;

if (undoStack.pop(undoCard,
                  undoAmount,
                  stackCounter))
{
    std::cout << "Undo card: "
              << undoCard
              << '\n';

    std::cout << "Undo amount: "
              << undoAmount
              << '\n';
}

std::cout << "Transactions remaining: "
          << undoStack.getCount()
          << '\n';

std::cout << "Stack steps: "
          << stackCounter.getSteps()
          << '\n';

std::cout << "Stack comparisons: "
          << stackCounter.getComparisons()
          << '\n';

TopUpStack emptyStack;
OperationCounter emptyStackCounter;

long long emptyCard = 0;
double emptyAmount = 0.0;

if (!emptyStack.pop(emptyCard,
                    emptyAmount,
                    emptyStackCounter))
{
    std::cout << "Empty stack handled correctly.\n";
}

TopUpStack copiedStack = undoStack;

std::cout << "Copied stack count: "
          << copiedStack.getCount()
          << '\n';

TopUpStack assignedStack;

assignedStack = undoStack;

std::cout << "Assigned stack count: "
          << assignedStack.getCount()
          << '\n';

std::cout
    << "\n=== TRANSACTION LOG TEST ===\n";

TransactionLog transactionLog;
OperationCounter logCounter;

transactionLog.append(
    TransactionLog::REGISTER_CARD,
    1234560000000001LL,
    0.0,
    -1,
    "08:00",
    logCounter);

transactionLog.append(
    TransactionLog::TOP_UP,
    1234560000000001LL,
    200.0,
    -1,
    "08:05",
    logCounter);

transactionLog.append(
    TransactionLog::TAP_IN,
    1234560000000001LL,
    0.0,
    3,
    "08:15",
    logCounter);

transactionLog.append(
    TransactionLog::TAP_OUT,
    1234560000000001LL,
    45.0,
    8,
    "08:42",
    logCounter);

std::cout
    << "Transactions today: "
    << transactionLog.getCount()
    << '\n';

std::cout
    << "\nEnd-of-day replay:\n";

transactionLog.replay();

std::cout
    << "Log steps: "
    << logCounter.getSteps()
    << '\n';

std::cout
    << "Log comparisons: "
    << logCounter.getComparisons()
    << '\n';

TransactionLog copiedLog =
    transactionLog;

std::cout
    << "\nCopied log transactions: "
    << copiedLog.getCount()
    << '\n';

TransactionLog assignedLog;

assignedLog = transactionLog;

std::cout
    << "Assigned log transactions: "
    << assignedLog.getCount()
    << '\n';

std::cout << "\n=== JOURNEY HISTORY TEST ===\n";

JourneyHistory history;
OperationCounter historyCounter;

Journey j1;
j1.setEntryStation(1);
j1.setExitStation(4);
j1.setEntryTime("09:00");
j1.setExitTime("09:20");
j1.setFare(30.0);

Journey j2;
j2.setEntryStation(4);
j2.setExitStation(7);
j2.setEntryTime("10:00");
j2.setExitTime("10:25");
j2.setFare(35.0);

Journey j3;
j3.setEntryStation(7);
j3.setExitStation(10);
j3.setEntryTime("11:00");
j3.setExitTime("11:30");
j3.setFare(40.0);

history.addJourney(j1, historyCounter);
history.addJourney(j2, historyCounter);
history.addJourney(j3, historyCounter);

std::cout << "Journey count: "
          << history.getCount()
          << '\n';

Journey currentJourney;

if (history.getCurrentJourney(currentJourney))
{
    std::cout << "Current journey fare: "
              << currentJourney.getFare()
              << '\n';
}

if (history.movePrevious(historyCounter))
{
    history.getCurrentJourney(currentJourney);

    std::cout << "After moving previous, fare: "
              << currentJourney.getFare()
              << '\n';
}

Journey middleJourney;
middleJourney.setEntryStation(5);
middleJourney.setExitStation(6);
middleJourney.setEntryTime("10:10");
middleJourney.setExitTime("10:15");
middleJourney.setFare(15.0);

history.insertAfterCurrent(
    middleJourney,
    historyCounter);

history.getCurrentJourney(currentJourney);

std::cout << "Inserted journey fare: "
          << currentJourney.getFare()
          << '\n';

std::cout << "Journey count after insert: "
          << history.getCount()
          << '\n';

history.deleteCurrent(historyCounter);

std::cout << "Journey count after delete: "
          << history.getCount()
          << '\n';

if (history.getCurrentJourney(currentJourney))
{
    std::cout << "Current fare after delete: "
              << currentJourney.getFare()
              << '\n';
}

std::cout << "History steps: "
          << historyCounter.getSteps()
          << '\n';

std::cout << "History comparisons: "
          << historyCounter.getComparisons()
          << '\n';

JourneyHistory limitHistory;
OperationCounter limitCounter;

for (int i = 0; i < 25; i++)
{
    Journey testJourney;

    testJourney.setEntryStation(i % 30);
    testJourney.setExitStation((i + 1) % 30);
    testJourney.setFare(10.0 + i);

    limitHistory.addJourney(
        testJourney,
        limitCounter);
}

std::cout << "History after adding 25 journeys: "
          << limitHistory.getCount()
          << '\n';

JourneyHistory copiedHistory = history;

std::cout << "Copied history count: "
          << copiedHistory.getCount()
          << '\n';

JourneyHistory assignedHistory;

assignedHistory = history;

std::cout << "Assigned history count: "
          << assignedHistory.getCount()
          << '\n';

std::cout
    << "\n=== CARD HASH TABLE TEST ===\n";

CardHashTable cardTable;
OperationCounter hashCounter;

Card c1;
c1.setCardNumber(
    1234560000000001LL);
c1.setHolderName("Ali");
c1.setCNIC("3520211111111");
c1.setBalance(500.0);

Card c2;
c2.setCardNumber(
    1234560000000002LL);
c2.setHolderName("Ahmed");
c2.setCNIC("3520222222222");
c2.setBalance(700.0);

Card c3;
c3.setCardNumber(
    1234560000000003LL);
c3.setHolderName("Sara");
c3.setCNIC("3520233333333");
c3.setBalance(900.0);

cardTable.insert(
    c1,
    hashCounter);

cardTable.insert(
    c2,
    hashCounter);

cardTable.insert(
    c3,
    hashCounter);

std::cout
    << "Cards stored: "
    << cardTable.getCount()
    << '\n';

std::cout
    << "Table capacity: "
    << cardTable.getCapacity()
    << '\n';

std::cout
    << "Load factor: "
    << cardTable.getLoadFactor()
    << '\n';

OperationCounter searchCounter;

Card* foundCard =
    cardTable.search(
        1234560000000002LL,
        searchCounter);

if (foundCard != nullptr)
{
    std::cout
        << "Found card holder: "
        << foundCard->getHolderName()
        << '\n';

    std::cout
        << "Found balance: "
        << foundCard->getBalance()
        << '\n';
}
else
{
    std::cout
        << "Card not found.\n";
}

std::cout
    << "Search steps: "
    << searchCounter.getSteps()
    << '\n';

std::cout
    << "Search comparisons: "
    << searchCounter.getComparisons()
    << '\n';

OperationCounter missingCounter;

Card* missing =
    cardTable.search(
        1234569999999999LL,
        missingCounter);

if (missing == nullptr)
{
    std::cout
        << "Missing card handled correctly.\n";
}

OperationCounter duplicateCounter;

if (!cardTable.insert(
        c2,
        duplicateCounter))
{
    std::cout
        << "Duplicate card rejected correctly.\n";
}

OperationCounter modifyCounter;

Card* cardToModify =
    cardTable.search(
        1234560000000001LL,
        modifyCounter);

if (cardToModify != nullptr)
{
    cardToModify->addBalance(100.0);

    std::cout
        << "New balance after top-up: "
        << cardToModify->getBalance()
        << '\n';
}

OperationCounter removeCounter;

if (cardTable.remove(
        1234560000000003LL,
        removeCounter))
{
    std::cout
        << "Card removed successfully.\n";
}

std::cout
    << "Cards after removal: "
    << cardTable.getCount()
    << '\n';

CardHashTable copiedTable =
    cardTable;

std::cout
    << "Copied table count: "
    << copiedTable.getCount()
    << '\n';

CardHashTable assignedTable;

assignedTable =
    cardTable;

std::cout
    << "Assigned table count: "
    << assignedTable.getCount()
    << '\n';

std::cout
    << "\n=== HASH TABLE SCALE TEST ===\n";

CardHashTable largeTable;
OperationCounter largeCounter;

for (int i = 0; i < 50000; i++)
{
    Card testCard;

    long long number =
        1234560000000000LL + i;

    testCard.setCardNumber(number);
    testCard.setHolderName("Test User");
    testCard.setCNIC("0000000000000");
    testCard.setBalance(500.0);

    largeTable.insert(
        testCard,
        largeCounter);
}

std::cout
    << "Large table card count: "
    << largeTable.getCount()
    << '\n';

std::cout
    << "Large table capacity: "
    << largeTable.getCapacity()
    << '\n';

std::cout
    << "Large table load factor: "
    << largeTable.getLoadFactor()
    << '\n';

OperationCounter largeSearchCounter;

Card* largeFound =
    largeTable.search(
        1234560000049999LL,
        largeSearchCounter);

if (largeFound != nullptr)
{
    std::cout
        << "50,000-card lookup successful.\n";
}

std::cout
    << "Large search steps: "
    << largeSearchCounter.getSteps()
    << '\n';

std::cout
    << "Large search comparisons: "
    << largeSearchCounter.getComparisons()
    << '\n';

std::cout
    << "\n=== HASH LOOKUP COMPARISON ===\n";

CardHashTable table5000;
OperationCounter insert5000;

for (int i = 0; i < 5000; i++)
{
    Card testCard;

    testCard.setCardNumber(
        1234560000000000LL + i);

    testCard.setHolderName("Test");
    testCard.setCNIC("0000000000000");
    testCard.setBalance(100.0);

    table5000.insert(
        testCard,
        insert5000);
}

OperationCounter search5000;

table5000.search(
    1234560000004999LL,
    search5000);

OperationCounter search50000;

largeTable.search(
    1234560000049999LL,
    search50000);

std::cout
    << "5,000 cards - comparisons: "
    << search5000.getComparisons()
    << '\n';

std::cout
    << "50,000 cards - comparisons: "
    << search50000.getComparisons()
    << '\n';

std::cout
    << "\n=== BLOCKED CARD HASH TABLE TEST ===\n";

BlockedCardHashTable blockedTable;
OperationCounter blockedCounter;

blockedTable.blockCard(
    1234560000000002LL,
    blockedCounter);

blockedTable.blockCard(
    1234560000000005LL,
    blockedCounter);

blockedTable.blockCard(
    1234560000000009LL,
    blockedCounter);

std::cout
    << "Blocked cards: "
    << blockedTable.getCount()
    << '\n';

OperationCounter blockedSearchCounter;

if (blockedTable.isBlocked(
        1234560000000005LL,
        blockedSearchCounter))
{
    std::cout
        << "Blocked card detected correctly.\n";
}

OperationCounter allowedSearchCounter;

if (!blockedTable.isBlocked(
        1234560000000007LL,
        allowedSearchCounter))
{
    std::cout
        << "Unblocked card allowed correctly.\n";
}

OperationCounter duplicateBlockCounter;

if (!blockedTable.blockCard(
        1234560000000005LL,
        duplicateBlockCounter))
{
    std::cout
        << "Duplicate block rejected correctly.\n";
}

OperationCounter unblockCounter;

if (blockedTable.unblockCard(
        1234560000000005LL,
        unblockCounter))
{
    std::cout
        << "Card unblocked successfully.\n";
}

std::cout
    << "Blocked cards remaining: "
    << blockedTable.getCount()
    << '\n';

OperationCounter verifyUnblockCounter;

if (!blockedTable.isBlocked(
        1234560000000005LL,
        verifyUnblockCounter))
{
    std::cout
        << "Unblock verified correctly.\n";
}

BlockedCardHashTable copiedBlockedTable =
    blockedTable;

std::cout
    << "Copied blocked count: "
    << copiedBlockedTable.getCount()
    << '\n';

BlockedCardHashTable assignedBlockedTable;

assignedBlockedTable =
    blockedTable;

std::cout
    << "Assigned blocked count: "
    << assignedBlockedTable.getCount()
    << '\n';

    return 0;
}








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