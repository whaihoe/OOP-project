#include "Wallet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "CSVReader.h"
#include "TransactionLogger.h"

Wallet::Wallet(const std::string& username)
    : username(username)
{
    loadFromCSV();
}

// To load wallet data from CSV file for Task 2
void Wallet::loadFromCSV()
{
    currencies.clear();

    std::ifstream file("wallet.csv");
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string user, currency, balanceStr;

        std::getline(ss, user, ',');
        std::getline(ss, currency, ',');
        std::getline(ss, balanceStr, ',');

        if (user == username)
        {
            currencies[currency] = std::stod(balanceStr);
        }
    }

    if (currencies.empty())
    {
        insertCurrency("BTC", 10);
        saveToCSV();
    }
}

// To save wallet data to CSV file for Task 2 and 3
void Wallet::saveToCSV()
{
    std::ifstream file("wallet.csv");
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line))
    {
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();

    std::ofstream out("wallet.csv", std::ios::trunc);

    // Write back other users
    for (const std::string& l : lines)
    {
        if (l.substr(0, username.size()) != username)
            out << l << std::endl;
    }

    // Write this user's wallet
    for (const auto& [currency, balance] : currencies)
    {
        out << username << ","
            << currency << ","
            << balance << std::endl;
    }
}

void Wallet::insertCurrency(std::string type, double amount)
{
    double balance;
    if (amount < 0)
    {
        throw std::exception{};
    }
    if (currencies.count(type) == 0) // not there yet
    {
        balance = 0;
    }
    else { // is there 
        balance = currencies[type];
    }
    balance += amount; 
    currencies[type] = balance; 
}

bool Wallet::removeCurrency(std::string type, double amount)
{
    if (amount < 0)
    {
        return false; 
    }
    if (currencies.count(type) == 0) // not there yet
    {
        //std::cout << "No currency for " << type << std::endl;
        return false;
    }
    else { // is there - do  we have enough
        if (containsCurrency(type, amount))// we have enough
        {
            //std::cout << "Removing " << type << ": " << amount << std::endl;
            currencies[type] -= amount;
            return true;
        } 
        else // they have it but not enough.
            return false; 
    }
}

// Deposit function for Task 3
bool Wallet::deposit(std::string currency, double amount)
{
    if (amount <= 0)
    {
        std::cout << "Invalid input: deposit amount must be positive." << std::endl;
        return false;
    }

    insertCurrency(currency, amount);
    return true;
}

// Withdraw function for Task 3
bool Wallet::withdraw(std::string currency, double amount)
{
    if (amount <= 0)
    {
        std::cout << "Invalid input: withdrawal amount must be positive." << std::endl;
        return false;
    }

    if (!removeCurrency(currency, amount))
    {
        std::cout << "Invalid input: insufficient balance." << std::endl;
        return false;
    }

    return true;
}

bool Wallet::containsCurrency(std::string type, double amount)
{
    if (currencies.count(type) == 0) // not there yet
        return false;
    else 
        return currencies[type] >= amount;
    
}

std::string Wallet::toString()
{
    std::string s;
    for (std::pair<std::string,double> pair : currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    return s;
}

double Wallet::getAmount(const std::string& currency) const
{
    if (currencies.count(currency) == 0)
        return 0.0;
    return currencies.at(currency);
}

bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
    // ask
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        return containsCurrency(currency, amount);
    }
    // bid
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }


    return false; 
}
      

void Wallet::processSale(OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
    // ask
    if (sale.orderType == OrderBookType::asksale)
    {
        TransactionLogger logger(sale.username);

        double outgoingAmount = sale.amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        std::string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

        // To log transaction as asksale in transactionlogger
        logger.log(sale.timestamp, sale.price, sale.amount, sale.product, "asksale");
    }
    // bid
    if (sale.orderType == OrderBookType::bidsale)
    {
        TransactionLogger logger(sale.username);

        double incomingAmount = sale.amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        std::string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

        // To log transaction as asksale in transactionlogger
        logger.log(sale.timestamp, sale.price, sale.amount, sale.product, "bidsale");
    }
}
std::ostream& operator<<(std::ostream& os,  Wallet& wallet)
{
    os << wallet.toString();
    return os;
}