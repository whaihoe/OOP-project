#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

class Wallet
{
public:
    Wallet(const std::string& username);

    void loadFromCSV();
    void saveToCSV();

    void insertCurrency(std::string type, double amount);
    bool removeCurrency(std::string type, double amount);

    bool deposit(std::string currency, double amount);
    bool withdraw(std::string currency, double amount);

    bool containsCurrency(std::string type, double amount);
    double getAmount(const std::string& currency) const;
    bool canFulfillOrder(OrderBookEntry order);
    void processSale(OrderBookEntry& sale);

    std::string toString();

private:
    std::string username; // foreign key
    std::map<std::string,double> currencies;
};
