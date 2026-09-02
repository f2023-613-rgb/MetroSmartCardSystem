#include <iostream>
#include "../include/Card.h"
#include "../include/Journey.h"
#include "../include/Station.h"
#include "../include/OperationCounter.h"
#include "../include/GateQueue.h"
#include "../include/TopUpStack.h"
#include "../include/TransactionLog.h"
#include "../include/JourneyHistory.h"
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

    return 0;
}