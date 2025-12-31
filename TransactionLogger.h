#pragma once

#include <string>
#include "OrderBookEntry.h"

class TransactionLogger
{
public:
    TransactionLogger(const std::string& username);

    void log(
        const std::string& timestamp,
        double price,
        double amount,
        const std::string& product,
        const std::string& orderType
    );

    static std::string getLastTransactionTime(const std::string& username);
    void computeSummary(const std::string& productFilter, const std::string& startTime);
    void printRecentTransactions(int limit, const std::string& productFilter);

private:
    std::string filename;
};
