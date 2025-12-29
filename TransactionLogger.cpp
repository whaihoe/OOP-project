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

