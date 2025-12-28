#include "TransactionLogger.h"
#include <fstream>

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
