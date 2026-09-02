#ifndef CARD_H
#define CARD_H

class Card
{
private:
    long long cardNumber;
    char holderName[100];
    char cnic[20];
    double balance;
    bool blocked;
    bool active;

public:
    // Constructor
    Card();

    // Get functions
    long long getCardNumber() const;
    const char* getHolderName() const;
    const char* getCNIC() const;
    double getBalance() const;
    bool isBlocked() const;
    bool isActive() const;

    // Set functions
    void setCardNumber(long long number);
    void setHolderName(const char* name);
    void setCNIC(const char* cnicValue);
    void setBalance(double amount);
    void setBlocked(bool value);
    void setActive(bool value);

    // Balance operations
    bool addBalance(double amount);
    bool deductBalance(double amount);
};

#endif