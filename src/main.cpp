#include <iostream>
#include "../include/Card.h"
#include "../include/Journey.h"
#include "../include/Station.h"

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

    return 0;
}