#include <iostream>

class Transaction {
public:
    Transaction()
    {
        init();
    }

    virtual void logTransaction() const;

private:
    void init()
    {
        logTransaction();
    }
};

void Transaction::logTransaction() const
{
    std::cout << "[Transaction] Transaction\n";
}

class BuyTransaction : public Transaction {
public:
    void logTransaction() const override
    {
        std::cout << "[BuyTransaction] BuyTransaction\n";
    }
};

int main()
{
    BuyTransaction b;
}