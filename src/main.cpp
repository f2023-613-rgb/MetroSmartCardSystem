#include <iostream>
#include "../include/Card.h"
#include "../include/Journey.h"
#include "../include/Station.h"
#include "../include/OperationCounter.h"
#include "../include/GateQueue.h"

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

    return 0;
}