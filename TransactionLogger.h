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

private:
    std::string filename;
};
