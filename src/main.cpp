#include <iostream>
#include "../include/Card.h"

int main()
{
    Card card;

    card.setCardNumber(1234560000000001LL);
    card.setHolderName("Ali Khan");
    card.setCNIC("3520212345671");
    card.setBalance(500.0);

    std::cout << "Card Number: " << card.getCardNumber() << '\n';
    std::cout << "Holder Name: " << card.getHolderName() << '\n';
    std::cout << "CNIC: " << card.getCNIC() << '\n';
    std::cout << "Balance: " << card.getBalance() << '\n';
    std::cout << "Blocked: " << (card.isBlocked() ? "Yes" : "No") << '\n';
    std::cout << "Active: " << (card.isActive() ? "Yes" : "No") << '\n';

    return 0;
}