#include "TransactionLogger.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

TransactionLogger::TransactionLogger(const std::string& username)
{
    filename = username + "Transactions.csv";

    // Create file with header if it does not exist
    std::ifstream infile(filename);
    if (!infile.good())
    {
        std::ofstream outfile(filename);
        outfile << "timestamp,price,amount,product,ordertype\n";
    }
}

void TransactionLogger::log(
    const std::string& timestamp,
    double price,
    double amount,
    const std::string& product,
    const std::string& orderType
)
{
    std::ofstream file(filename, std::ios::app);
    file << timestamp << ","
         << price << ","
         << amount << ","
         << product << ","
         << orderType << "\n";
}

std::string TransactionLogger::getLastTransactionTime(const std::string& username) {
    std::ifstream file(username + "Transactions.csv");
    if (!file.is_open()) return ""; // No transactions yet

    std::string line;
    std::string lastTimestamp;

    // Skip header if present
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string timestamp;
        std::getline(ss, timestamp, ','); // first column is timestamp
        lastTimestamp = timestamp;        // update each time
    }

    return lastTimestamp; // returns empty if no transactions
}

void TransactionLogger::computeSummary(
    const std::string& productFilter = "",          // empty string = all products
    const std::string& startTime = ""             // optional: "YYYY-MM-DD HH:MM"
) 
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No transactions found." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    int totalAsks = 0;
    int totalBids = 0;
    double totalMoney = 0.0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string timestamp, priceStr, amountStr, product, orderType;

        std::getline(ss, timestamp, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, amountStr, ',');
        std::getline(ss, product, ',');
        std::getline(ss, orderType, ',');

        double price = std::stod(priceStr);
        double amount = std::stod(amountStr);

        // Filter by product
        if (!productFilter.empty() && product != productFilter)
            continue;

        // Filter by timeframe
        if (!startTime.empty() && timestamp < startTime)
            continue;

        if (orderType == "ask") {
            totalAsks++;
            totalMoney += amount * price; // money received from selling
        } else if (orderType == "bid") {
            totalBids++;
            totalMoney += amount * price; // money spent buying
        }
    }

    std::cout << std::endl << "==== Transaction Summary ====" << std::endl;
    if (productFilter.empty())
        std::cout << "All products" << std::endl;
    else
        std::cout << "Product: " << productFilter << std::endl;

    if (!startTime.empty()) {
        std::cout << "Timeframe: ";
        if (!startTime.empty()) std::cout << startTime;
        else std::cout << "beginning";
        std::cout << " - ";
        std::cout << "now" << std::endl;
    }

    std::cout << "Total Asks: " << totalAsks << std::endl;
    std::cout << "Total Bids: " << totalBids << std::endl;
    std::cout << "Total Money Spent/Received: " << totalMoney << std::endl;
    std::cout << "============================" << std::endl;
}

void TransactionLogger::printRecentTransactions(int limit, const std::string& productFilter)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "No transaction history found." << std::endl;
        return;
    }

    std::vector<std::string> transactions;
    std::string line;

    // Skip header
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        if (!productFilter.empty())
        {
            if (line.find(productFilter) == std::string::npos)
                continue;
        }

        transactions.push_back(line);
    }

    int start = std::max(0, (int)transactions.size() - limit);

    std::cout << std::left
            << std::setw(26) << "Timestamp"
            << std::setw(12) << "Price"
            << std::setw(12) << "Amount"
            << std::setw(12) << "Product"
            << std::setw(10) << "Type"
            << std::endl;

    std::cout << std::string(72, '=') << std::endl;


    for (int i = start; i < transactions.size(); ++i)
    {
        // std::cout << transactions[i] << std::endl;
        std::stringstream ss(transactions[i]);
        std::string timestamp, price, amount, product, orderType;

        std::getline(ss, timestamp, ',');
        std::getline(ss, price, ',');
        std::getline(ss, amount, ',');
        std::getline(ss, product, ',');
        std::getline(ss, orderType, ',');
        std::cout << std::left
        << std::setw(26) << timestamp
        << std::setw(12) << std::fixed << std::setprecision(2) << price
        << std::setw(12) << amount
        << std::setw(12) << product
        << std::setw(10) << orderType
        << std::endl;

    }
}