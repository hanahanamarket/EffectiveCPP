#include <iostream>

class Transaction {
public:
    Transaction(){}

    void logTransaction() const
    {
        std::cout << "[Transaction] Transaction\n";
    }
};

class BuyTransaction : public Transaction {
public:
    BuyTransaction()
        : Transaction()
    {
        logOwnTransaction();
    }

private:
    void logOwnTransaction() const
    {
        std::cout << "[BuyTransaction] BuyTransaction\n";
    }
};

int main()
{
    BuyTransaction b;
}