#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

class Wallet
{
public:
    Wallet(const std::string& username);

    /// To load wallet data from CSV file for Task 2
    void loadFromCSV();
    // To save wallet data to CSV file for Task 2 and 3
    void saveToCSV();

    void insertCurrency(std::string type, double amount);
    bool removeCurrency(std::string type, double amount);

    // Deposit function for Task 3
    bool deposit(std::string currency, double amount);

    // Withdraw function for Task 3
    bool withdraw(std::string currency, double amount);

    bool containsCurrency(std::string type, double amount);
    double getAmount(const std::string& currency) const;
    bool canFulfillOrder(OrderBookEntry order);
    void processSale(OrderBookEntry& sale);

    std::string toString();

private:
    std::string username; // foreign key for CSV
    std::map<std::string,double> currencies;
};
