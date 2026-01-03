// To log transactions for Task 3

#pragma once

#include <string>
#include "OrderBookEntry.h"

class TransactionLogger
{
public:
    TransactionLogger(const std::string& username);

    // Logging function to save to CSV for Task 3
    void log(
        const std::string& timestamp,
        double price,
        double amount,
        const std::string& product,
        const std::string& orderType
    );
    // To get summary statistics of user activity for Task 3
    void computeSummary(const std::string& productFilter, const std::string& startTime);
    // To get last 5 transaction history for Task 3
    void printRecentTransactions(int limit, const std::string& productFilter);

private:
    std::string filename;
};
