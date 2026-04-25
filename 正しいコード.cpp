#include <iostream>

class Transaction {
public:
    explicit Transaction(const char* logInfo)
    {
        logTransaction(logInfo);
    }

    void logTransaction(const char* logInfo) const
    {
        std::cout << "[Transaction] " << logInfo << '\n';
    }
};

class BuyTransaction : public Transaction {
public:
    BuyTransaction()
        : Transaction(createLogString())
    {}

private:
    static const char* createLogString()
    {
        return "BuyTransaction";
    }
};

int main()
{
    BuyTransaction b;
}